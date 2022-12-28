typedef struct MultiValueHashTable_s *MultiValueHashTable;

MultiValueHashTable
createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue,
                          FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
                          EqualFunction equalValue,
                          TransformIntoNumberFunction transformIntoNumber, int hashNumber);

status destroyMultiValueHashTable(MultiValueHashTable mvht);

status addToMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value);

Element lookupInMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value);

LinkedList searchByKeyInTable(MultiValueHashTable m, Element key);

status removeFromMultiValueHashTable(MultiValueHashTable mvht, Element key, Element value);

status displayMultiValueHashElementsByKey(MultiValueHashTable mvht, Element key);

status displayMultiValueHashElements(MultiValueHashTable mvht);