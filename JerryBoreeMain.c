#include "KeyValuePair.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "Jerry.h"
#include "MultiValueHashTable.h"
#include "Defs.h"
#include <string.h>


#define debug

#define BUFFER 300


status printString(Element e) {
    char *c = (char *) e;
    printf("%s", c);
    return success;
}

bool compareString(Element a, Element b) {
    return strcmp((char *) a, (char *) b) == 0;
}

bool compareKey(Element a, Element b) {
    KeyValuePair k = (KeyValuePair) a;
    return compareString(getKey(k), b);
}

bool compareValue(Element a, Element b) {
    LinkedList l = (LinkedList) a;
    return compareString(getData(l), b);
}

status destroyString(Element e) {
    free(e);
    return success;
}

status doNothing(Element e) {
    return success;
}


Element copyString_deep(Element copyFrom) {
    char *c = (char *) copyFrom;
    char *copy = malloc(strlen(c) + 1);
    strcpy(copy, c);
    return (Element) copy;
}


int HashFunction(Element e) {
    char *c = (char *) e;
    int sum = 0;
    for (int i = 0; i < strlen(c); i++) {
        sum += c[i];
    }
    return sum;
}

Element shallowCopy(Element e) {
    return e;
}

bool equalJerry(Element a, Element b) {
    Jerry *j1 = (Jerry *) a;
    Jerry *j2 = (Jerry *) b;
    return strcmp(j1->ID, j2->ID) == 0;
}

bool equalPlanet(Element a, Element b) {
    Planet *p1 = (Planet *) a;
    Planet *p2 = (Planet *) b;
    return strcmp(p1->name, p2->name) == 0;
}

bool comparePlanet(Element a, Element b) {
    Jerry *j = (Jerry *) a;
    return strcmp(j->org->planet->name, (char *) b) == 0;
}

bool comparePC(Element a, Element b) {
    Jerry *j = (Jerry *) a;
    return doesJerryHavePc((char *)b, j) == -1;
}

Element copyPlanet_shallow(Element p) {
    return (Element) p;
}

status printInt(Element e) {
    char *i = (char *) e;
    int j = atoi(i);
    printf("%d", j);
    return success;
}

#ifndef debug
status printJerries(LinkedList jerries) { // A function that prints all the Jerries
    for (int i = 1; i <= numOfJerries; i++) { // For each Jerry
        printJerryInfo((Jerry *) getDataByIndex(jerries, i)); // Print the Jerry
    }
    return success;
}
#endif



bool compareStringValue(Element a, Element b) {
    return strcmp((char *) a, (char *) b)<0;
}

bool compareIntValue(Element a, Element b) {
    return *(int *) a < *(int *) b;
}



