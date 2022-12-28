#include "Defs.h"

typedef struct KeyValuePair_s *KeyValuePair;

KeyValuePair createKeyValuePair(Element key, Element value, FreeFunction destroyValue, FreeFunction destroyKey,
                                PrintFunction printValue, PrintFunction printKey,
                                EqualFunction compareKey, EqualFunction compareValue,
                                CopyFunction copyKey, CopyFunction copyValue);

status destroyKeyValuePair(Element kvp);

status displayValue(Element kvp);

status displayKey(Element kvp);

status displayKeyValuePair(Element kvp);

Element getKey(Element kvp);

Element getValue(Element kvp);

bool isEqualKey(Element kvp, Element key);

Element copyKeyValuePair(Element e);

bool isKeyValuePairValueEmpty(KeyValuePair kvp);

