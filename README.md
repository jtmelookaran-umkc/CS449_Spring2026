# Peg Solitaire — CS 449 Sprint 3

## Project Structure

```
File structure on system; was unable to upload it the same on github and my git was having problems throughout so I could not incrementally update everything. 
solitaire/
├── src/
│   ├── Board.h / Board.cpp          # Abstract board base class
│   ├── BoardTypes.h / BoardTypes.cpp# EnglishBoard, HexagonBoard, DiamondBoard
│   ├── Game.h / Game.cpp            # ManualGame, AutoplayGame
│   ├── GameRecorder.h               # Save/load/replay game files
│   └── ConsoleUI.h                  # Console display 
├── gui/
│   ├── BoardWidget.h / .cpp         # Custom Qt widget — draws board, handles clicks
│   └── MainWindow.h / .cpp          # Main Qt window with all controls
├── tests/
│   └── test_solitaire.cpp           # 35 unit tests (no external framework needed)
├── main.cpp                         # Console app entry point
├── main_gui.cpp                     # Qt GUI entry point
├── Solitaire.pro                    # Qt project file

└── records/                         # Saved game files go here
```

## Class Hierarchy

```
Board (abstract)
├── EnglishBoard   — 7×7 cross, orthogonal moves only
├── HexagonBoard   — hexagonal shape, orthogonal + diagonal moves
└── DiamondBoard   — diamond shape, orthogonal moves only

Game (abstract)
├── ManualGame     — human drives moves; supports randomize
└── AutoplayGame   — computer picks random valid moves
```

## Building the Qt GUI (Recommended)

### Prerequisites
- Qt 6 (or Qt 5.15+): https://www.qt.io/download
- A C++17-capable compiler (MSVC, GCC, or Clang)

### Windows (Qt Creator — easiest)
1. Open **Qt Creator**
2. File → Open File or Project → select `Solitaire.pro`
3. Configure the kit (Qt 6, Desktop)
4. Click **Build** then **Run**

### Linux / macOS (command line)
```bash
# Install Qt6 first:
# Ubuntu:  sudo apt install qt6-base-dev
# macOS:   brew install qt6

mkdir build && cd build
qmake6 ../Solitaire.pro
make -j$(nproc)
./Solitaire
```

### Windows (command line with Qt installed)
```cmd
mkdir build && cd build
qmake ..\Solitaire.pro
nmake          # or mingw32-make if using MinGW
Solitaire.exe
```

## Building the Console App (no Qt needed)

```bash
make           # builds ./solitaire
./solitaire
```

## Running Unit Tests

```bash
make test      # builds and runs all 35 tests
```

Expected output: `35 passed, 0 failed`

## How to Play (GUI)

1. Select **Board Type** (English / Hexagon / Diamond) on the left
2. Click **New Game**
3. Click a **peg** (blue marble) to select it — valid landing holes turn green
4. Click a **green hole** to make the jump
5. Use **Randomize** to shuffle the board state by one move
6. Use **Autoplay** to let the computer play (click again to stop)
7. Tick **Record game** before playing to save your game to a file after it ends
8. Use **Replay** to load a saved `.txt` file and watch it play back

## How to Play (Console)

```
Main Menu:
  1) New Manual Game
  2) New Autoplay Game
  3) Replay Saved Game
  4) Exit

Manual game commands:
  m  — enter a move: fromRow fromCol toRow toCol
  r  — randomize (one random move)
  v  — show valid moves
  q  — quit to menu
```

## Record File Format

Games are saved to `records/` as plain text:
```
BOARD_TYPE English
MOVE 3 1 3 3
MOVE 1 3 3 3
...
```

## GitHub
https://github.com/jtmelookaran-umkc/CS449_Spring2026
