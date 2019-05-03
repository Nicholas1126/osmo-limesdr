/*! \file stat_item.c
 * utility routines for keeping statistical values */
/*
 * (C) 2009-2010 by Harald Welte <laforge@gnumonks.org>
 * (C) 2015 by sysmocom - s.f.m.c. GmbH
 *
 * All Rights Reserved
 *
 * SPDX-License-Identifier: GPL-2.0+
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

/*! \addtogroup osmo_stat_item
 *  @{
 *
 *  This osmo_stat_item module adds instrumentation capabilities to
 *  gather measurement and statistical values in a similar fashion to
 *  what we have as \ref osmo_counter_group.
 *
 *  As opposed to counters, osmo_stat_item do not increment but consist
 *  of a configurable-sized FIFO, which can store not only the current
 *  (most recent) value, but also historic values.
 *
 *  The only supported value type is an int32_t.
 *
 *  Getting values from the osmo_stat_item does not modify its state to
 *  allow for multiple independent back-ends retrieving values (e.g. VTY
 *  and statd).
 *
 *  Each value stored in the FIFO of an osmo_stat_item has an associated
 *  value_id.  The value_id is derived from an application-wide globally
 *  incrementing counter, so (until the counter wraps) more recent
 *  values will have higher values.
 *
 *  When a new value is set, the oldest value in the FIFO gets silently
 *  overwritten.  Lost values are skipped when getting values from the
 *  item.
 *
 */

#include <stdint.h>
#include <string.h>

#include <osmocom/core/utils.h>
#include <osmocom/core/linuxlist.h>
#include <osmocom/core/talloc.h>
#include <osmocom/core/timer.h>
#include <osmocom/core/stat_item.h>

/*! global list of stat_item groups */
static LLIST_HEAD(osmo_stat_item_groups);
/*! counter for assigning globally unique value identifiers */
static int32_t global_value_id = 0;

/*! talloc context from which we allocate */
static void *tall_stat_item_ctx;

/*! Allocate a new group of counters according to description.
 *  Allocate a group of stat items described in \a desc from talloc context \a ctx,
 *  giving the new group the index \a idx.
 *  \param[in] ctx \ref talloc context
 *  \param[in] desc Statistics item group description
 *  \param[in] idx Index of new stat item group
 */
struct osmo_stat_item_group *osmo_stat_item_group_alloc(void *ctx,
					    const struct osmo_stat_item_group_desc *desc,
					    unsigned int idx)
{
	unsigned int group_size;
	unsigned long items_size = 0;
	unsigned int item_idx;
	void *items;

	struct osmo_stat_item_group *group;

	group_size = sizeof(struct osmo_stat_item_group) +
			desc->num_items * sizeof(struct osmo_stat_item *);

	if (!ctx)
		ctx = tall_stat_item_ctx;

	group = talloc_zero_size(ctx, group_size);
	if (!group)
		return NULL;

	group->desc = desc;
	group->idx = idx;

	/* Get combined size of all items */
	for (item_idx = 0; item_idx < desc->num_items; item_idx++) {
		unsigned int size;
		size = sizeof(struct osmo_stat_item) +
			sizeof(struct osmo_stat_item_value) *
			desc->item_desc[item_idx].num_values;
		/* Align to pointer size */
		size = (size + sizeof(void *) - 1) & ~(sizeof(void *) - 1);

		/* Store offsets into the item array */
		group->items[item_idx] = (void *)items_size;

		items_size += size;
	}

	items = talloc_zero_size(group, items_size);
	if (!items) {
		talloc_free(group);
		return NULL;
	}

	/* Update item pointers */
	for (item_idx = 0; item_idx < desc->num_items; item_idx++) {
		struct osmo_stat_item *item = (struct osmo_stat_item *)
			((uint8_t *)items + (unsigned long)group->items[item_idx]);
		unsigned int i;

		group->items[item_idx] = item;
		item->last_offs = desc->item_desc[item_idx].num_values - 1;
		item->last_value_index = -1;
		item->desc = &desc->item_desc[item_idx];

		for (i = 0; i <= item->last_offs; i++) {
			item->values[i].value = desc->item_desc[item_idx].default_value;
			item->values[i].id = OSMO_STAT_ITEM_NOVALUE_ID;
		}
	}

	llist_add(&group->list, &osmo_stat_item_groups);

	return group;
}

/*! Free the memory for the specified group of stat items */
void osmo_stat_item_group_free(struct osmo_stat_item_group *grp)
{
	llist_del(&grp->list);
	talloc_free(grp);
}

/*! Set the a given stat_item to the given value.
 *  This function adds a new value for the given stat_item at the end of
 *  the FIFO.
 *  \param[in] item The stat_item whose \a value we want to set
 *  \param[in] value The numeric value we want to store at end of FIFO
 */
void osmo_stat_item_set(struct osmo_stat_item *item, int32_t value)
{
	item->last_offs += 1;
	if (item->last_offs >= item->desc->num_values)
		item->last_offs = 0;

	global_value_id += 1;
	if (global_value_id == OSMO_STAT_ITEM_NOVALUE_ID)
		global_value_id += 1;

	item->values[item->last_offs].value = value;
	item->values[item->last_offs].id    = global_value_id;
}

