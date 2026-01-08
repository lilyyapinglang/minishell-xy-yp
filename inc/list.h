#ifndef MS_LIST_H
# define MS_LIST_H

// typedef struct s_list
// {
//     void            *content;
//     struct s_list   *next;
// }   t_list;

typedef struct s_list
{
	void *content;       // user-owned data
	struct s_list *next; // next node
	struct s_list *prev; // previous node
}							t_list;
#endif