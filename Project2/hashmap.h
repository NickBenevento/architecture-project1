#ifndef HASHMAP_H
#define HASHMAP_H

struct llnode {
        char* word;
        //char* document_id;
        //int num_occurrences;
        int df_score;
        double idf;
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
        int num_documents;
};

struct hashmap* hm_create(int num_buckets, int num_documents);
int hm_get(struct hashmap* hm, char* word, char* document_id, int compare_docID_or_nah);
struct llnode* hm_get_word(struct hashmap* hm, char *word);
void hm_put(struct hashmap* hm, char* word, char* document_id, int num_occurrences);
void hm_remove(struct hashmap* hm, char* word);
void hm_destroy(struct hashmap* hm);
int hash(struct hashmap* hm, char* word);

#endif
