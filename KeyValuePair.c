#include "KeyValuePair.h"
#include <stdlib.h>

typedef struct KeyValuePair_s { // A struct that holds a key and a value
    Element key; // The key
    Element value; // The value
    FreeFunction destroyValue; // A function that frees the value
    FreeFunction destroyKey; // A function that frees the key
    PrintFunction printValue; // A function that prints the value
    PrintFunction printKey; // A function that prints the key
    EqualFunction compareKey; // A function that compares two keys
    EqualFunction compareValue; // A function that compares two values
    CopyFunction copyKey; // A function that copies a key
    CopyFunction copyValue; // A function that copies a value
} kvp;

KeyValuePair createKeyValuePair(Element key, Element value,
                                FreeFunction destroyValue, FreeFunction destroyKey,
                                PrintFunction displayValue, PrintFunction displayKey,
                                EqualFunction equalKey, EqualFunction equalValue,
                                CopyFunction copyKey, CopyFunction copyValue) {
    KeyValuePair k = malloc(sizeof(struct KeyValuePair_s)); // Allocating memory for the key value pair
    if (!k) { // If the allocation failed
        //Memory Error
        return NULL; // Return NULL
    }
    k->key = copyKey(key); // Copy the key
    k->value = copyValue(value); // Copy the value
    k->destroyValue = destroyValue; // Set the destroy value function
    k->destroyKey = destroyKey; // Set the destroy key function
    k->printValue = displayValue; // Set the print value function
    k->printKey = displayKey; // Set the print key function
    k->compareKey = equalKey; // Set the compare key function
    k->compareValue = equalValue; // Set the compare value function
    k->copyKey = copyKey; // Set the copy key function
    k->copyValue = copyValue; // Set the copy value function
    return k; // Return the key value pair
}

status destroyKeyValuePair(Element e) { // A function that destroys a key value pair
    if (!e) { // If the key value pair is NULL
        return failure;
    }
    KeyValuePair k = (KeyValuePair) e; // Cast the key value pair
    if(k == NULL) return failure; // If the cast failed
    status s=success; // Set the status to success
    if(k->value != NULL) s= k->destroyValue(k->value); // Destroy the value
    if(s == empty){ // If the value is empty
        k->value=NULL; // Set the value to NULL
    }
    s = max(-1,k->destroyKey(k->key)); // Destroy the key
    if(s == empty){ // If the key is empty
        k->key=NULL; // Set the key to NULL
    }
    free(k); // Free the key value pair
    k=NULL; // Set the key value pair to NULL
    e=NULL; // Set the element to NULL

    return s; // Return the status
}

status displayValue(Element e) { // A function that displays the value of a key value pair
    if (!e) { // If the key value pair is NULL
        return failure; // Return failure
    }
    KeyValuePair kvp = (KeyValuePair) e; // Cast the key value pair
    return kvp->printValue(kvp->value); // Print the value
}

status displayKey(Element e) { // A function that displays the key of a key value pair
    if (!e) { // If the key value pair is NULL
        return failure; // Return failure
    }
    KeyValuePair kvp = (KeyValuePair) e; // Cast the key value pair
    return kvp->printKey(kvp->key); // Print the key
}

Element getKey(Element kvp) { // A function that returns the key of a key value pair
    if (!kvp) return NULL; // If the key value pair is NULL
    return ((KeyValuePair) kvp)->key; // Return the key
}

Element getValue(Element kvp) { // A function that returns the value of a key value pair
    if (!kvp) return NULL; // If the key value pair is NULL
    return ((KeyValuePair) kvp)->value; // Return the value
}

bool isEqualKey(Element kvp1, Element e) { // A function that checks if two keys are equal
    if (!kvp1 || !e) return false; // If one of the key value pairs is NULL
    KeyValuePair kvp = (KeyValuePair) kvp1; // Cast the key value pair
    return kvp->compareKey(kvp, e); // Compare the keys using the compare key function
}

Element copyKeyValuePair(Element e) { // A function that copies a key value pair
    return (KeyValuePair) e; // Return the key value pair
}

bool isKeyValuePairValueEmpty(KeyValuePair kvp) { // A function that checks if the value of a key value pair is empty
    return getValue(kvp) != NULL; // Return true if the value is not NULL
}