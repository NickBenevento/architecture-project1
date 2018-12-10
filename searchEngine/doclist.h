#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct docnode {
    int flag;
    char *document;
    double score;
    struct docnode* next;
};

struct docnode* doc_create(void);
void doc_add(struct docnode* head, char *document);
void doc_destroy(struct docnode* head);
void doc_print_order(struct docnode* head);
void doc_print(struct docnode* head);
void doc_reset(struct docnode* head);
void doc_delete(struct docnode* head);

#endif