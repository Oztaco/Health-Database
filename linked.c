
/* linked - linked list functions
*
* Implemented by Efe Ozturkoglu
*
*/

#include <stdio.h>
#include <stdlib.h>
#include "linked.h"
	
static struct node* head = NULL;

/************************************************************
 length - return length of a list
 ************************************************************/
int length()
{
  int counter = 0;
  struct node *node_ptr = NULL;

  if (!head)
    return 0;
  node_ptr = head;
  do
    counter++;
  while
    (node_ptr = node_ptr->next);
  return counter;
}


/************************************************************
 push - add new node at beginning of list
 ************************************************************/
void push(int data)
{
  struct node *data_ptr = NULL;

  data_ptr = malloc(sizeof(struct node));
  data_ptr->data = data;
  data_ptr->next = NULL;
  if (length() == 0) {
    head = data_ptr;
  }
  else {
    data_ptr->next = head;
    head = data_ptr;
  }
}

/************************************************************
 pop - delete node at beginning of non-empty list and return its data
 ************************************************************/
int pop()
{
  int data = -1;
  struct node *node_ptr;

  if (!head) return -1;
  data = head->data;
  node_ptr = head;
  head = head->next;
  free(node_ptr);
  return data;
}

/************************************************************
 appendNode - add new node at end of list
 ************************************************************/
void appendNode(int data)
{
  struct node *last = NULL;
  struct node *data_ptr = NULL;

  data_ptr = malloc(sizeof(struct node));
  data_ptr->data = data;
  data_ptr->next = NULL;
  last = getLastNode();
  last->next = data_ptr;
}

struct node *getLastNode() {
  struct node *node_ptr = NULL;

  node_ptr = head;
  while (node_ptr && node_ptr->next && // Check if 'next' is null
    (node_ptr = node_ptr->next)); // This is my favorite line of code I've ever written
  return node_ptr;
}

struct node *getNodeAt(int index) {
   struct node *node_ptr = NULL;
   int counter = 0;

  node_ptr = head;
  while (node_ptr && node_ptr->next &&
    (counter < index) &&
    (node_ptr = node_ptr->next)) 
    {counter++;}
  return node_ptr;
}
void setNodeAt(int index, int value) {
  struct node *node_ptr = NULL;
  int counter = 0;

  node_ptr = head;
    while (node_ptr && node_ptr->next &&
    (counter < index) &&
    (node_ptr = node_ptr->next)) 
    {counter++;}
  node_ptr->data = value;
}

/************************************************************
 copyList - return new copy of list
 ************************************************************/
struct node *copyList()
{
  struct node *current_ptr = NULL; // 
  struct node *new_elm_ptr = NULL;
  struct node *new_list_ptr = NULL;
  struct node *previous_ptr = NULL;

  if (!head) return NULL;

  new_list_ptr = malloc(sizeof(struct node));
  new_list_ptr->data = head->data;
  new_list_ptr->next = NULL;
  previous_ptr = new_list_ptr;

  current_ptr = head;

  while ((current_ptr = (current_ptr->next)) != NULL) {
    new_elm_ptr = malloc(sizeof(struct node));
    new_elm_ptr->data = current_ptr->data;
    new_elm_ptr->next = NULL;
    previous_ptr->next = new_elm_ptr;
    previous_ptr = new_elm_ptr;
  }

  return new_list_ptr;
}

/************************************************************
 freeList - release all memory you allocated for the linked list.
 NOTE -- add a unit test that calls this function and observes
 that you returned 1 for success on freeing all memory.
 The instructor will verify that you correctly freed all
 allocated memory.
 ************************************************************/
int freeList() {
  struct node *next = NULL;

  next = head->next;
  free(head);

  while (head = next) {
    next = head->next;
    free(head);
  }

  return 1 ;
}

void printlist(struct node *list_head) {
  struct node *next = list_head;
  int i = 0;

  printf("List values:\n");
  while (next) {
	  printf("list[%i] = %i;\n", i, next->data);
    i++;
    next = next->next;
  }
}

