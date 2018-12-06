#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "doclist.h"
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <glob.h>

void printList(struct hashmap* hm);
int training(struct hashmap *hm, struct docnode* doc_list);
char *format(char *string);
char *read_query(void);
void rank(struct hashmap *hm, struct docnode* doc_list, char *query);
void stop_word(struct hashmap *hm);

int main(void) {
        int numBuckets;
        char c;
        printf("Enter the number of buckets: ");
        while(scanf("%d", &numBuckets) != 1) {
                while ((c = getchar()) != '\n'); /* a buffer to get characters that may have been typed after the entry */
                printf("please enter a valid integer: ");
        }
        struct hashmap *hm = hm_create(numBuckets, 0);
        /* clears the buffer */
        while ((c = getchar()) != '\n'); /* a buffer to get characters that may have been typed after the entry */

        printf("Enter S to search, X to exit: ");
        char *valid = "SsXx";
        /* get a new char while the entered character wasn't a valid input */
        while(strchr(valid, (c = fgetc(stdin))) == NULL) {
                printf("Enter a valid input please: ");
                char buf;
                /* gets the characters in the buffer */
                while ((buf = getchar()) != '\n');
                //getchar();
        }
        /* if the user wants to exit */
        if(c == 'X' || c == 'x') {
                hm_destroy(hm);
                return 0;
        }
        while ((c = getchar()) != '\n'); /* a buffer to get characters that may have been typed after the entry */

        struct docnode* doc_list = doc_create();
        if(training(hm, doc_list) == -1) {
                        printf("error reading the search string\n");
                        doc_delete(doc_list);
                        hm_destroy(hm);
                        return 0;
        }
        stop_word(hm);
        /* loop to keep getting search queries */
        while(1) {
                //printList(hm); /* prints the hashmap for veiwing purposes */
                char *query;
                printf("Enter the search query (or # to exit) :");
                query = read_query();
                /* breaks if the user enters '#' */
                if(query[0] == '#') {
                        free(query);
                        break;
                }
                rank(hm, doc_list, query);
                query = NULL;
                free(query);
                doc_reset(doc_list); /* resets the scores for the documents */
        }
        /* free the memory */
        doc_delete(doc_list);
        hm_destroy(hm); /* destroy the list */
        return 0;
}

int training(struct hashmap *hm, struct docnode* doc_list) {
        printf("enter the search string: ");
        char *search_string = read_query(); /* uses the readquery function to get the search string */
        glob_t result;
        /* uses glob to search that directory for all the files and store them in result */
        int retval = glob(search_string, GLOB_ERR, NULL, &result);
        char str[1000];
        /* glob returns 0 on success, so if it wasn't 0, then there is a problem */
        if(retval != 0) {
                printf("error. could not open the files in the directory with the search string\n");
                free(search_string);
                return -1;
        }
        char **file; /* an array of strings that hold the filenames */
        /* loops through all the filenames that were read by glob */
        for(file = result.gl_pathv; *file != NULL; file++) {
                /* tries to open the file */
                FILE *fp = fopen(*file, "r");
                /* if the file doesn't exist, continues to the next loop */
                if(fp == NULL) {
                        printf("the file %s does not exist\n", *file);
                        continue;
                }
                int doc_len = strlen(*file);
                doc_len++;
                char *document_copy = (char *) malloc(doc_len*sizeof(char));
                if(!document_copy) {
                        return -1;
                }
                strcpy(document_copy, *file);
                /* add the document to the document list */
                doc_add(doc_list, document_copy);
                hm->num_documents++; /* the file was opened successfuly, so increase the number of documents */
                /* loops through all the lines in the file */
                while(fgets(str, 1000, fp) != NULL) {
                        char *token = strtok(str, " \n\t");
                        /* loops through all the words in the line */
                        while(token != NULL) {
                                int len = strlen(token); /* gets the length of the token */
                                /* creates a new char array to copy the token into */
                                char *word = (char *) malloc((len+1)*sizeof(char));
                                if(!word) {
                                        return -1;
                                }
                                strcpy(word, token);
                                word = format(word);
                                /* does the same as above for the document name */
                                char *document = (char *) malloc(doc_len*sizeof(char));
                                if(!document) {
                                        return -1;
                                }
                                strcpy(document, *file);
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
                                token = strtok(NULL, " \n\t");
                        }
                }
                fclose(fp);
        }
        free(search_string); /* free the search string */
        globfree(&result); /* free the object used for glob */
        return 0;
}

/* removes any stop words (words that appear in all the documents) */
void stop_word(struct hashmap *hm) {
        int i;
        /* loops through all the buckets in the map */
        for(i = 0; i < hm->num_buckets; i++) {
                struct llnode *node = hm->map[i];
                if(node->word == NULL) {
                        continue; /* if there's nothing in the bucket, continute to the next bucket */
                }
                while(node != NULL) {
                        struct llnode *temp = node->next; /* need a temp incase the node gets removed */
                        /* if the word appears in all the documents, remove it */
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

/* reads in a string of any size from the user by dynamically reallocating memory if the strin
 * runs out of space */
char *read_query(void) {
        int len = 5; /* start with a length of 5 */
        int size = 0; /* initial size of the string is 0 */
        char *query = malloc(len*sizeof(char)); /* start with a string of length 5 */
        if(!query) {
                return NULL;
        }
        char c;
        /* reads in 1 character at a time while it isn't the enter key */
        while( (c = fgetc(stdin)) != '\n') {
                c = tolower(c); /*converts char to lowercase */
                query[size] = c; /* puts the character in the string */
                size++;
                /* if the size of the string is equal to the length, increase the length and reallocate memory */
                if( size == len) {
                        len += 1;
                        query = realloc(query, sizeof(char)*len);
                        if(!query) {
                                return NULL;
                        }
                }
        }
        query[size] = '\0'; /* add the terminating character to the end */
        return query;
}

/* ranks the documents in terms of relevance to the search query */
void rank(struct hashmap *hm, struct docnode* doc_list, char *query) {
        double numerator = (double) hm->num_documents;
        //doc_print(doc_list);
        char *token = strtok(query, " \t\n");
        /* loops through all the words in the search query */
        while(token != NULL) {
                /* gets the node for the current word in the query */
                struct llnode *node = hm_get_word(hm, token);
                /* if the word isn't in any of the documents, continue to the next word */
                if(node == NULL) {
                        printf("the word '%s' was not found in any documents.\n", token);
                        token = strtok(NULL, " \t\n");
                        continue;
                }
                double denominator = node->df_score;
                if(denominator == 0) {
                        denominator = 1;
                }
                /* x is the # of documents divided by the document frequency */
                double x = numerator / denominator;
                double inverse_df = log10(x); /* gets the inverse doc freq using the log function */
                //printf("word: %s , idf: %f\n", node->word, inverse_df);
                node->idf = inverse_df;
                struct lldoc *iter = node->docptr;
                /* loops through all the documents the word appears in */
                while(iter != NULL) {
                        struct docnode* doc_iter = doc_list;
                        /* loops through the document list until it equals the iter's document id */
                        while(strcmp(doc_iter->document, iter->document_id) != 0) {
                                doc_iter = doc_iter->next;
                        }
                        doc_iter->score += node->idf*iter->num_occurrences; /* adds to that document score */
                        
                        iter = iter->doc_next;
                }
                token = strtok(NULL, " \t\n"); /* get the next search word */
        }
        doc_print_order(doc_list); /* prints the documents in order of relevance */
}


