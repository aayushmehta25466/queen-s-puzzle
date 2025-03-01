# Queen's Puzzle

Queen’s Puzzle is an SDL-based puzzle game written in C. The project demonstrates how to use SDL2 (and optionally SDL2_image and SDL2_ttf) for game development. This README explains the project structure and provides detailed instructions for setting up SDL2 on a Windows laptop using MinGW.

---

## Project Structure

- **.gitignore**  
- **.vscode/**  
  - `c_cpp_properties.json`
  - `settings.json`
- **assets/**  
  - `cell.png`
  - `font.ttf`
  - `queen.png`
- **game.c** – Main game source code  
- **game_terminal.c** – Alternate terminal version  
- **Makefile** – Build file for compiling the project  
- **SDL2.dll, SDL2_image.dll, SDL2_ttf.dll** – SDL runtime libraries  
- **src/**  
  - **include/**  
    - SDL header files (e.g. files in the SDL2 folder)
  - **lib/**  
    - SDL static libraries (e.g. `libSDL2.a`, `libSDL2main.a`, etc.)

---

## How to Build and Run

1. **Open the project folder** in Visual Studio Code.
2. Ensure you have MinGW installed.
3. From a terminal, run the following command to compile your desired source file (for example, `game.c`):

   ```sh
   make game

4. Run the executable (either by double-clicking game.exe or running it from the terminal):

   ```sh
   ./game.exe
   ```

---

## Setting Up SDL2 on Windows (MinGW)

Follow these steps to configure SDL2 for this project using MinGW.

### Step 1: Download and Extract SDL2

1. Download SDL 2.0.20 (or your preferred version) from the [official SDL website](https://www.libsdl.org/download-2.0.php).
2. Choose the MinGW package.
3. Extract the downloaded ZIP file to a convenient location.

### Step 2: Copy Necessary Files (Local SDL Setup)

1. Inside the extracted folder, locate the `include` and `lib` directories.
2. Copy both the folders.
3. In the project workspace, create (or confirm) the src folder.
4. Paste the `include` and `lib` folders into src so that SDL’s headers and libraries are available locally.

> **Alternate Setup:**  
> Alternatively, add the include and lib folders to the MinGW compiler's include and lib folders.

### Step 3: Copy SDL Runtime Libraries

1. Open the `bin` directory in the extracted SDL folder.
2. Copy SDL2.dll (and SDL2_image.dll, SDL2_ttf.dll as needed).
3. Paste these DLLs into the project’s root directory (where game.exe is generated).

### Step 4: Create/Review the Makefile

#### Option 1: SDL in Your Project Directory

If you copied SDL's `include` and `lib` folders to src, use this Makefile:

```makefile
# Directories
INCLUDE_DIR = src/include
LIB_DIR = src/lib

# Compiler and flags
CXX = g++
CXXFLAGS = -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

# Default target
all: help

# Rule to build a binary for any given C file
%: %.c
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# Help target
help:
	@echo "Usage: make <filename (without .c)>"
	@echo "Example: make game"
```

#### Option 2: SDL Installed in MinGW

If SDL is installed in MinGW itself, update the Makefile to reference the MinGW paths:

```makefile
# Compiler and flags
CXX = g++
CXXFLAGS = -I"C:/MinGW/include"
LDFLAGS = -L"C:/MinGW/lib" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

# Default target
all: help

# Rule to build a binary for any given C file
%: %.c
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# Help target
help:
	@echo "Usage: make <filename (without .c)>"
	@echo "Example: make game"
```

Choose the option that matches your SDL setup and adjust your Makefile accordingly.

### Step 5: Install Make (if needed)

1. **Install Chocolatey** (if you haven't already):
   - Open Command Prompt as Administrator.
   - Execute:
     ```powershell
     Set-ExecutionPolicy Bypass -Scope Process -Force; `
     [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; `
     iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
     ```
2. **Install Make** with Chocolatey:
   ```sh
   choco install make
   ```
3. Restart your terminal and verify installation:
   ```sh
   make --version
   ```

---

## Running the Game

After completing the SDL setup and compiling your project:

1. Run the executable (`game.exe`) in the project root.
2. Enjoy solving the Queen’s Puzzle!

---

## Conclusion

This project demonstrates SDL2-based game development with flexibility in how SDL is incorporated (either locally or via MinGW). Follow the instructions above to set up your environment and start developing or modifying the game.
