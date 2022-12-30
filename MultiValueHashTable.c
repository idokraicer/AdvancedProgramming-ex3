#include "HashTable.h"
#include "LinkedList.h"

#include "MultiValueHashTable.h"
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

Element shallowCopyKey(Element key) {
    return key;
}

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
    h->table = createHashTable(copyKey, freeKey, printKey, copyValue, destroyList, displayList, equalKey,
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
    status s =destroyHashTable(mvht->table);
    free(mvht);
    return s;
}

status addToMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value) {
    if (!mvht || !key || !value) return failure;
    LinkedList l = (LinkedList) getValue((KeyValuePair) lookupInHashTable(mvht->table, key));
    if (l == NULL) {
        l = createLinkedList(mvht->freeValue, mvht->printValue, mvht->equalValue, mvht->copyValue);
        if (!l) return memory_error;
        appendNode(l, value);
        addToHashTable(mvht->table, key, l);
        return success;
    }
    appendNode(l, value);
    return success;
}

LinkedList searchByKeyInTable(MultiValueHashTable m, Element key) {
    LinkedList l = (LinkedList) getValue(lookupInHashTable(m->table, key));
    return isEmpty(l) ? NULL : l;
}

Element lookupInMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value) {
    LinkedList l = searchByKeyInTable(mvht, key);
    if (!l) return NULL;
    Element e = searchByKeyInList(l, value);
    return e ? e : NULL;
}

status removeFromMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value) {
    LinkedList l = searchByKeyInTable(mvht, key);
    if (l == NULL) return failure;
    status s =success;
    if(getLengthList(l) == 0) return success;
    if(getLengthList(l) == 1){
        printf("Length of list is 1\n");
        s = removeFromHashTable(mvht->table, key);
    } else {

        s = deleteNode(l, key);
    }
    if (s == empty) {
        l = NULL;
    }
    return s;
}

status displayMultiValueHashElementsByKey(MultiValueHashTable mvht, Element key) {
    LinkedList l = searchByKeyInTable(mvht, key);
    if (l==NULL) return failure;
    status s = displayList(l);
    return s;
}
