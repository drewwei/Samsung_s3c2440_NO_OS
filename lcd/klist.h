#ifndef __KLIST
#define __KLIST

struct list_head {
	struct list_head *next, *prev;
};

static inline void INIT_LIST_HEAD(struct list_head *list)	/* inline函数前最好要加static 防止重复定义 */
{
	list->next = list;
	list->prev = list;
}


#define list_for_each(pos, head) \
	for(pos = (head)->next; pos != (head); pos = pos->next)
	

static inline void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next)
{
	next->prev = new;
	new->next  = next;
	prev->next = new;
	new->prev  = prev;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

#endif