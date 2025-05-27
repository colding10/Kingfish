#include "piece.h"
#include "consts.h"

char get_piece_identifier(i8 piece) {
    if (!is_piece(piece)) return '.';
    
    Color color = piece_color(piece);
    PieceType type = piece_type(piece);
    
    char base = 'P';
    switch (type) {
        case PT_PAWN: base = 'P'; break;
        case PT_KNIGHT: base = 'N'; break;
        case PT_BISHOP: base = 'B'; break;
        case PT_ROOK: base = 'R'; break;
        case PT_QUEEN: base = 'Q'; break;
        case PT_KING: base = 'K'; break;
        default: return '.';
    }
    
    return color == CL_WHITE ? base : std::tolower(base);
}

i8 piece_from_identifier(char ident) {
    if (ident == '.' || ident == ' ') return NO_PIECE;
    
    Color color = std::isupper(ident) ? CL_WHITE : CL_BLACK;
    PieceType type;
    
    switch (std::toupper(ident)) {
        case 'P': type = PT_PAWN; break;
        case 'N': type = PT_KNIGHT; break;
        case 'B': type = PT_BISHOP; break;
        case 'R': type = PT_ROOK; break;
        case 'Q': type = PT_QUEEN; break;
        case 'K': type = PT_KING; break;
        default: return NO_PIECE;
    }
    
    return make_piece(color, type);
}
