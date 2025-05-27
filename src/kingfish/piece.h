#ifndef KINGFISH_PIECE_H
#define KINGFISH_PIECE_H

#include "types.h"
#include <cctype>

// Piece constants
constexpr i8 NO_PIECE = 0;

// Piece manipulation functions
inline i8 make_piece(Color color, PieceType type) {
    return (color << 3) | type;
}

inline Color piece_color(i8 piece) {
    return (piece >> 3) & 1;
}

inline PieceType piece_type(i8 piece) {
    return piece & 7;
}

inline bool is_piece_type(i8 piece, PieceType type) {
    return piece_type(piece) == type;
}

inline bool is_piece_color(i8 piece, Color color) {
    return piece_color(piece) == color;
}

inline bool is_piece(i8 piece) {
    return piece != NO_PIECE;
}

inline bool is_pawn(i8 piece) {
    return is_piece_type(piece, PT_PAWN);
}

inline bool is_knight(i8 piece) {
    return is_piece_type(piece, PT_KNIGHT);
}

inline bool is_bishop(i8 piece) {
    return is_piece_type(piece, PT_BISHOP);
}

inline bool is_rook(i8 piece) {
    return is_piece_type(piece, PT_ROOK);
}

inline bool is_queen(i8 piece) {
    return is_piece_type(piece, PT_QUEEN);
}

inline bool is_king(i8 piece) {
    return is_piece_type(piece, PT_KING);
}

inline bool is_sliding_piece(i8 piece) {
    return is_bishop(piece) || is_rook(piece) || is_queen(piece);
}

inline bool is_piece_color_type(i8 piece, Color color, PieceType type) {
    return piece == make_piece(color, type);
}

// Piece string conversion functions
char get_piece_identifier(i8 piece);
i8 piece_from_identifier(char ident);

#endif