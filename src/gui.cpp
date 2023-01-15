#include "gui.hpp"

#include <SDL2/SDL.h>

#include <fstream>
#include <iostream>
#include <string>

#include "board.hpp"
#include "pieces.hpp"

// TODO: add board moving
// TODO: add move validation

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("C++ Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    Board board;

    board.readFen(STARTING_FEN);
    board.printBoard();

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                handleMouseClicked(event.button, &board);
            }
        }

        SDL_RenderClear(renderer);        
        drawChessboard(*renderer, &board); 

        SDL_RenderPresent(renderer);  
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void drawChessboard(SDL_Renderer& renderer, Board* board) {
    Piece p;
    std::pair<int, int> selected_piece = board->getSelectedPiece();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            SDL_Rect tile;

            tile.x = j * TILE_SIZE;
            tile.y = i * TILE_SIZE;
            tile.w = TILE_SIZE;
            tile.h = TILE_SIZE;

            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(&renderer, 0xEE, 0xEE, 0xEE, 0xFF);
            } else {
                SDL_SetRenderDrawColor(&renderer, 0x99, 0x99, 0x99, 0xFF);
            }

            if (selected_piece.first == i && selected_piece.second == j) {
                SDL_SetRenderDrawColor(&renderer, 0xFF, 0x00, 0x00, 0xFF);

                SDL_Rect highlight;
                highlight.x = tile.x + 5;
                highlight.y = tile.y + 3;
                highlight.w = tile.w - 10;
                highlight.h = tile.h - 10;

                SDL_SetRenderDrawColor(&renderer, 0xFF, 0x00, 0x00, 0xFF);
                SDL_RenderFillRect(&renderer, &tile);

                if ((i + j) % 2 == 0) {
                    SDL_SetRenderDrawColor(&renderer, 0xEE, 0xEE, 0xEE, 0xFF);
                } else {
                    SDL_SetRenderDrawColor(&renderer, 0x99, 0x99, 0x99, 0xFF);
                }

                SDL_RenderFillRect(&renderer, &highlight);
            } else {
                SDL_RenderFillRect(&renderer, &tile);
            }

            p = board->board[i][j];

            if (p != 0) {
                std::string filepath = "../data/bmp/" + getPieceFilename(p);

                SDL_Surface* image = SDL_LoadBMP(filepath.c_str());
                if (image == NULL) {
                    std::cerr << "image=" << image << " Reason: " << SDL_GetError() << " " << SDL_GetBasePath() << std::endl;
                }
                SDL_Texture* texture = SDL_CreateTextureFromSurface(&renderer, image);

                SDL_RenderCopy(&renderer, texture, NULL, &tile);

                SDL_FreeSurface(image);
                SDL_DestroyTexture(texture);
            }
        }
    }
}

void handleMouseClicked(SDL_MouseButtonEvent event, Board* board) {
    if (event.button == SDL_BUTTON_RIGHT) {
        return;
    }

    std::pair<int, int> board_indices = getBoardIndices(event.x, event.y);

    std::cout << "Mouse clicked: (" << event.x << ", " << event.y << ")";
    std::cout << " Board indices: (" << board_indices.first << ", " << board_indices.second << ")" << std::endl;

    if (board->getSelectedPiece() == board_indices) {
        board->clearSelectedPiece();
    } else if (!board->hasSelectedPiece()) {
        board->setSelectedPiece(board_indices.first, board_indices.second);
    }
}

std::pair<int, int> getBoardIndices(int x, int y) {
    /**
     * Gets board indices from a x, y coordinate of a mouse click.
     *
     * @param x: integer value of x coordiate of mouse click
     * @param y: integer value of y coordiate of mouse click
     *
     * @returns std::pair with integer values from 0-7 in first and second
     */

    std::pair<int, int> out;

    out.second = x / TILE_SIZE;
    out.first = y / TILE_SIZE;

    return out;
}