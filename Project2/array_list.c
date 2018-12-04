#include "array_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct array_list* al_create(void){
    /* allocating memory for the list */
    struct array_list *new_list = malloc(sizeof(struct array_list));
    /* allocating memory for the array in the list */
    new_list->array = malloc(INITIAL_SIZE*sizeof(char *));
    int i;
    for(i = 0; i < INITIAL_SIZE; i++) {
        new_list->array[i] = malloc(sizeof(char *));
    }
    new_list->capacity = INITIAL_SIZE; /* capacity is the initial size */
    new_list->size = 0; /* starts with nothing in it */
    return new_list;
}

void al_add(struct array_list* list, char *word){
    list->array[list->size] = word; /* sets the value in the next open index in the array */
    list->size++; /* increases the size by 1 */
    /* if the array becomes full, copy the contents into a new array */
    if(list->size == list->capacity) {
        /* allocate memory for the new array, with 1.5* the capacity */
        char **new_array = malloc(list->capacity*1.5*sizeof(char *));
        if(new_array == NULL) {
            printf("failed\n");
            return;
        }
        /* loops through and copies all the elements to the new array */
        int i;
        for(i = 0; i < list->size; i++) {
            new_array[i] = list->array[i];
        }
        free(list->array); /* free the memory used for the old array */
        list->array = NULL;
        //list->array = NULL;
        list->array = new_array; /* set the list's array to the new array */
        //printf("list->array = %l", list->array);
        list->capacity = list->capacity*1.5; /* change the capacity to 1.5* the old capacity */
    }
}

char * al_get(struct array_list* list, int index){
    /* if the index is out of bounds */
    if(index >= list->size) {
        printf("the index is out of bounds\n");
        return 0;
    }
    return list->array[index];
}

void al_set(struct array_list* list, int index, char *word){
    /* if the index is out of bounds */
    if(index >= list->capacity) {
        printf("the index is not valid\n");
        return;
    }
    list->array[index] = word;
}

void al_remove(struct array_list* list, int index){
    /* if the index is out of bounds */
    if(index >= list->capacity) {
        printf("not a valid index\n");
        return;
    }
    printf("removing element %s at index %d\n", list->array[index], index);
    /* removes the element in the index by shifting all the rest of the values
     * to the right of it by 1 space to the left */
    int i = 0;
    for(i = index; i < (list->size); i++) {
        list->array[i] = list->array[i+1];
    }
    list->size--; /* decrease the size */
}

void al_print(struct array_list *list) {
    int i;
    printf("printing list:\n");
    printf("[");
    for(i = 0; i < list->size; i++) {
        if(list->array[i] != NULL) {
            printf("%s, ", list->array[i]);
        }
    }
    printf("\b\b]\n");
}

int al_size(struct array_list* list){
    return list->size;
}

void al_destroy(struct array_list* list){
    /* frees the memory used for the structure */
    int i;
    printf("size: %d\n", list->size);
    for(i = 0; i < list->size; i++) {
        if(list->array[i] != NULL) {
            free(list->array[i]);
        }
    }
    free(list->array);
    free(list);
}
