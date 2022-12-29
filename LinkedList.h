#include "Defs.h"

typedef struct LinkedList_s *LinkedList;

LinkedList createLinkedList(FreeFunction destroyNode, PrintFunction printNode, EqualFunction compare,
                            CopyFunction copy);

bool isEmpty(LinkedList l);

status destroyList(Element l);

status appendNode(LinkedList l, Element toAdd);

status appendCondition(LinkedList l, Element toAdd, EqualFunction cmp);

status deleteNode(LinkedList l, Element toDelete);

status displayNode(Element l);

status displayList(Element l);

Element getData(LinkedList l);

LinkedList getNext(LinkedList l);

status updateData(LinkedList l, UpdateFunction update);

int getLengthList(LinkedList l);

Element searchByKeyInList(LinkedList l, Element key);

Element searchClosest(LinkedList l, float value, char *key, CompareFunction compareFunction);


status setDataByIndex(LinkedList l, float value_to_compare, char *nameOfValue, CompareFunction compareFunction);



