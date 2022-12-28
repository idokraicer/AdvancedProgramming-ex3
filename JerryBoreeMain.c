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
    char *c = (char *) e;
    free(c);
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
    return doesJerryHavePc((char *) b, j) > -1;
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

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

float absolute(float a) {
    return a > 0 ? a : -a;
}

float diffPC(Element e, float value, char* key){
    Jerry *j = (Jerry *) e;
    int pcIndex = doesJerryHavePc(key, j);
    if(pcIndex == -1) return -1;
    return absolute(j->pcs[pcIndex]->amount - value);
}

status interactWithFakeBeth(Element e) {
    Jerry *j = (Jerry *) e;
    if (!j) return failure;
    if (j->happiness >= 20) {
        j->happiness = min(j->happiness + 15, 100);
        return success;
    }
    j->happiness = max(j->happiness - 5, 0);
    return success;
}

status playGolf(Element e) {
    Jerry *j = (Jerry *) e;
    if (!j) return failure;
    if (j->happiness >= 50) {
        j->happiness = min(j->happiness + 10, 100);
        return success;
    }
    j->happiness = max(j->happiness - 10, 0);
    return success;
}

status adjustThePictureSettingsOn(Element e) {
    Jerry *j = (Jerry *) e;
    if (!j) return failure;
    j->happiness = min(j->happiness + 20, 100);
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
    return HashFunction((char *) a) > HashFunction((char *) b);
}

bool compareIntValue(Element a, Element b) {
    return *(int *) a < *(int *) b;
}


