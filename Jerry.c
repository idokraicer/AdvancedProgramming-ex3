
#include "Jerry.h"
#include <ctype.h>
#include "Defs.h"
#include "LinkedList.h"
#include "MultiValueHashTable.h"
#include "KeyValuePair.h"

#pragma warning( disable : 996)
#pragma warning( disable : 703)
#define BUFFER 300

#define debug
#ifdef debug
int MemoryError = 0;

bool compareJerries(Element a, Element b) {
    Jerry *j1 = (Jerry *) a, *j2 = (Jerry *) b;
    return strcmp(j1->ID, j2->ID);
}

Element copyJerry(Element e) {
    Jerry *j = (Jerry *) e;
    return j;
}


Planet **createPlanetArray(int size) {
    Planet **arr = malloc(sizeof(Planet *) * size);
    if (!arr) return NULL; // Memory Error
    for (int i = 0; i < size; i++) {
        arr[i] = NULL;
    }
    return arr;
}

#ifdef debug

Planet *locatePlanet(char *name, Planet **ps, int numOfPlanets) { // A function that locates a planet by its name
    //The function returns a pointer to the planet if it was found, and NULL otherwise

    for (int i = 0; i < numOfPlanets; i++) { // For each planet
        if (ps[i]) { // If the planet exists
            if (strcmp(ps[i]->name, name) == 0) { // If the planet name is equal to the name
                return ps[i]; // Return the planet
            }
            // at the end of the loop, the planet wasn't found, will be set next iteration
        }
    }
    return NULL; // Planet wasn't found, return NULL
}

bool compareHappiness(Element a, Element b) {
    int happiness = ((Jerry *) b)->happiness;
    Jerry *j1 = (Jerry *) a;
    return j1->happiness > happiness;
}


