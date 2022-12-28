#include "Defs.h"
#include "LinkedList.h"
#include "MultiValueHashTable.h"
#include "HashTable.h"

typedef struct PlanetStruct { // Defines a new type called "Planet" that contains a name and x,y,z cords
    float x, y, z;
    char *name;

//    void (*printPlanet)(void *p); // A function pointer that points to a function that prints a planet
//    PrintFunction printPlanet;
//    FreeFunction destroyPlanet;

//    void (*initPlanet)(float x, float y, float z,
//                       char *name); // A function pointer that points to a function that initializes a planet

//    status (*destroyPlanet)(void *p); // A function pointer that points to a function that destroys a planet
} Planet;

status destroyPlanet(Element p); // A function that destroys a planet

Planet *initPlanet(float x, float y, float z,
                   char *name); // A function that initializes a planet, taking its x,y,z cords and name

Planet *locatePlanet(char *name, Planet **ps, int numOfPlanets); // A function that locates a planet in an array of planets

typedef struct OriginStruct { // Defines a new type called "Origin" that contains a dimension name and a planet
    Planet *planet; // A planet pointer
    char *dimName; // A dimension name

//    PrintFunction printOrigin;
//    PrintFunction printPlanet;
//    FreeFunction destroyOrigin;

//    void (*printOrigin)(void *o); // A function pointer that points to a function that prints an origin
//
//    void (*printPlanet)(Planet *p); // A function pointer that points to a function that prints a planet
//
//    void (*initOrigin)(Planet *planet,
//                       char *dimName); // A function pointer that points to a function that initializes an origin
//
//    void (*destroyOrigin)(void *o); // A function pointer that points to a function that destroys an origin
} Origin;

void destroyOrigin(Origin *o); // A function that destroys an origin

Origin *initOrigin(Planet *planet,
                   char *dimName); // A function that initializes an origin, taking a planet and a dimension name

status printOrigin(Element o); // A function that prints an origin

status printPlanet(Element p); // A function that prints a planet


typedef struct CharacStruct { // Defines a new type called "PhysicalCharacteristics" that contains a name and a float amount
    float amount; // A float amount for the physical characteristic
    char *name; // A name for the physical characteristic

//    void (*printPc)(Element p); // A function pointer that points to a function that prints a physical characteristic
//
//    void (*initPc)(float amount,
//                   char *name); // A function pointer that points to a function that initializes a physical characteristic
//
//    status
//    (*destroyPc)(Element p); // A function pointer that points to a function that destroys a physical characteristic
} PhysicalCharacteristics;

PhysicalCharacteristics *
initPc(float amount, char *name); // A function that initializes a physical characteristic, taking an amount and a name

status destroyPc(PhysicalCharacteristics *p); // A function that destroys a physical characteristic

void printPc(PhysicalCharacteristics *p); // A function that prints a physical characteristic

status printPlanet(Element p); // A function that prints a planet


typedef struct JerryStruct { // Defines the Jerry structure.
    int happiness; // Happiness level as int
    Origin *org; // Origin of the Jerry - referencing to the Origin Struct defined in Defs.h
    int numOfPcs; // Number of PhysicalCaracteristics
    char *ID; // Jerry's ID as an array of chars
    PhysicalCharacteristics **pcs; // Array of PhysicalCharacteristics for the Jerry

//    int (*doesJerryHavePc)(char *name, void *j);
//    // This pointer points to a function checks if the Jerry has a PhysicalCharacteristic with the given name. returns true or false as bool.
//
//    status (*addPcToJerry)(PhysicalCharacteristics *pc, void *j);
//    // This pointer points to a function adds a PhysicalCharacteristic to the Jerry. returns a status code.
//
//    status (*removePcFromJerry)(char *name, void *j);
//    // This pointer points to a function removes a PhysicalCaracteristic from the jerry, going by the name. returns a status code.
//
//    void (*printJerryInfo)(void *j);
//    // This pointer points to a function which prints the Jerry's info.
//
//    status (*printJerriesPcs)(void *j);
//    // This pointer points to a function prints all the PhysicalCharacteristics of the Jerry.
//
//    void (*initJerry)(char *name, Origin *org, int happiness);
//    // This pointer points to a function initializes the Jerry. taking its Name, Origin and Happiness level.
//
//    void (*destroyJerry)(void *j); // This pointer points to a function that destroys the Jerry.
//    // This pointer points to a function which destroys the Jerry.

} Jerry;

// Jerry's inner function defs section:

Jerry *initJerry(char *name, Origin *org, int happiness);
// This function initializes the Jerry. taking its Name, Origin and Happiness level.

status destroyJerry(Element j);
// This function destroys the Jerry.

int doesJerryHavePc(char *name, Jerry *j);
// This function checks if the Jerry has a PhysicalCharacteristic with the given name.
// returns the index of the PhysicalCharacteristic. -1 if not found.

status addPcToJerry(PhysicalCharacteristics *pc, Jerry *j);
// This function adds a PhysicalCharacteristic to the Jerry.

status removePcFromJerry(char *name, Jerry *j);
// This function removes a PhysicalCaracteristic from the jerry, going by the name.

status printJerriesPcs(Element j);
// This function prints all the PhysicalCharacteristics of the Jerry.

status printJerryInfo(Element j);
// This function prints the Jerry's info.

status printAllJerries(Element l);

status readFromConfig(char *url, int numOfPlanets, Planet **planets,
                      LinkedList jerries, LinkedList jerriesSorted, int *numOfJerries,
                      hashTable byID, MultiValueHashTable byPC, MultiValueHashTable byPlanet);