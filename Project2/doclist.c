#include "doclist.h"
#include <stdio.h>
#include <stdlib.h>

struct docnode* doc_create(void) {
    struct docnode* doclist = malloc(sizeof(struct docnode));
    if(!doclist) {
        return NULL;
    }
    doclist->document = NULL;
    doclist->score = 0;
    doclist->next = NULL;
    doclist->flag = 0;
    return doclist;
}

void doc_add(struct docnode* head, char *document) {
    if(head->document == NULL) {
        head->document = document;
        head->flag = 0;
        head->score = 0;
        head->next = NULL;
        return;
    }
    struct docnode* behind = head;
    while(head != NULL) {
        behind = head;
        head = head->next;
    }
    behind->next = malloc(sizeof(struct docnode));
    if(!behind->next) {
        return;
    }
    behind->next->document = document;
    behind->next->next = NULL;
    behind->next->flag = 0;
    behind->next->score = 0;
}

/* prints out the order of the documents based on relevance */
void doc_print_order(struct docnode* head) {
    if(head == NULL) {
        return;
    }
    struct docnode* node = head;
    /* loops through all the nodes in the list */
    while(node != NULL) {
        double high = -1; /* keeps track of the most relevant document */
        struct docnode* iter = head; /* iter to loop through the other nodes and compare the relevance */
        struct docnode* placeHolder;
        while(iter != NULL) {
            /* makes sure the node hasn't been printed out already */
            if(iter->flag == 0) {
                if(iter->score > high) {
                    high = iter->score;
                    placeHolder = iter;
                }
            }
            iter = iter->next;
        }
        printf("%s: %f\n", placeHolder->document, placeHolder->score);
        placeHolder->flag = 1; /* set the flag for that doc because it was printed out */
        node = node->next;
    }
}

void doc_print(struct docnode* head) {
    while(head != NULL) {
        printf("%s: %f\n", head->document, head->score);
        head = head->next;
    }
}

void doc_delete(struct docnode* head) {
    if(head == NULL) {
        return;
    }
    while(head != NULL) {
        struct docnode* temp = head->next;
        free(head->document);
        free(head);
        head = temp;
    }
}