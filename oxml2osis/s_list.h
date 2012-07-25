#ifndef _s_list_h_
#define _s_list_h_

typedef struct s_list {
    char *s;
    struct s_list *next;
} s_list_t;

void s_list_append(s_list_t **list, const char *str);
void s_list_drop(s_list_t **list);
s_list_t **s_list_end(s_list_t **list);
void s_list_push(s_list_t **list, const char *str);
int s_list_pop(s_list_t **list, char **str);

#endif /* _s_list_h_ */
