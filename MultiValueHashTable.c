#include "HashTable.h"
#include "LinkedList.h"

#include "MultiValueHashTable.h"
#include "HashTable.h"
#include "KeyValuePair.h"
#include "LinkedList.h"
#include "Defs.h"

struct MultiValueHashTable_s {
    hashTable table;
    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalKey;
    EqualFunction equalValue;
    TransformIntoNumberFunction transformIntoNumber;
    int size;
};

MultiValueHashTable
createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue,
                          FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
                          EqualFunction equalValue,
                          TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    MultiValueHashTable h = malloc(sizeof(struct MultiValueHashTable_s));
    if (!h) {
        // Memory Error
        free(h);
        return NULL;
    }
    h->table = createHashTable(copyKey, freeKey, printKey, copyValue, freeValue, displayList, equalKey,
                               transformIntoNumber, hashNumber);
    if (!h->table) {
        // Memory Error
        free(h->table);
        free(h);
        return NULL;
    }
    h->copyKey = copyKey;
    h->copyValue = copyValue;
    h->freeValue = freeValue;
    h->freeKey = freeKey;
    h->printKey = printKey;
    h->printValue = printValue;
    h->equalKey = equalKey;
    h->equalValue = equalValue;
    h->transformIntoNumber = transformIntoNumber;
    h->size = hashNumber;
    return h;
}

status destroyMultiValueHashTable(MultiValueHashTable mvht) {
    destroyHashTable(mvht->table);
    free(mvht);
    return success;
}

status addToMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value) {
    if (!mvht || !key || !value) return failure;
    LinkedList l = (LinkedList) getValue((KeyValuePair )lookupInHashTable(mvht->table, key));
    if (isEmpty(l)) {
        l = createLinkedList(mvht->freeValue, mvht->printValue, mvht->equalValue, mvht->copyValue);
        appendNode(l, value);
        addToHashTable(mvht->table, key, l);
//        displayHashElements(mvht->table);
        return success;
    }
    appendNode(l, value);
//    displayHashElements(mvht->table);
    return success;
}

LinkedList searchByKeyInTable(MultiValueHashTable m, Element key) {
    LinkedList l = (LinkedList) getValue(lookupInHashTable(m->table, key));
    return l ? l : NULL;
}

Element lookupInMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value) {
    LinkedList l = searchByKeyInTable(mvht, key);
    if (!l) return NULL;
    Element e = searchByKeyInList(l, value);
    return e ? e : NULL;
}

status removeFromMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value) {
    LinkedList l = searchByKeyInTable(mvht, key);
    status s = deleteNode(l, value);
    if (getLengthList(l) == 0) {
        destroyList(l);
        removeFromHashTable(mvht->table, key);
    }
    return s;
}

status displayMultiValueHashElementsByKey(MultiValueHashTable mvht, Element key) {
    LinkedList l = searchByKeyInTable(mvht, key);
    if(!l) return failure;
    displayList(l);
}

status displayMultiValueHashElements(MultiValueHashTable mvht) {
    displayHashElements(mvht->table);
}