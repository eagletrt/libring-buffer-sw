# LIBSTM32-SW-TEMPLATE

This repository serves as a template for libraries compatible with the
[PlatformIO ecosystem](https://docs.platformio.org/en/latest/librarymanager/creating.html).

## Usage

Before starting to develop the library, a couple of things need to be done:
1. Change this README explaining the library and the functionalities that it offers
2. Modify the `library.json` including:
    - The **name** of the library
    - The library **version**
    - The **description** explaining what the library does and for which devices
    - The list of **keywords**
    - The repository **url** (and type if necessary)
    - The list of **authors**
    - The supported **frameworks** and **platforms** (if needed)
    - The list of **header files** of the library
    - The list of **examples**
    - The file of the library to **export** (if needed)

## Structure

The code of the library should be splitted in sources which must be placed inside
the `src` folder and headers which must be placed inside the `include` folder.

Inside the `example` folder multiple source files should be placed to further
explain how to use the library and how it works in different scenario.

The library must be tested with the maximum possible code coverage, the source
code used to run the unit tests should be put inside the `test` folder.

No other folders should be created besides the ones described before if not
necessary, to handle complex file structures nested folders can be used.

For more info check the READMEs inside the corresponding folders.
