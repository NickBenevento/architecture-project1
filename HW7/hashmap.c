#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* allocates memory for a new hashmap */
struct hashmap* hm_create(int num_buckets) {
    /* allocates memory for the hashmap structure */
    struct hashmap* hm = malloc(sizeof(struct hashmap));
    hm->num_buckets = num_buckets;
    /* allocates memory for the array of buckets that hold the nodes */
    hm->map = (struct llnode**) malloc(num_buckets*sizeof(struct llnode));
    int i;
    /* loops through the number of buckets and allocates memory for the nodes in the buckets */
    for(i = 0; i < num_buckets; i++) {
        hm->map[i] =  malloc(sizeof(struct llnode));
        hm->map[i]->next = NULL;
    }
    return hm;
}

/* returns the value associates with the key that is passed in 
 * within the hashmap that is passed in. If it is not found, -1
 * is returned */
int hm_get(struct hashmap* hm, char* word, char* document_id) {
    int index = hash(hm, word, document_id);
    struct llnode *node = hm->map[index];
    /* if the word is null, return -1 because the word isn't there */
    if (node->word == NULL) {
        return -1;
    }
    /* loops through the linked list at the bucket */
    while(node != NULL) {
        /* check if the words and the document are the same */
        if(strcmp(node->word, word) == 0 && strcmp(node->document_id, document_id) == 0) { 
            return node->num_occurrences; /* if they're both the same, return the number of occurances */
        }
        node = node->next; /* move to the next item in the list */
    }
    /* otherwise, we reached the end of the list and the word was not found */
    return -1;
}

/* puts the key value pair into the hashmap that is passed in. If the word and 
 * document ID combination already exist, its value is overriden with the new one */
void hm_put(struct hashmap* hm, char* word, char* document_id, int num_occurrences) {
    /* gets the bucket to put the word into */
    int index = hash(hm, word, document_id);
    struct llnode *node = hm->map[index];
    /* if there is no word in the bucket, a new one is put there */
    if(node->word == NULL) {
        node->word = word;
        node->document_id = document_id;
        node->num_occurrences = num_occurrences;
        node->next = NULL;
        return;
    }

    int count = 0;
    struct llnode *behind = node; /* pointer that is 1 item behind the iter node */
    while(node != NULL) {
        /* if the word/document pair is already found in the list, it is overriden with the new value */
        if(strcmp(node->word, word) == 0 && strcmp(node->document_id, document_id) == 0) {
            node->num_occurrences = num_occurrences;
            return;
        }
        node = node->next;
        /* check to make sure 'behind' is 1 element behind 'node' */
        if(count != 0) {
            behind = behind->next;
        }
        count++;
    }
    /* if the word was not found in the list, it is added at the end */
    struct llnode *newNode = malloc(sizeof(struct llnode));
    newNode->word = word;
    newNode->document_id = document_id;
    newNode->num_occurrences = num_occurrences;
    newNode->next = NULL;
    behind->next = newNode;
}

/* removes the key value pair in the hashmap */
void hm_remove(struct hashmap* hm, char* word, char* document_id) {
    int index = hash(hm, word, document_id);
    struct llnode *node = hm->map[index]; 
    /* if the word is null, the element was not in the bucket */
    if(node->word == NULL) {
        printf("%s, %s  was not found in the map\n", word, document_id);
        return;
    }
    int count = 0;
    struct llnode *behind = node;
    /* loops through the linked list to search for the element */
    while(node != NULL) {
        /* if the element was found, free it */
        if(strcmp(node->word, word) == 0 && strcmp(node->document_id, document_id) == 0) {
            /* could be 3 cases: the node is the head node, in the middle, or at the end */
            if(count == 0) {
                
                /* if it's the head, set the bucket equal to the next node */
                
                
                if(node->next == NULL) {
                    node->next = malloc(sizeof(struct llnode));
                }
                hm->map[index] = node->next;
                node->next = NULL;
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
            /* free the memory used for the node */
            free(node->word);
            free(node->document_id);
            free(node);
            return;
        }
        node = node->next;
        /* ensures that 'behind' is 1 node behind the iter node */
        if(count != 0) {
            behind = behind->next;
        }
        count++;
    }
    /* the element was not found in the list */
    printf("%s, %s  was not found in the map\n", word, document_id);
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
            /* free the word and document id */
            free(iter->word);
            free(iter->document_id);
            free(iter); /* free the node itself */
            iter = temp;
        }
    }
    free(hm);
}

/* takes the given word and document ID and maps them to a bucket in the hashmap.
 * (easy way to do this is to sum the ASCII codes of all the characters and then
 * modulo by the number of buckets) */
int hash(struct hashmap* hm, char* word, char* document_id) {
    int wordLen = strlen(word);
    int documentLen = strlen(document_id);
    int i;
    int ascii_sum = 0;
    int bucket = -1;
    /* loops through all the character in the word */
    for(i = 0; i < wordLen; i++) {
        ascii_sum += word[i]; /* adds their ascii value to sum */
    }
    for(i = 0; i < documentLen; i++) {
        //ascii_sum += document_id[i];
    }
    /* the bucket is equal to the sum modulo the number of buckets */
    bucket = (int) ascii_sum % (hm->num_buckets);
    return bucket;
}
