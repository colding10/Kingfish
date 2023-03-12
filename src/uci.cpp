#include <atomic>
#include <csignal>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include "ai.hpp"
#include "board.hpp"
#include "bonustables.hpp"
#include "game.hpp"
#include "location.hpp"
#include "move.hpp"
#include "openingbook.hpp"
#include "pieces.hpp"

std::atomic<bool> should_stop(false);

Piece pieceFromChar(const char& c) {
    PieceColor color = isupper(c) ? WHITE : BLACK;

    switch (tolower(c)) {
        case 'p':
            return Pieces::makePiece(PAWN, color);
        case 'r':
            return Pieces::makePiece(ROOK, color);
        case 'n':
            return Pieces::makePiece(KNIGHT, color);
        case 'b':
            return Pieces::makePiece(BISHOP, color);
        case 'q':
            return Pieces::makePiece(QUEEN, color);
        case 'k':
            return Pieces::makePiece(KING, color);
        default:
            return Pieces::makePiece(PAWN, color);
    }
}
Move parseMove(const std::string& move_str, const PieceColor& current_color) {
    if (move_str.size() < 2) {
        throw std::invalid_argument("Invalid move string: " + move_str);
    }

    // Parse the starting and ending locations
    Location starting = Location(move_str.substr(0, 2));
    Location ending = Location(move_str.substr(2, 2));

    // Check if the move has a promotion (e.g. "e7e8q")
    Piece promotion = 0x00;
    if (move_str.size() == 5 && move_str[4] == 'q') {
        promotion = Pieces::makePiece(QUEEN, current_color);
    }

    // Check if the move captures a piece (e.g. "exd5")
    Piece captured = 0x00;
    if (move_str.size() == 4 && move_str[2] == 'x') {
        captured = pieceFromChar(move_str[1]);
    }

    // Create the Move object
    Move move(starting, ending, captured, 0);
    move.setCaptured(captured);
    move.setPromoted(promotion);

    return move;
}

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    while ((end = str.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

/*
 * Handles a singular command in a string.
 * @returns a tuple of `int`, `string` that has the code and info
 */

/*
 * CODE 0 = nothing to worry about
 * CODE 1 = setup board, opt will be the fen
 * CODE 2 = start searching
 *
 */
std::tuple<int, std::string> handleCommand(const std::string& command, Board& board) {
    if (command == "uci") {  // Send engine info to GUI
        std::cout << "id name Kingfish\n";
        std::cout << "id author Colin Ding\n";
        std::cout << "uciok\n";
        return {0, ""};
    }
    if (command == "isready") {
        std::cout << "readyok";
        return {0, ""};
    }
    if (command.substr(0, 8) == "position") {  // Parse FEN string or moves and set up board
        if (command.substr(9, 4) == "startpos") {
            return {1, STARTING_FEN};
        } else {  // Set up position from FEN string
            std::string fen = command.substr(9);
            return {1, fen};
        }
        if (command.find("moves") != std::string::npos) {  // Play moves on the board
            std::vector<std::string> moves = split(command.substr(command.find("moves") + 6), " ");
            for (const std::string& move : moves) {
                board.makeMove(parseMove(move, board.getActiveColor()));
            }
            return {0, ""};
        }
    }
    if (command == "go") {  // Start searching for the best move
        return {2, ""};
    }
    if (command == "stop") { // Stop searching for the best move
        should_stop = true;
    }
    if (command == "quit") {  // Quit the program
        return {10, ""};
    }
}



int uciLoop() {
    readTables("./assets/");
    TranspositionTable transtable;

    std::string input;
    std::string opt;

    int return_code;

    Board board;

    while (getline(std::cin, input)) {
        std::tie(return_code, opt) = handleCommand(input, board);

        switch (return_code) {
            case 10:  // QUIT
                return 0;
            case 1:  // setup board
                board.readFen(opt);
                break;
            case 2:
                std::thread negamax_thread(AI::findBestMove, &board, board.getActiveColor(), 4, 0, transtable);
                break;
        }
    }
    return 0;
}
