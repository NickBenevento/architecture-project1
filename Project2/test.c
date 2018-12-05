#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "array_list.h"
#include <string.h>
#include <ctype.h>

void printList(struct hashmap* hm);
void training(struct hashmap *hm);
char *format(char *string);
char *read_query(void);
void rank(struct hashmap *hm, char *query);
void stop_word(struct hashmap *hm);

int numDocuments;

int main(void) {
        //struct array_list *list = al_create(); /* used to hold the words in the search query */
        int numBuckets;
        //char query[100];
        char *query;
        char c;
        numDocuments = 3;
        printf("Enter the number of buckets: ");
        while(scanf("%d", &numBuckets) != 1) {
                /* gets the newline character because scanf does not */
                //while ((c = getchar()) != EOF && c != '\n');
                getchar();
                printf("please enter a valid integer: ");
        }
        /* reads in the newline character so we can successfully get input */
        char *valid = "SsXx";
        getchar();
        printf("Enter S to search, X to exit: ");
        scanf("%c", &c);
        /* get a new char while the entered character wasn't a valid input */
        while(strchr(valid, c) == NULL) {
            printf("Enter a valid input please: ");
            scanf("%c", &c);    
        }
        if(c == 'X' || c == 'x') {
                return 0;
        }
        getchar(); /* a buffer to get the newline character */

        struct hashmap *hm = hm_create(numBuckets);
        training(hm);
        //printList(hm);
        hm_remove(hm, "computer");
        //stop_word(hm);
        printList(hm);
        query = read_query();
        rank(hm, query);
        //read_query(query, list);
        hm_destroy(hm); /* destroy the list */
        //al_destroy(list);
        
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
                        char *token = strtok(str, " \n\t");
                        /* loops through all the words in the line */
                        while(token != NULL) {
                                int len = strlen(token); /* gets the length of the token */
                                //if(token == NULL) {
                                //        continue;
                                //}
                                /* creates a new char array to copy the token into */
                                char *word = (char *) malloc((len+1)*sizeof(char));
                                strcpy(word, token);
                                word = format(word);
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
                                //free(word);
                                //free(document);
                                token = strtok(NULL, " \n\t");
                        }
                }
                fclose(fp);
        }
}

void stop_word(struct hashmap *hm) {
        int i;
        for(i = 0; i < hm->num_buckets; i++) {
                struct llnode *node = hm->map[i];
                if(node->word == NULL) {
                        continue; /* if there's nothing in the bucket, continute to the next bucket */
                }
                while(node != NULL) {
                        printf("word: %s\n", node->word);
                        struct llnode *temp = node->next;
                        if(node->df_score == numDocuments) {
                                hm_remove(hm, node->word);
                        }
                        node = temp;
                }
        }
}

/* converts the letters to lowercase and removes any characters that aren't alphanumeric */
char *format(char *string) {
        int len = strlen(string);
        len++; /* to include the end of line character */
        char *newString = (char *) malloc((len)*sizeof(char)); /* make space for the formatted new string */
        /* loops through all the characters of the string */ 
        for(int i = 0; i < len; i++) {
                /* if the character is alphanumeric, copy it over to the new string */
                if(isalnum(string[i])) {
                        newString[i] = tolower(string[i]);
                }
                else {
                        newString[i] = string[i];
                }
        }
        free(string); /* free the memeory used for the original string */
        return newString;
}


char *read_query(void) {
        printf("Enter the search query: ");
        char *query = malloc(10*sizeof(char));
        
        //char 
        if(fgets(query, sizeof(query), stdin) != NULL) {
                //printf("too small\n");
                //char newString[sizeof(query)*2];
                //strcat(newString, query);
                
        }
        printf("%s", query);
        return query;
        //char *token = strtok(query, " \t\n");
        //while(token != NULL) {
        //        int len = strlen(token);
        //        char *word = (char *) malloc((len+1)*sizeof(char));
        //        strcpy(word, token);
        //        al_add(list, word);
        //        token = strtok(NULL, " \t\n");
        //}
        //al_print(list);
}

void rank(struct hashmap *hm, char *query) {
        int i;
        char *token = strtok(query, " \t\n");
        while(token != NULL) {
                
                token = strtok(NULL, " \t\n");
        }
        for(i = 0; i < hm->num_buckets; i++) {
                //printf("%s\n", list->array[i]);
        }
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
