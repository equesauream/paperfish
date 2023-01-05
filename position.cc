#include "position.h"
#include "square.h"
#include "piece.h"

#include <sstream>
#include <cmath>
#include <bitset>
#include <cctype>
#include <iostream>

namespace engine {

const std::map<char, int> Position::pieceMap = 
    std::map<char, int>{std::pair<char, int>('P', 0), 
                        std::pair<char, int>('N', 1), 
                        std::pair<char, int>('B', 2), 
                        std::pair<char, int>('R', 3), 
                        std::pair<char, int>('Q', 4), 
                        std::pair<char, int>('K', 5), 
                        std::pair<char, int>('p', 6), 
                        std::pair<char, int>('n', 7), 
                        std::pair<char, int>('b', 8), 
                        std::pair<char, int>('r', 9), 
                        std::pair<char, int>('q', 10), 
                        std::pair<char, int>('k', 11)};

const std::map<int, char> Position::intMap = 
    std::map<int, char>{std::pair<int, char>(0, 'P'), 
                        std::pair<int, char>(1, 'N'), 
                        std::pair<int, char>(2, 'B'), 
                        std::pair<int, char>(3, 'R'), 
                        std::pair<int, char>(4, 'Q'), 
                        std::pair<int, char>(5, 'K'), 
                        std::pair<int, char>(6, 'p'), 
                        std::pair<int, char>(7, 'n'), 
                        std::pair<int, char>(8, 'b'), 
                        std::pair<int, char>(9, 'r'), 
                        std::pair<int, char>(10, 'q'), 
                        std::pair<int, char>(11, 'k')};

Position::Position() : Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}

Position::Position(const std::string& FEN) {
    std::stringstream ss;
    ss << FEN;

    std::string pieces, t, castle, enP, hm, fm;
    ss >> pieces >> t >> castle >> enP >> hm >> fm;

    std::vector<char> v = std::vector<char>{'P', 'N', 'B', 'R', 'Q', 'K',
                                            'p', 'n', 'b', 'r', 'q', 'k'};


    // Pieces //
    thePosition = std::vector<U64>(12, 0);


    for (const auto& p : v) {
        U64 board = 0;
        int r = 0;
        int c = 0;
        for (const auto& pos : pieces) {
            if (pos == '/') { 
                ++r; 
                c = 0;
            } else if (pos >= '1' && pos <= '8') {
                c += int(pos - '0');
            } else if (pos == p) {
                board = board + (1ULL << (8*r + c));
                ++c;
            } else {
                ++c;
            }
        }
        thePosition.at(pieceMap.at(p)) = board;
    }


    // Turn //
    if (t == "b") turn = 1;
    else turn = 0;


    // Castling Rights //

    castleRights = 0;

    for (const auto& c : castle) {
        if (c == '-') break;
        if (c == 'K') castleRights += 1 << 3;
        if (c == 'Q') castleRights += 1 << 2;
        if (c == 'k') castleRights += 1 << 1;
        if (c == 'q') castleRights += 1 << 0;
    }


    // En Passent //
    if (enP == "-")
        enPassant = NoSquare;
    else
        enPassant = squareToBit(enP);


    halfMove = std::stoi(hm);
    fullMove = std::stoi(fm);


    // set up the whitepieces and blackpieces bitboard //
    updateBoards();

    OthePosition = thePosition;
    OwhitePieces = whitePieces;
    OblackPieces = blackPieces;

    Oturn = turn;
    OcastleRights = castleRights;
    OenPassant = enPassant;
    OhalfMove = halfMove;
    OfullMove = fullMove;
}

Position::Position(const Position& other) :
    thePosition {other.thePosition},
    whitePieces {other.whitePieces},
    blackPieces {other.blackPieces},
    
    OthePosition {other.OthePosition},
    OwhitePieces {other.OwhitePieces},
    OblackPieces {other.OblackPieces},

    turn {other.turn},
    castleRights {other.castleRights},
    enPassant {other.enPassant},
    halfMove {other.halfMove},
    fullMove {other.fullMove},

    Oturn {other.Oturn},
    OcastleRights {other.OcastleRights},
    OenPassant {other.OenPassant},
    OhalfMove {other.OhalfMove},
    OfullMove {other.OfullMove}
{}

