# Arcade

Arcade is a **gaming platform** : a program that lets the user choose a game to play and keeps a register of player scores.
To be able to deal with the elements of your gaming plate- form at run-time, your graphics libraries and your games must be implemented as **dynamic libraries** , loaded at run-time.

Each GUI available for the program must be used as a shared library that will be loaded and used dynamically by the main program.

```
It is STRICTLY FORBIDDEN to refer to
a graphics library explicitly in your main
program.
Only your dynamic libraries can do so.
This also applies to your games.
```

### DYNAMIC LIBRARIES

You must use dynamic libraries at run-time.
This means that you MUST use thedlopen,dlclose,dlsymanddlerrorfunctions to handle your dynamic libraries.

```
libdl is a C library. You MUST provide an encapsulation. Also, add-fno-gnu-unique to
your compilation flags to ensuredlcloseunload your libraries properly.
```

```
Running ldd on your program must not show any dependency towards your libraries.
```

These dynamic libraries can be seen as plug-ins providing various capabilities to your main program.
In NO CASE must graphics libraries influence the game logic.
Game libraries must not contain any information about screen rendering or low-level events.
Your libraries (games and graphics) binaries (\*.so) must be placed in the./lib/directory at the root of your
repository.

```
You MUST NOT differentiate your libraries.
Each of your libraries MUST be handled in a generic and uniform manner.
Keep genericity!
```

### GRAPHICS LIBRARIES

You must implement the nCurses (arcade_ncurses.so) and SDL2 (arcade_sdl2.so) graphical libraries, and at
least one more from the following list:

-   NDK++(arcade_ndk++.so)
-   aa-lib(arcade_aalib.so)
-   libcaca(arcade_libcaca.so)
-   Allegro5(arcade_allegro5.so)
-   Xlib(arcade_xlib.so)
-   GTK+(arcade_gtk+.so)
-   SFML(arcade_sfml.so)
-   Irrlicht(arcade_irrlicht.so)
-   OpenGL(arcade_opengl.so)
-   Vulkan(arcade_vulkan.so)
-   Qt5(arcade_qt5.so)

### GAMES LIBRARIES

You must implement at least two games from the following list:

-   Snake (arcade_snake.so)
-   Nibbler (arcade_nibbler.so)
-   Pacman (arcade_pacman.so)
-   Qix (arcade_qix.so)
-   Centipede (arcade_centipede.so)
-   Solarfox (arcade_solarfox.so)

Descriptions and rules of these games are given at the end of this document.

### USAGE

The program must take as a startup argument the graphics library to use initially. It must nevertheless be
possible to change the graphics library at run-time.

```
∼/B-OOP-400> ./arcade ./lib/arcade_ncurses.so
```

When the program starts, it must display in separated boxes:

-   the games libraries available in the./lib/directory.
-   the graphics libraries available in the./lib/directory.
-   scores.
-   a field for the user to enter their name.

You MUST handle the following cases:

-   if there is more or less than 1 argument, your program must print a usage message and exit properly (84).
-   if the dynamic library passed as argument does not exist or is not compatible, your program must display a relevant error message and exit properly (84).

When your program is running, keys must be mapped to the following actions:

-   next graphics library.
-   next game.
-   restart the game.
-   go back to the menu.
-   exit.

```
Did you know that the nCurses library can handle mouse events?
```

### BUILD

You can build your project using either aMakefileorCMake.

### MAKEFILE

Your Makefile **must** have the following rules (in addition toall,re,cleanandfclean):

-   core: it must **only** build the core of your arcade (not the games nor the graphical librairies).
-   games: it must **only** build your games librairies.
-   graphicals: it must **only** build your graphical librairies.

The results of running a simplemakecommand in your turn in directory must generate a program, at least
three graphics dynamic libraries and at least two game dynamic libraries.

### CMAKE

Your CMakeLists.txt **must** build a program, at least three graphics dynamic libraries and at least two game
dynamic libraries at the root of the repository.

```
∼/B-OOP-400> mkdir ./build/ && cd ./build/
∼/B-OOP-400> cmake .. -G “Unix Makefiles” -DCMAKE_BUILD_TYPE=Release
[...]
∼/B-OOP-400> cmake --build.
[...]
∼/B-OOP-400> cd ..
∼/B-OOP-400> ls ./arcade ./lib/
./arcade
```

```
./lib/:
arcade_ncurses.so
arcade_pacman.so
arcade_sdl2.so
arcade_sfml.so
arcade_solarfox.so
```

### DOCUMENTATION

More than any other kind of program, an **extensible** program like the Arcade project MUST be documented.
In order to clarify the way your program and interface work, you must write a documentation. Turn-in the
following document in PDF format (or any revelant format), in a./docdirectory:

-   A document explaining how to implement new graphics libraries or game libraries compatible with your system.

As a bonus, it will be greatly appreciated if you also provide the following documents. Working on these
will save you development time, as you can make sure your program is well designed before you start
implementation:

-   A class diagram for your program, featuring at least the relationships between classes and their public member functions.
-   An explanatory manual that accompanies your diagram and descrbies how procedures are linked in your program.

