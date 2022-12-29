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

status destroyList(Element e) {
    LinkedList l = (LinkedList) e;
//    LinkedList head = l->head;
    if (!l) { return empty; }
    status flag = success;

    if (!l->next) {
        flag = l->destroyNode(l->data);
        free(l);
        if (flag == success) return empty;
        else return failure;
    }
    while (l) {
        LinkedList temp = l;
        l = l->next;
        flag = max(temp->destroyNode(temp->data), flag);
        temp->data = NULL;
        free(temp);
        temp = NULL;
        if (flag != success) return failure;
    }
    l = NULL;
    e = NULL;
    return flag;
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
    if (l_toAdd == NULL) return memory_error;
    l_toAdd->data = l->copy(toAdd);
    l_toAdd->head = l->head;

    while (notLast(l)) {
        l = l->next;
    }
    l_toAdd->prev = l;
    l->next = l_toAdd;
    return success;
}

status switchData(LinkedList a, LinkedList b) {
    if (a == NULL || b == NULL) return failure;
    Element temp = a->data;
    a->data = b->data;
    b->data = temp;
    return success;
}

status appendCondition(LinkedList l, Element toAdd, EqualFunction cmp) {
    if (!l || !toAdd || !cmp) return failure;
    if (!l->data) {
        l->data = l->copy(toAdd);
        l->head = l;
        l->next = NULL;
        return success;
    }

    LinkedList head = l->head;
    LinkedList newl = createLinkedList(head->destroyNode, head->printNode, head->compare, head->copy);
    if (!newl) return failure;
    newl->data = newl->copy(toAdd);
    newl->head = head->head;
    if (cmp(head->data, toAdd)) { // if the new element is smaller than the head
        switchData(head, newl); // head is now the node we want in the start
        newl->next = head->next;
        if(head->next) head->next->prev = newl;
        head->next = newl;
        newl->prev = head;
        newl->head = head;
        return success;
    }
    if(head->next == NULL){
        head->next = newl;
        newl->prev = head;
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
        newl->next = head;
        newl->prev = head->prev;
        head->prev->next = newl;
        head->prev = newl;
        return success;
    }
}

status displayList(Element e) {
    LinkedList l = (LinkedList) e;
    if (l == NULL) {
        return failure; // failure
    }
    l = l->head;
    if(l->data == NULL) return empty;
    PrintFunction pf;
    while (!isEmpty(l)) {
        pf = l->printNode;
        if (pf(l->data) == success) printf("\n");
        l = l->next;
    }
    return success;
}

status doNothing2(Element e) {
    return success;
}

