#include "KeyValuePair.h"
#include <stdlib.h>

typedef struct KeyValuePair_s {
    Element key;
    Element value;
    FreeFunction destroyValue;
    FreeFunction destroyKey;
    PrintFunction printValue;
    PrintFunction printKey;
    EqualFunction compareKey;
    EqualFunction compareValue;
    CopyFunction copyKey;
    CopyFunction copyValue;
} kvp;

KeyValuePair createKeyValuePair(Element key, Element value,
                                FreeFunction destroyValue, FreeFunction destroyKey,
                                PrintFunction displayValue, PrintFunction displayKey,
                                EqualFunction equalKey, EqualFunction equalValue,
                                CopyFunction copyKey, CopyFunction copyValue) {
    KeyValuePair k = malloc(sizeof(struct KeyValuePair_s));
    if (!k) {
        //Memory Error
        return NULL;
    }
    k->key = copyKey(key);
    k->value = copyValue(value);
    k->destroyValue = destroyValue;
    k->destroyKey = destroyKey;
    k->printValue = displayValue;
    k->printKey = displayKey;
    k->compareKey = equalKey;
    k->compareValue = equalValue;
    k->copyKey = copyKey;
    k->copyValue = copyValue;
    return k;
}

status destroyKeyValuePair(Element e) {
    if (!e) {
        return failure;
    }
    KeyValuePair k = (KeyValuePair) e;
    if(k == NULL) return failure;
    status s=success;
    if(k->value != NULL) s= k->destroyValue(k->value);
    if(s == empty){
        k->value=NULL;
    }
    s = max(-1,k->destroyKey(k->key));
    if(s == empty){
        k->key=NULL;
    }
    free(k);
    k=NULL;
    e=NULL;

    return s;
}

status displayValue(Element e) {
    if (!e) {
        return failure;
    }
    KeyValuePair kvp = (KeyValuePair) e;
    return kvp->printValue(kvp->value);
}

status displayKey(Element e) {
    if (!e) {
        return failure;
    }
    KeyValuePair kvp = (KeyValuePair) e;
    return kvp->printKey(kvp->key);
}

status displayKeyValuePair(Element kvp) {
    KeyValuePair k = (KeyValuePair) kvp;
//    k->printKey(k->key);
//    printf(" : ");
    return k->printValue(k->value);
}

Element getKey(Element kvp) {
    if (!kvp) return NULL;
    return ((KeyValuePair) kvp)->key;
}

Element getValue(Element kvp) {
    if (!kvp) return NULL;
    return ((KeyValuePair) kvp)->value;
}

bool isEqualKey(Element kvp1, Element e) {
    if (!kvp1 || !e) return false;
    KeyValuePair kvp = (KeyValuePair) kvp1;
    return kvp->compareKey(kvp, e);
}

Element copyKeyValuePair(Element e) {
    return (KeyValuePair) e;
}

bool isKeyValuePairValueEmpty(KeyValuePair kvp) {
    return getValue(kvp) != NULL;
}