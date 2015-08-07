#include <malloc.h>
#include <string.h>
#include "list.h"

t_list __attribute__((visibility("hidden"))) *list_new(void) {
  t_list *list;

  if (!(list = malloc(sizeof(*list)))) {
    perror("malloc");
    return NULL;
  }
  return memset(list, 0, sizeof(*list));
}

int __attribute__((visibility("hidden"))) list_push_back(t_list *const list, void *const data) {
  t_node *node;

  if (!(node = malloc(sizeof(*node)))) {
    perror("malloc");
    return 0;
  }
  node->data = data;
  node->next = NULL;
  node->prev = list->tail;
  if (list->tail)
    list->tail->next = node;
  list->tail = node;
  if (!list->head)
    list->head = node;
  ++list->len;
  return 1;
}

void __attribute__((visibility("hidden"))) list_pop_search(t_list *const list, void *const data) {
  t_node *tmp;

  if (!list->cmp) {
    fprintf(stderr, "%s: hook_cmp not defined\n", __FUNCTION__);
    return;
  }
  tmp = list->head;
  while (tmp) {
    if (list->cmp(tmp->data, data)) {
      t_node *rm = tmp->next;
      if (tmp->prev)
	tmp->prev->next = tmp->next;
      else
	list->head = tmp->next;
      if (tmp->next)
	tmp->next->prev = tmp->prev;
      else
	list->tail = tmp->prev;
      if (list->pop)
	list->pop(tmp->data);
      free(tmp);
      tmp = rm;
    }
    else
      tmp = tmp->next;
  }
}

void __attribute__((visibility("hidden"))) list_empty(t_list *const list) {
  t_node *tmp = list->head;

  while (tmp) {
    if (list->pop)
      list->pop(tmp->data);
    t_node *rm = tmp;
    tmp = tmp->next;
    free(rm);
  }
  list->head = list->tail = NULL;
  list->len = 0;
}

void __attribute__((visibility("hidden"))) list_delete(t_list *const list) {
  list_empty(list);
  free(list);
}