status deleteNode(LinkedList l, Element toDelete) {
    if (!l) return failure;
    LinkedList curr = l->head;
    if (!curr) return failure;
    if (!curr->data) return failure;
    if (curr->compare(curr->data, toDelete)) { // if head is the node to delete
        if(curr->next == NULL){ // if head is the only node
            curr->destroyNode(curr->data);
            l->head = NULL;
            curr->data = NULL;
            free(l);
            printf("Empty on %s \n", (char*)toDelete);
            return empty;
        }
        // if head is not the only node
        LinkedList temp = curr->next;
        curr->destroyNode(curr->data);
        curr->data = NULL;
        switchData(curr, temp);
        curr->next = curr->next->next;
        temp->destroyNode(temp->data);
        free(temp);
        temp = NULL;
        return success;
    } else {
        while(curr != NULL && !curr->compare(curr->data, toDelete)) {
            curr = curr->next;
        }
        if(!curr) return failure;
//        if(!curr->compare(curr->data, toDelete)) return failure;
        if(curr->next != NULL) {
            LinkedList temp = curr;
            curr->next->prev = temp->prev;
            curr->prev->next = temp->next;
            temp->destroyNode(temp->data);
            free(temp);
            temp = NULL;
            printf("Im here dn\n");
            return success;
        }
        else {
            LinkedList temp = curr;
            curr->prev->next = NULL;
            temp->destroyNode(temp->data);
            free(temp);
            temp = NULL;
            return success;
        }
    }
    return failure;
    /*
    status s = success;
    LinkedList head = (*l);
    if (!head || !toDelete) return failure;
    LinkedList curr = head;
    if(head->compare(head->data, toDelete)) { // if head is the node to delete
        if(isEmpty(head->next)) { // if head is the only node
            s=max(s,head->destroyNode((head)->data));
            head->data = NULL;
            free(*l);
            *l = NULL;
            return empty;
            printf("I'm here for some reason\n");
        }
        // if head is not the only node
        curr->head = curr->next;
        (*l)=(*l)->next;
        curr->head = head;
        curr->prev = NULL;
        s = curr->destroyNode(curr->data);
        curr->data = NULL;
        free(curr);
        curr=NULL;

        return s;
    }
    while(!isEmpty(curr) && !curr->compare(curr->data, toDelete)) {
        curr = curr->next;
    }
    if (isEmpty(curr)) return failure;
    if(curr->compare(curr->data, toDelete)) {
        if(isEmpty(curr->next)) { // if curr is the last node
            printf("I'm here \n");
            curr->prev->next = NULL;
            s = curr->destroyNode(curr->data);
            free(curr);
            curr = NULL;
            return s;
        }
        curr->prev->next = curr->next; // If has before and next nodes
        curr->next->prev = curr->prev;
        s = curr->destroyNode(curr->data);
        free(curr);
        curr = NULL;
        return s;
    }
//    while (notLast(curr)) {
//        if (curr->compare(curr->data, toDelete)) {
//            if (!curr->prev) { // If first
//                if (!curr->next) { // If only one node
//                    printf("first only one\n");
//                    head->head = head;
//                    s = max(s, head->destroyNode(head->data));
//                    free(head);
//                    return s;
//                } else { // If first but not only one node
//                    LinkedList temp = curr;
//                    printf("first not only one\n");
//                    head->head = curr;
//                    head->prev = NULL;
//                    s = max(s, temp->destroyNode(temp->data));
//                    free(temp);
//                    return s;
//                }
//            } else if (curr->next != NULL) { // if not first and has next
//                printf("not first and has next \n");
//                LinkedList temp = curr;
//                curr->prev->next = curr->next;
//                curr->next->prev = curr->prev;
//                s = max(s, head->destroyNode(temp->data));
//                temp->data = NULL;
//                free(temp);
//                printf("Did it not first and has next \n");
//                return s;
//            } else { // if last
//                printf("last \n");
//                curr->prev->next = NULL;
//                s = max(s, head->destroyNode(curr->data));
//                curr->data = NULL;
//                free(curr);
//                printf("Did it last \n");
//
//                return s;
//            }
//        }
//        curr = curr->next;
//    }
//    if (head->compare(head->data, toDelete)) {
//        if (!head->prev) { // If first
//            if (!head->next) { // If only one node
//                head->destroyNode(head);
//                head->data = NULL;
//                free(head);
//                printf("Did it first and only one \n");
//
//                return success;
//            }
//        }
//    }
    return failure;*/
}

Element getData(LinkedList l) {
    if (!l) return NULL;
    return l->head->data;
}

LinkedList getNext(LinkedList l) {
    return l->next;
}

status updateData(LinkedList l, UpdateFunction update) {
    if (!l || !update) return failure;
    LinkedList curr = l;
    status s = success;
    while (!isEmpty(curr)) {
        s = max(update(curr->data), s);
        curr = curr->next;
    }
    return s;
}

Element searchByKeyInList(LinkedList l, Element key) {
    if (!l || !key) return NULL;
    LinkedList curr = l;
    while (!isEmpty(curr)) {
        if (curr->compare(curr->data, key)) {
            return curr->data;
        }
        curr = curr->next;
    }
    return NULL;
}

Element searchClosest(LinkedList l, float value, char *key, CompareFunction compareFunction) {
    if (isEmpty(l) || !value) return NULL;
    if (!l->next) return l->data;
    LinkedList closest = NULL, curr;
    float closestValueFromKey = compareFunction(l->data, value, key);
    curr = l->next;
    while (curr) {
        float diff = compareFunction(curr->data, value, key);
        if (diff == 0.0) return curr->data;
        if (diff == -1.0) continue;
        if (diff < closestValueFromKey) {
            closest = curr;
            closestValueFromKey = diff;
        }
        curr = curr->next;
    }
    return closest->data;
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