int main() {
//    LinkedList a = createLinkedList(doNothing, printInt, compareIntValue, shallowCopy);
//    appendCondition(a,"1", compareIntValue);
//    appendCondition(a,"5", compareIntValue);
//    appendCondition(a,"3", compareIntValue);
//    appendCondition(a,"8", compareIntValue);

//    appendNode(a, "1");
//    appendNode(a, "2");
//    displayList(a);
#ifdef debug
    int numOfJerries = 0, numOfPlanets = 4;
    Planet **planets = calloc(sizeof(Planet *), numOfPlanets);// Allocating memory for the planets array
    if (!planets) { // If the allocations failed
//        MemoryError = true; // Set the memory error flag to true
        return failure;
    }
    LinkedList jerries = createLinkedList(destroyJerry, printJerryInfo, equalJerry,
                                          shallowCopy); // Create a linked list of Jerries

    LinkedList byHappiness = createLinkedList(destroyJerry, printJerryInfo, equalJerry, shallowCopy);

    // TODO: Jerry id hash
// TODO: Jerry PC MultiHash
// TODO: Planet MultiValueHash (Jerry id)
// TODO: Saddest Jerry linked list (LIFO)
    hashTable byID = createHashTable(copyString_deep, destroyString, printString, shallowCopy, destroyJerry, printJerryInfo,
                                     compareKey, HashFunction, 3);
    MultiValueHashTable byPC = createMultiValueHashTable(copyString_deep, destroyString, printString, shallowCopy,
                                                         destroyJerry, printJerryInfo, compareKey, comparePC,
                                                         HashFunction, 4);
    MultiValueHashTable byPlanet = createMultiValueHashTable(copyString_deep, destroyString, printString, shallowCopy,
                                                             destroyJerry, printJerryInfo, compareKey, comparePlanet,
                                                             HashFunction, 4);

    readFromConfig("../configuration_file.txt", numOfPlanets, planets, jerries, byHappiness, &numOfJerries, byID, byPC, byPlanet); // Read the configuration file
    LinkedList l = createLinkedList(destroyJerry, printJerryInfo, equalJerry, shallowCopy);
//    displayList(jerries);
//    displayMultiValueHashElements(byPlanet);
//    printf("By ID: \n");
//    KeyValuePair k = (KeyValuePair )lookupInHashTable(byID, "S5d2");
//    Jerry *j = (Jerry *) getValue(k);
//    if(j) {printJerryInfo(j);printf("\n");} else printf("Jerry not found\n");
//    printf("By PC: \n");
//    displayMultiValueHashElementsByKey(byPC,"Age");
//    printf("By Planet: \n");
//    displayMultiValueHashElementsByKey(byPlanet,"Earth");
    char c = '0'; // A variable for the user's choice
    while (c != '9') { // While the user didn't choose to exit
//        if (MemoryError) { // If there was a memory error
//            //destroyEverything(); // Destroy everything
//            printf("Memory Error"); // Print "Memory Error"
//            break; // Break the loop
//        }
        // Print the menu
        printf("Welcome Rick, what are your Jerry's needs today ? \n");
        printf("1 : Take this Jerry away from me \n");
        printf("2 : I think I remember something about my Jerry \n");
        printf("3 : Oh wait. That can't be right \n");
        printf("4 : I guess I will take back my Jerry now \n");
        printf("5 : I can't find my Jerry. Just give me a similar one \n");
        printf("6 : I lost a bet. Give me your saddest Jerry \n");
        printf("7 : Show me what you got \n");
        printf("8 : Let the Jerries play \n");
        printf("9 : I had enough. Close this place \n");
        scanf(" %c", &c); // Get the user's choice
        while (getchar() != '\n'); // Clear the buffer


        switch (c) { // Switch the user's choice
            case '1': // If the user chose
                printf("What is your Jerry's ID ? \n");
                char id[30]; // A variable for the Jerry's ID
                scanf("%s", id);
                KeyValuePair k = (KeyValuePair )lookupInHashTable(byID, id);
                if(k) {
                    printf("Rick did you forgot ? you already left him here ! \n");
                    break;
                }
                Jerry *j = (Jerry *) getValue(k);
                if(j) {
                    printf("Rick did you forgot ? you already left him here ! \n");
                    break;
                }
                printf("What planet is your Jerry from ? \n");
                char planet[30]; // A variable for the Jerry's planet
                scanf("%s", planet);
                Planet *p = locatePlanet(planet, planets, numOfPlanets);
                if(!p) {
                    printf("%s is not a known planet ! \n", planet);
                    break;
                }
                printf("What is your Jerry's dimension ? \n");
                char dimension[30]; // A variable for the Jerry's dimension
                scanf("%s", dimension);
                printf("How happy is your Jerry now ? \n");
                int happiness;
                scanf("%d", &happiness);
                j = initJerry(id, initOrigin(p, dimension), happiness);
                appendNode(jerries, j);
                //appendCondition(byHappiness, j, compareHappiness);
                addToHashTable(byID, j->ID, j);
                addToMultiValueHashTable(byPlanet, j->org->planet->name, j);
                printJerryInfo(j);

                break;

            case '2': //
                printf("What is your Jerry's ID ? \n");
                char id2[30]; // A variable for the Jerry's ID
                scanf("%s", id2);
                KeyValuePair k2 = (KeyValuePair )lookupInHashTable(byID, id2);
                if(!k2) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                Jerry *j2 = (Jerry *) getValue(k2);
                if(!j2) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }

                printf("What physical characteristic can you add to Jerry - %s ? \n", id2);
                char pc2[30];
                scanf("%s", pc2);
                if(doesJerryHavePc(pc2, j2) > 0) {
                    printf("The information about his %s already available to the daycare ! \n", pc2);
                    break;
                }
                printf("What is the value of his %s ? \n", pc2);
                float amount2;
                scanf("%f", &amount2);
                addPcToJerry(initPc( amount2,pc2), j2);
                addToMultiValueHashTable(byPC, pc2, j2);
                displayMultiValueHashElementsByKey(byPC,pc2);
                break;

            case '3': // If the user chose to
                printf("What is your Jerry's ID ? \n");
                char id3[30]; // A variable for the Jerry's ID
                scanf("%s", id3);
                KeyValuePair k3 = (KeyValuePair )lookupInHashTable(byID, id3);
                if(!k3) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                Jerry *j3 = (Jerry *) getValue(k3);
                if(!j3) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                printf("What physical characteristic do you want to remove from Jerry - %s ? \n", id3);
                char pc3[30];
                scanf("%s", pc3);
                if(doesJerryHavePc(pc3, j3) < 0) {
                    printf("The information about his %s is not available to the daycare ! \n", pc3);
                    break;
                }
                removePcFromJerry(pc3, j3);
                removeFromMultiValueHashTable(byPC, pc3, j3);
                printJerryInfo(j3);
                break;

            case '4': // If the user chose to
                printf("What is your Jerry's ID ? \n");
                char id4[30]; // A variable for the Jerry's ID
                scanf("%s", id4);
                KeyValuePair k4 = (KeyValuePair )lookupInHashTable(byID, id4);
                if(!k4) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                Jerry *j4 = (Jerry *) getValue(k4);
                if(!j4) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                removeFromHashTable(byID, id4);
                removeFromMultiValueHashTable(byPlanet, j4->org->planet->name, j4);
                for(int i =0; i < j4->numOfPcs; i++){
                    removeFromMultiValueHashTable(byPC, j4->pcs[i]->name, j4);
                }
                deleteNode(jerries, j4);
                break;

            case '5': // If the user chose to
                //
                break;

            case '6': // If the user chose to
                //
                break;

            case '7': // If the user chose to
                printf("What information do you want to know ? \n");
                printf("1 : All Jerries \n");
                printf("2 : All Jerries by physical characteristics \n");
                printf("3 : All known planets \n");
                char c2;
                scanf(" %c", &c2);
                while (getchar() != '\n'); // Clear the buffer
                switch (c2) {
                    case '1':

                        if(isEmpty(jerries)){
                            printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                            break;
                        }
                        displayList(jerries);
                        break;
                    case '2':
                        printf("What physical characteristic do you want to know ? \n");
                        char pc7[30];
                        scanf("%s", pc7);
                        if(isEmpty(searchByKeyInTable(byPC, pc7))){
                            printf("Rick we can not help you - we do not know any Jerry's %s ! \n", pc7);
                            break;
                        }
                        printf("%s : \n", pc7);
                        displayMultiValueHashElementsByKey(byPC, pc7);
                        break;
                    case '3':
                        for (int i = 0; i < numOfPlanets; ++i) {
                            printPlanet(planets[i]);
                        }
                        break;
                    default:
                        printf("Rick this option is not known to the daycare ! \n");
                        break;
                }
                break;

            case '8': // If the user chose to
                printf("What activity do you want the Jerries to partake in ? \n");
                printf("1 : Interact with fake Beth \n");
                printf("2 : Play golf \n");
                printf("3 : Adjust the picture settings on the TV \n");
                char c3;
                scanf(" %c", &c3);
                while (getchar() != '\n'); // Clear the buffer
                switch (c3){
                    case '1':
                        break;
                    case '2':
                        break;
                    case '3':
                        break;
                    default:
                        printf("Rick this option is not known to the daycare ! \n");
                        break;
                }
                break;

            case '9': // If the user chose to
                //
                printf("The daycare is now clean and close ! \n");
                break;

            default: // If the user chose an invalid option
                printf("Rick this option is not known to the daycare ! \n");
        }
    }
#endif
#ifdef debug

//    KeyValuePair kvp = createKeyValuePair("key", "value", destroyString, destroyString,
//                                          printString, printString,
//                                          compareKey, compareString, copyString_shallow, copyString_shallow);

//    hashTable h = createHashTable(copyString_shallow, doNothing, printString, copyString_shallow, doNothing,
//                                  printString, compareKey, HashFunction, 10);


//    MultiValueHashTable h = createMultiValueHashTable(copyString_shallow, destroyString, printString,
//                                                      copyString_shallow,
//                                                      doNothing, printString,
//                                                      compareKey, compareString, HashFunction, 10);

#endif
    return 0;
}

