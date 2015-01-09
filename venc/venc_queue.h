
#ifndef _VENC_QUEUE_H_
#define _VENC_QUEUE_H_
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

struct my_struct
{
  //int num;
  int bufferIndex;
  int handleIndex;  
  struct my_struct* next;
};

struct my_list
{
  struct my_struct* head;
  struct my_struct* tail;
};

struct my_list* list_add_element( struct my_list*, const int,const int);
struct my_list* list_remove_element_and_get_removed_elem( struct my_list* s, struct my_struct* pt);
struct my_list* list_remove_element( struct my_list*);
int is_exist_elem(const struct my_list* ps, const struct my_struct* pSearch);
struct my_list* list_new(void);
struct my_list* list_free( struct my_list* );
void list_print( const struct my_list* );
void list_print_element(const struct my_struct* );
#endif
