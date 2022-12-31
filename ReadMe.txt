This project is about managing Jerries instance.
The project is divided into 3 parts:
First part is about managing the Jerry, which is done by the Jerry.c
Second part is about managing the Jerry instances, which is done by the ADT's in the various files.
Third part is about managing all the Jerries in the JerryBoree, which is done by the JerryBoree.c using the ADT's.

There are plenty of planets in every dimension, and each dimension has its own Planets.
Each Jerry is defined by his Origin (as the Dimension and the Planet),
and by his Physical Characteristics (such as Weight, Height, Age and many more).

Every Planet contains:
    x,y,z coordinates as float values,
    char pointer to an array for its name,
    and function pointer to functions:
        to print the Planet's name and coordinates,
        to initialize the Planet,
        to destroy the Planet.

Every Origin contains:
    char pointer to an array for its name,
    a pointer to a Planet,
    and function pointer to functions:
        to print the dimension name and print the Planet as described above,
        to initialize the Origin,
        to destroy the Origin. (Will not destroy the Planet, since there are plenty
        of planets in any dimension)


Every Physical Characteristic (PhysicalCharacteristics) contains:
    char pointer to an array for its name,
    float value for its value,
    the PC's functions can be used to:
        to print the Physical Characteristic's name and value,
        to initialize the Physical Characteristic,
        to destroy the Physical Characteristic.

Every Jerry contains:
    happiness level as int,
    Pointer to an Origin,
    number of Physical Characteristics as int,
    array of chars as ID,
    array of Physical Characteristics,
    the Jerry's functions can be used to:
        check if jerry has a physical characteristic,
        add Physical Characteristic to Jerry,
        remove Physical Characteristic from Jerry,
        print Jerry,
        print Physical Characteristics of Jerry,
        initializes and allocates space for a jerry,
        frees space allocated for a jerry and its components.

Generic ADTs:
    LinkedList: contains a pointer to next, prev and head nodes, a pointer to data, and functions to:
        add an Element to the list,
        add an Element to the list in a specific order,
        remove an Element from the list,
        print the list,
        initialize the list,
        destroy the list.

    KeyValuePair: contains a key and a value, and functions to:
        print the key and value,
        get the Key and Value,
        initialize the KeyValuePair,
        destroy the KeyValuePair.

    HashTable: contains an array of Elements, a size, and functions to:
        add an Element to the HashTable,
        remove an Element from the HashTable,
        search for an Element in the HashTable,
        print the HashTable,
        initialize the HashTable,
        destroy the HashTable.

    MultiValueHashTable: contains a hash table, size and functions to:
        add an Element to the MultiValueHashTable,
        remove an Element from the MultiValueHashTable,
        search for an Element in the MultiValueHashTable,
        print the MultiValueHashTable,
        initialize the MultiValueHashTable,
        destroy the MultiValueHashTable.


The main assignment was to search for a Jerry in the JerryBoree as fast as possible, which is achieved by using the various ADTs.
LinkedList:
    The Jerries created are assigned to Jerry Linked list, which has the ability to destroy a Jerry instance.
    The jerries are also assigned to byHappiness which is a sorted LinkedList, going by saddest Jerry to the Happiest.

HashTable:
    The jerries are sorted by ID in the byID hash table. since ID is one of a kind, it is a good way to search for a Jerry.
    Every cell contains a list of KeyValuePair, which contains the ID as Key and the Jerry as Value.

MultiValueHashTable:
    The jerries are sorted by Physical Characteristics in the byPhysicalCharacteristics multi value hash table.
    since there are many PC's to a Jerry, it is a good way to search for a Jerry by PC.
    Every cell contains a KeyValuePair which contains the PC's name as Key and a LinkedList of Jerries as Value.

    The jerries are also sorted by Planet in the byPlanet multi value hash table.
    Every cell contains a KeyValuePair which contains the Planet's name as Key and a LinkedList of Jerries as Value.

The main menu goes by:
    1. Add a Jerry
    2. Add PC to Jerry
    3. Remove PC from Jerry
    4. Remove a Jerry
    5. Similar Jerry remove (by PC)
    6. Saddest Jerry remove
    7. Print functions, opens a new menu:
        1. Print all Jerries
        2. Print all Jerries by PC
        3. Print all Planets
    8. Interact with jerries, which either increase or reduce the Jerry's happiness
    9. Exit
