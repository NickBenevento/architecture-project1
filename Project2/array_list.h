#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#define INITIAL_SIZE 3

struct array_list {
        char **array;
        int size;
        int capacity;
};

struct array_list* al_create(void);
void al_add(struct array_list* list, char *word);
char * al_get(struct array_list* list, int index);
void al_set(struct array_list* list, int index, char *word);
void al_remove(struct array_list* list, int index);
int al_size(struct array_list* list);
void al_print(struct array_list *list);
void al_destroy(struct array_list* list);

#endif
