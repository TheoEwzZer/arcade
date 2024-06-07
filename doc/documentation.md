# Arcade

## Documentation

## Creating a New Graphics Library

1. Create a new directory under `src/graphicals/` for your library. For example, `src/graphicals/mylib/`.

2. In this directory, create a header file (`Mylib.hpp`) and a source file (`Mylib.cpp`). 

3. Your header file should include the `ADisplayModule.hpp` file and your class should inherit from the `ADisplayModule` abstract class.

4. Implement all the pure virtual functions declared in the `ADisplayModule` abstract class in your class.

5. Your library should be compiled to a shared library (`.so` file). Update the `Makefile` to include your new library in the build process.

## Creating a New Game Library

1. Create a new directory under `src/games/` for your game. For example, `src/games/mygame/`.

2. In this directory, create a header file (`MyGame.hpp`) and a source file (`MyGame.cpp`).

3. Your header file should include the `AGameModule.hpp` file and your class should inherit from the `AGameModule` abstract class.

4. Implement all the pure virtual functions declared in the `AGameModule` abstract class in your class.

5. Your game should be compiled to a shared library (`.so` file). Update the `Makefile` to include your new game in the build process.

Remember to follow the coding standards and conventions used in the rest of the Arcade project. This will ensure that your new libraries are compatible with the existing system.
