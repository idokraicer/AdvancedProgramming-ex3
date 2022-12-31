#include "LinkedList.h"
#include <stdlib.h>
#include "KeyValuePair.h"

typedef struct LinkedList_s { // A linked list is a list of nodes
    Element data; // The data of the node
    struct LinkedList_s *next; // The next node in the list
    struct LinkedList_s *prev; // The previous node in the list
    struct LinkedList_s *head; // The head of the list

    FreeFunction destroyNode; // A function that frees the data of the node
    PrintFunction printNode; // A function that prints the data of the node
    EqualFunction compare; // A function that compares two nodes
    CopyFunction copy; // A function that copies a node
} node;

LinkedList createLinkedList(FreeFunction destroyNode, PrintFunction printNode, EqualFunction compare,
                            CopyFunction copy) { // A function that creates a linked list
    LinkedList n = malloc(sizeof(struct LinkedList_s)); // Allocating memory for the linked list
    if (n == NULL) { // If the allocation failed
        //Memory Error
        return NULL; // Return NULL
    }
    n->destroyNode = destroyNode; // Set the destroy node function
    n->printNode = printNode; // Set the print node function
    n->compare = compare; // Set the compare function
    n->copy = copy;     // Set the copy function
    n->next = NULL;
    n->prev = NULL;
    n->head = NULL;
    n->data = NULL;
    // Set the next, previous and head nodes to NULL and the data to NULL
    return n; // Return the linked list
}

status destroyList(Element e) { // A function that destroys a linked list
    LinkedList l = (LinkedList) e; // Cast the linked list
    if (l == NULL) { return empty;} // If the cast failed
    status flag = success; // Set the status to success
    if(l->data == NULL){ // If the node is empty
        free(l); // Free the node
        return empty; // Return empty
    }
    if (l->next == NULL) { // If the next node is NULL
        flag = l->destroyNode(l->data); // Destroy the data
        free(l); // Free the node
        return empty; // Return empty
    }
    while (l != NULL) { // While the node is not NULL
        LinkedList temp = l; // Set a temporary node to the current node
        l = l->next; // Set the current node to the next node
        flag = max(temp->destroyNode(temp->data), flag); // Destroy the data
        if (flag == failure) printf("Oops\n"); // If the destruction failed
        temp->data = NULL; // Set the data to NULL
        free(temp); // Free the node
        temp = NULL; // Set the node to NULL
    }
    l = NULL; // Set the linked list to NULL
    e = NULL; // Set the element to NULL
    return flag; // Return the status
}


bool notLast(LinkedList n) { // A function that checks if the node is not the last node
    return n->next != NULL; // Return true if the next node is not NULL
}

bool isEmpty(LinkedList l) { // A function that checks if the linked list is empty
    if (l==NULL) return true; // If the linked list is NULL return true
    else if (l->data==NULL) return true;  // If the data is NULL return true
    return false; // Else return false
}

status appendNode(LinkedList l, Element toAdd) { // A function that appends a node to the linked list
    if (l == NULL) return failure; // If the linked list is NULL return failure

    if (l->head == NULL) { // If the head is NULL
        l->data = l->copy(toAdd); // Copy the data
        l->head = l; // Set the head to the current node
        return success;
    }

    LinkedList l_toAdd = createLinkedList(l->destroyNode, l->printNode, l->compare, l->copy); // Create a new node
    if (l_toAdd == NULL) return memory_error; // If the creation failed return memory error
    l_toAdd->data = l->copy(toAdd); // Copy the data
    l_toAdd->head = l->head; // Set the head to the head of the linked list

    while (notLast(l)) { // While the node is not the last node
        l = l->next; // Set the current node to the next node
    }
    l_toAdd->prev = l; // Set the new node's previous node to the current node
    l->next = l_toAdd; // Set the next node of the last node to the new node
    return success;
}

status switchData(LinkedList a, LinkedList b) { // A function that switches the data of two nodes
    if (a == NULL || b == NULL) return failure; // If one of the nodes is NULL return failure
    Element temp = a->data; // Set a temporary element to the data of the first node
    a->data = b->data; // Set the data of the first node to the data of the second node
    b->data = temp; // Set the data of the second node to the temporary element
    return success;
}