Position::Position(const Position&& other) : 
    thePosition {other.thePosition},
    whitePieces {other.whitePieces},
    blackPieces {other.blackPieces},
    
    OthePosition {other.OthePosition},
    OwhitePieces {other.OwhitePieces},
    OblackPieces {other.OblackPieces},

    turn {other.turn},
    castleRights {other.castleRights},
    enPassant {other.enPassant},
    halfMove {other.halfMove},
    fullMove {other.fullMove},

    Oturn {other.Oturn},
    OcastleRights {other.OcastleRights},
    OenPassant {other.OenPassant},
    OhalfMove {other.OhalfMove},
    OfullMove {other.OfullMove}
{}

Position& Position::operator=(Position other) {
    thePosition = std::move(other.thePosition);
    whitePieces = std::move(other.whitePieces);
    blackPieces = std::move(other.blackPieces);

    OthePosition = std::move(other.OthePosition);
    OwhitePieces = std::move(other.OwhitePieces);
    OblackPieces = std::move(other.OblackPieces);

    turn = std::move(other.turn);
    castleRights = std::move(other.castleRights);
    enPassant = std::move(other.enPassant);
    halfMove = std::move(other.halfMove);
    fullMove = std::move(other.fullMove);

    Oturn = std::move(other.Oturn);
    OcastleRights = std::move(other.OcastleRights);
    OenPassant = std::move(other.OenPassant);
    OhalfMove = std::move(other.OhalfMove);
    OfullMove = std::move(other.OfullMove);
    return *this;
}

Position::~Position() {}

std::string Position::toFEN() const {
    std::string tmp = std::string(64, '-');
    int count = 0;
    char c;
    for (const auto& board : thePosition) {
        c = intMap.at(count);

        U64 cpy = board;
        int pos = 0;
        while (cpy != 0) {
            while (cpy < (1ULL << 63)) {
                ++pos;
                cpy = cpy << 1;
            }
            tmp.at(63 - pos) = c;
            ++pos;
            cpy = cpy << 1;
        }
        ++count;
    }

    // insert `/` every 8 characters
    for (int i = 56; i > 0; i = i - 8) {
        tmp.insert(i, "/");
    }

    // remove consecutive `-`s
    std::vector<std::string> dashes = std::vector<std::string>{ "--------",
                                                                "-------",
                                                                "------",
                                                                "-----",
                                                                "----",
                                                                "---",
                                                                "--",
                                                                "-",
                                                                };

    for (const auto& i : dashes) {
        int len = i.length();
        while (tmp.find(i) != std::string::npos) {
            tmp.replace(tmp.find(i), len, std::to_string(len));
        }
    }

    // turn
    tmp += " ";
    if (turn == 0) tmp += "w";
    else tmp += "b";

    // castling rights
    tmp += " ";
    if (castleRights == 0) tmp += "-";
    else {
        for (int i = 0; i < 4; ++i) {
            if (castleRights & (1 << 3)) {
                if (i == 0) tmp += "K";
                if (i == 1) tmp += "Q";
                if (i == 2) tmp += "k";
                if (i == 3) tmp += "q";
            }
        }
    }

    // en passent
    tmp += " ";
    tmp += enPassant;


    // halfmoves
    tmp += " ";
    tmp += std::to_string(halfMove);

    
    // fullmoves
    tmp += " ";
    tmp += std::to_string(fullMove);
    return tmp;
}

std::ostream& operator<<(std::ostream& out, const Position& pos) {
    std::vector<std::vector<char>> tmp = std::vector<std::vector<char>>(8, std::vector<char>(8, '-'));
    int count = 0;
    char c;
    for (const auto& board : pos.thePosition) {
        c = Position::intMap.at(count);

        U64 cpy = board;
        int pos = 0;
        while (cpy != 0) {
            while (cpy < (1ULL << 63)) {
                ++pos;
                cpy = cpy << 1;
            }
            tmp.at(7 - (pos/8)).at(7 - (pos % 8)) = c;
            ++pos;
            cpy = cpy << 1;
        }
        ++count;
    }
    for (const auto& i : tmp) {
        for (const auto& j : i) {
            out << j;
        }
        out << '\n';
    }
    return out;
}

