#include "LinkedList.h"
#include <stdlib.h>
#include "KeyValuePair.h"

typedef struct LinkedList_s {
    Element data;
    struct LinkedList_s *next;
    struct LinkedList_s *prev;
    struct LinkedList_s *head;

    FreeFunction destroyNode;
    PrintFunction printNode;
    EqualFunction compare;
    CopyFunction copy;
} node;

LinkedList createLinkedList(FreeFunction destroyNode, PrintFunction printNode, EqualFunction compare,
                            CopyFunction copy) {
    LinkedList n = malloc(sizeof(struct LinkedList_s));
    if (n == NULL) {
        //Memory Error
        return NULL;
    }
    n->destroyNode = destroyNode;
    n->printNode = printNode;
    n->compare = compare;
    n->copy = copy;
    n->next = NULL;
    n->prev = NULL;
    n->head = NULL;
    n->data = NULL;
    return n;
}

status destroyList(LinkedList l) {
    if (!l || !l->data) return success;
    status flag = success;
    int count = 1;
    while (l && l->data) {
        LinkedList temp = l;
//        FreeFunction ff = l->destroyNode;
        //flag = flag > l->destroyNode(l) ? failure : success;
        l = l->next;
        temp->destroyNode(temp);
        if (flag == failure) return failure;
        // TODO: free data
    }
    return success;
}

bool notLast(LinkedList n) {
    return n->next != NULL;
}

bool isEmpty(LinkedList l) {
    if (!l) return true;
    else if (!l->data) return true;
    else return false;
}

status appendNode(LinkedList l, Element toAdd) {
    if (l == NULL) return failure;

    if (l->head == NULL) {
        l->data = l->copy(toAdd);
        l->head = l;
        return success;
    }

    LinkedList l_toAdd = createLinkedList(l->destroyNode, l->printNode, l->compare, l->copy);
    if (l_toAdd == NULL) return failure;
    l_toAdd->data = l->copy(toAdd);
    l_toAdd->head = l->head;
    while (notLast(l)) {
        l = l->next;
    }
    l_toAdd->prev = l;
    l->next = l_toAdd;
    return success;
}

status displayNode(Element e) {
    LinkedList l = (LinkedList) e;
    if (!l) return failure;
    if (!l->data) {
        printf("Got an empty node \n");
        return failure;
    }
    PrintFunction pf = l->printNode;
    pf(l->data);
    return success;
}

status appendCondition(LinkedList l, Element toAdd, EqualFunction cmp) {
    if (!l || !toAdd || !cmp) return failure;
    if(!l->data) return appendNode(l,toAdd);
    LinkedList newl= createLinkedList(l->destroyNode, l->printNode, l->compare, l->copy);
    if(!newl) return failure;
    newl->data = l->copy(toAdd);
    newl->head = l->head;
    if(!l->data || cmp(l->data,toAdd) == true){
        newl->next = l;
        l->prev = newl;
        l->head = newl;
        return success;
    } else {
        LinkedList current = l;
        while(current->next && cmp(current->next->data,toAdd) == false){
            current = current->next;
        }

        newl->next = current;
        newl->prev = current->prev;
        current->prev = newl;
        return success;
    }
}

status displayList(Element e) {
    LinkedList l = (LinkedList) e;
    if (!l) return failure;
    if (!l->data) {
        return failure; // failure
    }
    PrintFunction pf;
    while (l && l->data) {
        pf = l->printNode;
        if (pf(l->data) == success) printf("\n");
        l = l->next;
    }
    return success;
}

status deleteNode(LinkedList l, Element toDelete) {
    if (!l || !toDelete) return failure;
    while (notLast(l)) {
        if (l->compare(l->data, toDelete)) {
            if (!l->prev) { // If first
                if (!l->next) { // If only one node
                    l->data = NULL;
                    l->head = NULL;
                    l->destroyNode(l);
                    return success;
                } else { // If first but not only one node
                    l->next->prev = NULL;
                    l->head = l->next;
                    l->data = NULL;
                    *l = *(l->head);
                    return success;
                }
            } else if (l->next != NULL) { // if not first and has next
                LinkedList temp = l;
                l->prev->next = l->next;
                l->next->prev = l->prev;
                temp->data = NULL;
                return success;
            } else { // if last
                l->prev->next = NULL;
                l->data = NULL;
                return success;
            }
        }
        l = l->next;
    }
    if (l->compare(l->data, toDelete)) {
        if (!l->prev) { // If first
            if (!l->next) { // If only one node
                l->data = NULL;
                l->head = NULL;
                return success;
            }
        }
    }
    return failure;
}

Element getData(LinkedList l) {
    if (!l) return NULL;
    return l->data;
}

LinkedList getNext(LinkedList l) {
    return l->next;
}

Element searchByKeyInList(LinkedList l, Element key) {
    if (!l || !key) return NULL;
    while (l && l->data) {
        if (l->compare(l->data, key)) {
            return l->data;
        }
        l = l->next;
    }
    return NULL;
}

int getLengthList(LinkedList l) {
    if (!l || !l->data) return 0;
    int i = 0;
    while (l && l->data) {
        i++;
        l = l->next;
    }
    return i;
}


