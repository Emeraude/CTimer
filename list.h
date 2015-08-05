#pragma once

typedef struct	s_node {
  void		*data;
  struct s_node	*next;
  struct s_node	*prev;
}		t_node;

typedef struct	s_list {
  int		len;
  t_node	*head;
  t_node	*tail;
  int		(*cmp)(void const *const, void const *const);
  void		(*pop)(void *const);
}		t_list;

t_list *list_new(void);
int list_push_back(t_list *const list, void *const data);
void list_pop_search(t_list *const list, void *const data);
void list_empty(t_list *const list);
void list_delete(t_list *const list);
