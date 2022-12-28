This project is about managing Jerries instance.
The project is divided into two parts:
First part is about managing the Jerry, which is done by the Jerry.c
Second part is about managing all the Jerries, which is done by the ManageJerries.c

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
    and function pointer to functions:
        to print the Physical Characteristic's name and value,
        to initialize the Physical Characteristic,
        to destroy the Physical Characteristic.

Every Jerry contains:
    happiness level as int,
    Pointer to an Origin,
    number of Physical Characteristics as int,
    array of chars as ID,
    array of Physical Characteristics,
    and pointer functions:
        check if jerry has a physical characteristic,
        add Physical Characteristic to Jerry,
        remove Physical Characteristic from Jerry,
        print Jerry,
        print Physical Characteristics of Jerry,
        initializes and allocates space for a jerry,
        frees space allocated for a jerry and its components.

The program is managed by a menu, which is printed by the ManageJerries.c file.
First, it reads data from a configuration file given by the user in the terminal.
Then, it prints the menu, and the user can choose what to do.
The menu contains 7 options:
    1. Print all the Jerries
    2. Print all the Planets
    3. Add Physical Characteristic to Jerry
    4. Remove Physical Characteristic from Jerry
    5. Print Jerries by a planet name
    6. Print Jerries by a Physical Characteristic
    7. Exit and clear the memory

The program will take in the value from the user. any other value will send out a message
and print the menu again.

There is also a fallback option, if the program is unable to allocate memory, at any stage
of the program, print an error message and exit the program.
