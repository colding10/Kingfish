# C++ Chess w/ SDL2

- making chess in C++ as a learning experience
- pull request for comments, concerns, anything

## Development Timeline

*italics: done*
**bold: next**

1. *Complete negamax implementation solidly*
2. *Complete GUI and features (very basic)*
3. **Add more improvements/optimization until it can consistently beat me (ELO ~1000-1200)**
4. Complete GUI and features (make feature rich, nice UX)
5. Convert program (GUI) into library (while still maintaining interface)
6. Implement engine interfaces and advanced improvements
7. Play engine against other programs to rank ELO

## GUI Features

*italics: done*
**bold: todo**

- **Sidebar menu**
    1. Board flipping available
    2. Undo moves
    3. View move history
    4. Have a clock
- Show engine evaluation of game (left bar, like `chess.com`)
- Show engine running status
- Improved highlighting (check, checkmate, pins)
- Keyboard shortcuts for everything
- Help menu

## Implementation

- SDL2 for GUI
- Self-written classes for board and piece representation
- Negamax function with alpha-beta pruning for AI opponent

## Project Structure

bin     - the output binary <br/>
src     - the source files (.cpp) <br/>
include - the header files (.hpp) <br/>

## Dependecies

- SDL2
- SDL2_image
- SDL2_ttf

this project uses the `C++11` standard and compiles with `g++-12`

## Compiling

- run the `make` command.
- this will create object files in `./obj`.
- these get combined and linked together
- creating an executable in `./bin`

**SUGGESTIONS WELCOME**
