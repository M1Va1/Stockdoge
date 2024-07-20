#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using Bitboard = uint64_t;

uint8_t FROM_BITS = 10;
uint8_t TO_BITS = 4;

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

enum Rank : Bitboard {
    RANK_1 = 0b11111111ULL,
    RANK_2 = RANK_1 << UP,
    RANK_3 = RANK_2 << UP,
    RANK_4 = RANK_3 << UP,
    RANK_5 = RANK_4 << UP,
    RANK_6 = RANK_5 << UP,
    RANK_7 = RANK_6 << UP,
    RANK_8 = RANK_7 << UP
};

enum File : Bitboard {
    FILE_A = 0x0101010101010101ULL,
    FILE_B = FILE_A << 1,
    FILE_C = FILE_A << 2,
    FILE_D = FILE_A << 3,
    FILE_E = FILE_A << 4,
    FILE_F = FILE_A << 5,
    FILE_G = FILE_A << 6,
    FILE_H = FILE_A << 7
};

enum MoveType : uint16_t {
    NORMAL,

    PROMOTION = 1 << 14,
    EN_PASSANT = 2 << 14,
    CASTLING = 3 << 14
};

enum MoveMask : uint16_t {
    FROM_MASK = 0b1111110000000000,
    TO_MASK = 0b0000001111110000,
};

inline Square WhichSquare(uint8_t rank, uint8_t file) {
    return (Square)(rank * 8 + file);
}

std::map<char, PieceType> FenPieceCodes = {{'p', PAWN}, {'n', KNIGHT}, {'b', BISHOP},
                                           {'r', ROOK}, {'q', QUEEN},  {'k', KING}};
std::map<PieceType, char> PieceLetters = {{NONE, '.'}, {PAWN, 'p'},  {KNIGHT, 'n'}, {BISHOP, 'b'},
                                          {ROOK, 'r'}, {QUEEN, 'q'}, {KING, 'k'}};
std::map<int, char> FileLetters = {{0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}, {4, 'E'}, {5, 'F'}, {6, 'G'}, {7, 'H'}};

std::array<std::vector<Direction>, 8> KnightMoves = {{{UP, UP, LEFT},
                                                      {UP, UP, RIGHT},
                                                      {RIGHT, RIGHT, UP},
                                                      {RIGHT, RIGHT, DOWN},
                                                      {DOWN, DOWN, RIGHT},
                                                      {DOWN, DOWN, LEFT},
                                                      {LEFT, LEFT, DOWN},
                                                      {LEFT, LEFT, UP}}};

std::string SquareToString(Square sq) {
    return std::string(1, FileLetters[sq % 8]) + std::to_string(sq / 8 + 1);
}

Bitboard SquareToBitboard(const Square square) {
    return Bitboard(1) << square;
}

bool isWithinBounds(Bitboard position, Direction dir) {
    if ((position & FILE_A) != 0) {
        if (dir == LEFT || dir == UP_LEFT || dir == DOWN_LEFT) {
            return false;
        }
    }
    if ((position & FILE_H) != 0) {
        if (dir == RIGHT || dir == UP_RIGHT || dir == DOWN_RIGHT) {
            return false;
        }
    }
    if ((position & RANK_8) != 0) {
        if (dir == UP || dir == UP_LEFT || dir == UP_RIGHT) {
            return false;
        }
    }
    if ((position & RANK_1) != 0) {
        if (dir == DOWN || dir == DOWN_LEFT || dir == DOWN_RIGHT) {
            return false;
        }
    }
    return true;
}

Bitboard MoveSquare(Bitboard bb, Direction dir) {
    if (!isWithinBounds(bb, dir)) {
        return bb;
    }
    if (dir > 0) {
        return bb << dir;
    }
    return bb >> -dir;
}

Bitboard MoveSquare(Bitboard bb, std::vector<Direction> dirs) {
    Bitboard initial_bb = bb;
    for (Direction dir : dirs) {
        if (!isWithinBounds(bb, dir)) {
            return initial_bb;
        }
        bb = MoveSquare(bb, dir);
    }
    return bb;
}

std::vector<Square> GetSquares(Bitboard bb) {
    std::vector<Square> squares;
    while (bb) {
        squares.push_back(static_cast<Square>(std::countr_zero(bb)));
        bb &= bb - 1;
    }
    return squares;
}

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
    uint16_t description;
};

bool IsOccupied(Bitboard bb, Square sq);

bool AvailableMove(Move move);

class ChessBoard {
public:
    ChessBoard(std::string input);
    ChessBoard();

    void SetPiece(PieceType pt, Color c, Square square);
    Piece RemovePiece(Square square);
    Piece PieceOnSquare(Square sq);
    Bitboard GetPieces(Color color, PieceType piece) const;
    Bitboard GetEmptySquares() const {
        return pieces[NONE];
    };

    void MakeMove(Move cur_move);
    std::vector<Move> GenPawnMoves(const Color color);
    std::vector<Move> GenKnightMoves(const Color color);

    void PrintBoard() const;

    ChessBoard& operator=(const ChessBoard& rhs) = default;

private:
    std::array<Bitboard, PIECE_NB> pieces;
    std::array<Bitboard, COLOR_NB> colors;
};
