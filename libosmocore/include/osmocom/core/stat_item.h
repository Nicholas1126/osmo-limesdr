#pragma once

/*! \defgroup osmo_stat_item Statistics value item
 *  @{
 * \file stat_item.h */

#include <stdint.h>

#include <osmocom/core/linuxlist.h>

struct osmo_stat_item_desc;

#define OSMO_STAT_ITEM_NOVALUE_ID 0
#define OSMO_STAT_ITEM_NO_UNIT NULL

/*! Individual entry in value FIFO */
struct osmo_stat_item_value {
	int32_t id;		/*!< identifier of value */
	int32_t value;		/*!< actual value */
};

/*! data we keep for each actual item */
struct osmo_stat_item {
	/*! back-reference to the item description */
	const struct osmo_stat_item_desc *desc;
	/*! the index of the freshest value */
	int32_t last_value_index;
	/*! offset to the freshest value in the value FIFO */
	int16_t last_offs;
	/*! value FIFO */
	struct osmo_stat_item_value values[0];
};

/*! Statistics item description */
struct osmo_stat_item_desc {
	const char *name;	/*!< name of the item */
	const char *description;/*!< description of the item */
	const char *unit;	/*!< unit of a value */
	unsigned int num_values;/*!< number of values to store in FIFO */
	int32_t default_value;	/*!< default value */
};

/*! Description of a statistics item group */
struct osmo_stat_item_group_desc {
	/*! The prefix to the name of all values in this group */
	const char *group_name_prefix;
	/*! The human-readable description of the group */
	const char *group_description;
	/*! The class to which this group belongs */
	int class_id;
	/*! The number of values in this group (size of item_desc) */
	const unsigned int num_items;
	/*! Pointer to array of value names, length as per num_items */
	const struct osmo_stat_item_desc *item_desc;
};

/*! One instance of a counter group class */
struct osmo_stat_item_group {
	/*! Linked list of all value groups in the system */
	struct llist_head list;
	/*! Pointer to the counter group class */
	const struct osmo_stat_item_group_desc *desc;
	/*! The index of this value group within its class */
	unsigned int idx;
	/*! Actual counter structures below */
	struct osmo_stat_item *items[0];
};

struct osmo_stat_item_group *osmo_stat_item_group_alloc(
	void *ctx,
	const struct osmo_stat_item_group_desc *desc,
	unsigned int idx);

static inline void osmo_stat_item_group_udp_idx(
	struct osmo_stat_item_group *grp, unsigned int idx)
{
	grp->idx = idx;
}

void osmo_stat_item_group_free(struct osmo_stat_item_group *statg);

void osmo_stat_item_set(struct osmo_stat_item *item, int32_t value);

int osmo_stat_item_init(void *tall_ctx);

struct osmo_stat_item_group *osmo_stat_item_get_group_by_name_idx(
	const char *name, const unsigned int idx);

const struct osmo_stat_item *osmo_stat_item_get_by_name(
	const struct osmo_stat_item_group *statg, const char *name);

int osmo_stat_item_get_next(const struct osmo_stat_item *item, int32_t *idx, int32_t *value);

/*! Get the last (freshest) value */
static int32_t osmo_stat_item_get_last(const struct osmo_stat_item *item);

int osmo_stat_item_discard(const struct osmo_stat_item *item, int32_t *idx);

int osmo_stat_item_discard_all(int32_t *idx);

typedef int (*osmo_stat_item_handler_t)(
	struct osmo_stat_item_group *, struct osmo_stat_item *, void *);

typedef int (*osmo_stat_item_group_handler_t)(struct osmo_stat_item_group *, void *);

int osmo_stat_item_for_each_item(struct osmo_stat_item_group *statg,
	osmo_stat_item_handler_t handle_item, void *data);

int osmo_stat_item_for_each_group(osmo_stat_item_group_handler_t handle_group, void *data);

static inline int32_t osmo_stat_item_get_last(const struct osmo_stat_item *item)
{
	return item->values[item->last_offs].value;
}
/*! @} */