bool Position::isValid(const Move& m) {
    return isValidMove(m, m.piece, m.piece);
}

bool Position::isValidMove(const Move& m, char piece, char orig) {
    Square source = m.source;
    Square dest = m.dest;
    char prom = m.promotionPiece;

    for (const auto& s : possibleMoves(piece, source)) {
        if (s == m) goto NEXT;
    }
    return false;
  NEXT:
    
    if (tolower(piece) == piece && (dest & blackPieces) != 0) return false;
    if (toupper(piece) == piece && (dest & whitePieces) != 0) return false;

    if (piece == 'p') {
        if (colNumber(source) != colNumber(dest) && rowNumber(source) != rowNumber(dest)) {
            if (dest != enPassant && (dest & whitePieces) == 0) return false;
        } else {
            if ((dest & blackPieces) != 0 || (dest & whitePieces) != 0) return false;
        }
    } else if (piece == 'P') {
        if (colNumber(source) != colNumber(dest) && rowNumber(source) != rowNumber(dest)) {
            if (dest != enPassant && (dest & blackPieces) == 0) return false;
        } else {
            if ((dest & whitePieces) != 0 || (dest & blackPieces) != 0) return false;
        }
    }
    
    if (tolower(piece) == 'n') {
        move(m);
        if (tolower(piece) == piece && blackInCheck()) {
            unmove();
            return false;
        }
        if (toupper(piece) == piece && whiteInCheck()) {
            unmove();
            return false;
        }
        unmove();
        return true;
    }
    // check if path is blocked for b, r, q
    if (piece == 'r' || piece == 'b') return isValidMove(m, 'q', orig);
    if (piece == 'R' || piece == 'B') return isValidMove(m, 'Q', orig);

    if (piece == 'K') {
        std::string enemyKing = bitToSquare(thePosition.at(pieceMap.at('k')));

        if (enemyKing.at(0) - colNumber(dest) <= 1 && enemyKing.at(0) - colNumber(dest) >= -1 &&
            enemyKing.at(1) - rowNumber(dest) <= 1 && enemyKing.at(1) - rowNumber(dest) >= -1) {
            return false;
        }

        move(m);
        if (whiteInCheck()) {
            unmove();
            return false;
        }
        unmove();

        if (dest == C1 && source == E1) {
            if ((B1 & blackPieces) != 0 || 
                (B1 & whitePieces) != 0) return false;
        }

        if ((dest == G1 && source == E1) || (dest == C1 && source == E1)) {
            if (whiteInCheck()) return false;
            Square kpos, rpos, rook, king;
            king = E1;
            kpos = dest;
            if (kpos == G1) {
                if ((castleRights & (1 << 3)) == 0) return false;
                rook = H1;
                rpos = F1;
            } else if (kpos == C1) {
                if ((castleRights & (1 << 2)) == 0) return false;
                rook = A1;
                rpos = D1;
            } else {
                return false;
            }

            if (!((thePosition.at(pieceMap.at('K'))) & king) ||
                !((thePosition.at(pieceMap.at('R'))) & rook)) return false;

            if ((kpos & blackPieces) != 0 || 
                (rpos & blackPieces) != 0) return false;
            if ((kpos & whitePieces) != 0 || 
                (rpos & whitePieces) != 0) return false;

            move(Move(E1, rpos, 'K'));
            if (whiteInCheck()) {
                unmove();
                return false;
            }
            unmove();

        }
        return true;
    } else if (piece == 'k') {
        move(m);
        if (blackInCheck()) {
            unmove();
            return false;
        }
        unmove();

        if (dest == C8 && source == E8) {
            if ((B8 & blackPieces) != 0 || 
                (B8 & whitePieces) != 0) return false;
        }

        if ((dest == G8 && source == E8) || (dest == C8 && source == E8)) {
            if (blackInCheck()) return false;
            Square kpos, rpos, rook, king;
            king = E8;
            kpos = dest;
            if (kpos == G8) {
                if ((castleRights & (1 << 1)) == 0) return false;
                rook = H8;
                rpos = F8;
            } else if (kpos == C8) {
                if ((castleRights & (1 << 0)) == 0) return false;
                rook = A8;
                rpos = D8;
            } else {
                return false;
            }

            if (!((thePosition.at(pieceMap.at('k'))) & king) ||
                !((thePosition.at(pieceMap.at('r'))) & rook)) return false;

            if ((kpos & blackPieces) != 0 || 
                (rpos & blackPieces) != 0) return false;
            if ((kpos & whitePieces) != 0 || 
                (rpos & whitePieces) != 0) return false;

            move(Move(E8, rpos, 'k'));
            if (blackInCheck()) {
                unmove();
                return false;
            }
            unmove();
        }
        return true;
    }

    if (isMoveBlocked(Move(source, dest, orig, prom))) return false;

    move(Move(source, dest, orig, prom));
    if (tolower(piece) == piece && blackInCheck()) {
        unmove();
        return false;
    }
    if (toupper(piece) == piece && whiteInCheck()) {
        unmove();
        return false;
    }
    unmove();

    return true;
} // isValidMove

