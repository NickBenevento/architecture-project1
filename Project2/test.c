#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "array_list.h"
#include <string.h>

void printList(struct hashmap* hm);
void training(struct hashmap *hm);
void read_query(char *query, struct array_list *list);

int main(void) {
        struct array_list *list = al_create(); /* used to hold the words in the search query */
        int numBuckets;
        char query[100];
        char c;
        printf("Enter the number of buckets: ");
        while(scanf("%d", &numBuckets) != 1) {
                /* gets the newline character because scanf does not */
                while ((c = getchar()) != EOF && c != '\n');
                printf("please enter a valid integer: ");
        }
        /* reads in the newline character so we can successfully get input */
        while ((c = getchar()) != EOF && c != '\n');
        struct hashmap *hm = hm_create(numBuckets);
        training(hm);
        printf("Enter the search query: ");
        // need to write loop for entering a string of any length
        if(fgets(query, sizeof(query), stdin) != NULL) {
                //printf("too small\n");
                char newString[sizeof(query)*2];
                strcat(newString, query);
                
        }
        printf("%s", query);
        read_query(query, list);
        //printList(hm); /* print the list for viewing purposes */
        //hm_remove(hm, "is", "D3.txt"); /* testing the remove function */
        //printList(hm);
        hm_destroy(hm); /* destroy the list */
        al_destroy(list);
        //printList(hm);
        return 0;
}

void training(struct hashmap *hm) {
        char str[1000];
        int i;
        /* loops through the 3 documents */
        for(i = 1; i < 4; i++) {
                /* creates either D1, D2, or D3 based on the current loop */
                char filename[7] = "D .txt";
                /* adds '1', '2', or '3' based on the iteration */
                char letter = (char) i + '0'; /* converts the int to char */
                filename[1] = letter;
                
                /* tries to open the file */
                FILE *fp = fopen(filename, "r");
                /* if the file doesn't exist, continues to the next loop */
                if(fp == NULL) {
                        printf("the file %s does not exist\n", filename);
                        continue;
                }
                /* loops through all the lines in the file */
                while(fgets(str, 1000, fp) != NULL) {
                        char *token = strtok(str, " \t\n");
                        /* loops through all the words in the line */
                        while(token != NULL) {
                                int len = strlen(token); /* gets the length of the token */
                                //if(token == NULL) {
                                //        continue;
                                //}
                                /* creates a new char array to copy the token into */
                                char *word = (char *) malloc(len*sizeof(char));
                                strcpy(word, token);
                                /* does the same as above for the document name */
                                char *document = (char *) malloc(7*sizeof(char));
                                strcpy(document, filename);
                                /* gets the number of occurances of the word + document pair */
                                int num_count = hm_get(hm, word, document);
                                /* if the word does not exist in the hashmap, add it to the map */
                                if(num_count == -1) {
                                        hm_put(hm, word, document, 1);
                                }
                                else {
                                        num_count++; /* increase the count by 1 */
                                        hm_put(hm, word, document, num_count); /* put it back in the hashmap */
                                }
                                token = strtok(NULL, " \t\n");
                        }
                }
        }
}

/* breaks the search query up into individual words and stores those words in
 * an arraylist. An arraylist is used because the number of words in the search
 * query is unspecified, so an arraylist will handle a search query of any length */
void read_query(char *query, struct array_list *list) {
        char *token = strtok(query, " \t\n");
        while(token != NULL) {
                al_add(list, token);
                token = strtok(NULL, " \t\n");
        }
        al_print(list);
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
                        printf("   '%s' : %d , ", currentNode->word, currentNode->num_occurrences);
                        printf("%s\n", currentNode->document_id); 
                        currentNode = currentNode->next;
                }
        }
}
