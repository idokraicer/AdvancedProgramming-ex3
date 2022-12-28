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
        LinkedList l = (LinkedList) h->table[i];
        h->table[i] = (LinkedList) NULL;
        s = destroyList(l);
    }

    free(h->table);
    free(h);
    return s;
}

status addToHashTable(hashTable h, Element key, Element value) {
    if (!key || !value) {
        return failure;
    }
    Element newKey = h->copyKey(key);
    Element newValue = h->copyValue(value);
    KeyValuePair kvp = createKeyValuePair(newKey, newValue, h->freeValue, h->freeKey,
                                          h->printValue, h->printKey,
                                          h->equalKey, isEqualKey, h->copyKey, h->copyValue);

    if (!kvp) {
        // Memory Error
        return failure;
    }
    int index = h->transformIntoNumber(key) % h->size;
    if (!h->table[index]) {
        h->table[index] = (LinkedList) createLinkedList(destroyKeyValuePair, displayKeyValuePair,
                                                        h->equalKey, copyKeyValuePair);
        if (!h->table[index]) { return failure; }//Memory Error
        return appendNode((LinkedList) h->table[index], (KeyValuePair) kvp);;
    }
    return appendNode(h->table[index], kvp);
}

Element lookupInHashTable(hashTable h, Element key) {
    int index = h->transformIntoNumber(key) % h->size;
    if (!h->table[index]) {
        return NULL;
    }
    return searchByKeyInList((LinkedList) h->table[index], key);
}


status removeFromHashTable(hashTable h, Element key) {
    int index = h->transformIntoNumber(key) % h->size;
    if (!h->table[index]) {
        return failure;
    }
    status s = deleteNode(h->table[index], key);
    if (getLengthList(h->table[index]) == 0) {
        destroyList(h->table[index]);
        h->table[index] = NULL;
    }
    return s;
}

status displayHashElements(hashTable h) {
    if (!h) return failure;
    for (int i = 0; i < h->size; i++) {
        printf("Index %d, Length: %d \n", i, getLengthList(h->table[i]));
        displayList((LinkedList) h->table[i]);
        if (h->table[i]) printf("\n");
    }
    return success;
}

