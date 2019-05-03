/*! \file linuxlist.h
 *
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole llists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

#pragma once

/*! \defgroup linuxlist Simple doubly linked list implementation
 *  @{
 * \file linuxlist.h */

#include <stddef.h>

#ifndef inline
#define inline __inline__
#endif

static inline void prefetch(const void *x) {;}

/*! Cast a member of a structure out to the containing structure.
 *  \param[in] ptr    the pointer to the member.
 *  \param[in] type   the type of the container struct this is embedded in.
 *  \param[in] member the name of the member within the struct.
 */
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type, member) );})


/*!
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized llist entries.
 */
#define LLIST_POISON1  ((void *) 0x00100100)
#define LLIST_POISON2  ((void *) 0x00200200)

/*! (double) linked list header structure */
struct llist_head {
	/*! Pointer to next and previous item */
	struct llist_head *next, *prev;
};

/*! Define a new llist_head pointing to a given llist_head.
 *  \param[in] name another llist_head to be pointed.
 */
#define LLIST_HEAD_INIT(name) { &(name), &(name) }

/*! Define a statically-initialized variable of type llist_head.
 *  \param[in] name variable (symbol) name.
 */
#define LLIST_HEAD(name) \
	struct llist_head name = LLIST_HEAD_INIT(name)

/*! Initialize a llist_head to point back to itself.
 *  \param[in] ptr llist_head to be initialized.
 */
#define INIT_LLIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal llist manipulation where we know
 * the prev/next entries already!
 */
static inline void __llist_add(struct llist_head *_new,
			       struct llist_head *prev,
			       struct llist_head *next)
{
	next->prev = _new;
	_new->next = next;
	_new->prev = prev;
	prev->next = _new;
}

/*! Add a new entry into a linked list (at head).
 *  \param _new the entry to be added.
 *  \param head llist_head to prepend the element to.
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void llist_add(struct llist_head *_new, struct llist_head *head)
{
	__llist_add(_new, head, head->next);
}

/*! Add a new entry into a linked list (at tail).
 *  \param _new the entry to be added.
 *  \param head llist_head to append the element to.
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void llist_add_tail(struct llist_head *_new, struct llist_head *head)
{
	__llist_add(_new, head->prev, head);
}

/*
 * Delete a llist entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal llist manipulation where we know
 * the prev/next entries already!
 */
