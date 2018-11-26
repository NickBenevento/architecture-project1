#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include <string.h>

void printList(struct hashmap* hm);

int main(void) {
        char str[1000];
        int i;
        struct hashmap *hm = hm_create(15);
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
        printList(hm); /* print the list for viewing purposes */
        //hm_remove(hm, "is", "D3.txt"); /* testing the remove function */
        //printList(hm);
        hm_destroy(hm); /* destroy the list */
        //printList(hm);
        return 0;
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
