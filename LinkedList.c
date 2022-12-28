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
    l=l->head;
    if (isEmpty(l)) return success;
    status flag = success;
    while (!isEmpty(l)) {
        LinkedList temp = l;
        l = l->next;
        flag = max(temp->destroyNode(temp->data), flag);
//        if (temp) free(temp);
        if (flag != success) return failure;
    }
    free(l);
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


LinkedList appendCondition(LinkedList l, Element toAdd, EqualFunction cmp) {
    LinkedList head = l->head;
    if (!l || !toAdd || !cmp) return l;
    if (!l->data) {
        appendNode(l, toAdd);
        return l;
    };
    LinkedList newl = createLinkedList(l->destroyNode, l->printNode, l->compare, l->copy);
    if (!newl) return NULL;
    newl->data = l->copy(toAdd);
    newl->head = l->head;
    if (cmp(l->data, toAdd)) {
        newl->next = l;
        l->prev = newl;
        newl->head = newl;
        head = newl;
        return head;
    }
    while (!isEmpty(l->next) && !cmp(l->next->data, toAdd)) {
        l = l->next;
    }
    if (cmp(l->data, toAdd)) {
        newl->next = l;
        newl->prev = l->prev;
        l->next->prev = newl;
        l->next = newl;
        return head;
    } else {
        if (isEmpty(l->next)) {
            l->next = newl;
            newl->prev = l;
            return head;
        } else {
            newl->next = l->next;
            newl->prev = l;
            l->next->prev = newl;
            l->next = newl;
            return head;
        }
    }
}

status displayList(Element e) {
    LinkedList l = (LinkedList) e;
    if (!l) return failure;
    if (!l->data) {
        return failure; // failure
    }
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

status deleteNode(LinkedList head, Element toDelete) {
    status s = success;
    printf("Trying to remove\n");
    if (!head || !toDelete) return failure;
    LinkedList curr = head;
    while (notLast(curr)) {
        if (curr->compare(curr->data, toDelete)) {
            if (!curr->prev) { // If first
                if (!curr->next) { // If only one node
                    printf("first only one\n");
                    head->head = head;
                    s = max(s, head->destroyNode(head->data));
                    free(head);
                    return s;
                } else { // If first but not only one node
                    LinkedList temp = curr;
                    printf("first not only one\n");
                    head->head = curr;
                    head->prev = NULL;
                    s = max(s, temp->destroyNode(temp->data));
                    free(temp);
                    return s;
                }
            } else if (curr->next != NULL) { // if not first and has next
                printf("not first and has next \n");
                LinkedList temp = curr;
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                s = max(s, head->destroyNode(temp->data));
                temp->data = NULL;
                free(temp);
                printf("Did it not first and has next \n");
                return s;
            } else { // if last
                printf("last \n");
                curr->prev->next = NULL;
                s = max(s, head->destroyNode(curr->data));
                curr->data = NULL;
                free(curr);
                printf("Did it last \n");

                return s;
            }
        }
        curr = curr->next;
    }
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
    return failure;
}

Element getData(LinkedList l) {
    if (!l) return NULL;
    return l->data;
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
    LinkedList closest, curr;
    float closestValueFromKey = compareFunction(l->data, value, key);
    curr = l->next;
    while (!isEmpty(curr)) {
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