status readFromConfig(char *url, int numOfPlanets, Planet **planets,
                      LinkedList jerries, LinkedList jerriesSorted, int *numOfJerries, hashTable byID,
                      MultiValueHashTable byPC, MultiValueHashTable byPlanet) { // A function that reads the config file
    // The program takes in a number of planets and a file name
    // The program reads the config file and sets the number of planets and Jerries,
    // and builds the Jerries and planets from the file
    status s;
    char row[BUFFER]; // A buffer for the row

    FILE *file; // A pointer to the file
    file = fopen(url, "r"); // Open the file
    if (!file || ferror(file)) {
        printf("File not found.\n");
        return failure;
    } // If the file doesn't exist or there is an error, return failure
    fgets(row, BUFFER, file); // Get the first row
    if (strcmp(row, "Planets\n") != 0) { // If the first row isn't "Planets"
        fclose(file); // Close the file
        return failure; // Faulty file, return failure
    }
    int records = 0;
    do { // While the end of the file hasn't been reached
        fgets(row, BUFFER, file); // Get the next row
        if (strcmp(row, "Jerries\n") == 0) { // If the row is "Jerries"
            break; // Break the loop
        }
        char *name = strtok(row, ","); // Get the planet name
        float x = atof(strtok(NULL, ",")); // Get the x coordinate
        float y = atof(strtok(NULL, ",")); // Get the y coordinate
        float z = atof(strtok(NULL, ",")); // Get the z coordinate
        Planet *planet = locatePlanet(name, planets, numOfPlanets); // Get the planet from the array
        if (!planet) { // If the planet doesn't exist
            records++; // Increase the number of records
            planets[records - 1] = (Planet *) initPlanet(x, y, z, name);
            if (!planets[records - 1]) { // If the planet wasn't created
                fclose(file); // Close the file
                return memory_error; // Return failure
            }
            // Initialize a new planet and add it to the array of planets
        } else return failure; // If the planet already exists, return failure
    } while (!feof(file)); // While the end of the file hasn't been reached
    records = 0; // Reset the number of records

    do { // While the end of the file hasn't been reached
        Jerry *j; // A pointer to a Jerry
        fgets(row, BUFFER, file); // Get the next row
        if (feof(file)) { // If the end of the file has been reached
            fclose(file); // Close the file
            return success; // Config file was read successfully, return success
        }
        if (row[0] == '\t' && !feof(file)) { // If the row begins with a tab
            char *id = strtok(row, "\t:"); // Get the Physical characteristic name
            float amount = atof(strtok(NULL, ":"));  // Get the Physical characteristic amount

            PhysicalCharacteristics *pc = initPc(amount, id); // Initialize a new Physical characteristic
            if (!pc) { // If the Physical characteristic wasn't created
                fclose(file); // Close the file
                return memory_error; // Return failure
            }
            addPcToJerry(pc, j);
            addToMultiValueHashTable(byPC, (char *) pc->name, j);

            // Add the Physical characteristic to the Jerry
            continue; // Continue to the next iteration
        } // If the row doesn't begin with a tab- it's a new Jerry
        records++; // Increase the number of records
        numOfJerries++; // Increase the number of Jerries
        char *ID = strtok(row, ","); // Get the Jerry ID
        char *orgName = strtok(NULL, ","); // Get the Jerry origin name
        char *planetName = strtok(NULL, ","); // Get the Jerry planet name
        Planet *p = locatePlanet(planetName, planets, numOfPlanets); // Get the planet from the array
        if (!p) { // If the planet doesn't exist
            fclose(file); // Close the file
            return failure; // Faulty file, return failure
        }
        Origin *org = initOrigin(p, orgName); // Initialize a new origin
        if (!org) { // If the origin wasn't created
            fclose(file); // Close the file
            return memory_error; // Return failure
        }
        int happiness = atoi(strtok(NULL, ",")); // Get the Jerry happiness
        j = (Jerry *) initJerry(ID, org, happiness); // Initialize a new Jerry
        if (!j) { // If the Jerry wasn't created
            fclose(file); // Close the file
            return memory_error; // Return failure
        }
        s = max(s, appendNode(jerries, (Jerry *) j)); // Add the Jerry to the array of Jerries
        s = max(s, appendCondition(jerriesSorted, (Jerry *) j,
                                   compareHappiness)); // Add the Jerry to the sorted array of Jerries
        s = max(s, addToHashTable(byID, ID, j)); // Add the Jerry to the hash table by ID
        s = max(s, addToMultiValueHashTable(byPlanet, planetName, j)); // Add the Jerry to the hash table by planet

    } while (!feof(file)); // While the end of the file hasn't been reached
    fclose(file); // Close the file
    return success; // Config file was read successfully, return success
}

#endif

Planet *initPlanet(float x, float y, float z,
                   char *name) { // A function that initializes a planet, taking its x,y,z cords and name
    // The function allocates memory for a new planet and returns a pointer to it
    Planet *p = (Planet *) malloc(sizeof(Planet)); // Allocate memory for a planet
    if (!p) { // If the allocation failed
        free(p); // Free the memory
        return NULL;
    }
    // Assign the x,y,z cords to the planet
    p->x = x;
    p->y = y;
    p->z = z;

    char *n = (char *) malloc(sizeof(char) * (strlen(name) + 1)); // Allocate memory for the name
    if (!n) { // If the allocation failed
        free(p); // Free the memory of the new planet pointer
        free(n); // Free the memory of the variable created
        return NULL;
    }
    strcpy(n, name); // Copy the name to the new variable
    p->name = n; // Assign the name to the planet
    return p; // Return the new planet instance
}

Origin *initOrigin(Planet *planet,
                   char *dimName) { // A function that initializes an origin, taking a planet and a dimension name
    // The function allocates memory for a new origin and returns a pointer to it
    Origin *p = (Origin *) malloc(sizeof(Origin)); // Allocate memory for an origin
    if (!p) { // If the allocation failed
        free(p); // Free the memory
        return NULL;
    }
    p->planet = planet; // Assign the planet to the origin
    char *n = (char *) malloc(sizeof(char) * (strlen(dimName) + 1)); // Allocate memory for the dimension name
    if (!n) { // If the allocation failed
        free(p); // Free the memory of the new origin pointer
        free(n); // Free the memory of the variable created
        return NULL;
    }
    strcpy(n, dimName); // Copy the dimension name to the new variable
    n[strlen(dimName)] = '\0'; // Add a null terminator to the end of the string
    p->dimName = n; // Assign the dimension name to the origin
    return p; // Return the new origin instance
}

