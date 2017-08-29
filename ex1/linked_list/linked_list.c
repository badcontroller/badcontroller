#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

list_t list_create()
{
    list_t l = malloc(sizeof(struct list));
	// consider using memset
	l->head = NULL;
	l->tail = NULL;
	l->length = 0;
	return l;
}

void list_delete(list_t list)
{
    node* i = list->head;
    node* j;
    while(i != NULL) {
         j = i->next;
         free(i);
         i = j;        
    }
    free(list);
}

void list_insert(list_t list, int index, int data)
{
    if (index == list->length) {
        list_append(list, data);
        return;
    }

    list->length++;
    node* new_node = malloc(sizeof(node));
    new_node->data = data;
        
    // handle empty list
    if (index == 0) {
        list->head->prev = new_node;
        new_node->next = list->head;
        list->head = new_node;
        new_node->prev = NULL;
        return;
    }            
   
    node* n = list->head; 
    int i;
    for (i = 0; i < index-1; ++i) {
            n = n->next;
    }
    new_node->next = n->next;
    new_node->prev = n;
    n->next->prev = new_node;
    n->next = new_node;
}

void list_append(list_t list, int data)
{

    node* new_node = malloc(sizeof(node));
    new_node->data = data;
    
	if (list->length == 0) {
		list->head = new_node;
		list->tail = new_node;
		list->length++;
		return;
	}
	
    list->tail->next = new_node;
    new_node->next = NULL;
    new_node->prev = list->tail;
    list->tail = new_node;
    list->length++;
}

void list_print(list_t list)
{
    node *i = list->head;
    while(i != NULL){
        printf("%d ", i->data);
        i = i->next;
    }
    printf("\n");
}

long list_sum(list_t list)
{
    long sum = 0;
    node* n = list->head;
    while(n != NULL){
        sum += n->data;
        n = n->next;
    }
    return sum;
}

int list_get(list_t list, int index)
{
    node* n = list->head; 
    
    int i;
    for (i = 0; i < index; ++i) {
            n = n->next;
    }
    return n->data;
}

int list_extract(list_t list, int index)
{
	int val;
	if (list->length == 1) {
		val = list->head->data;
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		list->length = 0;	
	}
	else if (index == 0) {
		val = list->head->data;
		node* n = list->head->next;
		n->prev = NULL;
		free(list->head);
		list->head = n;
		list->length--;
	}
	
	else if (index == list->length) {
		val = list->head->data;
		node *n1 = list->tail->prev;
		n1->next = NULL;
		free(list->tail);
		list->tail = n1;
		list->length--;
	}
	else {
		node* n = list->head; 
		int i;
		for (i = 0; i < index; ++i) {
		        n = n->next;
		}
		val = n->data;
		n->prev->next = n->next;
		n->next->prev = n->prev;
		free(n);
		list->length--;
	}
	return val;
}
