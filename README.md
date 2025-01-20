# Preview
![Image 1](Images/ms1.png)
![Image 2](Images/ms2.png)

# Installation

## Option 1: Compile from Source

If you already have a C++ compiler and SFML installed, you can compile the game yourself. Follow these steps:

1. Navigate to the `src` directory.
2. Run the following command to build the project:

    ```bash
    make
    ```

3. After the build completes, you can launch the game by running `minesweeper.exe`.

## option 2: Build from source

If you have a C++ compiler and SFML installed, you can build from source \
the game can run without SFML or a compiler when installed.

1. Navigate to `project root`
2. run the following commands:
    ```bash
    cmake -B build
    cmake --build build --config Release
    ```

## Option 3: Run Precompiled Version (not recommended)

If you don't have a C++ compiler or SFML installed, you can still play the game using the precompiled version:

1. Navigate to the `Release` folder.
2. Launch `minesweeper.exe`, which comes with SFML libraries and all the necessary game assets included.
