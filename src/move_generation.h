#pragma once
#include "basic_classes.h"
#include "precomputed.h"
#include "magic.h"
#include "move.h"

bool IsOccupied(Bitboard bb, Square sq);

void PrintKnightMasks();
void PrintKingMask();

class ChessBoard {
public:
    ChessBoard(const std::string& input);
    ChessBoard();

    void SetPiece(PieceType pt, Color c, Square square);
    Piece RemovePiece(Square square);
    Piece PieceOnSquare(Square sq);
    Bitboard GetPieces(Color color, PieceType piece) const;
    Bitboard GetEmptySquares() const {
        return pieces[NONE];
    };
    Bitboard MoveToFriendSide(Bitboard bb);
    Bitboard CalcNewAttackMap(Move move);

    void MakeMove(Move cur_move);
    void GenPawnMoves(Color color);
    void GenKnightMoves(Color color);
    void GenBishopMoves(Color color, const MagicGenerator& magic_generator);
    void GenRookMoves(Color color, const MagicGenerator& magic_generator);
    void GenQueenMoves(Color color, const MagicGenerator& magic_generator);
    void GenKingMoves(Color color);
    void ClearMoves();
    void GenPromotions(Square from, Square to);
    void GenAllMoves(Color color, const MagicGenerator& magic_generator);

    void PrintBoard() const;

    bool IsDoublePush(Move move);
    bool IsInCheck(Color color, const MagicGenerator &magic_generator);

    ChessBoard& operator=(const ChessBoard& rhs) = default;

    // private:
    std::vector<Move> moves;
    std::array<Bitboard, PIECE_NB> pieces;
    std::array<Bitboard, COLOR_NB> colors;
    Bitboard AttackMap;

    Move last_move;
};

bool AvailableMove(Move move);

// std::vector<Move> GenLegalMoves(ChessBoard cur_board);
