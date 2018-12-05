#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "array_list.h"
#include <string.h>
#include <ctype.h>
#include <math.h>

void printList(struct hashmap* hm);
void training(struct hashmap *hm);
char *format(char *string);
char *read_query(void);
void rank(struct hashmap *hm, char *query);
void stop_word(struct hashmap *hm);

/* TODO LIST:
 * fix bug with more than 1 search word
 * clean code up -- get rid of unecessary methods
 * add ability to work for any number of documents (Extra Credit)
 * check after each malloc
 */

int main(void) {
        int numBuckets;
        char *query;
        char c;
        int num_documents = 3;
        printf("Enter the number of buckets: ");
        while(scanf("%d", &numBuckets) != 1) {
                while ((c = getchar()) != '\n'); /* a buffer to get characters that may have been typed after the entry */
                //fflush(stdin);
                printf("please enter a valid integer: ");
        }
        /* reads in the newline character so we can successfully get input */
        char *valid = "SsXx";
        while ((c = getchar()) != '\n'); /* a buffer to get characters that may have been typed after the entry */
        printf("Enter S to search, X to exit: ");
        scanf("%c", &c);
        /* get a new char while the entered character wasn't a valid input */
        while(strchr(valid, c) == NULL) {
            printf("Enter a valid input please: ");
            c = fgetc(stdin);  
            while ((c = getchar()) != '\n');
        }
        if(c == 'X' || c == 'x') {
                return 0;
        }
        while ((c = getchar()) != '\n'); /* a buffer to get characters that may have been typed after the entry */
        struct hashmap *hm = hm_create(numBuckets, num_documents);
        training(hm);
        stop_word(hm);
        printList(hm);
        query = read_query();
        rank(hm, query);
        hm_destroy(hm); /* destroy the list */
        free(query);
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
                                /* creates a new char array to copy the token into */
                                char *word = (char *) malloc((len+1)*sizeof(char));
                                strcpy(word, token);
                                word = format(word);
                                /* does the same as above for the document name */
                                char *document = (char *) malloc(7*sizeof(char));
                                strcpy(document, filename);
                                /* gets the number of occurances of the word + document pair */
                                int num_count = hm_get(hm, word, document, 1);
                                /* if the word does not exist in the hashmap, add it to the map */
                                if(num_count == -1) {
                                        hm_put(hm, word, document, 1);
                                }
                                else {
                                        num_count++; /* increase the count by 1 */
                                        hm_put(hm, word, document, num_count); /* put it back in the hashmap */
                                }
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
                        //printf("word: %s\n", node->word);
                        struct llnode *temp = node->next;
                        if(node->df_score == hm->num_documents) {
                                hm_remove(hm, node->word);
                        }
                        node = temp;
                }
        }
}

/* converts the letters to lowercase and removes any characters that aren't alphanumeric */
char *format(char *string) {
        int len = strlen(string) +1;
        /* loops through all the characters of the string */ 
        for(int i = 0; i < len; i++) {
                /* if the character is alphanumeric, make it lowercase */
                if(isalnum(string[i])) {
                        string[i] = tolower(string[i]);
                }
        }
        return string;
}


char *read_query(void) {
        printf("Enter the search query: ");
        int len = 5;
        int size = 0;
        char *query = malloc(len*sizeof(char)); /* start with a string of length 5 */
        char c;
        while( (c = fgetc(stdin)) != '\n') {
                c = tolower(c); /*converts char to lowercase */
                query[size] = c; /* puts the character in the string */
                size++;
                /* if the size of the string is equal to the length, increase the length and reallocate memory */
                if( size == len) {
                        len += 1;
                        query = realloc(query, sizeof(char)*len);
                }
        }
        query[size] = '\0'; /* add the terminating character to the end */
        printf("query: %s\n", query);
        return query;
}

void rank(struct hashmap *hm, char *query) {
        double numerator = (double) hm->num_documents;
        double d1_score = 0;
        double d2_score = 0;
        double d3_score = 0;
        char *token = strtok(query, " \t\n");
        while(token != NULL) {
                struct llnode *node = hm_get_word(hm, token);
                if(node == NULL) {
                        printf("the word '%s' was not found in any documents.\n", token);
                        token = strtok(NULL, " \t\n");
                        continue;
                }
                double x = numerator / (double) node->df_score;
                double inverse_df = log10(x);
                printf("word: %s , idf: %f\n", node->word, inverse_df);
                node->idf = inverse_df;

                printf("node->score: %f\n", node->idf);
                struct lldoc *iter = node->docptr;
                int count = 0;
                while(iter != NULL) {
                        if(count == 0) {
                                d1_score += node->idf*iter->num_occurrences;
                        }
                        else if(count == 1) {
                                d2_score += node->idf*iter->num_occurrences;
                        }
                        else if(count == 2) {
                                d3_score += node->idf*iter->num_occurrences;
                        }
                        printf("iter->doc: %s\n", iter->document_id);
                        count++;
                        iter = iter->doc_next;
                }
                //hm_get_doc_freq(hm, token);
                token = strtok(NULL, " \t\n");
        }
        printf("d1_score: %f\n", d1_score);
        printf("d2_score: %f\n", d2_score);
        printf("d3_score: %f\n", d3_score);
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
