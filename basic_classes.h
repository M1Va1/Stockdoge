#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using Bitboard = uint64_t;

// clang-format off
enum Square : uint16_t {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8, 
};
// clang-format on

inline Square WhichSquare(uint8_t rank, uint8_t file) {
    return (Square)(rank * 8 + file);
}

enum PieceType : uint8_t {
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,

    PIECE_NB = 7
};

std::map<char, PieceType> FenPieceCodes = {{'p', PAWN}, {'n', KNIGHT}, {'b', BISHOP},
                                           {'r', ROOK}, {'q', QUEEN},  {'k', KING}};
std::map<PieceType, char> PieceLetters = {{NONE, '.'}, {PAWN, 'p'},  {KNIGHT, 'n'}, {BISHOP, 'b'},
                                          {ROOK, 'r'}, {QUEEN, 'q'}, {KING, 'k'}};

enum Color : uint8_t {
    WHITE,
    BLACK,

    COLOR_NB = 2
};

struct Piece {
    PieceType type;
    Color color;
};

enum Direction : int8_t {
    UP = 8,
    DOWN = -8,
    LEFT = -1,
    RIGHT = 1,

    UP_LEFT = 7,
    UP_RIGHT = 9,
    DOWN_LEFT = -9,
    DOWN_RIGHT = -7
};

Bitboard SquareToBitboard(const Square square) {
    return Bitboard(1) << square;
}

Bitboard MoveSquare(const Bitboard bb, const Direction dir) {
    return dir > 0 ? bb << dir : bb >> -dir;
}

enum MoveType : uint16_t {
    NORMAL,

    PROMOTION = 1 << 14,
    EN_PASSANT = 2 << 14,
    CASTLING = 3 << 14
};

/*
0-5  -- initial square
6-11 -- destination square
...  -- something in future
*/
class Move {
public:
    Move() : description(0) {};
    Move(Square from, Square to);

    void SetFrom(const Square from);
    void SetTo(const Square to);

    Square GetFrom() const;
    Square GetTo() const;

private:
    Bitboard description;
};

enum MoveMask : uint16_t {
    FROM_MASK = 0b1111110000000000,
    TO_MASK   = 0b0000001111110000,
};

bool IsOccupied(const Bitboard bb, const Square sq);

bool AvailableMove(const Move move);

class ChessBoard {
public:
    ChessBoard(std::string input);
    ChessBoard();

    void SetPiece(const PieceType pt, const Color c, const Square square);
    Piece RemovePiece(const Square square);
    Piece PieceOnSquare(const Square sq);
    Bitboard GetPieces(const Color color, const PieceType piece) const;

    void MakeMove(const Move cur_move);
    
    void PrintBoard() const;

    ChessBoard& operator=(const ChessBoard& rhs) = default;

private:
    std::array<Bitboard, PIECE_NB> pieces;
    std::array<Bitboard, COLOR_NB> colors;
};
