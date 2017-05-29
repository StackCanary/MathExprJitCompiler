#include "list.h"

list_t* create_list()
{
	list_t *list =  calloc(sizeof(list_t), 1);
	list->next = list;
	list->prev = list;
	list->data = NULL;  

	return list;
}

void free_list(list_t *list)
{
	list_t *conductor = list->next;

	while (conductor != list) 	
	{
		list_t *next = conductor->next;

		free(conductor->data);
		free(conductor);

		conductor = next;
	}

	free(list);
}

void print_list(list_t *list)
{
	list_t *conductor = list->next;

	while (conductor != list) 	
	{
		// printf("%s\n", token_to_string( *(conductor->data) );
		conductor = conductor->next;
	}

}

void add_to_list(list_t* list, token_t *data)
{
	list_t * node = create_list();
	node->data = data;

	node->prev = list->prev;
	list->prev->next = node;

	list->prev = node;
	node->next = list;

}