PhysicalCharacteristics *
initPc(float amount, char *name) { // A function that initializes a PhysicalCharacteristic, taking an amount and a name
    // The function allocates memory for a new PhysicalCharacteristic and returns a pointer to it
    if (name[0] == '\0' || amount < 0) { // If the name is empty or the amount is negative
        return NULL;
    }
    PhysicalCharacteristics *p = (PhysicalCharacteristics *) malloc(sizeof(PhysicalCharacteristics));
    // Allocate memory for a PhysicalCharacteristic
    if (!p) { // If the allocation failed
        free(p); // Free the memory
        return NULL;
    }
    p->amount = amount; // Assign the amount to the PhysicalCharacteristic
    char *n = (char *) malloc(sizeof(char) * (strlen(name) + 1)); // Allocate memory for the name
    if (!n) { // If the allocation failed
        free(p); // Free the memory of the new PhysicalCharacteristic pointer
        free(n); // Free the memory of the variable created
        return NULL;
    }
    strcpy(n, name); // Copy the name to the new variable
    p->name = n; // Assign the name to the PhysicalCharacteristic
    return p; // Return the new PhysicalCharacteristic instance
}

Jerry *initJerry(char *name, Origin *org,
                 int happiness) { // A function that initializes a Jerry, taking a name, an origin and a happiness
    // The function allocates memory for a new Jerry and returns a pointer to it
    Jerry *newInstance = (Jerry *) malloc(sizeof(Jerry)); // Allocate memory for a Jerry
    if (!newInstance) { // If the allocation failed
        free(newInstance); // Free the memory of the new Jerry
        return NULL;
    }
    if (!name) { // If the name is empty
        return NULL;
    }
    if (happiness < 0 || happiness > 100) { // If the happiness is negative or greater than 100
        return NULL; // Return a blank Jerry as NULL
    }
    char *n = (char *) malloc(sizeof(char) * (strlen(name) + 1)); // Allocate memory for the name
    if (!n) { // If the allocation failed
        free(newInstance); // Free the memory of the new Jerry pointer
        free(n); // Free the memory of the variable created
        return NULL; // Return a blank Jerry as NULL
    }
    strcpy(n, name); // Copy the name to the new variable
    newInstance->ID = n; // Assign the name to the Jerry
    newInstance->happiness = happiness; // Assign the happiness to the Jerry
    newInstance->numOfPcs = 0; // Set the number of PhysicalCharacteristics to 0
    newInstance->org = org; // Assign the origin to the Jerry
    newInstance->pcs = (PhysicalCharacteristics **) NULL; // Set the PhysicalCharacteristics array to NULL
    return newInstance; // Return the new Jerry instance
}

bool checkIfJerryHasPc(Jerry *jerry,
                       char *pcName) { // A function that checks if a Jerry has a PhysicalCharacteristic, taking a Jerry and a PhysicalCharacteristic name
    // The function returns true if the Jerry has the PhysicalCharacteristic, false otherwise
    // This function is unused in the program, A function called doesJerryHavePc is used instead
    // Because of efficiency considerations. doesJerryHavePc returns the index of the PhysicalCharacteristic in the array,
    // which is better used in the code
    if (!jerry) { // If the Jerry is empty
        return false; // Return false
    }
    if (!pcName) { // If the PhysicalCharacteristic name is empty
        return false; // Return false
    }
    for (int i = 0; i < jerry->numOfPcs; i++) { // For each PhysicalCharacteristic in the Jerry's array
        if (strcmp(jerry->pcs[i]->name, pcName) == 0) { // If the PhysicalCharacteristic name is the same as the name given
            return true; // PC found, Return true
        }
    }
    return false; // PC was not found, Return false
}