status appendCondition(LinkedList l, Element toAdd, EqualFunction cmp) { // A function that appends a node to the linked list if the condition is met
    if (!l || !toAdd || !cmp) return failure; // If one of the parameters is NULL return failure
    if (!l->data) { // If the data is NULL
        l->data = l->copy(toAdd); // Copy the data
        l->head = l; // Set the head to the current node
        l->next = NULL; // Set the next node to NULL
        return success;
    }

    LinkedList head = l->head; // Set the head to the head of the linked list
    LinkedList newl = createLinkedList(head->destroyNode, head->printNode, head->compare, head->copy); // Create a new node
    if (!newl) return memory_error; // If the creation failed return memory error
    newl->data = newl->copy(toAdd); // Copy the data to the new node
    newl->head = head->head; // Set the head to the head of the linked list
    if (cmp(head->data, toAdd)) { // if the new element is smaller than the head
        switchData(head, newl); // head is now the node we want in the start
        newl->next = head->next; // Set the next node of the new node to the next node of the head
        if(head->next) head->next->prev = newl; // If the next node of the head is not NULL
        // set the previous node of the next node to the new node
        head->next = newl; // Set the next node of the head to the new node
        newl->prev = head; // Set the previous node of the new node to the head
        newl->head = head; // Set the head of the new node to the head
        return success;
    } // If the new element is smaller than the head
    if(head->next == NULL){ // If the next node of the head is NULL
        head->next = newl; // Set the next node of the head to the new node
        newl->prev = head; // Set the previous node of the new node to the head
        return success;
    }
    while (head->next != NULL && !cmp(head->data, toAdd)) { // Iterates until finds a match
        head = head->next;
        if(!head->next) { // if the new element is bigger than the last element
            // append to the end
            head->next = newl;
            newl->prev = head;
            return success;
        }
    }
    if (cmp(head->data, toAdd)) { // if the new element is smaller than the current element
        newl->next = head; // Set the next node of the new node to the current node
        newl->prev = head->prev; // Set the previous node of the new node to the previous node of the current node
        head->prev->next = newl; // Set the next node of the previous node of the current node to the new node
        head->prev = newl; // Set the previous node of the current node to the new node
        return success;
    }
}

status displayList(Element e) {
    if(e == NULL) return failure;
    LinkedList l = (LinkedList) e; // Cast the element to a linked list
    if (l == NULL) { // If the linked list is NULL
        return failure; // failure
    }
    l = l->head; // Set the linked list to the head of the linked list
    if(l->data == NULL) return empty; // If the data is NULL return empty
    PrintFunction pf;
    while (!isEmpty(l)) { // While the linked list is not empty
        pf = l->printNode; // Set the print function to the print function of the linked list
        if (pf(l->data) == success) printf("\n"); // If the print function succeeded print a new line
        l = l->next; // Set the linked list to the next node
    }
    return success;
}

status deleteNode(LinkedList l, Element toDelete) { // A function that deletes a node from the linked list
    if (l == NULL) return failure; // If the linked list is NULL return failure
    LinkedList curr = l->head; // Set the current node to the head of the linked list
    if (!curr) return failure; // If the current node is NULL return failure
    if (!curr->data) {free(l);return empty;}; // If the data is NULL free the linked list and return empty
    if (curr->compare(curr->data, toDelete)) { // if head is the node to delete
        if(curr->next == NULL){ // if head is the only node
            curr->destroyNode(curr->data); // Destroy the data
            l->head = NULL; // Set the head to NULL
            curr->data = NULL; // Set the data to NULL
            curr->next=NULL; // Set the next node to NULL
            curr->prev=NULL; // Set the previous node to NULL
            free(l); // Free the linked list
            l=NULL; // Set the linked list to NULL
            return empty; // return empty
        }
        // if head is not the only node
        LinkedList temp = curr->next; // Set a temporary node to the next node of the head
        curr->destroyNode(curr->data); // Destroy the data of the head
        curr->data = NULL; // Set the data of the head to NULL
        switchData(curr, temp); // Switch the data of the head and the temporary node
        curr->next = curr->next->next; // Set the next node of the head to the next node of the temporary node
        temp->destroyNode(temp->data); // Destroy the data of the temporary node
        free(temp); // Free the temporary node
        curr = NULL; // Set the current node to NULL
        temp = NULL; // Set the temporary node to NULL
        return success;
    } else { // If the head is not the node to delete
        while(curr != NULL && !curr->compare(curr->data, toDelete)) { // Iterate until the node is found
            curr = curr->next;
        }
        if(!curr) return failure; // If the current node is NULL return failure
        if(!curr->compare(curr->data, toDelete) && curr->next == NULL) return failure;
        // If the last node is not the node to delete return failure
        if(curr->next != NULL) { // If the next node of the current node is not NULL
            LinkedList temp = curr; // Set a temporary node to the current node
            curr->next->prev = temp->prev; // Set the previous node of the next node of the current node to the previous node of the temporary node
            curr->prev->next = temp->next; // Set the next node of the previous node of the current node to the next node of the temporary node
            temp->destroyNode(temp->data); // Destroy the data of the temporary node
            free(temp); // Free the temporary node
            temp = NULL; // Set the temporary node to NULL
            return success;
        }
        else { // If the next node of the current node is NULL
            LinkedList temp = curr; // Set a temporary node to the current node
            curr->next = NULL; // Set the next node of the current node to NULL
            curr->prev->next = NULL; // Set the next node of the previous node of the current node to NULL
            temp->destroyNode(temp->data); // Destroy the data of the temporary node
            free(temp); // Free the temporary node
            temp = NULL; // Set the temporary node to NULL
            return success;
        }
    }
    return failure;
}

