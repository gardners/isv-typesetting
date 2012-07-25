#include <err.h>
#include <stdlib.h>
#include <string.h>
#include "s_list.h"

void s_list_append(s_list_t **list, const char *str) {
    char *s = NULL;
    if (!list) errx(1, "s_list_append: list is null.");
    if (str) {
        s = strdup(str);
        if (!s) err(1, "strdup");
    }
    while (*list) list = &((*list)->next);
    *list = malloc(sizeof(s_list_t));
    if (!*list) err(1, "malloc");
    (*list)->s = s;
    (*list)->next = NULL;
}

s_list_t **s_list_end(s_list_t **list) {
    if (!list) errx(1, "s_list_end: list is null.");
    while ((*list)->next) list = &((*list)->next);
    return list;
}

void s_list_drop(s_list_t **list) {
    if (!list) errx(1, "s_list_drop: list is null.");
    list = s_list_end(list);
    if ((*list)->s) free((*list)->s);
    free(*list);
    *list = NULL;
}

void s_list_push(s_list_t **list, const char *str) {
    char *s = NULL;
    s_list_t *new;
    if (!list) errx(1, "s_list_push: list is null.");
    new = malloc(sizeof(s_list_t));
    if (!new) err(1, "malloc");
    if (str) {
        s = strdup(str);
        if (!s) err(1, "strdup");
    }
    new->s = s;
    new->next = *list;
    *list = new;
}

/* must be free()'d after if not NULL */
int s_list_pop(s_list_t **list, char **str) {
    if (!list) errx(1, "s_list_push: list is null.");
    if (!*list) {
        *str = NULL;
        return 0; /* no more items in list */
    }
    *str = (*list)->s;
    *list = (*list)->next;
    return 1;
}