static inline void __llist_del(struct llist_head * prev, struct llist_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/*! Delete a single entry from a linked list.
 *  \param entry the element to delete.
 *
 * Note: llist_empty on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void llist_del(struct llist_head *entry)
{
	__llist_del(entry->prev, entry->next);
	entry->next = (struct llist_head *)LLIST_POISON1;
	entry->prev = (struct llist_head *)LLIST_POISON2;
}

/*! Delete a single entry from a linked list and reinitialize it.
 *  \param entry the element to delete and reinitialize.
 */
static inline void llist_del_init(struct llist_head *entry)
{
	__llist_del(entry->prev, entry->next);
	INIT_LLIST_HEAD(entry);
}

/*! Delete from one llist and add as another's head.
 *  \param llist the entry to move.
 *  \param head  the head that will precede our entry.
 */
static inline void llist_move(struct llist_head *llist, struct llist_head *head)
{
	__llist_del(llist->prev, llist->next);
	llist_add(llist, head);
}

/*! Delete from one llist and add as another's tail.
 *  \param llist the entry to move.
 *  \param head  the head that will follow our entry.
 */
static inline void llist_move_tail(struct llist_head *llist,
				   struct llist_head *head)
{
	__llist_del(llist->prev, llist->next);
	llist_add_tail(llist, head);
}

/*! Test whether a linked list is empty.
 *  \param[in] head the llist to test.
 *  \returns 1 if the list is empty, 0 otherwise.
 */
static inline int llist_empty(const struct llist_head *head)
{
	return head->next == head;
}

static inline void __llist_splice(struct llist_head *llist,
				  struct llist_head *head)
{
	struct llist_head *first = llist->next;
	struct llist_head *last = llist->prev;
	struct llist_head *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

/*! Join two linked lists.
 *  \param llist the new linked list to add.
 *  \param head  the place to add llist within the other list.
 */
static inline void llist_splice(struct llist_head *llist, struct llist_head *head)
{
	if (!llist_empty(llist))
		__llist_splice(llist, head);
}

/*! Join two llists and reinitialise the emptied llist.
 *  \param llist the new linked list to add.
 *  \param head  the place to add it within the first llist.
 *
 * The llist is reinitialised.
 */
static inline void llist_splice_init(struct llist_head *llist,
				     struct llist_head *head)
{
	if (!llist_empty(llist)) {
		__llist_splice(llist, head);
		INIT_LLIST_HEAD(llist);
	}
}

/*! Get the struct containing this list entry.
 *  \param ptr    the llist_head pointer.
 *  \param type   the type of the struct this is embedded in.
 *  \param member the name of the llist_head within the struct.
 */
#define llist_entry(ptr, type, member) \
	container_of(ptr, type, member)

/*! Get the first element from a linked list.
 *  \param ptr    the list head to take the element from.
 *  \param type   the type of the struct this is embedded in.
 *  \param member the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define llist_first_entry(ptr, type, member) \
	llist_entry((ptr)->next, type, member)

/*! Get the last element from a list.
 *  \param ptr    the list head to take the element from.
 *  \param type   the type of the struct this is embedded in.
 *  \param member the name of the llist_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define llist_last_entry(ptr, type, member) \
	llist_entry((ptr)->prev, type, member)

/*! Get the first element from a list, or NULL.
 *  \param ptr    the list head to take the element from.
 *  \param type   the type of the struct this is embedded in.
 *  \param member the name of the list_head within the struct.
 *
 * Note that if the list is empty, it returns NULL.
 */
#define llist_first_entry_or_null(ptr, type, member) \
	(!llist_empty(ptr) ? llist_first_entry(ptr, type, member) : NULL)

/*! Iterate over a linked list.
 *  \param pos  the llist_head to use as a loop counter.
 *  \param head the head of the list over which to iterate.
 */
#define llist_for_each(pos, head) \
	for (pos = (head)->next, prefetch(pos->next); pos != (head); \
		pos = pos->next, prefetch(pos->next))

/*! Iterate over a linked list (no prefetch).
 *  \param pos  the llist_head to use as a loop counter.
 *  \param head the head of the list over which to iterate.
 *
 * This variant differs from llist_for_each() in that it's the
 * simplest possible llist iteration code, no prefetching is done.
 * Use this for code that knows the llist to be very short (empty
 * or 1 entry) most of the time.
 */
#define __llist_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/*! Iterate over a linked list backwards.
 *  \param pos  the llist_head to use as a loop counter.
 *  \param head the head of the list over which to iterate.
 */
#define llist_for_each_prev(pos, head) \
	for (pos = (head)->prev, prefetch(pos->prev); pos != (head); \
		pos = pos->prev, prefetch(pos->prev))

/*! Iterate over a linked list, safe against removal of llist entry.
 *  \param pos  the llist_head to use as a loop counter.
 *  \param n    another llist_head to use as temporary storage.
 *  \param head the head of the list over which to iterate.
 */
#define llist_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/*! Iterate over a linked list of a given type.
 *  \param pos    the 'type *' to use as a loop counter.
 *  \param head   the head of the list over which to iterate.
 *  \param member the name of the llist_head within the struct pos.
 */
#define llist_for_each_entry(pos, head, member)				\
	for (pos = llist_entry((head)->next, typeof(*pos), member),	\
		     prefetch(pos->member.next);			\
	     &pos->member != (head); 					\
	     pos = llist_entry(pos->member.next, typeof(*pos), member),	\
		     prefetch(pos->member.next))

/*! Iterate backwards over a linked list of a given type.
 *  \param pos    the 'type *' to use as a loop counter.
 *  \param head   the head of the list over which to iterate.
 *  \param member the name of the llist_head within the struct pos.
 */
#define llist_for_each_entry_reverse(pos, head, member)			\
	for (pos = llist_entry((head)->prev, typeof(*pos), member),	\
		     prefetch(pos->member.prev);			\
	     &pos->member != (head); 					\
	     pos = llist_entry(pos->member.prev, typeof(*pos), member),	\
		     prefetch(pos->member.prev))

/*! Iterate over a linked list of a given type,
 *  continuing after an existing point.
 *  \param pos    the 'type *' to use as a loop counter.
 *  \param head   the head of the list over which to iterate.
 *  \param member the name of the llist_head within the struct pos.
 */
#define llist_for_each_entry_continue(pos, head, member) 		\
	for (pos = llist_entry(pos->member.next, typeof(*pos), member),	\
		     prefetch(pos->member.next);			\
	     &pos->member != (head);					\
	     pos = llist_entry(pos->member.next, typeof(*pos), member),	\
		     prefetch(pos->member.next))

/*! Iterate over llist of given type, safe against removal of
 *  non-consecutive(!) llist entries.
 *  \param pos    the 'type *' to use as a loop counter.
 *  \param n      another 'type *' to use as temporary storage.
 *  \param head   the head of the list over which to iterate.
 *  \param member the name of the llist_head within the struct pos.
 */
#define llist_for_each_entry_safe(pos, n, head, member)			\
	for (pos = llist_entry((head)->next, typeof(*pos), member),	\
		n = llist_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = llist_entry(n->member.next, typeof(*n), member))

/*! Iterate over an rcu-protected llist.
 *  \param pos  the llist_head to use as a loop counter.
 *  \param head the head of the list over which to iterate.
 */
#define llist_for_each_rcu(pos, head) \
	for (pos = (head)->next, prefetch(pos->next); pos != (head); \
		pos = pos->next, ({ smp_read_barrier_depends(); 0;}), prefetch(pos->next))

#define __llist_for_each_rcu(pos, head) \
	for (pos = (head)->next; pos != (head); \
		pos = pos->next, ({ smp_read_barrier_depends(); 0;}))

/*! Iterate over an rcu-protected llist, safe against removal of llist entry.
 *  \param pos  the llist_head to use as a loop counter.
 *  \param n    another llist_head to use as temporary storage.
 *  \param head the head of the list over which to iterate.
 */
#define llist_for_each_safe_rcu(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, ({ smp_read_barrier_depends(); 0;}), n = pos->next)

/*! Iterate over an rcu-protected llist of a given type.
 *  \param pos    the 'type *' to use as a loop counter.
 *  \param head   the head of the list over which to iterate.
 *  \param member the name of the llist_struct within the struct.
 */
#define llist_for_each_entry_rcu(pos, head, member)			\
	for (pos = llist_entry((head)->next, typeof(*pos), member),	\
		     prefetch(pos->member.next);			\
	     &pos->member != (head); 					\
	     pos = llist_entry(pos->member.next, typeof(*pos), member),	\
		     ({ smp_read_barrier_depends(); 0;}),		\
		     prefetch(pos->member.next))


/*! Iterate over an rcu-protected llist, continuing after existing point.
 *  \param pos  the llist_head to use as a loop counter.
 *  \param head the head of the list over which to iterate.
 */
#define llist_for_each_continue_rcu(pos, head) \
	for ((pos) = (pos)->next, prefetch((pos)->next); (pos) != (head); \
		(pos) = (pos)->next, ({ smp_read_barrier_depends(); 0;}), prefetch((pos)->next))

/*! Count number of llist items by iterating.
 *  \param head the llist head to count items of.
 *  \returns Number of items.
 *
 * This function is not efficient, mostly useful for small lists and non time
 * critical cases like unit tests.
 */
static inline unsigned int llist_count(const struct llist_head *head)
{
	struct llist_head *entry;
	unsigned int i = 0;
	llist_for_each(entry, head)
		i++;
	return i;
}

/*!
 *  @}
 */