Element getData(LinkedList l) { // A function that returns the data of the linked list
    if (l==NULL) return NULL; // If the linked list is NULL return NULL
    if(l->data == NULL) return NULL; // If the data is NULL return NULL
    return l->data;
}

LinkedList getNext(LinkedList l) { // A function that returns the next node of the linked list
    return l->next; // Return the next node of the linked list
}

status updateData(LinkedList l, UpdateFunction update) { // A function that updates the data of the linked list
    if (!l || !update) return failure; // If the linked list or the update function is NULL return failure
    LinkedList curr = l; // Set the current node to the linked list
    status s = success;
    while (!isEmpty(curr)) { // While the current node is not empty
        s = max(update(curr->data), s); // Set the status to the maximum of the status and the update function of the data of the current node
        curr = curr->next; // Set the current node to the next node
    }
    return s;
}

Element searchByKeyInList(LinkedList l, Element key) { // A function that searches for a node in the linked list by a key
    if (!l || !key) return NULL; // If the linked list or the key is NULL return NULL
    LinkedList curr = l; // Set the current node to the linked list
    while (!isEmpty(curr)) { // While the current node is not empty
        if (curr->compare(curr->data, key)) { // If the data of the current node is equal to the key
            return curr->data; // Return the data of the current node
        }
        curr = curr->next; // Iterate to the next node
    }
    return NULL; // If the node is not found return NULL
}

Element searchClosest(LinkedList l, float value, char *key, CompareFunction compareFunction) {
    if (isEmpty(l) || !value) return NULL; // If the linked list is empty or the value is NULL return NULL
    if (!l->next) return l->data; // If the next node of the linked list is NULL return the data of the linked list
    LinkedList closest = NULL, curr; // Set the closest node to NULL and the current node to the linked list
    float closestValueFromKey = compareFunction(l->data, value, key);
    // Set the closest value from the key to the compare function of the data of the linked list
    closest = l; // Set the closest node to the linked list
    curr = l->next; // Iterate to the next node
    while (curr != NULL) { // While the current node is not NULL
        float diff = compareFunction(curr->data, value, key); // Set the difference to the compare function of the data of the current node
        if (diff == 0.0) return curr->data; // If the difference is 0 return the data of the current node
        if (diff == -1.0) continue; // If the difference is -1 continue
        if(diff == closestValueFromKey) { // If the difference is equal to the closest value from the key
            curr= curr->next; // Iterate to the next node
            continue;
        }
        if (diff < closestValueFromKey) { // If the difference is less than the closest value from the key
            closest = curr; // Set the closest node to the current node
            closestValueFromKey = diff; // Set the closest value from the key to the difference
        }
        curr = curr->next; // Iterate to the next node
    }
    if(closest == NULL) return NULL; // If the closest node is NULL return NULL
    return closest->data; // Return the data of the closest node
}


int getLengthList(LinkedList l) { // A function that returns the length of the linked list
    if (l == NULL || l->data == NULL) return 0; // If the linked list is NULL or the data is NULL return 0
    int i = 0; // Set the counter to 0
    while (l != NULL && l->data != NULL) { // While the linked list is not NULL and the data is not NULL
        i++; // Increment the counter
        l = l->next; // Iterate to the next node
    }
    return i; // Return the counter
}


