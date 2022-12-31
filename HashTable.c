#include "HashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hashTable_s { // The hash table struct
    int size; // The size of the hash table
    Element *table; // The table itself
    CopyFunction copyKey; // A function that copies a key
    FreeFunction freeKey; // A function that frees a key
    PrintFunction printKey; // A function that prints a key
    CopyFunction copyValue; // A function that copies a value
    FreeFunction freeValue; // A function that frees a value
    PrintFunction printValue; // A function that prints a value
    EqualFunction equalKey; // A function that compares two keys
    TransformIntoNumberFunction transformIntoNumber; // A hashing function
} ht;

hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue,
                          FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
                          TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    hashTable h = malloc(sizeof(struct hashTable_s)); // Allocating memory for the hash table
    if (!h) { // If the allocation failed
        //Memory Error
        return NULL; // Return NULL
    }
    h->size = hashNumber; // Set the size of the hash table
    h->table = calloc(hashNumber, sizeof(void *)); // Allocate memory for the table
    if (!h->table) { // If the allocation failed
        //Memory Error
        free(h); // Free the hash table
        free(h->table); // Free the table
        return NULL; // Return NULL
    }
    for (int i = 0; i < h->size; i++) { // For each cell in the table
        h->table[i] = (LinkedList) NULL; // Set the cell to NULL
    }
    h->copyKey = copyKey; // Set the copy key function
    h->freeKey = freeKey; // Set the free key function
    h->printKey = printKey; // Set the print key function
    h->copyValue = copyValue; // Set the copy value function
    h->freeValue = freeValue; // Set the free value function
    h->printValue = printValue; // Set the print value function
    h->equalKey = equalKey; // Set the equal key function
    h->transformIntoNumber = transformIntoNumber; // Set the hashing function
    return h; // Return the hash table
}

status destroyHashTable(hashTable h) { // A function that destroys a hash table

    status s = success;
    for (int i = 0; i < h->size; i++) { // For each cell in the table
        if (h->table[i] == NULL) { // If the cell is empty
            continue; // Continue to the next cell
        } // Else
        LinkedList l = (LinkedList) h->table[i]; // Take the list from the cell
        if (l != NULL) { // If the list is not empty
            s = max(s, destroyList(l)); // Destroy the list
            if (s == memory_error) return memory_error; // If the destruction failed
            if (s == empty) { h->table[i] = NULL; } // If the list is empty set the cell to NULL
            h->table[i] = NULL; // Set the cell to NULL
        }
    }
    free(h->table); // Free the table
    h->table = NULL; // Set the table to NULL
    free(h); // Free the hash table
    h = NULL; // Set the hash table to NULL
    return s == empty ? success : s; // Return the status
}

status addToHashTable(hashTable h, Element key, Element value) {
    if (!h || !key || !value) { // If the hash table, key or value are NULL
        return failure; // Return failure
    }
    KeyValuePair kvp = createKeyValuePair(key, value, h->freeValue, h->freeKey,
                                          h->printValue, h->printKey,
                                          h->equalKey, isEqualKey, h->copyKey, h->copyValue);
    // Create a key value pair
    if (!kvp) { // If the creation failed
        // Memory Error
        return memory_error;
    }
    int index = h->transformIntoNumber(key) % h->size; // Get the index of the key
    if (h->table[index] == NULL) { // If the cell is empty
        h->table[index] = (LinkedList) createLinkedList(destroyKeyValuePair, displayValue,
                                                        h->equalKey, copyKeyValuePair);
        // Create a new list in the cell
        if (!h->table[index]) { return memory_error; }//Memory Error
        // If the creation failed
        return appendNode((LinkedList) h->table[index], (KeyValuePair) kvp);
        // Append the key value pair to the list
    } // If the cell is not empty
    return appendNode(h->table[index], kvp); // Append the key value pair to the list
}

Element lookupInHashTable(hashTable h, Element key) { // A function that looks up a key in the hash table
    int index = h->transformIntoNumber(key) % h->size; // Get the index of the key
    if (h->table[index] == NULL) { // If the cell is empty
        return NULL; // Return NULL
    }
    return searchByKeyInList((LinkedList) h->table[index], key); // Search for the key in the list
    // And return it's value
}


status removeFromHashTable(hashTable h, Element key) { // A function that removes a key from the hash table
    int index = h->transformIntoNumber(key) % h->size; // Get the index of the key
    if (h->table[index] == NULL) { // If the cell is empty
        return success; // Return success
    }
    LinkedList l = h->table[index]; // Take the list from the cell
    status s = deleteNode(l, key); // Delete the key from the list
    if (s == empty) { h->table[index] = NULL; } // If the list is empty set the cell to NULL

    return s; // Return the status
}

status displayHashElements(hashTable h) { // A function that displays the hash table
    if (!h) return failure; // If the hash table is NULL return failure
    status s = success; // Set the status to success
    for (int i = 0; i < h->size; i++) { // For each cell in the table
        s = max(s,displayList((LinkedList) h->table[i])); // Display the list in the cell
        if (h->table[i]) printf("\n"); // If the cell is not empty print a new line
    }
    return s; // Return the status
}