status printPlanet(Element p1) { // A function that prints a planet
    Planet *p = (Planet *) p1;
    printf("Planet : %s (%.2f,%.2f,%.2f) \n", p->name, p->x, p->y, p->z);
    return success;
}

status printOrigin(Element e) { // A function that prints an origin
    Origin *o = (Origin *) e;
    printf("Origin : %s \n", o->dimName);
    printPlanet(o->planet); // Print the planet using the printPlanet function
    return success;
}

void printPc(PhysicalCharacteristics *p) { // A function that prints a PhysicalCharacteristic
    printf("%s : %.2f ", p->name, p->amount);
}

int doesJerryHavePc(char *name, Jerry *j) {
    // A function that checks if a Jerry has a PhysicalCharacteristic,
    // taking in a name and a Jerry.
    // The function returns the index of the PhysicalCharacteristic in the
    // PhysicalCharacteristics array, -1 if not found.
    if (j->numOfPcs == 0) { // If the Jerry has no PhysicalCharacteristics
        return -1;
    }
    for (int i = 0; i < j->numOfPcs; i++) { // For each PhysicalCharacteristic in the Jerry's array
        if (strcmp(j->pcs[i]->name, name) == 0) { // If the names are equal
            return i; // PC found, return the index
        }
    }
    return -1; // PC not found, return -1
}

status
addPcToJerry(PhysicalCharacteristics *pc, Jerry *j) { // A function that adds a PhysicalCharacteristic to a Jerry,
    // taking a PhysicalCharacteristic and a Jerry
    // Returns success or failure status
    int idx = doesJerryHavePc(pc->name, j); // Check if the Jerry has the PhysicalCharacteristic, return the index
    if (idx != -1) { // If the Jerry doesn't have the PhysicalCharacteristic
        return failure;
    }
    if (j->numOfPcs == 0) { // If the Jerry has no PhysicalCharacteristics
        j->pcs = (PhysicalCharacteristics **) malloc(
                sizeof(PhysicalCharacteristics *)); // Allocate memory for the array containing one element
        if (!j->pcs) { // If the allocation failed
            MemoryError = true; // Set the memory error flag to true
            free(j->pcs); // Free the memory of the new array pointer
            return failure;
        }
        j->pcs[0] = (PhysicalCharacteristics *) pc; // Assign the PhysicalCharacteristic to the Jerry
        j->numOfPcs++; // Increment the number of PhysicalCharacteristics
        return success;
    } else {
        j->pcs = (PhysicalCharacteristics **) realloc(j->pcs, sizeof(PhysicalCharacteristics *) * (j->numOfPcs + 1));
        // Reallocate memory for the array containing one more element
    }
    if (!j->pcs) { // If the allocation failed
        MemoryError = true; // Set the memory error flag to true
        free(j->pcs); // Free the memory of the new array pointer
        return failure;
    }
    j->pcs[j->numOfPcs] = (PhysicalCharacteristics *) pc; // Assign the PhysicalCharacteristic to the Jerry
    j->numOfPcs = j->numOfPcs + 1; // Increment the number of PhysicalCharacteristics
    return success;
}