// source and dest are 2-length strings
bool Position::isMoveBlocked(const Move& m) const {
    Square source = m.source;
    Square dest = m.dest;
    Square cur = source;
    if (colNumber(source) == colNumber(dest) && rowNumber(source) <= rowNumber(dest)) {
        cur >>= 8;
        while (cur != dest) {
            if ((cur & blackPieces) != 0 || 
                (cur & whitePieces) != 0) return true;
            cur >>= 8;
        }
    } else if (colNumber(source) == colNumber(dest) && rowNumber(source) >= rowNumber(dest)) {
        cur <<= 8;
        while (cur != dest) {
            if ((cur & blackPieces) != 0 || 
                (cur & whitePieces) != 0) return true;
            cur <<= 8;
        }
    } else if (colNumber(source) >= colNumber(dest) && rowNumber(source) == rowNumber(dest)) {
        cur >>= 1;
        while (cur != dest) {
            if ((cur & blackPieces) != 0 || 
                (cur & whitePieces) != 0) return true;
            cur >>= 1;
        }
    } else if (colNumber(source) <= colNumber(dest) && rowNumber(source) == rowNumber(dest)) {
        cur <<= 1;
        while (cur != dest) {
            if ((cur & blackPieces) != 0 || 
                (cur & whitePieces) != 0) return true;
            cur <<= 1;
        }
    } else if (colNumber(source) <= colNumber(dest) && rowNumber(source) <= rowNumber(dest)) {
        cur >>= 7;
        while (cur != dest) {
            if ((cur & blackPieces) != 0 || 
                (cur & whitePieces) != 0) return true;
            cur >>= 7;
        }
    } else if (colNumber(source) >= colNumber(dest) && rowNumber(source) >= rowNumber(dest)) {
        cur <<= 7;
        while (cur != dest) {
            if ((cur & blackPieces) != 0 || 
                (cur & whitePieces) != 0) return true;
            cur <<= 7;
        }
    } else if (colNumber(source) <= colNumber(dest) && rowNumber(source) >= rowNumber(dest)) {
        cur <<= 9;
        while (cur != dest) {
            if ((cur & blackPieces) != 0 || 
                (cur & whitePieces) != 0) return true;
            cur <<= 9;
        }
    } else if (colNumber(source) >= colNumber(dest) && rowNumber(source) <= rowNumber(dest)) {
        cur >>= 9;
        while (cur != dest) {
            if ((cur & blackPieces) != 0 || 
                (cur & whitePieces) != 0) return true;
            cur >>= 9;
        }
    }
    return false;
}

// pos is a square, e.g. "e4"
std::vector<Move> Position::validMoves(char p, Square pos) {
    std::vector<Move> tmp;
    for (const auto& i : possibleMoves(p, pos)) {
        if (isValid(i)) {
            tmp.push_back(i);
        }
    }
    return tmp;
}

