/*
 * linked_list.h
 *
 *  Created on: Apr 3, 2014
 *      Author: fanl
 */

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stddef.h>

#ifndef container_of
#define container_of(ptr, type, member) ({ \
                const typeof( ((type *)0)->member ) *__mptr = (ptr); \
                (type *)( (char *)__mptr - offsetof(type,member) );})
#endif

struct s_linked_list_head
{
    struct s_linked_list_head * next;
};

typedef struct s_linked_list_head linked_list_t;


/**
 * Initialize a linked_list_t within structure
 * @param
 */
#define linked_list_init(structure, field) \
({ \
        typeof(structure) ___str = structure; \
        if (___str) {___str->field.next = (linked_list_t *)0;} \
})

/**
 * Get the next node.
 * @param ptr_to_current
 * @return Pointer to the next node.
 */
#define linked_list_next(ptr_to_current, field) \
({ \
        typeof(ptr_to_current) ___ret = ptr_to_current; linked_list_t * ___head;\
        if (___ret) \
        { \
           ___head = ___ret->field.next; \
           if (___head) { ___ret  = container_of(___head, typeof(*(ptr_to_current)), field); }\
           else {___ret = 0; } \
        } \
        ___ret; \
})

/**
 * Get the last node of the list, including ptr_to_current.
 *
 * @param ptr_to_current
 * @return Pointer to the last node of the list, including ptr_to_current.
 */
#define linked_list_last(ptr_to_current, field) \
({ \
        typeof(ptr_to_current) ___ret = ptr_to_current; linked_list_t * ___head;\
        if (___ret) \
        { \
            ___head = &___ret->field; \
            if (___head->next) { while (___head->next) ___head = ___head->next;} \
            if (___head) { ___ret  = container_of(___head, typeof(*(ptr_to_current)), field); } \
            else {___ret = 0; } \
        } \
        ___ret; \
})

/**
 * Insert new node between current and current's next.
 *
 * @param ptr_to_current
 * @param ptr_to_new
 */
#define linked_list_insert_after(ptr_to_current, ptr_to_new, field) \
({ \
        typeof(ptr_to_current) ___curr = ptr_to_current; \
        typeof(ptr_to_new) ___new = ptr_to_new; \
        if (___curr && ___new) \
        { \
            ___new->field.next = ___curr->field.next; \
            ___curr->field.next = &___new->field; \
        } \
})

/**
 * Remove the next node counting from the current and return it.
 *
 * @param current
 * @return Return the removed node.
 */
#define linked_list_remove_next(ptr_to_current, field) \
({ \
        typeof(ptr_to_current) ___cur = ptr_to_current; \
        typeof(ptr_to_current) ___ret = 0; \
        linked_list_t * ___head; \
        if (___cur) \
        { \
           ___head = ___cur->field.next; \
           ___cur->field.next =  ___head->next; \
           if (___head) { ___ret  = container_of(___head, typeof(*(ptr_to_current)), field); } \
        } \
        ___ret; \
})

/**
 * Free all list members, counting from ptr_to_current.
 *
 * @param ptr_to_current
 *
 * @warning
 *      Any node pointing to ptr_to_corrent will have an invalid
 *      pointer.
 */
#define linked_list_free(ptr_to_current, field) \
({ \
        typeof(ptr_to_current) ___item = ptr_to_current; \
        typeof(ptr_to_current) ___item_to_delete; \
        while (___item) \
        { \
            ___item_to_delete = ___item; \
            ___item = linked_list_next(___item_to_delete, field); \
            free (___item_to_delete); \
        } \
})

/**
 * Free all list members, counting from ptr_to_current,
 * but instead calling free itself, this function calls
 * user free function passed on free_callback.
 *
 * @param ptr_to_current
 *
 * @warning
 *      Any node pointing to ptr_to_corrent will have an invalid
 *      pointer.
 */
#define linked_list_free_cb(ptr_to_current, field, free_callback) \
({ \
        typeof(ptr_to_current) ___item = ptr_to_current; \
        typeof(ptr_to_current) ___item_to_delete; \
        while (___item) \
        { \
            ___item_to_delete = ___item; \
            ___item = linked_list_next(___item_to_delete, field); \
            free_callback(___item_to_delete); \
        } \
})

/**
 * Free all list members, counting from ptr_to_current,
 * but instead calling free itself, this function calls
 * user free function passed on free_callback.
 *
 * @param ptr_to_current
 *
 * @warning
 *      Any node pointing to ptr_to_corrent will have an invalid
 *      pointer.
 */
#define linked_list_find(ptr_to_current, field, comparator_func, comparator_seed) \
({ \
        typeof(ptr_to_current) ___item = ptr_to_current; \
        while (___item) \
        { \
        	if (comparator_func(comparator_seed, ___item) == true) break; \
            ___item = linked_list_next(___item, field); \
        } \
        ___item; \
})

#endif /* LINKED_LIST_H_ */
