#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* allocates memory for a new hashmap */
struct hashmap* hm_create(int num_buckets, int num_documents) {
    /* allocates memory for the hashmap structure */
    struct hashmap* hm = malloc(sizeof(struct hashmap));
    if(!hm) {
        return NULL;
    }
    hm->num_buckets = num_buckets;
    hm->num_elements = 0;
    hm->num_documents = num_documents;
    /* allocates memory for the array of buckets that hold the nodes */
    hm->map = (struct llnode**) malloc(num_buckets*sizeof(struct llnode));
    if(!hm->map) {
        return NULL;
    }
    int i;
    /* loops through the number of buckets and allocates memory for the nodes in the buckets */
    for(i = 0; i < num_buckets; i++) {
        hm->map[i] =  malloc(sizeof(struct llnode));
        if(!hm->map[i]) {
            return NULL;
        }
        hm->map[i]->next = NULL;
        hm->map[i]->docptr = NULL;
        hm->map[i]->word = NULL;
    }
    return hm;
}

/* returns the value associates with the key that is passed in 
 * within the hashmap that is passed in. If it is not found, -1
 * is returned */
int hm_get(struct hashmap* hm, char* word, char* document_id) {
    int index = hash(hm, word);
    struct llnode *node = hm->map[index];
    /* if the word is null, return -1 because the word isn't there */
    if (node->word == NULL) {
        return -1;
    }
    /* loops through the linked list at the bucket */
    while(node != NULL) {
        /* check if the words are the same */
        if(strcmp(node->word, word) == 0) {
            /* Search through the word's doc list to try to find the docuement */
            struct lldoc* iter = node->docptr;
            while(iter != NULL) {
                if(strcmp(iter->document_id, document_id) == 0) {
                    return iter->num_occurrences; /* if they're both the same, return the number of occurances */
                }
                iter = iter->doc_next;
            }
            /* the word/document pair was not found in the list */
            return -1;
        }
        node = node->next; /* move to the next item in the list */
    }
    /* otherwise, we reached the end of the list and the word was not found */
    return -1;
}