int main() {
#ifdef debug


    int numOfJerries = 0, numOfPlanets = 4;
    Planet **planets = createPlanetArray(numOfPlanets);
    if (!planets) { // If the allocations failed
        return memory_error; // Return the memory error code
    }

    LinkedList jerries = createLinkedList(destroyJerry, printJerryInfo, equalJerry,
                                          shallowCopy); // Create a linked list of Jerries
    if (!jerries) return memory_error;
    LinkedList byHappiness = createLinkedList(doNothing, printJerryInfo, equalJerry, shallowCopy);
    if (!byHappiness) return memory_error;
// TODO: Saddest Jerry linked list (LIFO)
    hashTable byID = createHashTable(copyString_deep, destroyString, printString, shallowCopy, doNothing,
                                     printJerryInfo,
                                     compareKey, HashFunction, 3);
    if (!byID) return memory_error;

    MultiValueHashTable byPC = createMultiValueHashTable(copyString_deep, destroyString, printString, shallowCopy,
                                                         doNothing, printJerryInfo, compareKey, comparePC,
                                                         HashFunction, 4);
    if (!byPC) return memory_error;

    MultiValueHashTable byPlanet = createMultiValueHashTable(copyString_deep, destroyString, printString, shallowCopy,
                                                             doNothing, printJerryInfo, compareKey, comparePlanet,
                                                             HashFunction, 4);

    if (!byPlanet) return memory_error;

    status s = success;
    s = max(readFromConfig("../configuration_file.txt", numOfPlanets, planets, jerries, &byHappiness, &numOfJerries,
                           byID, byPC,
                           byPlanet), s); // Read the configuration file

    Jerry *saddest = NULL;
    char c = '0'; // A variable for the user's choice
    while (c != '9' && s != memory_error) { // While the user didn't choose to exit
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
                char id[BUFFER]; // A variable for the Jerry's ID
                scanf("%s", id);
                KeyValuePair k = (KeyValuePair) lookupInHashTable(byID, id);
                if (k) {
                    printf("Rick did you forgot ? you already left him here ! \n");
                    break;
                }
                Jerry *j = (Jerry *) getValue(k);
                if (j) {
                    printf("Rick did you forgot ? you already left him here ! \n");
                    break;
                }
                printf("What planet is your Jerry from ? \n");
                char planet[BUFFER]; // A variable for the Jerry's planet
                scanf("%s", planet);
                Planet *p = locatePlanet(planet, planets, numOfPlanets);
                if (!p) {
                    printf("%s is not a known planet ! \n", planet);
                    break;
                }
                printf("What is your Jerry's dimension ? \n");
                char dimension[BUFFER]; // A variable for the Jerry's dimension
                scanf("%s", dimension);
                printf("How happy is your Jerry now ? \n");
                int happiness;
                scanf("%d", &happiness);
                j = initJerry(id, initOrigin(p, dimension), happiness);
                appendNode(jerries, j);
                byHappiness = appendCondition(byHappiness, j, compareHappiness);
                //appendCondition(byHappiness, j, compareHappiness);
                addToHashTable(byID, j->ID, j);
                addToMultiValueHashTable(byPlanet, j->org->planet->name, j);
                printJerryInfo(j);

                break;

            case '2': //
                printf("What is your Jerry's ID ? \n");
                char id2[BUFFER]; // A variable for the Jerry's ID
                scanf("%s", id2);
                KeyValuePair k2 = (KeyValuePair) lookupInHashTable(byID, id2);
                if (!k2) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                Jerry *j2 = (Jerry *) getValue(k2);
                if (!j2) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }

                printf("What physical characteristic can you add to Jerry - %s ? \n", id2);
                char pc2[BUFFER];
                scanf("%s", pc2);
                pc2[strlen(pc2)] = '\0';
                if (doesJerryHavePc(pc2, j2) > 0) {
                    printf("The information about his %s already available to the daycare ! \n", pc2);
                    break;
                }
                printf("What is the value of his %s ? \n", pc2);
                float amount2;
                scanf("%f", &amount2);
                addPcToJerry(initPc(amount2, pc2), j2);
                addToMultiValueHashTable(byPC, pc2, j2);
                displayMultiValueHashElementsByKey(byPC, pc2);
                break;

            case '3': // If the user chose to
                printf("What is your Jerry's ID ? \n");
                char id3[BUFFER]; // A variable for the Jerry's ID
                scanf("%s", id3);
                KeyValuePair k3 = (KeyValuePair) lookupInHashTable(byID, id3);
                if (!k3) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                Jerry *j3 = (Jerry *) getValue(k3);
                if (!j3) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                printf("What physical characteristic do you want to remove from Jerry - %s ? \n", id3);
                char pc3[BUFFER];
                scanf("%s", pc3);
                if (doesJerryHavePc(pc3, j3) < 0) {
                    printf("The information about his %s not available to the daycare ! \n", pc3);
                    break;
                }
                removePcFromJerry(pc3, j3);
                removeFromMultiValueHashTable(byPC, pc3, j3);
                printJerryInfo(j3);
                printf("\n");
                break;

            case '4': // If the user chose to
                printf("What is your Jerry's ID ? \n");
                char id4[BUFFER]; // A variable for the Jerry's ID
                scanf("%s", id4);
                KeyValuePair k4 = (KeyValuePair) lookupInHashTable(byID, id4);
                if (!k4) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                Jerry *j4 = (Jerry *) getValue(k4);
                if (!j4) {
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                s = max(s, removeFromHashTable(byID, id4));
                s = max(s, removeFromMultiValueHashTable(byPlanet, j4->org->planet->name, j4));
                for (int i = 0; i < j4->numOfPcs; i++) {
                    s = max(s, removeFromMultiValueHashTable(byPC, j4->pcs[i]->name, j4));
                }
                s = max(s, deleteNode(byHappiness, j4));
                s = max(s, deleteNode(jerries, j4));
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
                break;

            case '5': // If the user chose to
                printf("What do you remember about your Jerry ? \n");
                char pc5[BUFFER];
                scanf("%s", pc5);
                printf("What do you remember about the value of his %s ? \n", pc5);
                float amount5;
                scanf("%f", &amount5);
                //check pc in hash
                LinkedList list = (LinkedList ) searchByKeyInTable(byPC, pc5);
                if (!list || isEmpty(list)) {
                    // if none with the pc print no jerry with this pc
                    printf("Rick we can't help you - we do not know any Jerry's %s ! \n ",pc5);
                    break;
                }
                Jerry *j5 = (Jerry *)searchClosest(list, amount5, pc5, diffPC);
                printJerryInfo(j5);
                s = max(s, removeFromHashTable(byID, j5->ID));
                s = max(s, removeFromMultiValueHashTable(byPlanet, j5->org->planet->name, j5));
                for (int i = 0; i < j5->numOfPcs; i++) {
                    s = max(s, removeFromMultiValueHashTable(byPC, j5->pcs[i]->name, j5));
                }
                s = max(s, deleteNode(byHappiness, j5));
                s = max(s, deleteNode(jerries, j5));
                // if found give one with the closest value

                break;

            case '6': // If the user chose to
                saddest = (Jerry *) getData(byHappiness);
                if (!saddest) {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    break;
                }
                printf("Rick this is the most suitable Jerry we found : \n");
                char *saddestID = saddest->ID;
                s = max(s, printJerryInfo(saddest));
                s = max(s, removeFromHashTable(byID, saddestID));
                s = max(s, removeFromMultiValueHashTable(byPlanet, saddest->org->planet->name, saddest));
                for (int i = 0; i < saddest->numOfPcs; i++) {
                    removeFromMultiValueHashTable(byPC, saddest->pcs[i]->name, saddest);
                }
                s = max(s, deleteNode(jerries, saddest));
                s = max(s, deleteNode(byHappiness, saddest));
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

                        if (isEmpty(jerries)) {
                            printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                            break;
                        }
                        s = max(s, displayList(jerries));
                        break;
                    case '2':
                        printf("What physical characteristics ? \n");
                        char pc7[BUFFER];
                        scanf("%s", pc7);
                        if (isEmpty(searchByKeyInTable(byPC, pc7))) {
                            printf("Rick we can not help you - we do not know any Jerry's %s ! \n", pc7);
                            break;
                        }
                        printf("%s : \n", pc7);
                        s = max(s, displayMultiValueHashElementsByKey(byPC, pc7));
                        break;
                    case '3':
                        for (int i = 0; i < numOfPlanets; ++i) {
                            s = max(s, printPlanet(planets[i]));
                        }
                        break;
                    default:
                        printf("Rick this option is not known to the daycare ! \n");
                        break;
                }
                break;

            case '8': // If the user chose to
                if (isEmpty(jerries)) {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    break;
                }
                printf("What activity do you want the Jerries to partake in ? \n");
                printf("1 : Interact with fake Beth \n");
                printf("2 : Play golf \n");
                printf("3 : Adjust the picture settings on the TV \n");
                char c3;
                scanf(" %c", &c3);
                while (getchar() != '\n'); // Clear the buffer
                status status1 = success;
                switch (c3) {
                    case '1':
                        status1 = updateData(jerries, interactWithFakeBeth);
                        break;
                    case '2':
                        status1 = updateData(jerries, playGolf);
                        break;
                    case '3':
                        status1 = updateData(jerries, adjustThePictureSettingsOn);
                        break;
                    default:
                        printf("Rick this option is not known to the daycare ! \n");
                        break;
                }
                if (status1 == success) {
                    printf("The activity is now over ! \n");
                    s = max(s, displayList(jerries));
                }
                break;

            case '9': // If the user chose to
                s = max(destroyHashTable(byID), s);
                s = max(destroyMultiValueHashTable(byPC), s);
                s = max(destroyMultiValueHashTable(byPlanet), s);
                for (int i = 0; i < numOfPlanets; i++) {
                    s = max(destroyPlanet(planets[i]), s);
                }
                free(planets);
                s = max(destroyList(byHappiness), s);
                s = max(destroyList(jerries), s);
//                free(byHappiness);
//                free(jerries);
                if (s == success) {
                    printf("The daycare is now clean and close ! \n");
                    return 0;
                }
                break;

            default: // If the user chose an invalid option
                printf("Rick this option is not known to the daycare ! \n");
        }
    }
#endif
    if (s == memory_error) { // If there was a memory error
        // free all the allocated memory
        // exit the program
        printf("Memory error");
        destroyHashTable(byID);
        destroyMultiValueHashTable(byPC);
        destroyMultiValueHashTable(byPlanet);
        for (int i = 0; i < numOfPlanets; i++) {
            destroyPlanet(planets[i]);
        }
        free(planets);
        destroyList(byHappiness);
        destroyList(jerries);
        return 1;
    }
    return 0;
}

