#include "HashTable.h"
#include "LinkedList.h"

#include "MultiValueHashTable.h"
#include "KeyValuePair.h"
#include "LinkedList.h"
#include "Defs.h"

struct MultiValueHashTable_s { // The multi value hash table struct
    hashTable table; // The hash table
    CopyFunction copyKey; // A function that copies a key
    FreeFunction freeKey; // A function that frees a key
    PrintFunction printKey; // A function that prints a key
    CopyFunction copyValue; // A function that copies a value
    FreeFunction freeValue; // A function that frees a value
    PrintFunction printValue; // A function that prints a value
    EqualFunction equalKey; // A function that compares two keys
    EqualFunction equalValue; // A function that compares two values
    TransformIntoNumberFunction transformIntoNumber; // A hashing function
    int size; // The size of the hash table
};


MultiValueHashTable
createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue,
                          FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
                          EqualFunction equalValue,
                          TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    // A function that creates a multi value hash table
    MultiValueHashTable h = malloc(
            sizeof(struct MultiValueHashTable_s)); // Allocating memory for the multi value hash table
    if (!h) { // If the allocation failed
        // Memory Error
        free(h); // Free the multi value hash table
        return NULL; // Return NULL
    }
    h->table = createHashTable(copyKey, freeKey, printKey, copyValue, destroyList, displayList, equalKey,
                               transformIntoNumber, hashNumber);
    // Creating the hash table
    if (!h->table) { // If the creation failed
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
    // Setting the functions
    return h; // Return the multi value hash table

}


status destroyMultiValueHashTable(MultiValueHashTable mvht) { // A function that destroys a multi value hash table
    status s = destroyHashTable(mvht->table); // Destroying the hash table
    free(mvht); // Freeing the multi value hash table
    return s;
}

status addToMultiValueHashTable(MultiValueHashTable mvht, Element key,
                                Element value) { // A function that adds a key value pair to the multi value hash table
    if (!mvht || !key || !value) return failure; // If the multi value hash table, key or value are NULL return failure
    LinkedList l = (LinkedList) getValue(
            (KeyValuePair) lookupInHashTable(mvht->table, key)); // Getting the linked list of the key value pair
    if (l == NULL) { // If the linked list is NULL
        l = createLinkedList(mvht->freeValue, mvht->printValue, mvht->equalValue,
                             mvht->copyValue); // Create a linked list
        if (!l) return memory_error; // If the creation failed return memory error
        appendNode(l, value); // Append the value to the linked list
        addToHashTable(mvht->table, key, l); // Add the key value pair to the hash table
        return success;
    }
    return appendNode(l, value); // Append the value to the linked list and return the result
}

LinkedList searchByKeyInTable(MultiValueHashTable m,
                              Element key) { // A function that searches for a key in the multi value hash table
    LinkedList l = (LinkedList) getValue(
            lookupInHashTable(m->table, key)); // Getting the linked list of the key value pair
    if (isEmpty(l)) {
        free(l);
        return NULL;
    }
    return l; // If the linked list is empty return NULL, else return the linked list
}

Element lookupInMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value) { // A function that looks up a value in the multi value hash table
    LinkedList l = searchByKeyInTable(mvht, key); // Searching for the key in the multi value hash table
    if (!l) return NULL; // If the linked list is NULL return NULL
    Element e = searchByKeyInList(l, value); // Searching for the value in the linked list
    return e ? e : NULL; // If the element is NULL return NULL, else return the element
}

status removeFromMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value) {
    // A function that removes a value from the multi value hash table
    if (!mvht || !key || !value) return failure; // If the multi value hash table, key or value are NULL return failure
    KeyValuePair kvp = lookupInHashTable(mvht->table, key); // Getting the key value pair
    LinkedList toDeleteFrom = (LinkedList) getValue(kvp); // Getting the linked list of the key value pair
    if (toDeleteFrom == NULL) return failure; // If the linked list is NULL return failure
    status s = success;
    if (getLengthList(toDeleteFrom) == 0) { // If the linked list is empty
        return success;
    }
    if (getLengthList(toDeleteFrom) == 1) { // If the linked list has only one element
        s = removeFromHashTable(mvht->table, key); // Remove the key value pair from the hash table
        return s;
    } else { // If the linked list has more than one element
        s = deleteNode(toDeleteFrom, value);  // Delete the value from the linked list
    }
    if (s == empty) { // If the linked list is empty
        toDeleteFrom = NULL; // Set the linked list to NULL
    }
    return s;
}

status displayMultiValueHashElementsByKey(MultiValueHashTable mvht, Element key) { // A function that displays the values of a key in the multi value hash table
    LinkedList l = searchByKeyInTable(mvht, key); // Searching for the key in the multi value hash table
    if (l == NULL) return failure; // If the linked list is NULL return failure
    status s = displayList(l); // Display the linked list
    return s;
}
