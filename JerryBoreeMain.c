#include "KeyValuePair.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "Jerry.h"
#include "MultiValueHashTable.h"
#include "Defs.h"
#include <string.h>


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
    return compareString(getKey(k), (char*)b);
}

status destroyString(Element e) {
    char *c = (char *) e;
    free(c);
    e=NULL;
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



int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

float absolute(float a) {
    return a > 0 ? a : -a;
}

float diffPC(Element e, float value, char *key) {
    Jerry *j = (Jerry *) e;
    int pcIndex = doesJerryHavePc(key, j);
    if (pcIndex == -1) return -1;
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


int main(int argc, char *argv[]) // Main menu for the JerryBoree Interface
{
    if(argc < 3) return -1; // If the program doesn't get enough arguments, return -1

    int numOfJerries = 0, numOfPlanets = atoi(argv[1]);
    char *fileName = argv[2];
    Planet **planets = createPlanetArray(numOfPlanets); //create planet array
    if (!planets) { // If the allocations failed
        return memory_error; // Return the memory error code
    }

    LinkedList jerries = createLinkedList(destroyJerry, printJerryInfo, equalJerry,
                                          shallowCopy); // Create a linked list of Jerries
    if (!jerries) return memory_error;

    LinkedList byHappiness = createLinkedList(doNothing, printJerryInfo, equalJerry, shallowCopy);
    // Create a linked list of Jerries sorted by happiness
    if (!byHappiness) return memory_error; // If the allocations failed

    hashTable byID = createHashTable(copyString_deep, destroyString, printString, shallowCopy, doNothing,
                                     printJerryInfo,
                                     compareKey, HashFunction, 3);
    // Create a hash table of Jerries sorted by ID
    if (!byID) return memory_error; // If the allocations failed

    MultiValueHashTable byPC = createMultiValueHashTable(copyString_deep, destroyString, printString, shallowCopy,
                                                         doNothing, printJerryInfo, compareKey, equalJerry,
                                                         HashFunction, 4);
    // Create a multi value hash table of Jerries sorted by PC
    if (!byPC) return memory_error; // If the allocations failed

    MultiValueHashTable byPlanet = createMultiValueHashTable(copyString_deep, destroyString, printString, shallowCopy,
                                                             doNothing, printJerryInfo, compareKey, equalJerry,
                                                             HashFunction, 4);
    // Create a multi value hash table of Jerries sorted by Planet
    if (!byPlanet) return memory_error; // If the allocations failed

    status s = success;
    s = max(readFromConfig(fileName, numOfPlanets, planets, jerries, byHappiness, &numOfJerries,
                           byID, byPC,
                           byPlanet), s); // Read the configuration file
    if (s == failure) s=memory_error; // If the function failed, skip main menu failure
    Jerry *saddest = NULL; // Create a pointer to the saddest Jerry
//    s=memory_error;
    char c = '0'; // A variable for the user's choice
    while (c != '9') { // While the user didn't choose to exit
        if (memory_error == s) { // If the program ran out of memory
            printf("Memory error\n"); // Print the error
            break; // Break the main menu loop
        }
        if (jerries == NULL) { // If the linked list of Jerries is empty
            jerries = createLinkedList(destroyJerry, printJerryInfo, equalJerry,
                                       shallowCopy); // Create a linked list of Jerries

            if (!jerries) { // If the allocations failed
                s = memory_error; // Return the memory error code
                break; // Break the main menu loop
            }
        }
        if (byHappiness == NULL) { // If the linked list of Jerries sorted by happiness is empty
            byHappiness = createLinkedList(doNothing, printJerryInfo, equalJerry, shallowCopy);
            // Create a linked list of Jerries sorted by happiness
            if (!byHappiness) { // If the allocations failed
                s = memory_error; // Return the memory error code
                break; // Break the main menu loop
            }
        }
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
                while (getchar() != '\n'); // Clear the buffer
                KeyValuePair k = (KeyValuePair) lookupInHashTable(byID, id);
                // Get the Jerry from the hash table
                if (k) { // If the Jerry exists
                    printf("Rick did you forgot ? you already left him here ! \n");
                    break; // Break the switch
                }
                Jerry *j = (Jerry *) getValue(k); // Get the Jerry from the key value pair
                if (j) { // If the Jerry exists
                    printf("Rick did you forgot ? you already left him here ! \n");
                    break;
                }
                printf("What planet is your Jerry from ? \n");
                char planet[BUFFER]; // A variable for the Jerry's planet
                scanf("%s", planet);
                while (getchar() != '\n'); // Clear the buffer
                Planet *p = locatePlanet(planet, planets, numOfPlanets); // Get the planet
                if (!p) { // If the planet doesn't exist
                    printf("%s is not a known planet ! \n", planet);
                    break;
                }
                printf("What is your Jerry's dimension ? \n");
                char dimension[BUFFER]; // A variable for the Jerry's dimension
                scanf("%s", dimension);
                while (getchar() != '\n'); // Clear the buffer
                printf("How happy is your Jerry now ? \n");
                int happiness;
                scanf("%d", &happiness); // Get the Jerry's happiness
                Jerry *j1 = initJerry(id, initOrigin(p, dimension), happiness);
                // Create a Jerry with the given parameters
                if (!j1) { // If the allocations failed
                    s = memory_error; // Return the memory error code
                    break;
                }
                appendNode(jerries, j1); // Add the Jerry to the linked list
                appendCondition(byHappiness, j1, compareHappiness); // Add the Jerry to the linked list sorted by happiness
                addToHashTable(byID, j1->ID, j1); // Add the Jerry to the hash table sorted by ID
                addToMultiValueHashTable(byPlanet, j1->org->planet->name, j1);  // Add the Jerry to the multi value hash table sorted by planet
                printJerryInfo(j1); // Print the Jerry's info

                break;

            case '2': //
                printf("What is your Jerry's ID ? \n");
                char id2[BUFFER]; // A variable for the Jerry's ID
                scanf("%s", id2);
                while (getchar() != '\n'); // Clear the buffer
                KeyValuePair k2 = (KeyValuePair) lookupInHashTable(byID, id2); // Get the Jerry from the hash table
                if (!k2) { // If the Jerry doesn't exist
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                Jerry *j2 = (Jerry *) getValue(k2); // Get the Jerry from the key value pair
                if (!j2) { // If the Jerry doesn't exist
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }

                printf("What physical characteristic can you add to Jerry - %s ? \n", id2);
                char pc2[BUFFER];
                scanf("%s", pc2);
                while (getchar() != '\n'); // Clear the buffer
                pc2[strlen(pc2)] = '\0'; // Add the null terminator
                if (doesJerryHavePc(pc2, j2) >= 0) { // If the Jerry already has the physical characteristic
                    printf("The information about his %s already available to the daycare ! \n", pc2);
                    break;
                }
                printf("What is the value of his %s ? \n", pc2);
                float amount2;
                scanf("%f", &amount2);
                while (getchar() != '\n'); // Clear the buffer
                addPcToJerry(initPc(amount2, pc2), j2); // Add the physical characteristic to the Jerry
                addToMultiValueHashTable(byPC, pc2, j2); // Add the Jerry to the multi value hash table sorted by physical characteristic
                displayMultiValueHashElementsByKey(byPC, pc2); // Display the Jerries with the given physical characteristic
                break;

            case '3': // If the user chose to
                printf("What is your Jerry's ID ? \n");
                char id3[BUFFER]; // A variable for the Jerry's ID
                scanf("%s", id3);
                while (getchar() != '\n'); // Clear the buffer
                KeyValuePair k3 = (KeyValuePair) lookupInHashTable(byID, id3);
                // Get the Jerry from the hash table
                if (k3 == NULL) { // If the Jerry doesn't exist
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                Jerry *j3 = (Jerry *) getValue(k3); // Get the Jerry from the key value pair
                if (j3 == NULL) { // If the Jerry doesn't exist
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                printf("What physical characteristic do you want to remove from Jerry - %s ? \n", id3);
                char pc3[BUFFER];
                scanf("%s", pc3);
                while (getchar() != '\n'); // Clear the buffer
                if (doesJerryHavePc(pc3, j3) < 0) { // If the Jerry doesn't have the physical characteristic
                    printf("The information about his %s not available to the daycare ! \n", pc3);
                    break;
                }
                removeFromMultiValueHashTable(byPC, pc3, j3); // Remove the Jerry from the multi value hash table sorted by physical characteristic
                removePcFromJerry(pc3, j3); // Remove the physical characteristic from the Jerry
                printJerryInfo(j3); // Print the Jerry's info
                break;

            case '4': // If the user chose to
                printf("What is your Jerry's ID ? \n");
                char id4[BUFFER]; // A variable for the Jerry's ID
                scanf("%s", id4);
                while (getchar() != '\n'); // Clear the buffer
                KeyValuePair k4 = (KeyValuePair) lookupInHashTable(byID, id4); // Get the Jerry from the hash table
                if (k4 == NULL) { // If the Jerry doesn't exist
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                Jerry *j4 = (Jerry *) getValue(k4); // Get the Jerry from the key value pair
                if (j4 == NULL) { // If the Jerry doesn't exist
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                s = max(s, removeFromHashTable(byID, j4->ID)); // Remove the Jerry from the hash table sorted by ID
                s = max(s, removeFromMultiValueHashTable(byPlanet, j4->org->planet->name, j4));
                // Remove the Jerry from the multi value hash table sorted by planet
                for (int i = 0; i < j4->numOfPcs; i++) {
                    s = max(success, removeFromMultiValueHashTable(byPC, j4->pcs[i]->name, j4));
                    // Remove the Jerry from the multi value hash table sorted by physical characteristic
                    // For every Jerry's physical characteristic
                }
                if (s == memory_error) break; // If the removal failed
                s = max(success, deleteNode(byHappiness, j4)); // Remove the Jerry from the linked list sorted by happiness
                if (s == empty) { // If the Jerry doesn't exist
                    byHappiness = NULL; // Set the linked list to NULL
                }
                s = max(success, deleteNode(jerries, j4)); // Remove the Jerry from the linked list
                if (s == empty) { // If the Jerry doesn't exist
                    jerries = NULL; // Set the linked list to NULL
                }
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
                break;

            case '5': // If the user chose to
                printf("What do you remember about your Jerry ? \n");
                char pc5[BUFFER];
                scanf("%s", pc5);
                while (getchar() != '\n'); // Clear the buffer
                //check pc in hash
                LinkedList list = (LinkedList) searchByKeyInTable(byPC, pc5); // Get the linked list of Jerries with the given physical characteristic
                if (list == NULL || isEmpty(list)) { // If the linked list is empty
                    // if none with the pc print no jerry with this pc
                    printf("Rick we can't help you - we do not know any Jerry's %s ! \n ", pc5);
                    break;
                }
                printf("What do you remember about the value of his %s ? \n", pc5);
                float amount5;
                scanf("%f", &amount5);
                while (getchar() != '\n'); // Clear the buffer
                // if found give one with the closest value
                Jerry *j5 = (Jerry *) searchClosest(list, amount5, pc5, diffPC);
                // Get the Jerry with the closest physical characteristic value
                if (j5 == NULL) {
                    printf("Rick we can't help you - we do not know any Jerry's %s ! \n ", pc5);
                    //Fallback option
                    break;
                }
                printJerryInfo(j5); // Print the Jerry's info
                s = max(s, removeFromHashTable(byID, j5->ID)); // Remove the Jerry from the hash table sorted by ID
                s = max(s, removeFromMultiValueHashTable(byPlanet, j5->org->planet->name, j5));
                // Remove the Jerry from the multi value hash table sorted by planet
                if (j5->numOfPcs > 0) {
                    for (int i = 0; i < j5->numOfPcs; i++) {
                        s = max(s, removeFromMultiValueHashTable(byPC, j5->pcs[i]->name, j5));
                        // Remove the Jerry from the multi value hash table sorted by physical characteristic
                    }
                }
                if (s == memory_error) return s;
                s = max(success, deleteNode(byHappiness, j5));
                // Remove the Jerry from the linked list sorted by happiness
                if (s == empty) byHappiness = NULL; // If the linked list is empty set it to NULL
                s = max(success, deleteNode(jerries, j5));
                // Remove the Jerry from the linked list and destroy it
                if (s == empty) jerries = NULL; // If the linked list is empty set it to NULL

                break;

            case '6': // If the user chose to
                saddest = (Jerry *) getData(byHappiness); // Get the saddest Jerry
                if (!saddest) { // If the linked list is empty
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    break;
                }
                printf("Rick this is the most suitable Jerry we found : \n");
                char *saddestID = saddest->ID; // Get the saddest Jerry's ID
                s = max(success, printJerryInfo(saddest)); // Print the saddest Jerry's info
                s = max(s, removeFromHashTable(byID, saddestID)); // Remove the saddest Jerry from the hash table sorted by ID
                s = max(s, removeFromMultiValueHashTable(byPlanet, saddest->org->planet->name, saddest));
                // Remove the saddest Jerry from the multi value hash table sorted by planet
                for (int i = 0; i < saddest->numOfPcs; i++) {
                    s = max(s, removeFromMultiValueHashTable(byPC, saddest->pcs[i]->name, saddest));
                    // Remove the saddest Jerry from the multi value hash table sorted by physical characteristic
                }
                status listStatus;
                listStatus = max(success, deleteNode(byHappiness, saddest));
                // Remove the saddest Jerry from the linked list sorted by happiness
                if (listStatus == empty) { // If the linked list is empty
                    byHappiness = NULL; // Set it to NULL
                }
                listStatus = max(success, deleteNode(jerries, saddest)); // Remove the saddest Jerry from the linked list
                if (listStatus == empty) { // If the linked list is empty
                    jerries = NULL; // Set it to NULL
                }
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
                    case '1': // If the user chose to print all Jerries
                        if (isEmpty(jerries)) { // If the linked list is empty
                            printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                            break;
                        }
                        s = max(s, displayList(jerries)); // Print all Jerries
                        break;
                    case '2':
                        if (isEmpty(jerries)) { // If the linked list is empty
                            printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                            break;
                        }
                        printf("What physical characteristics ? \n");
                        char pc7[BUFFER];
                        scanf("%s", pc7);
                        while (getchar() != '\n'); // Clear the buffer
                        if (isEmpty(searchByKeyInTable(byPC, pc7))) { // search for the PC in the hash table
                            printf("Rick we can not help you - we do not know any Jerry's %s ! \n", pc7);
                            break;
                        }
                        printf("%s : \n", pc7);
                        s = max(s, displayMultiValueHashElementsByKey(byPC, pc7)); // Print all Jerries with the given physical characteristic
                        break;
                    case '3':
                        for (int i = 0; i < numOfPlanets; ++i) { // For each planet
                            s = max(s, printPlanet(planets[i])); // Print the planet's info
                        }
                        break;
                    default:
                        printf("Rick this option is not known to the daycare ! \n");
                        break;
                }
                break;

            case '8': // If the user chose to
                if (isEmpty(jerries)) { // If the linked list is empty
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
                switch (c3) { // If the user chose to
                    case '1': // Interact with fake Beth
                        status1 = updateData(jerries, interactWithFakeBeth); // Update the Jerries' happiness
                        break;
                    case '2': // Play golf
                        status1 = updateData(jerries, playGolf); // Update the Jerries' happiness
                        break;
                    case '3': // Adjust the picture settings on the TV
                        status1 = updateData(jerries, adjustThePictureSettingsOn); // Update the Jerries' happiness
                        break;
                    default:
                        printf("Rick this option is not known to the daycare ! \n");
                        break;
                }
                if (status1 == success) {
                    printf("The activity is now over ! \n");
                    s = max(s, displayList(jerries)); // Print all updated Jerries
                }
                break;

            case '9': // If the user chose to exit
                s = success;
                s = max(destroyMultiValueHashTable(byPC), s);
                // Destroy the multi value hash table sorted by physical characteristic
                byPC = NULL;
                s = max(destroyMultiValueHashTable(byPlanet), s);
                // Destroy the multi value hash table sorted by planet
                byPlanet = NULL;
                s = max(destroyHashTable(byID), s);
                // Destroy the hash table sorted by ID
                byID = NULL;
                s = max(destroyList(byHappiness), s);
                // Destroy the linked list sorted by happiness
                byHappiness = NULL;
                s = max(destroyList(jerries), s);
                // Destroy the linked list of jerries and free all Jerries' memory
                jerries = NULL;
                for (int i = 0; i < numOfPlanets; i++) { // For each planet
                    s = max(destroyPlanet(planets[i]), s); // Destroy the planet
                }
                free(planets); // Free the planets array
                planets = NULL; // Set it to NULL
                printf("The daycare is now clean and close ! \n");
                break;

            default: // If the user chose an invalid option
                printf("Rick this option is not known to the daycare ! \n"); // Print an error message
        }
    }

    if (s == memory_error) { // If there was a memory error
        // free all the allocated memory
        // exit the program
        printf("Memory error");
        destroyHashTable(byID);
        destroyMultiValueHashTable(byPlanet);
        destroyMultiValueHashTable(byPC);
        destroyList(byHappiness);
        destroyList(jerries);
        for (int i = 0; i < numOfPlanets; i++) {
            destroyPlanet(planets[i]);
        }
        free(planets);
        return 1; // Exit the program with failure status
    }
    return 0;
}

