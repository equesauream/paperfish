#include "position.h"
#include "square.h"
#include "piece.h"
#include "table.h"
#include "magic.h"
#include "type.h"

#include <memory>

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

    ZHash z;
    key = z(*this);
    Okey = key;
}

Position::Position(const Position& other) :
    key {other.key},
    Okey {other.Okey},

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
    key {other.key},
    Okey {other.Okey},

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

    key = std::move(other.key);
    Okey = std::move(other.Okey);
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
    Piece p = NoPiece;
    Piece proPiece = NoPiece;

    for (int i = 0; i < 12; ++i) {
        if (thePosition.at(i) & r) {
            p = i;
            break;
        }
    }

    if (s.length() > 4 && turn == 0)
        proPiece = pieceMap.at(toupper(s.at(4)));
    else if (s.length() > 4 && turn == 1)
        proPiece = pieceMap.at(tolower(s.at(4)));

    return Move(r, d, p, getMoveType(r, d, p), proPiece);
}

MoveType Position::getMoveType(Square source, Square dest, Piece p) const {
    MoveType mt = Basic;
    for (int i = 0; i < 12; ++i) {
        if (i == p)
            continue;
        
        if (thePosition.at(i) & dest)
            mt = Capture;
    }

    int diff = rowNumber(source) - rowNumber(dest);
    if (isPawn(p) && (diff == 2 || diff == -2))
        mt = LongPawn;

    if (dest == enPassant)
        mt = enPassant;

    if ((p == whiteKing && source == E1 && dest == G1) ||
        (p == blackKing && source == E8 && dest == G8))
        mt = ShortCastle;
    else if ((p == whiteKing && source == E1 && dest == C1) ||
             (p == blackKing && source == E8 && dest == C8))
        mt = LongCastle;

    return mt;
}

bool Position::isValid(const Move& m) {
    return isValidMove(m, m.piece);
}

bool Position::isValidMove(const Move& m, Piece piece) {
    Square source = getSquare(m.source);
    Square dest   = getSquare(m.dest);

    // if the destination square has a piece with the same colour
    if (isBlack(piece) && (dest & blackPieces) != 0) return false;
    if (isWhite(piece) && (dest & whitePieces) != 0) return false;

    // if a pawn is trying to move forward into a piece of their own colour
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
        if ((isBlack(piece) && blackInCheck()) ||
            (isWhite(piece) && whiteInCheck())) {
            unmove();
            return false;
        }
        unmove();
        return true;
    }

    if (piece == whiteKing) {
        move(m);
        if (whiteInCheck()) {
            unmove();
            return false;
        }
        unmove();

        // if there is something on B1 to prevent O-O-O
        if (m.type == LongCastle) {
            if (((B1 & blackPieces) | 
                (B1 & whitePieces)) != 0) return false;
        }

        if (m.type == ShortCastle || m.type == LongCastle) {
            if (whiteInCheck()) return false;
            Square kpos, rpos, rook, king;
            king = E1;
            kpos = dest;
            // if castle rights are not enabled
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

            // if the king or rook are not on the correct squares
            if (!((thePosition.at(whiteKing)) & king) ||
                !((thePosition.at(whiteRook)) & rook)) return false;

            // if there is something blocking the eventual king and rook positions
            if ((kpos & blackPieces) != 0 || 
                (rpos & blackPieces) != 0) return false;
            if ((kpos & whitePieces) != 0 || 
                (rpos & whitePieces) != 0) return false;

            move(Move(E1, rpos, whiteKing, Basic));
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

        if (m.type == LongCastle) {
            if ((B8 & blackPieces) != 0 || 
                (B8 & whitePieces) != 0) return false;
        }

        if (m.type == ShortCastle || m.type == LongCastle) {
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

            move(Move(E8, rpos, blackKing, Basic));
            if (blackInCheck()) {
                unmove();
                return false;
            }
            unmove();
        }
        return true;
    }
    
    // check if pawn move is blocked
    if (piece == whitePawn && m.type == LongPawn) {
        if (((source >> 8) & whitePieces) || 
            ((source >> 8) & blackPieces))
            return false;
    } else if (piece == blackPawn && m.type == LongPawn) {
        if (((source << 8) & whitePieces) || 
            ((source << 8) & blackPieces))
            return false;
    }

    move(m);
    if ((isBlack(piece) && blackInCheck()) ||
        (isWhite(piece) && whiteInCheck())) {
        unmove();
        return false;
    }
    unmove();

    return true;
} // isValidMove

std::vector<Move> Position::possibleMoves(Piece p, Square pos) const {
    U64 magic = magic::getAttacks(p, pos, whitePieces | blackPieces);
    std::vector<Move> tmp;
    tmp.reserve(24);
    while (magic != 0) {
        Square s = magic & (~magic + 1);

        MoveType mt = getMoveType(pos, s, p);

        if (p == blackPawn && rowNumber(s) == 1) {
            tmp.push_back(Move(pos, s, p, mt, blackKnight));
            tmp.push_back(Move(pos, s, p, mt, blackBishop));
            tmp.push_back(Move(pos, s, p, mt, blackRook));
            tmp.push_back(Move(pos, s, p, mt, blackQueen));
        } else if (p == whitePawn && rowNumber(s) == 8) {
            tmp.push_back(Move(pos, s, p, mt, whiteKnight));
            tmp.push_back(Move(pos, s, p, mt, whiteBishop));
            tmp.push_back(Move(pos, s, p, mt, whiteRook));
            tmp.push_back(Move(pos, s, p, mt, whiteQueen));
        } else {
            tmp.push_back(Move(pos, s, p, mt));
        }
        magic ^= s;
    }
    //tmp.shrink_to_fit();
    return tmp;
} // possibleMoves