status removePcFromJerry(char *name, Jerry *j) { // A function that removes a PhysicalCharacteristic from a Jerry
    // Takes in a name and a Jerry
    // Returns success if the PhysicalCharacteristic was removed, failure if not
    if (j->numOfPcs == 0) { // If the Jerry has no PhysicalCharacteristics
        return failure;
    }
    if (j->numOfPcs == 1) { // If the Jerry has only one PhysicalCharacteristic
        if (strcmp(name, j->pcs[0]->name) == 0) { // If the names are equal
            free(j->pcs[0]->name); // Free the memory of the PC name
            free(j->pcs[0]); // Free the memory of the PC
            free(j->pcs); // Free the memory of the array
            j->pcs = (PhysicalCharacteristics **) NULL; // Set the array to NULL
            j->numOfPcs = 0; // Set the number of PCs to 0
            return success;
        }
    }
    int idx = doesJerryHavePc(name, j); // Check if the Jerry has the PhysicalCharacteristic, return the index

    if (idx == -1) { // If the Jerry doesn't have the PhysicalCharacteristic
        return failure;
    }
    destroyPc(j->pcs[idx]); // Destroy the PhysicalCharacteristic
    for (int i = idx; i < j->numOfPcs - 1; i++) { // For each PhysicalCharacteristic in the Jerry's array
        j->pcs[i] = j->pcs[i + 1]; // Shift the PhysicalCharacteristics to the left
    }
    j->numOfPcs--; // Decrement the number of PhysicalCharacteristics
    j->pcs = (PhysicalCharacteristics **) realloc(j->pcs, sizeof(PhysicalCharacteristics *) * j->numOfPcs);
    // Reallocate memory for the array containing one less element
    return success;
}


status printJerriesPcs(Element e) { // A function that prints the PhysicalCharacteristics of a Jerry
    Jerry *j = (Jerry *) e;
    if (j->numOfPcs == 0) { // If the Jerry has no PhysicalCharacteristics
        return empty; // Exit function
    } else { // If the Jerry has PhysicalCharacteristics
        printf("Jerry's physical Characteristics available : \n\t");
        for (int i = 0; i < j->numOfPcs; i++) { // For each PhysicalCharacteristic in the Jerry's array
            printf("%s : %.2f ", j->pcs[i]->name,
                   j->pcs[i]->amount); // Print the name and amount of the PhysicalCharacteristic
            if (j->numOfPcs > 1 &&
                i !=
                j->numOfPcs - 1) { // If the Jerry has more than one PhysicalCharacteristic, and it's not the last one
                printf(", "); // Print a comma
            }
        }
    }
    return success;
}


status printJerryInfo(Element e) { // A function that prints the information of a Jerry
    Jerry *j = (Jerry *) e;
    printf("Jerry , ID - %s : \nHappiness level : %d \n", j->ID, j->happiness);
    // Print the ID and happiness level of the Jerry
    printOrigin(j->org); // Print the origin of the Jerry
    status s =printJerriesPcs(j); // Print the PhysicalCharacteristics of the Jerry
    if(s!=empty) printf("\n");

}

status printAllJerries(Element e) {
    LinkedList l = (LinkedList) e;
    if (isEmpty(l)) printf("List is empty.\n");
    displayList(l);
}


void destroyOrigin(Origin *p) { // A function that destroys an origin
    free(p->dimName); // Freeing the dimension name
    free(p); // Freeing the origin
}

status destroyPlanet(Element e) { // A function that destroys a planet
    Planet *p = (Planet *) e;
    if (!p) return failure; // If the planet is NULL, return failure
    free(p->name); // Freeing the name of the planet
    free(p); // Freeing the planet
    return success;
}

status destroyPc(PhysicalCharacteristics *p) { // A function that destroys a physical characteristic
    if (!p) return failure; // If the physical characteristic is NULL, return failure
    free(p->name); // Freeing the name of the physical characteristic
    free(p); // Freeing the physical characteristic
    return success;
}

status destroyJerry(Element e) { // A function that destroys a Jerry
    Jerry *p = (Jerry *) e;
    if (!p) return failure; // If the Jerry is NULL, return failure
    free(p->ID); // Freeing the ID
    destroyOrigin(p->org); // Destroying the origin
    status flag = success; // Setting the flag to true
    for (int i = 0; i < p->numOfPcs; i++) { // Freeing all the physical characteristics
        flag = max(destroyPc(p->pcs[i]), flag); // Destroying the physical characteristic
    }
    free(p->pcs); // Freeing the array of physical characteristics
    free(p); // Freeing the Jerry
    return flag;
}

#endif