/*! Retrieve the next value from the osmo_stat_item object.
 * If a new value has been set, it is returned. The idx is used to decide
 * which value to return.
 * On success, *idx is updated to refer to the next unread value. If
 * values have been missed due to FIFO overflow, *idx is incremented by
 * (1 + num_lost).
 * This way, the osmo_stat_item object can be kept stateless from the reader's
 * perspective and therefore be used by several backends simultaneously.
 *
 * \param val	the osmo_stat_item object
 * \param idx	identifies the next value to be read
 * \param value	a pointer to store the value
 * \returns  the increment of the index (0: no value has been read,
 *           1: one value has been taken,
 *           (1+n): n values have been skipped, one has been taken)
 */
int osmo_stat_item_get_next(const struct osmo_stat_item *item, int32_t *next_idx,
	int32_t *value)
{
	const struct osmo_stat_item_value *next_value;
	const struct osmo_stat_item_value *item_value = NULL;
	int idx_delta;
	int next_offs;

	next_offs = item->last_offs;
	next_value = &item->values[next_offs];

	while (next_value->id - *next_idx >= 0 &&
		next_value->id != OSMO_STAT_ITEM_NOVALUE_ID)
	{
		item_value = next_value;

		next_offs -= 1;
		if (next_offs < 0)
			next_offs = item->desc->num_values - 1;
		if (next_offs == item->last_offs)
			break;
		next_value = &item->values[next_offs];
	}

	if (!item_value)
		/* All items have been read */
		return 0;

	*value = item_value->value;

	idx_delta = item_value->id + 1 - *next_idx;

	*next_idx = item_value->id + 1;

	return idx_delta;
}

/*! Skip/discard all values of this item and update \a idx accordingly */
int osmo_stat_item_discard(const struct osmo_stat_item *item, int32_t *idx)
{
	int discarded = item->values[item->last_offs].id + 1 - *idx;
	*idx = item->values[item->last_offs].id + 1;

	return discarded;
}

/*! Skip all values of all items and update \a idx accordingly */
int osmo_stat_item_discard_all(int32_t *idx)
{
	int discarded = global_value_id + 1 - *idx;
	*idx = global_value_id + 1;

	return discarded;
}

/*! Initialize the stat item module. Call this once from your program.
 *  \param[in] tall_ctx Talloc context from which this module allocates */
int osmo_stat_item_init(void *tall_ctx)
{
	tall_stat_item_ctx = tall_ctx;

	return 0;
}

/*! Search for item group based on group name and index
 *  \param[in] name Name of stats_item_group we want to find
 *  \param[in] idx Index of the group we want to find
 *  \returns pointer to group, if found; NULL otherwise */
struct osmo_stat_item_group *osmo_stat_item_get_group_by_name_idx(
	const char *name, const unsigned int idx)
{
	struct osmo_stat_item_group *statg;

	llist_for_each_entry(statg, &osmo_stat_item_groups, list) {
		if (!statg->desc)
			continue;

		if (!strcmp(statg->desc->group_name_prefix, name) &&
				statg->idx == idx)
			return statg;
	}
	return NULL;
}

/*! Search for item based on group + item name
 *  \param[in] statg group in which to search for the item
 *  \param[in] name name of item to search within \a statg
 *  \returns pointer to item, if found; NULL otherwise */
const struct osmo_stat_item *osmo_stat_item_get_by_name(
	const struct osmo_stat_item_group *statg, const char *name)
{
	int i;
	const struct osmo_stat_item_desc *item_desc;

	if (!statg->desc)
		return NULL;

	for (i = 0; i < statg->desc->num_items; i++) {
		item_desc = &statg->desc->item_desc[i];

		if (!strcmp(item_desc->name, name)) {
			return statg->items[i];
		}
	}
	return NULL;
}

/*! Iterate over all items in group, call user-supplied function on each
 *  \param[in] statg stat_item group over whose items to iterate
 *  \param[in] handle_item Call-back function, aborts if rc < 0
 *  \param[in] data Private data handed through to \a handle_item
 */
int osmo_stat_item_for_each_item(struct osmo_stat_item_group *statg,
	osmo_stat_item_handler_t handle_item, void *data)
{
	int rc = 0;
	int i;

	for (i = 0; i < statg->desc->num_items; i++) {
		struct osmo_stat_item *item = statg->items[i];
		rc = handle_item(statg, item, data);
		if (rc < 0)
			return rc;
	}

	return rc;
}

/*! Iterate over all stat_item groups in system, call user-supplied function on each
 *  \param[in] handle_group Call-back function, aborts if rc < 0
 *  \param[in] data Private data handed through to \a handle_group
 */
int osmo_stat_item_for_each_group(osmo_stat_item_group_handler_t handle_group, void *data)
{
	struct osmo_stat_item_group *statg;
	int rc = 0;

	llist_for_each_entry(statg, &osmo_stat_item_groups, list) {
		rc = handle_group(statg, data);
		if (rc < 0)
			return rc;
	}

	return rc;
}

/*! @} */