struct llnode* hm_get_word(struct hashmap* hm, char *word) {
    int index = hash(hm, word);
    struct llnode *node = hm->map[index];
    /* if the word is null, return -1 because the word isn't there */
    if (node->word == NULL) {
        return NULL;
    }
    /* loops through the linked list at the bucket */
    while(node != NULL) {
        /* check if the words and the document are the same */
        if(strcmp(node->word, word) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

/* puts the key value pair into the hashmap that is passed in. If the word and 
 * document ID combination already exist, its value is overriden with the new one */
void hm_put(struct hashmap* hm, char* word, char* document_id, int num_occurrences) {
    /* gets the bucket to put the word into */
    int index = hash(hm, word);
    struct llnode *node = hm->map[index];
    /* if there is no word in the bucket, a new one is put there */
    if(node->word == NULL) {
        node->docptr = malloc(sizeof(struct lldoc));
        if(!node->docptr) {
            return;
        }
        node->word = word;
        node->df_score = 1; /* appears in 1 document so far, so df is 1 */
        node->docptr->document_id = document_id;
        node->docptr->num_occurrences = num_occurrences;
        node->docptr->doc_next = NULL;
        hm->num_elements++;
        node->next = NULL;
        return;
    }

    struct llnode *behind = node; /* pointer that is 1 item behind the iter node */
    while(node != NULL) {
        /* if the word is found in the list */
        if(strcmp(node->word, word) == 0) {
            struct lldoc* iter = node->docptr;
            struct lldoc* doc_behind = iter;
            /* need to loop through the document list for the word */
            while(iter != NULL) {
                if(strcmp(iter->document_id, document_id) == 0) {
                    /* free the word/document passed in because a pair already exists */
                    free(document_id);
                    free(word);
                    iter->num_occurrences++; /* if they're both the same, increase the number of occurances */
                    return;
                }
                doc_behind = iter;
                iter = iter->doc_next;
            }
            /* the word was in the list but not the document */
            struct lldoc* newDoc = malloc(sizeof(struct lldoc));
            if(!newDoc) {
                return;
            }
            free(word); /* free the memory used for the word that was passed in because we don't need to use it */
            node->df_score++;
            newDoc->document_id = document_id;
            newDoc->num_occurrences = 1;
            newDoc->doc_next = NULL;
            doc_behind->doc_next = newDoc;
            return;
        }
        behind = node;
        node = node->next;
    }
    /* if the word was not found in the list, it is added at the end */
    struct llnode *newNode = malloc(sizeof(struct llnode));
    if(!newNode) {
        return;
    }
    newNode->docptr = malloc(sizeof(struct lldoc));
    if(!newNode->docptr) {
        return;
    }
    newNode->word = word;
    newNode->df_score = 1;
    newNode->docptr->document_id = document_id;
    newNode->docptr->num_occurrences = num_occurrences;
    newNode->docptr->doc_next = NULL;
    hm->num_elements++;
    newNode->next = NULL;
    behind->next = newNode;
}

/* removes the key value pair in the hashmap */
void hm_remove(struct hashmap* hm, char* word) {
    int index = hash(hm, word);
    int count = 0;
    struct llnode *node = hm->map[index]; 
    /* if the word is null, the element was not in the bucket */
    if(node->word == NULL) {
        printf("%s  was not found in the map\n", word);
        return;
    }
    struct llnode *behind = node;
    /* loops through the linked list to search for the element */
    while(node != NULL) {
        /* if the element was found, free it and the doc list */
        if(strcmp(node->word, word) == 0) {
            /* could be 3 cases: the node is the head node, in the middle, or at the end */
            if(count == 0) {
                /* if the node is the head node and the only node in the bucket */
                if(node->next == NULL) {
                    free(node->word);
                    free(node);
                    node = NULL;
                    hm->num_elements--;
                    return;
                }
                /* the bucket is set to the next node */
                hm->map[index] = node->next;
            }
            /* if the node is inbetween 2 other nodes */
            else if(node->next != NULL) {
                /* move the pointer around the current node and remove the pointers to it */
                behind->next = node->next;
                node->next = NULL;
            }
            /* otherwise its at the end */
            else {
                behind->next = NULL; /* set the pointer to this node to NULL to remove it from the list */
            }
            /* free the memory used for the node, including the list of documents */
            struct lldoc* iter = node->docptr;
            while(iter != NULL) {
                /* need a temp because we are freeing iter */
                struct lldoc* temp = iter->doc_next;
                free(iter->document_id);
                free(iter);
                iter = temp;
            }
            /* then, free the node word and the node itself */ 
            free(node->word);
            free(node);
            node = NULL;
            hm->num_elements--;
            return;
        }
        behind = node;
        node = node->next;
        count++;
    }
    /* the element was not found in the list */
    printf("%s was not found in the map\n", word);
}

void printList(struct hashmap* hm) {
        int i;
        for(i = 0; i < hm->num_buckets; i++) {
                struct llnode *currentNode = hm->map[i];
                printf("bucket %d: \n", i);
                if(currentNode->word == NULL) {
                        continue; /* if there's nothing in the bucket, continute to the next bucket */
                }
                /* prints out all the elements in the list at the current bucket */
                while(currentNode != NULL) {
                        printf("   '%s' : df = %d\n", currentNode->word, currentNode->df_score);
                        struct lldoc* iter = currentNode->docptr;
                        while(iter != NULL) {
                                printf("\t%s: %d\n", iter->document_id, iter->num_occurrences);
                                iter = iter->doc_next;
                        }
                        currentNode = currentNode->next;
                }
        }
}

/* deallocates the hashmap and all of its elements */
void hm_destroy(struct hashmap* hm) {
    int i;
    /* loops through all the buckets to free their contents */
    for (i = 0; i < hm->num_buckets; i++) {
        struct llnode *iter = hm->map[i];
        /* loops through the linked list */
        while(iter != NULL) {
            /* need a temp because we are going to free each node */
            struct llnode *temp = iter->next;
            /* need to free the document list for each word */
            struct lldoc* ptr = iter->docptr;
            while(ptr != NULL) {
                struct lldoc* temp2 = ptr->doc_next;
                free(ptr->document_id);
                free(ptr);
                ptr = temp2;
            }
            /* free the word and document id */
            free(iter->word);
            free(iter); /* free the node itself */
            iter = temp;
        }
    }
    free(hm->map);
    free(hm);
}

/* takes the given word and document ID and maps them to a bucket in the hashmap.
 * (easy way to do this is to sum the ASCII codes of all the characters and then
 * modulo by the number of buckets) */
int hash(struct hashmap* hm, char* word) {
    int len = strlen(word);
    int ascii_sum = 0;
    int i;
    /* loops through all the character in the word */
    for(i = 0; i < len; i++) {
        ascii_sum += word[i]; /* adds their ascii value to sum */
    }
    /* the bucket is equal to the sum modulo the number of buckets */
    int bucket = (int) ascii_sum % (hm->num_buckets);
    return bucket;
}
