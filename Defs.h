#ifndef DEFS_H_
#define DEFS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum e_bool {
    false, true
} bool;
typedef enum e_status {
    success, failure, memory_error
} status;

typedef void *Element;

typedef Element(*CopyFunction)(Element);

typedef status(*FreeFunction)(Element);

typedef status(*PrintFunction)(Element);

typedef int(*TransformIntoNumberFunction)(Element);

typedef bool(*EqualFunction)(Element, Element);

char *strupr(char *origin); // A function that converts a string to uppercase

typedef status(*UpdateFunction)(Element);

typedef float(*CompareFunction)(Element, float, char*);

int min(int a, int b);

int max(int a, int b);

#endif /* DEFS_H_ */
