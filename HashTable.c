#include "HashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hashTable_s {
    int size;
    Element *table;
    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalKey;
    TransformIntoNumberFunction transformIntoNumber;
} ht;

hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue,
                          FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
                          TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    hashTable h = malloc(sizeof(struct hashTable_s));
    if (!h) {
        //Memory Error
        return NULL;
    }
    h->size = hashNumber;
    h->table = calloc(hashNumber, sizeof(void *));
    if (!h->table) {
        //Memory Error
        return NULL;
    }
    for (int i = 0; i < h->size; i++) {
        h->table[i] = (LinkedList) NULL;
    }
    h->copyKey = copyKey;
    h->freeKey = freeKey;
    h->printKey = printKey;
    h->copyValue = copyValue;
    h->freeValue = freeValue;
    h->printValue = printValue;
    h->equalKey = equalKey;
    h->transformIntoNumber = transformIntoNumber;
    return h;
}

status destroyHashTable(hashTable h) {

    status s = success;
    for (int i = 0; i < h->size; i++) {
        if(h->table[i] == NULL) {
            free(h->table[i]);
            continue;
        }
        LinkedList l = (LinkedList) h->table[i];
        if (l != NULL) {
            s = max(s, destroyList(l));
            if(s == memory_error) return memory_error;
            if(s == empty){ h->table[i] =NULL;}
            h->table[i]=NULL;
        }
    }
    free(h->table);
    h->table=NULL;
    free(h);
    h=NULL;
    return s == empty ? success : s;
}

status addToHashTable(hashTable h, Element key, Element value) {
    if (!key || !value) {
        return failure;
    }
    KeyValuePair kvp = createKeyValuePair(key, value, h->freeValue, h->freeKey,
                                          h->printValue, h->printKey,
                                          h->equalKey, isEqualKey, h->copyKey, h->copyValue);

    if (!kvp) {
        // Memory Error
        return memory_error;
    }
    int index = h->transformIntoNumber(key) % h->size;
    if (h->table[index] == NULL) {
        h->table[index] = (LinkedList) createLinkedList(destroyKeyValuePair, displayKeyValuePair,
                                                        h->equalKey, copyKeyValuePair);
        if (!h->table[index]) { return memory_error; }//Memory Error
        return appendNode((LinkedList) h->table[index], (KeyValuePair) kvp);
    }
    return appendNode(h->table[index], kvp);
}

Element lookupInHashTable(hashTable h, Element key) {
    int index = h->transformIntoNumber(key) % h->size;
    if (h->table[index] == NULL) {
        return NULL;
    }
    return searchByKeyInList((LinkedList) h->table[index], key);
}


status removeFromHashTable(hashTable h, Element key) {
    int index = h->transformIntoNumber(key) % h->size;
    if (h->table[index] == NULL) {
        return success;
    }
    LinkedList l = h->table[index];
    status s = deleteNode( l, key);
    if(s == empty) {h->table[index] = NULL;}

    return s;
}

status displayHashElements(hashTable h) {
    if (!h) return failure;
    status s = success;
    for (int i = 0; i < h->size; i++) {
        s = displayList((LinkedList) h->table[i]);
        if (h->table[i]) printf("\n");
    }
    return s;
}

