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
    printf("printing documents in order of relevance: \n");
    struct docnode* node = head;
    int size = 0;
    int count = 0;
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
        /* only print out the document if it is somewhat relevant (i.e. not a score of 0) */
        if(placeHolder->score != 0) {
            printf("%s: %f\n", placeHolder->document, placeHolder->score);
        }
        else {
            count++;
        }
        placeHolder->flag = 1; /* set the flag for that doc because it was printed out */
        node = node->next;
        size++;
    }
    if(count == size) {
        printf("none of the entered words appeared in the documents\n");
    }
}

void doc_print(struct docnode* head) {
    while(head != NULL) {
        printf("%s: %f\n", head->document, head->score);
        head = head->next;
    }
}

/* resets the values such as score and flag for the documents */
void doc_reset(struct docnode* head) {
    if(head == NULL) {
        return;
    }

    while(head != NULL) {
        head->flag = 0;
        head->score = 0;
        head = head->next;
    }
}

void doc_delete(struct docnode* head) {
    if(head == NULL) {
        return;
    }
    /* frees the memory for the lsit */
    while(head != NULL) {
        struct docnode* temp = head->next;
        free(head->document);
        head->document = NULL;
        free(head);
        head = NULL;
        head = temp;
    }
}