#ifndef HASHMAP_H
#define HASHMAP_H

struct llnode {
        char* word;
        //char* document_id;
        //int num_occurrences;
        int df_score;
        struct llnode* next;
        struct lldoc* docptr;
};

struct lldoc {
        char *document_id;
        int num_occurrences;
        struct lldoc* doc_next;
};

struct hashmap {
        struct llnode** map;
        int num_buckets;
        int num_elements;
};

struct hashmap* hm_create(int num_buckets);
int hm_get(struct hashmap* hm, char* word, char* document_id);
void hm_put(struct hashmap* hm, char* word, char* document_id, int num_occurrences);
void hm_remove(struct hashmap* hm, char* word);
void hm_destroy(struct hashmap* hm);
int hash(struct hashmap* hm, char* word);

#endif
