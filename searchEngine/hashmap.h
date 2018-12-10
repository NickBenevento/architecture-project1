#ifndef HASHMAP_H
#define HASHMAP_H

struct llnode {
        char* word;
        int df_score; /* the document frequency score */
        double idf; /* the inverse document frequency score */
        struct llnode* next;
        struct lldoc* docptr;
};

struct lldoc {
        char *document_id;
        int num_occurrences; /* the number of occurances (or tf) */
        struct lldoc* doc_next;
};

struct hashmap {
        struct llnode** map;
        int num_buckets;
        int num_elements;
        int num_documents;
};

struct hashmap* hm_create(int num_buckets, int num_documents);
int hm_get(struct hashmap* hm, char* word, char* document_id);
struct llnode* hm_get_word(struct hashmap* hm, char *word);
void hm_put(struct hashmap* hm, char* word, char* document_id, int num_occurrences);
void hm_remove(struct hashmap* hm, char* word);
void printList(struct hashmap* hm);
void hm_destroy(struct hashmap* hm);
int hash(struct hashmap* hm, char* word);

#endif
