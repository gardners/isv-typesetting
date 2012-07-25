/* Read ISV MS Office Open XML document and generate OSIS file */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "mxml.h"
#include "s_list.h" /* linked list for strings */
#include "isv_docx.h"

typedef struct {
    int capture;
    s_list_t *first;
    s_list_t *smart_tags;
} state_t;

#define num_attrs(node) (((mxml_element_t)(node->value))->num_attrs)
#define attrs(node) (((mxml_element_t)(node->value))->attrs)

static void check_doc(mxml_node_t *node, mxml_sax_event_t event, void *data);
void set_smart_tags(mxml_node_t *node, s_list_t **tags);
void move_before_smart_tags(mxml_node_t *node);

static int path_match(s_list_t *list, const char *path);
static int path_match_any(s_list_t *list, const char **paths);

static void extract_p(mxml_node_t *p);

int main(void) {
    mxml_node_t *doc, *node;
    state_t state = {0};

    doc = mxmlSAXLoadFile(NULL, stdin, MXML_OPAQUE_CALLBACK, check_doc, &state);

    mxmlSaveFile(doc, stdout, MXML_NO_CALLBACK);
    return 0;

    node = mxmlFindPath(doc, "w:document/w:body");

    if (!node) errx(1, "No body element found.");

    do {
        const char *element;

        if (mxmlGetType(node) != MXML_ELEMENT) continue;

        element = mxmlGetElement(node);

        if (strcmp(element, "w:p") == 0) {
            extract_p(node);
        } else if (strcmp(element, "w:bookmarkEnd") == 0
                || strcmp(element, "w:sectPr") == 0) {
            /* ignore */
        } else {
            warnx("Unknown element in body: %s", element);
        }

    } while ((node = mxmlGetNextSibling(node)));
    return 0;
}

static void extract_p(mxml_node_t *p) {
    mxml_node_t *child;

    child = mxmlGetFirstChild(p);

    do {
        if (mxmlGetType(child) != MXML_ELEMENT) continue;

        if (strcmp(mxmlGetElement(child), "w:pPr") == 0) {
        }
    } while (0);
}

static void check_doc(mxml_node_t *node, mxml_sax_event_t event, void *data) {
    s_list_t **list, **tags, *link;
    state_t *state;
    const char *element;
    char *tag;

    state = (state_t *) data;
    if (!state) errx(1, "NULL state.");
    list = &(state->first);
    tags = &(state->smart_tags);

    switch (event) {
        case MXML_SAX_CDATA:
            errx(1, "CDATA not handled at this stage.");
            break;

        case MXML_SAX_COMMENT:
        case MXML_SAX_DIRECTIVE:
            break;

        case MXML_SAX_DATA:
            if (state->capture) {
                mxmlRetain(node);
            }
            break;

        case MXML_SAX_ELEMENT_OPEN:
            element = mxmlGetElement(node);

            /* handle smart tags */
            if (strcmp(element, "w:smartTag") == 0) {
                s_list_push(tags, mxmlElementGetAttr(node, "w:element"));
                break;
            } else if (*tags) set_smart_tags(node, tags);

            s_list_append(list, element);

            if (path_match_any(state->first, keep_paths)) {
                /* important node */
                mxmlRetain(node);
                /* should we capture text inside this? */
                if (strcmp(element, "w:t") == 0) state->capture = 1;

            } else if (!path_match_any(state->first, other_valid_paths)) {
                /* unrecognised node */
                fprintf(stderr, "Unknown element: ");
                for (link = state->first; link; link = link->next) {
                    fprintf(stderr, "/%s", link->s);
                }
                fprintf(stderr, "\n");
            }
            break;

        case MXML_SAX_ELEMENT_CLOSE:
            element = mxmlGetElement(node);
            /* close smart tag */
            if (strcmp(element, "w:smartTag") == 0
                    && s_list_pop(tags, &tag)) {
                free(tag);
                break;
            } else if (*tags) move_before_smart_tags(node);

            if (!*list) errx(1, "Too many elements closed (%s)?", element);

            list = s_list_end(list);
            if (strcmp(element, (*list)->s) != 0) {
                errx(1, "Element (%s) closed before (%s).",
                        element, (*list)->s);
            }

            if (strcmp(element, "w:t") == 0) state->capture = 0;

            s_list_drop(list);
            break;

        default:
            errx(1, "Unknown SAX event.");
            break;
    }
}

static int path_match(s_list_t *list, const char *path) {
    const char *slash, *element;
    int len;

    slash = strchr(path, '/');
    if (slash) {
        len = slash-path;
        slash += 1;
    } else {
        len = strlen(path);
    }

    if (len == 2 && strncmp(path, "**", len) == 0) {
        /* match any number of elements */
        if (!slash) return 1; /* everything matches from here */
        /* try matching none, then progressively more elements */
        do {
            if (path_match(list, slash)) return 1;
            if (list) list = list->next;
        } while (list);
        return 0;
    }

    if (len == 1 && strncmp(path, "*", len) == 0) {
        /* match exactly one element */
        if (!list) return 0;
        if (slash) {
            return path_match(list->next, slash);
        } else if (list->next) {
            return 0;
        }
        return 1;
    }

    /* plain text match */
    if (!list) {
        /* end of element list - only match if path is empty */
        if (*path == '\0') return 1;
        return 0;
    }
    if (list->s) {
        element = list->s;
    } else {
        element = "";
    }
    if (len != strlen(element)) return 0;
    if (strncmp(element, path, len)) return 0;
    if (!slash) {
        if (list->next) return 0;
        return 1;
    }
    return path_match(list->next, slash);
}

static int path_match_any(s_list_t *list, const char **paths) {
    int i;
    for (i = 0; paths[i]; i++) {
        if (path_match(list, paths[i])) {
            return 1;
        }
    }
    return 0;
}

void set_smart_tags(mxml_node_t *node, s_list_t **tags) {
    s_list_t *link;
    char *tag;
    int len = 0;

    for (link = *tags; link; link = link->next) {
        if (!link->s || !*(link->s)) continue;
        if (len) len++;
        len += strlen(link->s);
    }
    if (!len) return;

    tag = malloc(len+1);
    if (!tag) err(1, "malloc");
    *tag = '\0';

    for (link = *tags; link; link = link->next) {
        if (!link->s || !*(link->s)) continue;
        if (*tag != '\0') strcat(tag, ",");
        strcat(tag, link->s);
    }

    mxmlElementSetAttr(node, "osis:tags", tag);
}

void move_before_smart_tags(mxml_node_t *node) {
    mxml_node_t *parent, *last;
    if (!node) return;

    /* find the highest smartTag ancestor and its parent
     *  body   -> smartTag -> smartTag -> node
     *   ||          ||
     * parent       last */
    parent = NULL;
    do {
        last = parent;
        if (!parent) parent = node;
        parent = mxmlGetParent(parent);
        if (!parent) errx(1, "Hit root node while climbing smart tags");
    } while(mxmlGetType(parent) == MXML_ELEMENT
            && strcmp(mxmlGetElement(parent), "w:smartTag") == 0);

    if (!last) return; /* no smartTag ancestors */

    mxmlRemove(node); /* remove node from current position */
    mxmlAdd(parent, MXML_ADD_BEFORE, last, node);
}