// pos is a square, e.g. "e4"
std::vector<Move> Position::possibleMoves(char p, Square pos) const {
    if (p == 'p') {
        std::vector<Move> tmp;
        if (rowNumber(pos) == 7 && colNumber(pos) < 'h' && colNumber(pos) > 'a') {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8,  'p'),
                    Move(pos, pos << 16, 'p'),
                    Move(pos, pos << 7,  'p'),
                    Move(pos, pos << 9,  'p')
            };
        } else if (rowNumber(pos) == 7 && colNumber(pos) == 'a') {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8,  'p'),
                    Move(pos, pos << 16, 'p'),
                    Move(pos, pos << 9,  'p')
            };
        } else if (rowNumber(pos) == 7 && colNumber(pos) == 'h') {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8,  'p'),
                    Move(pos, pos << 16, 'p'),
                    Move(pos, pos << 7,  'p')
            };
        } else if (rowNumber(pos) == 2 && colNumber(pos) < 'h' && colNumber(pos) > 'a') {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8, 'p', 'n'),
                    Move(pos, pos << 8, 'p', 'b'),
                    Move(pos, pos << 8, 'p', 'r'),
                    Move(pos, pos << 8, 'p', 'q'),

                    Move(pos, pos << 7, 'p', 'n'),
                    Move(pos, pos << 7, 'p', 'b'),
                    Move(pos, pos << 7, 'p', 'r'),
                    Move(pos, pos << 7, 'p', 'q'),

                    Move(pos, pos << 9, 'p', 'n'),
                    Move(pos, pos << 9, 'p', 'b'),
                    Move(pos, pos << 9, 'p', 'r'),
                    Move(pos, pos << 9, 'p', 'q'),
            };
        } else if (rowNumber(pos) == 2 && colNumber(pos) == 'a') {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8, 'p', 'n'),
                    Move(pos, pos << 8, 'p', 'b'),
                    Move(pos, pos << 8, 'p', 'r'),
                    Move(pos, pos << 8, 'p', 'q'),

                    Move(pos, pos << 9, 'p', 'n'),
                    Move(pos, pos << 9, 'p', 'b'),
                    Move(pos, pos << 9, 'p', 'r'),
                    Move(pos, pos << 9, 'p', 'q'),
            };
        } else if (rowNumber(pos) == 2 && colNumber(pos) == 'h') {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8, 'p', 'n'),
                    Move(pos, pos << 8, 'p', 'b'),
                    Move(pos, pos << 8, 'p', 'r'),
                    Move(pos, pos << 8, 'p', 'q'),

                    Move(pos, pos << 7, 'p', 'n'),
                    Move(pos, pos << 7, 'p', 'b'),
                    Move(pos, pos << 7, 'p', 'r'),
                    Move(pos, pos << 7, 'p', 'q'),
            };
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) < 'h' && colNumber(pos) > 'a') {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8, 'p'),
                    Move(pos, pos << 7, 'p'),
                    Move(pos, pos << 9, 'p')
            };
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) == 'h') {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8, 'p'),
                    Move(pos, pos << 7, 'p')
            };
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) == 'a') {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8, 'p'),
                    Move(pos, pos << 9, 'p')
            };
        } else {
            std::cout << "invalid black pawn position: " << bitToSquare(pos) << '\n';
        }
        return tmp;
    } else if (p == 'P') {
        std::vector<Move> tmp;
        if (rowNumber(pos) == 2 && colNumber(pos) < 'h' && colNumber(pos) > 'a') {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8,  'P'),
                    Move(pos, pos >> 16, 'P'),
                    Move(pos, pos >> 7,  'P'),
                    Move(pos, pos >> 9,  'P')
            };
        } else if (rowNumber(pos) == 2 && colNumber(pos) == 'a') {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8,  'P'),
                    Move(pos, pos >> 16, 'P'),
                    Move(pos, pos >> 7,  'P')
            };
        } else if (rowNumber(pos) == 2 && colNumber(pos) == 'h') {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8,  'P'),
                    Move(pos, pos >> 16, 'P'),
                    Move(pos, pos >> 9,  'P')
            };
        } else if (rowNumber(pos) == 7 && colNumber(pos) < 'h' && colNumber(pos) > 'a') {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8, 'P', 'N'),
                    Move(pos, pos >> 8, 'P', 'B'),
                    Move(pos, pos >> 8, 'P', 'R'),
                    Move(pos, pos >> 8, 'P', 'Q'),

                    Move(pos, pos >> 7, 'P', 'N'),
                    Move(pos, pos >> 7, 'P', 'B'),
                    Move(pos, pos >> 7, 'P', 'R'),
                    Move(pos, pos >> 7, 'P', 'Q'),

                    Move(pos, pos >> 9, 'P', 'N'),
                    Move(pos, pos >> 9, 'P', 'B'),
                    Move(pos, pos >> 9, 'P', 'R'),
                    Move(pos, pos >> 9, 'P', 'Q'),
            };
        } else if (rowNumber(pos) == 7 && colNumber(pos) == 'a') {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8, 'P', 'N'),
                    Move(pos, pos >> 8, 'P', 'B'),
                    Move(pos, pos >> 8, 'P', 'R'),
                    Move(pos, pos >> 8, 'P', 'Q'),

                    Move(pos, pos >> 7, 'P', 'N'),
                    Move(pos, pos >> 7, 'P', 'B'),
                    Move(pos, pos >> 7, 'P', 'R'),
                    Move(pos, pos >> 7, 'P', 'Q'),
            };
        } else if (rowNumber(pos) == 7 && colNumber(pos) == 'h') {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8, 'P', 'N'),
                    Move(pos, pos >> 8, 'P', 'B'),
                    Move(pos, pos >> 8, 'P', 'R'),
                    Move(pos, pos >> 8, 'P', 'Q'),

                    Move(pos, pos >> 9, 'P', 'N'),
                    Move(pos, pos >> 9, 'P', 'B'),
                    Move(pos, pos >> 9, 'P', 'R'),
                    Move(pos, pos >> 9, 'P', 'Q'),
            };
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) < 'h' && colNumber(pos) > 'a') {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8, 'P'),
                    Move(pos, pos >> 7, 'P'),
                    Move(pos, pos >> 9, 'P')
            };
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) == 'h') {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8, 'P'),
                    Move(pos, pos >> 9, 'P')
            };
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) == 'a') {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8, 'P'),
                    Move(pos, pos >> 7, 'P')
            };
        } else {
            std::cout << "invalid white pawn position: " << bitToSquare(pos) << '\n';
        }
        return tmp;
    } else if (tolower(p) == 'n') {
        std::vector<Move> tmp;
        if (colNumber(pos) > 'a' && rowNumber(pos) > 2)
            tmp.push_back(Move(pos, pos << 15, p));

        if (colNumber(pos) > 'a' && rowNumber(pos) < 7)
            tmp.push_back(Move(pos, pos >> 17, p));

        if (colNumber(pos) > 'b' && rowNumber(pos) > 1)
            tmp.push_back(Move(pos, pos << 6, p));

        if (colNumber(pos) > 'b' && rowNumber(pos) < 8)
            tmp.push_back(Move(pos, pos >> 10, p));

        if (colNumber(pos) < 'h' && rowNumber(pos) > 2)
            tmp.push_back(Move(pos, pos << 17, p));

        if (colNumber(pos) < 'h' && rowNumber(pos) < 7)
            tmp.push_back(Move(pos, pos >> 15, p));

        if (colNumber(pos) < 'g' && rowNumber(pos) > 1)
            tmp.push_back(Move(pos, pos << 10, p));

        if (colNumber(pos) < 'g' && rowNumber(pos) < 8)
            tmp.push_back(Move(pos, pos >> 6, p));

        return tmp;
    } else if (tolower(p) == 'b') {
        std::vector<Move> tmp;
        char r = colNumber(pos);
        int c = rowNumber(pos);
        int i = 1;
        while (r > 'a' && c > 1) {
            --r;
            --c;
            tmp.push_back(Move(pos, pos << i * 7, p));
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (r > 'a' && c < 8) {
            --r;
            ++c;
            tmp.push_back(Move(pos, pos >> i * 9, p));
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (r < 'h' && c > 1) {
            ++r;
            --c;
            tmp.push_back(Move(pos, pos << i * 9, p));
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (r < 'h' && c < 8) {
            ++r;
            ++c;
            tmp.push_back(Move(pos, pos >> i * 7, p));
            ++i;
        }
        return tmp;
    } else if (tolower(p) == 'r') {
        std::vector<Move> tmp;
        char r = colNumber(pos);
        int c = rowNumber(pos);
        int i = 1;
        while (r > 'a') {
            --r;
            tmp.push_back(Move(pos, pos >> i, p));
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (r < 'h') {
            ++r;
            tmp.push_back(Move(pos, pos << i, p));
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (c > 1) {
            --c;
            tmp.push_back(Move(pos, pos << i * 8, p));
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (c < 8) {
            ++c;
            tmp.push_back(Move(pos, pos >> i * 8, p));
            ++i;
        }

        return tmp;
    } else if (tolower(p) == 'q') {
        auto r = possibleMoves('r', pos);
        const auto b = possibleMoves('b', pos);
        for (const auto& i : b) {
            r.push_back(i);
        }
        for (auto& i : r) {
            i.piece = p;
        }
        return r;
    } else if (tolower(p) == 'k') {
        std::vector<Move> tmp;

        if (colNumber(pos) > 'a')
            tmp.push_back(Move(pos, pos >> 1, p));
        
        if (colNumber(pos) < 'h')
            tmp.push_back(Move(pos, pos << 1, p));
        
        if (rowNumber(pos) > 1)
            tmp.push_back(Move(pos, pos << 8, p));
        
        if (rowNumber(pos) < 8)
            tmp.push_back(Move(pos, pos >> 8, p));
        
        if (rowNumber(pos) > 1 && colNumber(pos) > 'a')
            tmp.push_back(Move(pos, pos << 7, p));
        
        if (rowNumber(pos) > 1 && colNumber(pos) < 'h')
            tmp.push_back(Move(pos, pos << 9, p));
        
        if (rowNumber(pos) < 8 && colNumber(pos) > 'a')
            tmp.push_back(Move(pos, pos >> 9, p));
        
        if (rowNumber(pos) < 8 && colNumber(pos) < 'h')
            tmp.push_back(Move(pos, pos >> 7, p));


        if (toupper(p) == p) {
            if (castleRights & (1 << 3) && pos == E1) tmp.push_back(Move(E1, G1, 'K'));
            if (castleRights & (1 << 2) && pos == E1) tmp.push_back(Move(E1, C1, 'K'));
        } else if (tolower(p) == p) {
            if (castleRights & (1 << 1) && pos == E8) tmp.push_back(Move(E8, G8, 'k'));
            if (castleRights & (1 << 0) && pos == E8) tmp.push_back(Move(E8, C8, 'k'));
        }
        return tmp;
        
    }
    return std::vector<Move>();
} // possibleMoves

// pos is a square, e.g. "e4"
std::vector<Square> Position::checkingSquares(char p, Square pos) const {
    std::vector<Move> tmp = possibleMoves(p, pos);
    if (tolower(p) == 'n') {
        std::vector<Square> r;
        for (const auto& i : tmp) {
            r.push_back(i.dest);
        }
        return r;
    }

    std::vector<Square> cpy;
    for (const auto& i : tmp) {
        if (!isMoveBlocked(i) && 
            !(tolower(p) == 'p' && colNumber(i.source) == colNumber(i.dest))) {
            cpy.push_back(i.dest);
        }
    }
    return cpy;
}

std::vector<Move> Position::legalMoves() {
    std::vector<Move> tmp;
    for (int i = 0; i < 12; ++i) {
        if ((i <= 5 && turn == true) ||
            (i >= 6 && turn == false)) continue;
        for (const auto& square : bbToSquares(thePosition.at(i))) {
            for (const auto& j : validMoves(intMap.at(i), square)) {
                tmp.push_back(j);
            }
        }
    }
    return tmp;
}

void Position::move(Move m) {
    // set the source bit to 0
    thePosition.at(pieceMap.at(m.piece)) &= ~m.source;
    
    // set all destination bits to 0
    for (auto& board : thePosition) {
        board &= ~m.dest;
    }

    // set the destination bit of the moving piece to 1
    thePosition.at(pieceMap.at(m.piece)) |= m.dest;

    // if promotion
    if (m.promotionPiece != '-') {
        // set the destination square of the pawn to 0
        thePosition.at(pieceMap.at(m.piece)) &= ~m.dest;

        thePosition.at(pieceMap.at(m.promotionPiece)) |= m.dest;
    }

    // if castle
    if (m.piece == 'K' && m.source == E1 && m.dest == G1) {
        thePosition.at(pieceMap.at('R')) &= ~H1;
        thePosition.at(pieceMap.at('R')) |= F1;
    } else if (m.piece == 'K' && m.source == E1 && m.dest == C1) {
        thePosition.at(pieceMap.at('R')) &= ~A1;
        thePosition.at(pieceMap.at('R')) |= D1;
    } else if (m.piece == 'k' && m.source == E8 && m.dest == G8) {
        thePosition.at(pieceMap.at('r')) &= ~H8;
        thePosition.at(pieceMap.at('r')) |= F8;
    } else if (m.piece == 'k' && m.source == E8 && m.dest == C8) {
        thePosition.at(pieceMap.at('r')) &= ~A8;
        thePosition.at(pieceMap.at('r')) |= D8;
    }

    // if en passent
    if (m.piece == 'P' && m.dest == enPassant) {
        thePosition.at(pieceMap.at('p')) &= ~(m.dest << 8);
    } else if (m.piece == 'p' && m.dest == enPassant) {
        thePosition.at(pieceMap.at('P')) &= ~(m.dest >> 8);
    }

    turn = !turn;
    enPassant = m.enPassantSquare;
    for (int i = 0; i < 4; ++i) {
        if ((m.castleRights & (1 << i)) == 0) {
            castleRights &= ~(1 << i);
        }
    }

    int before = countBits(whitePieces) + countBits(blackPieces);
    updateBoards();
    int after = countBits(whitePieces) + countBits(blackPieces);
    // if capture or pawn move
    if (before != after || tolower(m.piece) == 'p') {
        halfMove = 0;
    } else {
        ++halfMove;
    }

    if (tolower(m.piece) == m.piece) ++fullMove;
}

void Position::unmove() {
    thePosition  = OthePosition;
    whitePieces  = OwhitePieces;
    blackPieces  = OblackPieces;

    turn         = Oturn;
    castleRights = OcastleRights;
    enPassant    = OenPassant;
    halfMove     = OhalfMove;
    fullMove     = OfullMove;    
}

bool Position::whiteInCheck() {
    for (int i = 6; i <= 10; ++i) {
        for (const auto& j : bbToSquares(thePosition.at(i))) {
            for (const auto& k : checkingSquares(intMap.at(i), j)) {
                if (thePosition.at(pieceMap.at('K')) & k) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Position::blackInCheck() {
    for (int i = 0; i <= 4; ++i) {
        for (const auto& j : bbToSquares(thePosition.at(i))) {
            for (const auto& k : checkingSquares(intMap.at(i), j)) {
                if (thePosition.at(pieceMap.at('k')) & k) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Position::whiteInCheckmate() {
    return (turn == 0 && whiteInCheck() && legalMoves().size() == 0);
}

bool Position::blackInCheckmate() {
    return (turn == 1 && blackInCheck() && legalMoves().size() == 0);
}

void Position::updateBoards() {
    whitePieces = thePosition.at(0)  | thePosition.at(1) |
                  thePosition.at(2)  | thePosition.at(3) |
                  thePosition.at(4)  | thePosition.at(5);

    blackPieces = thePosition.at(6)  | thePosition.at(7) |
                  thePosition.at(8)  | thePosition.at(9) |
                  thePosition.at(10) | thePosition.at(11);
}

void Position::resetOriginal() {
    OthePosition  = thePosition;
    OwhitePieces  = whitePieces;
    OblackPieces  = blackPieces;

    Oturn         = turn;
    OcastleRights = castleRights;
    OenPassant    = enPassant;
    OhalfMove     = halfMove;
    OfullMove     = fullMove;
}

}