std::vector<Move> Position::legalMoves() {
    std::vector<Move> tmp;
    tmp.reserve(50);
    int start, end;
    if (turn == Black) {
        start = 6;
        end = 11;
    } else {
        start = 0;
        end = 5;
    }
    for (int i = start; i <= end; ++i) {
        U64 cpy = thePosition.at(i);
        while (cpy != 0) {
            Square s = cpy & (~cpy + 1);
            const auto moves = possibleMoves(i, s);
            for (const auto& j : moves) {
                if (isValid(j)) {
                    tmp.push_back(j);
                }
            }
            cpy ^= s;
        }
    }
    //tmp.shrink_to_fit();
    return tmp;
}

void Position::move(const Move& m) {

    key ^= Zobrist::blackToMove;

    Square s = getSquare(m.source);
    Square d = getSquare(m.dest);

    // set the source bit to 0
    thePosition.at(m.piece) ^= s;
    key ^= Zobrist::table[m.piece][m.source];
    
    // set destination bit to 0
    if (m.type == Capture) {
        for (int board = 0; board < 12; ++board) {
            if ((thePosition[board] & d) != 0) {
                thePosition[board] &= ~d;
                key ^= Zobrist::table[board][m.dest];
                break;
            }
        }
    }

    // set the destination bit of the moving piece to 1
    thePosition.at(m.piece) |= d;
    key ^= Zobrist::table[m.piece][m.dest];

    // if promotion
    if (m.promotionPiece != NoPiece) {
        // set the destination square of the pawn to 0
        thePosition.at(m.piece) ^= d;
        key ^= Zobrist::table[m.piece][m.dest];

        thePosition.at(m.promotionPiece) |= d;
        key ^= Zobrist::table[m.promotionPiece][m.dest];
    }

    // if castle
    if (m.piece == whiteKing && m.type == ShortCastle) {
        thePosition.at(whiteRook) ^= H1;
        thePosition.at(whiteRook) |= F1;

        key ^= Zobrist::table[whiteRook][getSquareIndex(H1)];
        key ^= Zobrist::table[whiteRook][getSquareIndex(F1)];
    } else if (m.piece == whiteKing && m.type == LongCastle) {
        thePosition.at(whiteRook) ^= A1;
        thePosition.at(whiteRook) |= D1;

        key ^= Zobrist::table[whiteRook][getSquareIndex(A1)];
        key ^= Zobrist::table[whiteRook][getSquareIndex(D1)];
    } else if (m.piece == blackKing && m.type == ShortCastle) {
        thePosition.at(blackRook) ^= H8;
        thePosition.at(blackRook) |= F8;

        key ^= Zobrist::table[blackRook][getSquareIndex(H8)];
        key ^= Zobrist::table[blackRook][getSquareIndex(F8)];
    } else if (m.piece == blackKing && m.type == LongCastle) {
        thePosition.at(blackRook) ^= A8;
        thePosition.at(blackRook) |= D8;

        key ^= Zobrist::table[blackRook][getSquareIndex(A8)];
        key ^= Zobrist::table[blackRook][getSquareIndex(D8)];
    }

    // if en passent
    if (m.piece == whitePawn && d == enPassant) {
        thePosition.at(blackPawn) ^= (d << 8);
        key ^= Zobrist::table[blackPawn][m.dest - 8];
    } else if (m.piece == blackPawn && d == enPassant) {
        thePosition.at(whitePawn) ^= (d >> 8);
        key ^= Zobrist::table[whitePawn][m.dest + 8];
    }

    turn = !turn;
    enPassant = getSquare(m.enPassantSquare);
    key ^= Zobrist::castling[castleRights];
    for (int i = 0; i < 4; ++i) {
        if ((m.castleRights & (uint8_t) (1 << i)) == 0) {
            castleRights ^= (uint8_t) (1 << i);
        }
    }
    key ^= Zobrist::castling[castleRights];

    updateBoards();
    // if capture or pawn move
    if (m.type == Capture || isPawn(m.piece)) {
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

    key          = Okey;
}

void Position::advance(const Move& m) {
    move(m);
    resetOriginal();
}

bool Position::whiteInCheck() {

    if (thePosition.at(whiteKing) == 0) {
        std::cout << '\n' << *this;
        std::cout << "white king not in position" << '\n';
    }
    if (magic::getChecks(whitePawn, thePosition.at(whiteKing), whitePieces | blackPieces) & thePosition.at(blackPawn))
        return true;

    for (int i = blackKnight; i <= blackKing; ++i) {
        if (magic::getChecks(i, thePosition.at(whiteKing), whitePieces | blackPieces) & thePosition.at(i))
            return true;
    }

    return false;
}

bool Position::blackInCheck() {

    if (thePosition.at(blackKing) == 0) {
        std::cout << '\n' << *this;
        std::cout << "black king not in position" << '\n';
    }

    if (magic::getChecks(blackPawn, thePosition.at(blackKing), whitePieces | blackPieces) & thePosition.at(whitePawn))
        return true;

    for (int i = whiteKnight; i <= whiteKing; ++i) {
        if (magic::getChecks(i, thePosition.at(blackKing), whitePieces | blackPieces) & thePosition.at(i))
            return true;
    }

    return false;
}

bool Position::whiteInCheckmate() {
    return (turn == White && whiteInCheck() && legalMoves().size() == 0);
}

bool Position::blackInCheckmate() {
    return (turn == Black && blackInCheck() && legalMoves().size() == 0);
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

    Okey          = key;
}

int Position::materialCount() const {
    int closedness = Position::closedness();
    int val[12] = {
    //  P     N     B                      R      Q      K
         200,  600,  600 + 10 * closedness,  1000,  1800, 0,
        -200, -600, -600 - 10 * closedness, -1000, -1800, 0
    };
    
    int value = 0;
    for (int i = 0; i < 11; ++i) {
        if (i == 5)
            continue;
        value += val[i] * __builtin_popcountll(thePosition.at(i));
    }
    return value;
}

int Position::pawnStructure(Colour c) const {
    int con = c == White ? 1 : -1;
    int pawn = c == White ? whitePawn : blackPawn;
    return con * 2 * (pawnSupport(thePosition.at(pawn), c) - isolatedPawns(thePosition.at(pawn)));
}


// counts the number of pawns protected by other pawns
int Position::pawnSupport(U64 board, Colour c) {
    U64 cpy = board;
    int pawnSupp = 0;
    while (cpy != 0) {
        Square s = cpy & (~cpy + 1);
        pawnSupp += __builtin_popcountll(board | magic::getChecks(c == whitePawn ? whitePawn : blackPawn, s, 0));
        cpy ^= s;
    }
    return pawnSupp;
}

// counts the number of isolated pawns
int Position::isolatedPawns(U64 board) {
    U64 cpy = board;
    int isoPawn = 0;
    while (cpy != 0) {
        Square s = cpy & (~cpy + 1);
        U64 nextFiles = 0;
        if (colNumber(s) != A) {
            Square cur = s >> 1;
            Square end = 0x000000ff;
            while ((cur & end) == 0) {
                nextFiles |= cur;
                cur >>= 8;
            }
            cur = s >> 1;
            end = 0xff'00'00'00'00'00'00'00;
            while ((cur & end) == 0) {
                nextFiles |= cur;
                cur <<= 8;
            }
        }
        if (colNumber(s) != H) {
            Square cur = s << 1;
            Square end = 0x000000ff;
            while ((cur & end) == 0) {
                nextFiles |= cur;
                cur >>= 8;
            }
            cur = s << 1;
            end = 0xff'00'00'00'00'00'00'00;
            while ((cur & end) == 0) {
                nextFiles |= cur;
                cur <<= 8;
            }
        }
        if ((nextFiles & board) == 0) {
            ++isoPawn;
        }

        cpy ^= s;
    }
    return isoPawn;
}

int Position::closedness() const {
    return __builtin_popcountll(thePosition.at(whitePawn) & (thePosition.at(blackPawn) << 8));
}

int pressureHeatMap[64] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 1,
    1, 2, 3, 3, 3, 3, 2, 1,
    1, 2, 3, 4, 4, 3, 2, 1,
    1, 2, 3, 4, 4, 3, 2, 1,
    1, 2, 3, 3, 3, 3, 2, 1,
    1, 2, 2, 2, 2, 2, 2, 1,
    1, 1, 1, 1, 1, 1, 1, 1
};

// counts the number of times each square is hit, and returns the weighted sum based on the heat map
int Position::pressure() const {
    
    int pressureBoard[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };
    for (int i = 0; i <= 5; ++i) {
        U64 cpy = thePosition.at(i);
        while (cpy != 0) {
            int s = __builtin_ctzll(cpy);
            pressureBoard[s] += pressureHeatMap[s];
            cpy &= cpy - 1;
        }
    }
    for (int i = 6; i <= 11; ++i) {
        U64 cpy = thePosition.at(i);
        while (cpy != 0) {
            int s = __builtin_ctzll(cpy);
            pressureBoard[s] -= pressureHeatMap[s];
            cpy &= cpy - 1;
        }
    }
    int total = 0;
    for (int i = 0; i < 64; ++i) 
        total += 2 * pressureBoard[i];

    
    return total / 2;
}

int Position::heurVal() {
    if (whiteInCheckmate())
        return -100000;
    if (blackInCheckmate())
        return 100000;

    return 
    materialCount() 
    + pawnStructure(White) - pawnStructure(Black) 
    + 5 * whiteInCheck() - 5 * blackInCheck()
    + pressure();
}

}
