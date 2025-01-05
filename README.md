
# PLATFORMER

## Overview
This project is an PLATFORMER game built using the [raylib](https://www.raylib.com/) library. The game uses raylib for rendering, input handling, and other graphical features.

## Prerequisites
Before building and running the game, ensure you have the following installed on your system:

- **raylib**: The game relies on raylib for graphical rendering. You can install it via Homebrew or other package managers. For Homebrew:
  ```bash
  brew install raylib
  ```
- **Make**: A build automation tool to compile the game.
- **C Compiler**: Ensure a C compiler like `gcc` or `clang` is installed on your system.

## Configuration
Update the `Makefile` to reflect your raylib installation paths. Below is an example configuration:

```Makefile
INCLUDE_PATH = -I/opt/homebrew/include
LIB_PATH = -L/opt/homebrew/Cellar/raylib/5.0/lib
LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit
```

Replace the paths if raylib is installed in a different location on your system.

## Building the Game

- **Build the game:**
  ```bash
  make
  ```

## Running the Game

- **Run the game:**
  ```bash
  make run
  ```

## Troubleshooting
- Ensure the paths in the `Makefile` point to the correct raylib installation directories.
- If raylib is not installed, install it using your package manager.
- For macOS users, ensure `-framework OpenGL`, `-framework Cocoa`, and `-framework IOKit` are included in the `LDFLAGS`.

## License
This project is distributed under the MIT License. See the `LICENSE` file for details.

## Contributions
Contributions are welcome! Feel free to fork the repository and submit pull requests.
