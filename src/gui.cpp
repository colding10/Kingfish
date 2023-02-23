#include "gui.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <tuple>

#include "board.hpp"
#include "game.hpp"
#include "location.hpp"
#include "pieces.hpp"

// TODO: add special moves like castle, en passant
// TODO: add showing checkmate

void GUI::initSDL() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    TTF_Init();
}
void GUI::cleanupSDL(SDL_Renderer *renderer, SDL_Window *window,
                     TTF_Font *font) {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
}

std::tuple<SDL_Window *, SDL_Renderer *, TTF_Font *> GUI::createObjects() {
    initSDL();

    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window,
                                &renderer);

    SDL_SetWindowTitle(window, "C++ Chess");

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    font = TTF_OpenFont("assets/MesloLGS NF Regular.ttf", 50);
    if (font == nullptr) {
        std::cerr << "Failed to load font. Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    return {window, renderer, font};
}

SDL_Renderer *GUI::createSDLRenderer(SDL_Window *window) {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    return SDL_CreateRenderer(window, -1,
                              SDL_RENDERER_PRESENTVSYNC | SDL_GL_DOUBLEBUFFER);
}

void GUI::drawChessboard(SDL_Renderer *renderer, Board *board, TTF_Font *font) {
    Piece p;
    Location selected_piece = board->getSelectedLocation();

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0);
    SDL_RenderClear(renderer);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            SDL_Rect tile;

            tile.x = (j * TILE_SIZE);
            tile.y = (i * TILE_SIZE);
            tile.w = TILE_SIZE;
            tile.h = TILE_SIZE;

            if ((i + j) % 2 == (board->isReversed() ? 1 : 0)) {
                SDL_SetRenderDrawColor(renderer, 0xEE, 0xEE, 0xEE, 0xFF);
            } else {
                SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
            }

            if (selected_piece.X == i && selected_piece.Y == j) {
                SDL_Rect highlight;
                highlight.x = tile.x + 5;
                highlight.y = tile.y + 3;
                highlight.w = tile.w - 10;
                highlight.h = tile.h - 10;

                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
                SDL_RenderFillRect(renderer, &tile);

                if ((i + j) % 2 == (board->isReversed() ? 1 : 0)) {
                    SDL_SetRenderDrawColor(renderer, 0xEE, 0xEE, 0xEE, 0xFF);
                } else {
                    SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
                }

                SDL_RenderFillRect(renderer, &highlight);
            } else {
                SDL_RenderFillRect(renderer, &tile);
            }

            p = board->getPieceAt(Location(i, j));

            if (p != 0) {
                std::string filepath = "../assets/icons/" + Pieces::getPieceFilename(p);

                SDL_Surface *image = SDL_LoadBMP_RW(SDL_RWFromFile(filepath.c_str(), "rb"), 1);
                if (image == NULL) {
                    std::cerr << "image=" << image << " Reason: " << SDL_GetError()
                              << std::endl;
                }
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);

                SDL_RenderCopy(renderer, texture, NULL, &tile);

                SDL_FreeSurface(image);
                SDL_DestroyTexture(texture);
            }

            if (board->hasSelectedPiece() &&
                    Game::isValidMove(board, board->getSelectedLocation(), Location(i, j),
                                      true)) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

                int centerX = tile.x + tile.w / 2;
                int centerY = tile.y + tile.h / 2;
                int radius = 15;
                SDL_RenderFillCircle(renderer, centerX, centerY, radius);
            }
        }
    }

    if (board->isGameOver()) {
        SDL_Color color = {0, 0, 0, 255};
        SDL_Surface *surface = TTF_RenderText_Solid(
                                   font,
                                   (std::string(board->getCheckmatedColor() == BLACK ? "White" : "Black") +
                                    std::string(" wins!"))
                                   .c_str(),
                                   color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        int texW = 0;
        int texH = 0;

        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        SDL_Rect dstrect = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 75, texW,
                            texH
                           };
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

int GUI::SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y,
                              int radius) {
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        } else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        } else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

void GUI::handleMouseClicked(SDL_MouseButtonEvent event, Board *board) {
    if (event.button == SDL_BUTTON_RIGHT) {
        return;
    }

    Location board_location = getBoardIndices(event.x, event.y);

    if (board->getSelectedLocation() == board_location) {  // de-select a piece
        board->clearSelectedPiece();
    } else if (!board->hasSelectedPiece()) {  // select a piece
        if (board->getPieceAt(board_location) != 0 &&
                Pieces::getPieceColor(board->getPieceAt(board_location)) ==
                board->getActiveColor()) {
            board->setSelectedPiece(board_location);
        }
    } else if (board->hasSelectedPiece()) {  // move a piece
        if (Pieces::getPieceColor(board->getPieceAt(board_location)) ==
                Pieces::getPieceColor(
                    board->getPieceAt(board->getSelectedLocation()))) {
            board->setSelectedPiece(board_location);
        } else {
            board->tryMove(Move(board->getSelectedLocation(), board_location, board->getPieceAt(board_location),
                                0));  // TODO: check args
        }
    }
}

void GUI::handleKeyPressed(SDL_KeyboardEvent event, Board *board) {
    if (event.keysym.sym == SDL_GetKeyFromName("r") ||
            event.keysym.sym == SDL_GetKeyFromName("f")) {
        std::cout << "r/f pressed, reversing board" << std::endl;
        board->reverse();
    } else if (event.keysym.sym == SDL_GetKeyFromName("u")) {
        std::cout << "u pressed, undoing move" << std::endl;
        board->undoLastMove();
    }
}

/**
 * Gets board indices from a x, y coordinate of a mouse click.
 *
 * @param x: integer value of x coordiate of mouse click
 * @param y: integer value of y coordiate of mouse click
 *
 * @returns std::pair with integer values from 0-7 in X and Y
 */
Location GUI::getBoardIndices(int x, int y) {
    Location out = Location(y / TILE_SIZE, x / TILE_SIZE);

    return out;
}