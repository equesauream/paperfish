#include "position.h"
#include "square.h"
#include "piece.h"
#include "table.h"

#include <sstream>
#include <bitset>
#include <cctype>
#include <iostream>
#include <random>

namespace engine {

const std::map<char, int> Position::pieceMap = 
    std::map<char, int>{std::pair<char, int>('P', whitePawn), 
                        std::pair<char, int>('N', whiteKnight), 
                        std::pair<char, int>('B', whiteBishop), 
                        std::pair<char, int>('R', whiteRook), 
                        std::pair<char, int>('Q', whiteQueen), 
                        std::pair<char, int>('K', whiteKing), 
                        std::pair<char, int>('p', blackPawn), 
                        std::pair<char, int>('n', blackKnight), 
                        std::pair<char, int>('b', blackBishop), 
                        std::pair<char, int>('r', blackRook), 
                        std::pair<char, int>('q', blackQueen), 
                        std::pair<char, int>('k', blackKing)};

const std::map<int, char> Position::intMap = 
    std::map<int, char>{std::pair<int, char>(whitePawn, 'P'), 
                        std::pair<int, char>(whiteKnight, 'N'), 
                        std::pair<int, char>(whiteBishop, 'B'), 
                        std::pair<int, char>(whiteRook, 'R'), 
                        std::pair<int, char>(whiteQueen, 'Q'), 
                        std::pair<int, char>(whiteKing, 'K'), 
                        std::pair<int, char>(blackPawn, 'p'), 
                        std::pair<int, char>(blackKnight, 'n'), 
                        std::pair<int, char>(blackBishop, 'b'), 
                        std::pair<int, char>(blackRook, 'r'), 
                        std::pair<int, char>(blackQueen, 'q'), 
                        std::pair<int, char>(blackKing, 'k')};

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
        if (c == 'K') castleRights |= (uint8_t) 1 << 3;
        if (c == 'Q') castleRights |= (uint8_t) 1 << 2;
        if (c == 'k') castleRights |= (uint8_t) 1 << 1;
        if (c == 'q') castleRights |= (uint8_t) 1 << 0;
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
    if (castleRights == 0)
        tmp += "-";
    else {
        for (int i = 0; i < 4; ++i) {
            if (castleRights & (1 << i)) {
                if (i == 0) 
                    tmp += "K";
                if (i == 1) 
                    tmp += "Q";
                if (i == 2) 
                    tmp += "k";
                if (i == 3) 
                    tmp += "q";
            }
        }
    }

    // en passent
    tmp += " ";
    if (enPassant == NoSquare)
        tmp += "-";
    else
        tmp += bitToSquare(enPassant);


    // halfmoves
    tmp += " ";
    tmp += std::to_string(halfMove);

    
    // fullmoves
    tmp += " ";
    tmp += std::to_string(fullMove);
    return tmp;
}