### INTERFACE SHARING

You must share your graphical and game library interfaces with at least one other project group. After turn-
in, it will therefore be possible to run your games using their launcher and graphical libraries, and vice-versa.

```
You must add a README.md file in your repository containing, at least, the email address of
the leader of the group with which you have collaborated for the interface.
```

## GAMES

### SNAKE (arcade_snake.so)

**Snake** is an arcade game first released during the mid 1970s and has maintened popularity since then, be-
comeing something of a classic. After it became the standard pre-loaded game on Nokia phones in 1998,
Snake found a massive audience. The simplicity and addictiveness of **Snake** made it available on almost
every existing platform under various names.

As you may know, **Snake** is about moving a snake around a map. The snake is represented by sections and
must eat food in order to grow. The game is over when the head of the snake hits an edge of the map or
one of the sections.

The goal of the game is to make the snake as long as possible.

Various versions of **Snake** exist. Some of them include obstacles, others have a core system, or bonuses,
etc. Sometimes, the map have no edge and has a cyclical play area where crossing through one boundary
would have you appear on the opposite side.

**Core rules**

-   The game area is a finite amount of cells. The edges of the area either cannot be passed through or are cyclical.
-   The snake starts with a size of 4 cells in the middle of the area.
-   The snake moves forward automatically at a constant speed. Each section of its tail follows the exact same path as the head.
-   The snake can turn right or left when the corresponding key is pressed.
-   The goal of the game is to feed the snake so that it can grow. The game area MUST NEVER have less than one element of food.
-   A food element fills a single cell.
-   When the head of the snake goes over a cell with food, the food disappears and a one-cell-long section is added at the tail of the snake. The new section appears in the first free tile next to the last cell of the tail. If there is no free cell, the game is over. If a new section is added, a new food element appears.
-   When the head of the snake runs into the border of the screen (if the map isn’t cyclical) or a part of its body, the game is over

**Bonus ideas**

-   Bonus food appears for a short period of time
-   The head section looks different from the other sections
-   Movement speed increases throughout the game
-   A speed boost when pressing the space bar

### NIBBLER (arcade_nibbler.so)

**Nibbler** is a simple arcade video game released in 1982. It looks like a **Snake** , but it isn’t the same game.
**Nibbler** itself was inspired by another great classic: **Blockade** , itself inspired from **Tron Light Cycle**.

In **Nibbler** , the player navigate a snake through an enclosed maze, eating food increasing its length. The
game was the first to include nine scoring digits, allowing players to surpass one billion points.

The goal is to eat all the food to go to the next level.

The difference with a simple snake is that the snake dies only when colliding with its own body. When col-
liding a wall of the maze, it will automatically turn left, right, or stop completely in case of a T-intersection.
The player only have a limited time to complete each level.

**Core rules**

-   The game area is a finite amount of cells. It contains a maze and outer walls that cannot be passed through.
-   The snake starts with a size of 4 cells at the bottom of the area.
-   The snake moves forward automatically at a constant speed. Each section of its tail follows the exact same path as the head.
-   The snake can turn right or left automatically when colliding with a wall, or stop at T-junction.
-   The player can turn left or right when the corresponding key is pressed.
-   The goal of the game is to feed the snake so that it can grow.
-   A food element fills a single cell.
-   When the head of the snake goes over a cell with food, the food disappears and a one-cell-long section is added at the tail of the snake. The new section appears in the first free tile next to the last cell of the tail.
-   When the head of the snake runs into a part of its body, the game is over.

**Bonus ideas**

-   Bonus items appears for a short period of time, giving bonus points.
-   The head section looks different from the other sections.
-   The game contains the 32 original levels of 1982.
-   Movement speed increases throughout the game.
-   A speed boost when pressing the space bar.

## FINAL MARK

### MARK: 33.5 / 35 (95.7%)

-   Compilation (1 / 1)
-   Core (5 / 5)
-   Graphicals (3 / 3)
-   Graphicals (1 / 1)
-   Preliminaries (2 / 2)
-   Dynamic Linking Library encapsulation (1 / 1)
-   Dynamic library change (1 / 1)
-   Games interface genericity (0.5 / 0.5)
-   Game interface independency (0.5 / 0.5)
-   Graphic interface genericity (0.5 / 0.5)
-   Graphic interface independency (0.5 / 0.5)
-   Pure virtual interfaces (0.5 / 0.5)
-   Libraries list (0.5 / 0.5)
-   Menu library (0 / 0.5)
-   Scores (0.5 / 0.5)
-   Player name (0.5 / 0.5)
-   Game loop (0.5 / 0.5)
-   nCurses (arcade_ncurses.so) (1 / 1.5)
-   SDL2 (arcade_sdl2.so) (1.5 / 1.5)
-   Third library (1.5 / 1.5)
-   Game #1 (1.5 / 1.5)
-   Game #2 (1.5 / 1.5)
-   Collaboration (0 / 1)
-   Documentation (0.5 / 0.5)
-   Exception (1 / 1)
-   Template (1 / 1)
-   Memory management (1 / 1)
