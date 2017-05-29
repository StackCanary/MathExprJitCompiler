#ifndef LIST_H
#define LIST_H

#include "lexer.h"
#include <stdlib.h>

typedef struct _list_t list_t;

struct _list_t {
	list_t *next;
	list_t *prev;	

	token_t *data;
};

list_t* create_list();
void free_list(list_t *list);
void add_to_list(list_t* list, token_t *data);
void print_list(list_t *list);

#endif 