bool Position::operator==(const Position &other) const {
    for(int i = 0; i < 12; ++i) {
        if (thePosition.at(i) != other.thePosition.at(i))
            return false;
    }
    if (enPassant != other.enPassant ||
        castleRights != other.castleRights)
        return false;
    return true;
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

Move Position::parseString(std::string s) {
    Square r = squareToBit(s.substr(0, 2));
    Square d = squareToBit(s.substr(2, 2));
    Piece p;
    Piece proPiece = NoPiece;

    for (int i = 0; i < 12; ++i) {
        if (thePosition.at(i) & r) {
            p = i;
        }
    }

    if (s.length() > 4 && turn == 0)
        proPiece = pieceMap.at(toupper(s.at(4)));
    else if (s.length() > 4 && turn == 1)
        proPiece = pieceMap.at(tolower(s.at(4)));
    return Move(r, d, p, proPiece);
}

bool Position::isValid(const Move& m) {
    return isValidMove(m, m.piece, m.piece);
}

bool Position::isValidMove(const Move& m, Piece piece, Piece orig) {
    Square source = getSquare(m.source);
    Square dest   = getSquare(m.dest);
    Piece prom = m.promotionPiece;

    for (const auto& s : possibleMoves(piece, source)) {
        if (s == m) goto NEXT;
    }
    return false;
  NEXT:
    if (isBlack(piece) && (dest & blackPieces) != 0) return false;
    if (isWhite(piece) && (dest & whitePieces) != 0) return false;

    if (piece == blackPawn) {
        if (colNumber(source) != colNumber(dest) && rowNumber(source) != rowNumber(dest)) {
            if (dest != enPassant && (dest & whitePieces) == 0) return false;
        } else {
            if ((dest & blackPieces) != 0 || (dest & whitePieces) != 0) return false;
        }
    } else if (piece == whitePawn) {
        if (colNumber(source) != colNumber(dest) && rowNumber(source) != rowNumber(dest)) {
            if (dest != enPassant && (dest & blackPieces) == 0) return false;
        } else {
            if ((dest & whitePieces) != 0 || (dest & blackPieces) != 0) return false;
        }
    }
    
    if (isKnight(piece)) {
        move(m);
        if (isBlack(piece) && blackInCheck()) {
            unmove();
            return false;
        }
        if (isWhite(piece) && whiteInCheck()) {
            unmove();
            return false;
        }
        unmove();
        return true;
    }
    // check if path is blocked for b, r, q
    if (piece == blackRook || piece == blackBishop) return isValidMove(m, blackQueen, orig);
    if (piece == whiteRook || piece == whiteBishop) return isValidMove(m, whiteQueen, orig);

    if (piece == whiteKing) {
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

            if (!((thePosition.at(whiteKing)) & king) ||
                !((thePosition.at(whiteRook)) & rook)) return false;

            if ((kpos & blackPieces) != 0 || 
                (rpos & blackPieces) != 0) return false;
            if ((kpos & whitePieces) != 0 || 
                (rpos & whitePieces) != 0) return false;

            move(Move(E1, rpos, whiteKing));
            if (whiteInCheck()) {
                unmove();
                return false;
            }
            unmove();

        }
        return true;
    } else if (piece == blackKing) {
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

            if (!((thePosition.at(blackKing)) & king) ||
                !((thePosition.at(blackRook)) & rook)) return false;

            if ((kpos & blackPieces) != 0 || 
                (rpos & blackPieces) != 0) return false;
            if ((kpos & whitePieces) != 0 || 
                (rpos & whitePieces) != 0) return false;

            move(Move(E8, rpos, blackKing));
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
    if (isBlack(piece) && blackInCheck()) {
        unmove();
        return false;
    }
    if (isWhite(piece) && whiteInCheck()) {
        unmove();
        return false;
    }
    unmove();

    return true;
} // isValidMove

// source and dest are 2-length strings
bool Position::isMoveBlocked(const Move& m) const {
    Square source = getSquare(m.source);
    Square dest   = getSquare(m.dest);
    Square cur    = source;
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
std::vector<Move> Position::validMoves(Piece p, Square pos) {
    std::vector<Move> tmp;
    auto moves = possibleMoves(p, pos);
    tmp.reserve(moves.size());
    for (const auto& i : moves) {
        if (isValid(i)) {
            tmp.push_back(i);
        }
    }
    tmp.shrink_to_fit();
    return tmp;
}

// pos is a square, e.g. "e4"
std::vector<Move> Position::possibleMoves(Piece p, Square pos) const {
    if (p == blackPawn) {
        std::vector<Move> tmp;
        if (rowNumber(pos) == 7 && colNumber(pos) < H && colNumber(pos) > A) {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8,  blackPawn),
                    Move(pos, pos << 16, blackPawn),
                    Move(pos, pos << 7,  blackPawn),
                    Move(pos, pos << 9,  blackPawn)
            };
        } else if (rowNumber(pos) == 7 && colNumber(pos) == A) {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8,  blackPawn),
                    Move(pos, pos << 16, blackPawn),
                    Move(pos, pos << 9,  blackPawn)
            };
        } else if (rowNumber(pos) == 7 && colNumber(pos) == H) {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8,  blackPawn),
                    Move(pos, pos << 16, blackPawn),
                    Move(pos, pos << 7,  blackPawn)
            };
        } else if (rowNumber(pos) == 2 && colNumber(pos) < H && colNumber(pos) > A) {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8, blackPawn, blackKnight),
                    Move(pos, pos << 8, blackPawn, blackBishop),
                    Move(pos, pos << 8, blackPawn, blackRook),
                    Move(pos, pos << 8, blackPawn, blackQueen),

                    Move(pos, pos << 7, blackPawn, blackKnight),
                    Move(pos, pos << 7, blackPawn, blackBishop),
                    Move(pos, pos << 7, blackPawn, blackRook),
                    Move(pos, pos << 7, blackPawn, blackQueen),

                    Move(pos, pos << 9, blackPawn, blackKnight),
                    Move(pos, pos << 9, blackPawn, blackBishop),
                    Move(pos, pos << 9, blackPawn, blackRook),
                    Move(pos, pos << 9, blackPawn, blackQueen),
            };
        } else if (rowNumber(pos) == 2 && colNumber(pos) == A) {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8, blackPawn, blackKnight),
                    Move(pos, pos << 8, blackPawn, blackBishop),
                    Move(pos, pos << 8, blackPawn, blackRook),
                    Move(pos, pos << 8, blackPawn, blackQueen),

                    Move(pos, pos << 9, blackPawn, blackKnight),
                    Move(pos, pos << 9, blackPawn, blackBishop),
                    Move(pos, pos << 9, blackPawn, blackRook),
                    Move(pos, pos << 9, blackPawn, blackQueen),
            };
        } else if (rowNumber(pos) == 2 && colNumber(pos) == H) {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8, blackPawn, blackKnight),
                    Move(pos, pos << 8, blackPawn, blackBishop),
                    Move(pos, pos << 8, blackPawn, blackRook),
                    Move(pos, pos << 8, blackPawn, blackQueen),

                    Move(pos, pos << 7, blackPawn, blackKnight),
                    Move(pos, pos << 7, blackPawn, blackBishop),
                    Move(pos, pos << 7, blackPawn, blackRook),
                    Move(pos, pos << 7, blackPawn, blackQueen),
            };
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) < H && colNumber(pos) > A) {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8, blackPawn),
                    Move(pos, pos << 7, blackPawn),
                    Move(pos, pos << 9, blackPawn)
            };
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) == H) {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8, blackPawn),
                    Move(pos, pos << 7, blackPawn)
            };
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) == A) {
            tmp = std::vector<Move>{
                    Move(pos, pos << 8, blackPawn),
                    Move(pos, pos << 9, blackPawn)
            };
        } else {
            std::cout << "invalid black pawn position: " << bitToSquare(pos) << " " << colNumber(pos) << '\n';
        }
        return tmp;
    } else if (p == whitePawn) {
        std::vector<Move> tmp;
        if (rowNumber(pos) == 2 && colNumber(pos) < H && colNumber(pos) > A) {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8,  whitePawn),
                    Move(pos, pos >> 16, whitePawn),
                    Move(pos, pos >> 7,  whitePawn),
                    Move(pos, pos >> 9,  whitePawn)
            };
        } else if (rowNumber(pos) == 2 && colNumber(pos) == A) {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8,  whitePawn),
                    Move(pos, pos >> 16, whitePawn),
                    Move(pos, pos >> 7,  whitePawn)
            };
        } else if (rowNumber(pos) == 2 && colNumber(pos) == H) {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8,  whitePawn),
                    Move(pos, pos >> 16, whitePawn),
                    Move(pos, pos >> 9,  whitePawn)
            };
        } else if (rowNumber(pos) == 7 && colNumber(pos) < H && colNumber(pos) > A) {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8, whitePawn, whiteKnight),
                    Move(pos, pos >> 8, whitePawn, whiteBishop),
                    Move(pos, pos >> 8, whitePawn, whiteRook),
                    Move(pos, pos >> 8, whitePawn, whiteQueen),

                    Move(pos, pos >> 7, whitePawn, whiteKnight),
                    Move(pos, pos >> 7, whitePawn, whiteBishop),
                    Move(pos, pos >> 7, whitePawn, whiteRook),
                    Move(pos, pos >> 7, whitePawn, whiteQueen),

                    Move(pos, pos >> 9, whitePawn, whiteKnight),
                    Move(pos, pos >> 9, whitePawn, whiteBishop),
                    Move(pos, pos >> 9, whitePawn, whiteRook),
                    Move(pos, pos >> 9, whitePawn, whiteQueen),
            };
        } else if (rowNumber(pos) == 7 && colNumber(pos) == A) {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8, whitePawn, whiteKnight),
                    Move(pos, pos >> 8, whitePawn, whiteBishop),
                    Move(pos, pos >> 8, whitePawn, whiteRook),
                    Move(pos, pos >> 8, whitePawn, whiteQueen),

                    Move(pos, pos >> 7, whitePawn, whiteKnight),
                    Move(pos, pos >> 7, whitePawn, whiteBishop),
                    Move(pos, pos >> 7, whitePawn, whiteRook),
                    Move(pos, pos >> 7, whitePawn, whiteQueen),
            };
        } else if (rowNumber(pos) == 7 && colNumber(pos) == H) {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8, whitePawn, whiteKnight),
                    Move(pos, pos >> 8, whitePawn, whiteBishop),
                    Move(pos, pos >> 8, whitePawn, whiteRook),
                    Move(pos, pos >> 8, whitePawn, whiteQueen),

                    Move(pos, pos >> 9, whitePawn, whiteKnight),
                    Move(pos, pos >> 9, whitePawn, whiteBishop),
                    Move(pos, pos >> 9, whitePawn, whiteRook),
                    Move(pos, pos >> 9, whitePawn, whiteQueen),
            };
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) < H && colNumber(pos) > A) {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8, whitePawn),
                    Move(pos, pos >> 7, whitePawn),
                    Move(pos, pos >> 9, whitePawn)
            };
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) == H) {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8, whitePawn),
                    Move(pos, pos >> 9, whitePawn)
            };
        } else if (rowNumber(pos) <= 6 && rowNumber(pos) >= 3 && 
                    colNumber(pos) == A) {
            tmp = std::vector<Move>{
                    Move(pos, pos >> 8, whitePawn),
                    Move(pos, pos >> 7, whitePawn)
            };
        } else {
            std::cout << "invalid white pawn position: " << bitToSquare(pos) << " " << colNumber(pos) << '\n';
        }
        return tmp;
    } else if (isKnight(p)) {
        std::vector<Move> tmp;
        tmp.reserve(8);
        if (colNumber(pos) > A && rowNumber(pos) > 2)
            tmp.push_back(Move(pos, pos << 15, p));

        if (colNumber(pos) > A && rowNumber(pos) < 7)
            tmp.push_back(Move(pos, pos >> 17, p));

        if (colNumber(pos) > B && rowNumber(pos) > 1)
            tmp.push_back(Move(pos, pos << 6, p));

        if (colNumber(pos) > B && rowNumber(pos) < 8)
            tmp.push_back(Move(pos, pos >> 10, p));

        if (colNumber(pos) < H && rowNumber(pos) > 2)
            tmp.push_back(Move(pos, pos << 17, p));

        if (colNumber(pos) < H && rowNumber(pos) < 7)
            tmp.push_back(Move(pos, pos >> 15, p));

        if (colNumber(pos) < G && rowNumber(pos) > 1)
            tmp.push_back(Move(pos, pos << 10, p));

        if (colNumber(pos) < G && rowNumber(pos) < 8)
            tmp.push_back(Move(pos, pos >> 6, p));

        tmp.shrink_to_fit();
        return tmp;
    } else if (isBishop(p)) {
        std::vector<Move> tmp;
        tmp.reserve(13);
        char r = colNumber(pos);
        int c = rowNumber(pos);
        int i = 1;
        while (r > A && c > 1) {
            --r;
            --c;
            tmp.push_back(Move(pos, pos << i * 7, p));
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (r > A && c < 8) {
            --r;
            ++c;
            tmp.push_back(Move(pos, pos >> i * 9, p));
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (r < H && c > 1) {
            ++r;
            --c;
            tmp.push_back(Move(pos, pos << i * 9, p));
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (r < H && c < 8) {
            ++r;
            ++c;
            tmp.push_back(Move(pos, pos >> i * 7, p));
            ++i;
        }
        tmp.shrink_to_fit();
        return tmp;
    } else if (isRook(p)) {
        std::vector<Move> tmp;
        tmp.reserve(14);
        char r = colNumber(pos);
        int c = rowNumber(pos);
        int i = 1;
        while (r > A) {
            --r;
            tmp.push_back(Move(pos, pos >> i, p));
            ++i;
        }

        r = colNumber(pos);
        c = rowNumber(pos);
        i = 1;
        while (r < H) {
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

        tmp.shrink_to_fit();
        return tmp;
    } else if (isQueen(p)) {
        auto r = possibleMoves(blackRook, pos);
        r.reserve(r.size() + 13);
        const auto b = possibleMoves(blackBishop, pos);
        for (const auto& i : b) {
            r.push_back(i);
        }
        for (auto& i : r) {
            i.piece = p;
        }
        r.shrink_to_fit();
        return r;
    } else if (isKing(p)) {
        std::vector<Move> tmp;
        tmp.reserve(8);

        if (colNumber(pos) > A)
            tmp.push_back(Move(pos, pos >> 1, p));
        
        if (colNumber(pos) < H)
            tmp.push_back(Move(pos, pos << 1, p));
        
        if (rowNumber(pos) > 1)
            tmp.push_back(Move(pos, pos << 8, p));
        
        if (rowNumber(pos) < 8)
            tmp.push_back(Move(pos, pos >> 8, p));
        
        if (rowNumber(pos) > 1 && colNumber(pos) > A)
            tmp.push_back(Move(pos, pos << 7, p));
        
        if (rowNumber(pos) > 1 && colNumber(pos) < H)
            tmp.push_back(Move(pos, pos << 9, p));
        
        if (rowNumber(pos) < 8 && colNumber(pos) > A)
            tmp.push_back(Move(pos, pos >> 9, p));
        
        if (rowNumber(pos) < 8 && colNumber(pos) < H)
            tmp.push_back(Move(pos, pos >> 7, p));

        if (isWhite(p)) {
            if (castleRights & (1 << 3) && pos == E1) tmp.push_back(Move(E1, G1, whiteKing));
            if (castleRights & (1 << 2) && pos == E1) tmp.push_back(Move(E1, C1, whiteKing));
        } else if (isBlack(p)) {
            if (castleRights & (1 << 1) && pos == E8) tmp.push_back(Move(E8, G8, blackKing));
            if (castleRights & (1 << 0) && pos == E8) tmp.push_back(Move(E8, C8, blackKing));
        }
        tmp.shrink_to_fit();
        return tmp;
        
    }
    return std::vector<Move>();
} // possibleMoves

// pos is a square, e.g. "e4"
std::vector<Square> Position::checkingSquares(Piece p, Square pos) const {
    std::vector<Move> tmp = possibleMoves(p, pos);
    if (isKnight(p)) {
        std::vector<Square> r;
        r.reserve(8);
        for (const auto& i : tmp) {
            r.push_back(getSquare(i.dest));
        }
        return r;
    }

    std::vector<Square> cpy;
    cpy.reserve(tmp.size());
    for (const auto& i : tmp) {
        if (!isMoveBlocked(i) && 
            !(isPawn(p) && colNumber(getSquare(i.source)) == colNumber(getSquare(i.dest)))) {
            cpy.push_back(getSquare(i.dest));
        }
    }
    cpy.shrink_to_fit();
    return cpy;
}

std::vector<Move> Position::legalMoves() {
    std::vector<Move> tmp;
    tmp.reserve(100);
    for (int i = 0; i < 12; ++i) {
        if ((i <= 5 && turn == true) ||
            (i >= 6 && turn == false)) continue;
        for (const auto& square : bbToSquares(thePosition.at(i))) {
            for (const auto& j : validMoves((i), square)) {
                tmp.push_back(j);
            }
        }
    }
    tmp.shrink_to_fit();
    return tmp;
}

void Position::move(Move m) {
    Square s = getSquare(m.source);
    Square d = getSquare(m.dest);
    // set the source bit to 0
    thePosition.at(m.piece) &= ~s;
    
    // set all destination bits to 0
    for (auto& board : thePosition) {
        board &= ~d;
    }

    // set the destination bit of the moving piece to 1
    thePosition.at(m.piece) |= d;

    // if promotion
    if (m.promotionPiece != NoPiece) {
        // set the destination square of the pawn to 0
        thePosition.at(m.piece) &= ~d;

        thePosition.at(m.promotionPiece) |= d;
    }

    // if castle
    if (m.piece == whiteKing && s == E1 && d == G1) {
        thePosition.at(whiteRook) &= ~H1;
        thePosition.at(whiteRook) |= F1;
    } else if (m.piece == whiteKing && s == E1 && d == C1) {
        thePosition.at(whiteRook) &= ~A1;
        thePosition.at(whiteRook) |= D1;
    } else if (m.piece == blackKing && s == E8 && d == G8) {
        thePosition.at(blackRook) &= ~H8;
        thePosition.at(blackRook) |= F8;
    } else if (m.piece == blackKing && s == E8 && d == C8) {
        thePosition.at(blackRook) &= ~A8;
        thePosition.at(blackRook) |= D8;
    }

    // if en passent
    if (m.piece == whitePawn && d == enPassant) {
        thePosition.at(blackPawn) &= ~(d << 8);
    } else if (m.piece == blackPawn && m.dest == enPassant) {
        thePosition.at(whitePawn) &= ~(d >> 8);
    }

    turn = !turn;
    enPassant = getSquare(m.enPassantSquare);
    for (int i = 0; i < 4; ++i) {
        if ((m.castleRights & (uint8_t) (1 << i)) == 0) {
            castleRights &= (uint8_t) ~(1 << i);
        }
    }

    int before = countBits(whitePieces) + countBits(blackPieces);
    updateBoards();
    int after = countBits(whitePieces) + countBits(blackPieces);
    // if capture or pawn move
    if (before != after || isPawn(m.piece)) {
        halfMove = 0;
    } else {
        ++halfMove;
    }

    if (isBlack(m.piece)) 
        ++fullMove;
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

void Position::advance(Move m) {
    move(m);
    resetOriginal();
}

bool Position::whiteInCheck() {
    for (int i = 6; i <= 10; ++i) {
        for (const auto& j : bbToSquares(thePosition.at(i))) {
            for (const auto& k : checkingSquares((i), j)) {
                if (thePosition.at(whiteKing) & k) {
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
            for (const auto& k : checkingSquares((i), j)) {
                if (thePosition.at(blackKing) & k) {
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

int Position::materialCount() const {
    int white = 0;
    int black = 0;

    int val[12] = {
        100 , 300 , 300 , 500 , 900 , 0,
        -100, -300, -300, -500, -900, 0
    };
    
    int value = 0;
    for (int i = 0; i < 12; ++i) {
        value += val[i] * __builtin_popcount(thePosition.at(i));
    }
    return value;
}

int Position::heurVal() const {
    return materialCount();
}

}
