#include "position.h"

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


    for (auto& p : v) {
        U64 board = 0;
        int r = 0;
        int c = 0;
        for (auto& pos : pieces) {
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

    castleRights = std::map<char, bool>{
        std::pair<char, bool>('K', false),
        std::pair<char, bool>('Q', false),
        std::pair<char, bool>('k', false),
        std::pair<char, bool>('q', false)};

    for (auto& c : castle) {
        if (c == '-') break;
        castleRights.at(c) = true;
    }


    // En Passent //
    enPassent = enP;


    halfMove = std::stoi(hm);
    fullMove = std::stoi(fm);


    // set up the whitepieces and blackpieces bitboard //
    updateBoards();

    OthePosition = thePosition;
    OwhitePieces = whitePieces;
    OblackPieces = blackPieces;

    Oturn = turn;
    OcastleRights = castleRights;
    OenPassent = enPassent;
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
    enPassent {other.enPassent},
    halfMove {other.halfMove},
    fullMove {other.fullMove},

    Oturn {other.Oturn},
    OcastleRights {other.OcastleRights},
    OenPassent {other.OenPassent},
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
    enPassent {other.enPassent},
    halfMove {other.halfMove},
    fullMove {other.fullMove},

    Oturn {other.Oturn},
    OcastleRights {other.OcastleRights},
    OenPassent {other.OenPassent},
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
    enPassent = std::move(other.enPassent);
    halfMove = std::move(other.halfMove);
    fullMove = std::move(other.fullMove);

    Oturn = std::move(other.Oturn);
    OcastleRights = std::move(other.OcastleRights);
    OenPassent = std::move(other.OenPassent);
    OhalfMove = std::move(other.OhalfMove);
    OfullMove = std::move(other.OfullMove);
    return *this;
}

Position::~Position() {}

std::string Position::toFEN() const {
    std::string tmp = std::string(64, '-');
    int count = 0;
    char c;
    for (auto& board : thePosition) {
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

    for (auto& i : dashes) {
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
    if (castleRights.at('K') == false &&
        castleRights.at('Q') == false &&
        castleRights.at('k') == false &&
        castleRights.at('q') == false) tmp += "-";
    else {
        for (auto& i : castleRights) {
            if (i.second == false) continue;
            tmp += i.first;
        }
    }

    // en passent
    tmp += " ";
    tmp += enPassent;


    // halfmoves
    tmp += " ";
    tmp += std::to_string(halfMove);

    
    // fullmoves
    tmp += " ";
    tmp += std::to_string(fullMove);
    return tmp;
}

// a8 = bit 0
// h8 = bit 7
// a1 = bit 56
// h8 = bit 63
// converts a bit to a square
std::string Position::bitToSquare(U64 bit) {
    int pos = 0;
    while (bit != 0) {
        while (bit < (1ULL << 63)) {
            ++pos;
            bit = bit << 1;
        }
        ++pos;
        bit = bit << 1;
    }
    --pos;
    return std::string(1, char('h' - ((pos % 8)))) + std::to_string((pos / 8) + 1);
}

// converts a square (e.g. "e4") to a bit
U64 Position::squareToBit(const std::string& s) {
    if (s.length() != 2) {
        std::cout << "incorrect string length for squareToBit(): given length " << s.length() << '\n';
    }
    U64 bit = 0;
    int c = s.at(0) - 'a';
    int r = 8 - (s.at(1) - '0');
    bit = (1ULL << (8*r + c));
    return bit;
}

std::ostream& operator<<(std::ostream& out, const Position& pos) {
    std::vector<std::vector<char>> tmp = std::vector<std::vector<char>>(8, std::vector<char>(8, '-'));
    int count = 0;
    char c;
    for (auto& board : pos.thePosition) {
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
    for (auto& i : tmp) {
        for (auto& j : i) {
            out << j;
        }
        out << '\n';
    }
    return out;
}

// s is in format "e2e4"
bool Position::isValidMove(const std::string& s, char piece, char orig) {
    if (s.length() != 4 && s.length() != 6) {
        std::cout << "incorrect string length for isValidMove(): given length " << s.length() << '\n';
    }
    std::string source = s.substr(0, 2);
    std::string dest = s.substr(2, 2);

    auto moves = possibleMoves(piece, source);
    for (auto& m : moves) {
        if (m == s) goto NEXT;
    }
    return false;
  NEXT:
    
    if (tolower(piece) == piece && (squareToBit(dest) & blackPieces) != 0) return false;
    if (toupper(piece) == piece && (squareToBit(dest) & whitePieces) != 0) return false;

    if (piece == 'p') {
        if (source.at(0) != dest.at(0) && source.at(1) != dest.at(1)) {
            if (dest != enPassent && (squareToBit(dest) & whitePieces) == 0) return false;
        } else {
            if ((squareToBit(dest) & blackPieces) != 0 || (squareToBit(dest) & whitePieces) != 0) return false;
        }
    } else if (piece == 'P') {
        if (source.at(0) != dest.at(0) && source.at(1) != dest.at(1)) {
            if (dest != enPassent && (squareToBit(dest) & blackPieces) == 0) return false;
        } else {
            if ((squareToBit(dest) & whitePieces) != 0 || (squareToBit(dest) & blackPieces) != 0) return false;
        }
    }
    
    if (tolower(piece) == 'n') {
        move(Move(s, orig));
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
    if (piece == 'r' || piece == 'b') return isValidMove(s, 'q', piece);
    if (piece == 'R' || piece == 'B') return isValidMove(s, 'Q', piece);

    if (piece == 'K') {
        std::string enemyKing = bitToSquare(thePosition.at(pieceMap.at('k')));

        if (enemyKing.at(0) - dest.at(0) <= 1 && enemyKing.at(0) - dest.at(0) >= -1 &&
            enemyKing.at(1) - dest.at(1) <= 1 && enemyKing.at(1) - dest.at(1) >= -1) {
            return false;
        }

        move(Move(s, orig));
        if (whiteInCheck()) {
            unmove();
            return false;
        }
        unmove();

        if (dest == "c1" && source == "e1") {
            if ((squareToBit("b1") & blackPieces) != 0 || 
                (squareToBit("b1") & whitePieces) != 0) return false;
        }

        if ((dest == "g1" && source == "e1") || (dest == "c1" && source == "e1")) {
            if (whiteInCheck()) return false;
            std::string kpos, rpos, rook, king;
            king = "e1";
            kpos = dest;
            if (kpos == "g1") {
                if (!castleRights.at('K')) return false;
                rook = "h1";
                rpos = "f1";
            } else if (kpos == "c1") {
                if (!castleRights.at('Q')) return false;
                rook = "a1";
                rpos = "d1";
            } else {
                return false;
            }

            if (!((thePosition.at(pieceMap.at('K'))) & squareToBit(king)) ||
                !((thePosition.at(pieceMap.at('R'))) & squareToBit(rook))) return false;

            if ((squareToBit(kpos) & blackPieces) != 0 || 
                (squareToBit(rpos) & blackPieces) != 0) return false;
            if ((squareToBit(kpos) & whitePieces) != 0 || 
                (squareToBit(rpos) & whitePieces) != 0) return false;

            move(Move("e1" + rpos, 'K'));
            if (whiteInCheck()) {
                unmove();
                return false;
            }
            unmove();

        }
        return true;
    } else if (piece == 'k') {
        std::string enemyKing = bitToSquare(thePosition.at(pieceMap.at('K')));

        if (enemyKing.at(0) - dest.at(0) <= 1 && enemyKing.at(0) - dest.at(0) >= -1 &&
            enemyKing.at(1) - dest.at(1) <= 1 && enemyKing.at(1) - dest.at(1) >= -1) {
            return false;
        }

        move(Move(s, orig));
        if (blackInCheck()) {
            unmove();
            return false;
        }
        unmove();

        if (dest == "c8" && source == "e8") {
            if ((squareToBit("b8") & blackPieces) != 0 || 
                (squareToBit("b8") & whitePieces) != 0) return false;
        }

        if ((dest == "g8" && source == "e8") || (dest == "c8" && source == "e8")) {
            if (blackInCheck()) return false;
            std::string kpos, rpos, rook, king;
            king = "e8";
            kpos = dest;
            if (kpos == "g8") {
                if (!castleRights.at('k')) return false;
                rook = "h8";
                rpos = "f8";
            } else if (kpos == "c8") {
                if (!castleRights.at('q')) return false;
                rook = "a8";
                rpos = "d8";
            } else {
                return false;
            }

            if (!((thePosition.at(pieceMap.at('k'))) & squareToBit(king)) ||
                !((thePosition.at(pieceMap.at('r'))) & squareToBit(rook))) return false;

            if ((squareToBit(kpos) & blackPieces) != 0 || 
                (squareToBit(rpos) & blackPieces) != 0) return false;
            if ((squareToBit(kpos) & whitePieces) != 0 || 
                (squareToBit(rpos) & whitePieces) != 0) return false;

            move(Move("e8" + rpos, 'k'));
            if (blackInCheck()) {
                unmove();
                return false;
            }
            unmove();
        }
        return true;
    }

    if (isMoveBlocked(source, dest)) return false;

    move(Move(s, orig));
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
bool Position::isMoveBlocked(const std::string& source, const std::string& dest) const {
    if (source.length() != 2 || dest.length() != 2) {
        std::cout << "incorrect string length for isMoveBlocked(): given source length " << 
            source.length() << " and dest length " << dest.length() << '\n';
    }
    std::string cur = source;
    if (source.at(0) == dest.at(0) && source.at(1) <= dest.at(1)) {
        int i = 1;
        cur = source.substr(0, 1) + std::string(1, source.at(1) + i);
        ++i;
        while (cur != dest) {
            if ((squareToBit(cur) & blackPieces) != 0 || 
                (squareToBit(cur) & whitePieces) != 0) return true;
            cur = source.substr(0, 1) + std::string(1, source.at(1) + i);
            ++i;
        }
    } else if (source.at(0) == dest.at(0) && source.at(1) >= dest.at(1)) {
        int i = 1;
        cur = source.substr(0, 1) + std::string(1, source.at(1) - i);
        ++i;
        while (cur != dest) {
            if ((squareToBit(cur) & blackPieces) != 0 || 
                (squareToBit(cur) & whitePieces) != 0) return true;
            cur = source.substr(0, 1) + std::string(1, source.at(1) - i);
            ++i;
        }
    } else if (source.at(0) >= dest.at(0) && source.at(1) == dest.at(1)) {
        int i = 1;
        cur = std::string(1, source.at(0) - i) + source.substr(1, 1);
        ++i;
        while (cur != dest) {
            if ((squareToBit(cur) & blackPieces) != 0 || 
                (squareToBit(cur) & whitePieces) != 0) return true;
            cur = std::string(1, source.at(0) - i) + source.substr(1, 1);
            ++i;
        }
    } else if (source.at(0) <= dest.at(0) && source.at(1) == dest.at(1)) {
        int i = 1;
        cur = std::string(1, source.at(0) + i) + source.substr(1, 1);
        ++i;
        while (cur != dest) {
            if ((squareToBit(cur) & blackPieces) != 0 || 
                (squareToBit(cur) & whitePieces) != 0) return true;
            cur = std::string(1, source.at(0) + i) + source.substr(1, 1);
            ++i;
        }
    } else if (source.at(0) <= dest.at(0) && source.at(1) <= dest.at(1)) {
        int i = 1;
        cur = std::string(1, source.at(0) + i) + std::string(1, source.at(1) + i);
        ++i;
        while (cur != dest) {
            if ((squareToBit(cur) & blackPieces) != 0 || 
                (squareToBit(cur) & whitePieces) != 0) return true;
            cur = std::string(1, source.at(0) + i) + std::string(1, source.at(1) + i);
            ++i;
        }
    } else if (source.at(0) >= dest.at(0) && source.at(1) >= dest.at(1)) {
        int i = 1;
        cur = std::string(1, source.at(0) - i) + std::string(1, source.at(1) - i);
        ++i;
        while (cur != dest) {
            if ((squareToBit(cur) & blackPieces) != 0 || 
                (squareToBit(cur) & whitePieces) != 0) return true;
            cur = std::string(1, source.at(0) - i) + std::string(1, source.at(1) - i);
            ++i;
        }
    } else if (source.at(0) <= dest.at(0) && source.at(1) >= dest.at(1)) {
        int i = 1;
        cur = std::string(1, source.at(0) + i) + std::string(1, source.at(1) - i);
        ++i;
        while (cur != dest) {
            if ((squareToBit(cur) & blackPieces) != 0 || 
                (squareToBit(cur) & whitePieces) != 0) return true;
            cur = std::string(1, source.at(0) + i) + std::string(1, source.at(1) - i);
            ++i;
        }
    } else if (source.at(0) >= dest.at(0) && source.at(1) <= dest.at(1)) {
        int i = 1;
        cur = std::string(1, source.at(0) - i) + std::string(1, source.at(1) + i);
        ++i;
        while (cur != dest) {
            if ((squareToBit(cur) & blackPieces) != 0 || 
                (squareToBit(cur) & whitePieces) != 0) return true;
            cur = std::string(1, source.at(0) - i) + std::string(1, source.at(1) + i);
            ++i;
        }
    }
    return false;
}

// pos is a square, e.g. "e4"
std::vector<std::string> Position::validMoves(char p, const std::string& pos) {
    if (pos.length() != 2) {
        std::cout << "incorrect string length for validMoves(): given length " << 
            pos.length() << '\n';
    }
    auto moves = possibleMoves(p, pos);
    std::vector<std::string> tmp;
    for (auto& i : moves) {
        if (isValidMove(i, p, p)) {
            tmp.push_back(i);
        }
    }
    return tmp;
}

// pos is a square, e.g. "e4"
std::vector<std::string> Position::possibleMoves(char p, const std::string& pos) const {
    if (pos.length() != 2) {
        std::cout << "incorrect string length for possibleMoves(): given length " << 
            pos.length() << '\n';
    }
    if (p == 'p') {
        std::vector<std::string> tmp;
        if (pos.at(1) == '7') {
            tmp = std::vector<std::string>{
                    pos + pos.substr(0, 1) + "6", pos + pos.substr(0, 1) + "5",
                    pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) - 1),
                    pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) - 1)};
        } else if (pos.at(1) == '2') {
            tmp = std::vector<std::string>{pos + pos.substr(0, 1) + "1=n",
                                           pos + pos.substr(0, 1) + "1=b",
                                           pos + pos.substr(0, 1) + "1=r",
                                           pos + pos.substr(0, 1) + "1=q",
                    pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) - 1) + "=n",
                    pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) - 1) + "=b",
                    pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) - 1) + "=r",
                    pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) - 1) + "=q",
                    pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) - 1) + "=n",
                    pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) - 1) + "=b",
                    pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) - 1) + "=r",
                    pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) - 1) + "=q"};
        } else if (pos.at(1) <= '6' && pos.at(1) >= '3') {
            tmp = std::vector<std::string>{
                    pos + pos.substr(0, 1) + std::string(1, pos.at(1) - 1),
                    pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) - 1),
                    pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) - 1)};
        } else {
            std::cerr << "invalid black pawn position" << '\n';
        }

        std::vector<std::string> cpy;
        for (auto& i : tmp) {
            if ((i.at(2) >= 'a' && i.at(2) <= 'h') && (i.at(3) >= '1' && i.at(3) <= '8')) {
                cpy.push_back(i);
            }
        }
        return cpy;
    } else if (p == 'P') {
        std::vector<std::string> tmp;
        if (pos.at(1) == '2') {
            tmp = std::vector<std::string>{pos + pos.substr(0, 1) + "3", pos + pos.substr(0, 1) + "4",
                    pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) + 1),
                    pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) + 1)};
        } else if (pos.at(1) == '7') {
            tmp = std::vector<std::string>{pos + pos.substr(0, 1) + "8=N",
                                           pos + pos.substr(0, 1) + "8=B",
                                           pos + pos.substr(0, 1) + "8=R",
                                           pos + pos.substr(0, 1) + "8=Q",
                    pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) + 1) + "=N",
                    pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) + 1) + "=B",
                    pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) + 1) + "=R",
                    pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) + 1) + "=Q",
                    pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) + 1) + "=N",
                    pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) + 1) + "=B",
                    pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) + 1) + "=R",
                    pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) + 1) + "=Q"};
        } else if (pos.at(1) <= '6' && pos.at(1) >= '3') {
            tmp = std::vector<std::string>{
                    pos + pos.substr(0, 1) + std::string(1, pos.at(1) + 1),
                    pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) + 1),
                    pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) + 1)};
        } else {
            std::cerr << "invalid white pawn position" << '\n';
        }
        
        std::vector<std::string> cpy;
        for (auto& i : tmp) {
            if ((i.at(2) >= 'a' && i.at(2) <= 'h') && (i.at(3) >= '1' && i.at(3) <= '8')) {
                cpy.push_back(i);
            }
        }
        return cpy;
    } else if (tolower(p) == 'n') {
        std::vector<std::string> tmp = std::vector<std::string>{
            pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) - 2), 
            pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) + 2), 
            pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) - 2), 
            pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) + 2), 
            pos + std::string(1, pos.at(0) - 2) + std::string(1, pos.at(1) - 1), 
            pos + std::string(1, pos.at(0) - 2) + std::string(1, pos.at(1) + 1), 
            pos + std::string(1, pos.at(0) + 2) + std::string(1, pos.at(1) - 1), 
            pos + std::string(1, pos.at(0) + 2) + std::string(1, pos.at(1) + 1), 
        };
        std::vector<std::string> cpy;
        for (auto& i : tmp) {
            if ((i.at(2) >= 'a' && i.at(2) <= 'h') && (i.at(3) >= '1' && i.at(3) <= '8')) {
                cpy.push_back(i);
            }
        }
        return cpy;
    } else if (tolower(p) == 'b') {
        std::vector<std::string> tmp;
        char c = pos.at(0);
        char cur = pos.at(1);
        while (cur >= '1' + 1 && c >= 'a' + 1) {
            --c;
            --cur;
            tmp.push_back(pos + std::string(1, c) + std::string(1, cur));
        }
        c = pos.at(0);
        cur = pos.at(1);
        while (cur >= '1' + 1 && c <= 'h' - 1) {
            ++c;
            --cur;
            tmp.push_back(pos + std::string(1, c) + std::string(1, cur));
        }
        c = pos.at(0);
        cur = pos.at(1);
        while (cur <= '8' - 1 && c <= 'h' - 1) {
            ++c;
            ++cur;
            tmp.push_back(pos +std::string(1, c) + std::string(1, cur));
        }
        c = pos.at(0);
        cur = pos.at(1);
        while (cur <= '8' - 1 && c >= 'a' + 1) {
            --c;
            ++cur;
            tmp.push_back(pos + std::string(1, c) + std::string(1, cur));
        }
        return tmp;
    } else if (tolower(p) == 'r') {
        std::vector<std::string> tmp;
        char c = pos.at(0);
        char r = pos.at(1);
        while (c >= 'a' + 1) {
            --c;
            tmp.push_back(pos + std::string(1, c) + std::string(1, r));
        }
        c = pos.at(0);
        r = pos.at(1);
        while (c <= 'h' - 1) {
            ++c;
            tmp.push_back(pos + std::string(1, c) + std::string(1, r));
        }
        c = pos.at(0);
        r = pos.at(1);
        while (r >= '1' + 1) {
            --r;
            tmp.push_back(pos + std::string(1, c) + std::string(1, r));
        }
        r = pos.at(1);
        c = pos.at(0);
        while (r <= '8' - 1) {
            ++r;
            tmp.push_back(pos + std::string(1, c) + std::string(1, r));
        }
        return tmp;
    } else if (tolower(p) == 'q') {
        auto r = possibleMoves('r', pos);
        auto b = possibleMoves('b', pos);
        for (auto i : b) {
            r.push_back(i);
        }
        return r;
    } else if (tolower(p) == 'k') {
        std::vector<std::string> tmp = std::vector<std::string>{
            pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) - 1), 
            pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1)), 
            pos + std::string(1, pos.at(0) - 1) + std::string(1, pos.at(1) + 1), 
            pos + std::string(1, pos.at(0)) + std::string(1, pos.at(1) - 1), 
            pos + std::string(1, pos.at(0)) + std::string(1, pos.at(1) + 1), 
            pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) - 1), 
            pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1)), 
            pos + std::string(1, pos.at(0) + 1) + std::string(1, pos.at(1) + 1)};
        
        std::vector<std::string> cpy;
        for (auto& i : tmp) {
            if ((i.at(2) >= 'a' && i.at(2) <= 'h') && (i.at(3) >= '1' && i.at(3) <= '8')) {
                cpy.push_back(i);
            }
        }
        if (toupper(p) == p) {
            if (castleRights.at('K')) cpy.push_back("e1g1");
            if (castleRights.at('Q')) cpy.push_back("e1c1");
        } else if (tolower(p) == p) {
            if (castleRights.at('k')) cpy.push_back("e8g8");
            if (castleRights.at('q')) cpy.push_back("e8c8");
        }
        return cpy;
        
    }
    return std::vector<std::string>();
} // possibleMoves

// pos is a square, e.g. "e4"
std::vector<std::string> Position::checkingSquares(char p, const std::string& pos) const {
    if (pos.length() != 2) {
        std::cout << "incorrect string length for checkingSquares(): given length " << 
            pos.length() << '\n';
    }
    std::vector<std::string> tmp = possibleMoves(p, pos);
    if (tolower(p) == 'n') return tmp;

    std::vector<std::string> cpy;
    for (auto& i : tmp) {
        if (!isMoveBlocked(i.substr(0, 2), i.substr(2, 2)) && 
            !(tolower(p) == 'p' && i.at(0) == i.at(2))) {
            cpy.push_back(i);
        }
    }
    return cpy;
}

std::vector<std::string> Position::bbToSquares(U64 board) {
    if (board == 0) return std::vector<std::string>();
    std::vector<U64> bitSquares;
    int c = 63;
    while (board != 0) {
        if (board >= (1ULL << 63)) {
            bitSquares.push_back(1ULL << c);
        }
        board = board << 1;
        --c;
    }
    std::vector<std::string> tmp;
    for (auto i : bitSquares) {
        tmp.push_back(bitToSquare(i));
    }
    return tmp;
}

std::vector<std::string> Position::legalMoves() {
    std::vector<std::string> tmp;
    for (int i = 0; i < 12; ++i) {
        if ((i <= 5 && turn == true) ||
            (i >= 6 && turn == false)) continue;
        for (auto& square : bbToSquares(thePosition.at(i))) {
            for (auto& j : validMoves(intMap.at(i), square)) {
                tmp.push_back(j);
            }
        }
    }
    return tmp;
}

void Position::move(Move m) {
    // set the source bit to 0
    thePosition.at(pieceMap.at(m.piece)) &= ~(squareToBit(m.source));
    
    // set all destination bits to 0
    for (auto& board : thePosition) {
        board &= ~(squareToBit(m.dest));
    }

    // set the destination bit of the moving piece to 1
    thePosition.at(pieceMap.at(m.piece)) |= (squareToBit(m.dest));

    // if promotion
    if (m.promotionPiece != '-') {
        // set the destination square of the pawn to 0
        thePosition.at(pieceMap.at(m.piece)) &= ~(squareToBit(m.dest));

        thePosition.at(pieceMap.at(m.promotionPiece)) |= squareToBit(m.dest);
    }

    // if castle
    if (m.piece == 'K' && m.source == "e1" && m.dest == "g1") {
        thePosition.at(pieceMap.at('R')) &= ~(squareToBit("h1"));
        thePosition.at(pieceMap.at('R')) |= (squareToBit("f1"));
    } else if (m.piece == 'K' && m.source == "e1" && m.dest == "c1") {
        thePosition.at(pieceMap.at('R')) &= ~(squareToBit("a1"));
        thePosition.at(pieceMap.at('R')) |= (squareToBit("d1"));
    } else if (m.piece == 'k' && m.source == "e8" && m.dest == "g8") {
        thePosition.at(pieceMap.at('r')) &= ~(squareToBit("h8"));
        thePosition.at(pieceMap.at('r')) |= (squareToBit("f8"));
    } else if (m.piece == 'k' && m.source == "e8" && m.dest == "c8") {
        thePosition.at(pieceMap.at('r')) &= ~(squareToBit("a8"));
        thePosition.at(pieceMap.at('r')) |= (squareToBit("d8"));
    }

    // if en passent
    if (m.piece == 'P' && m.dest == enPassent) {
        thePosition.at(pieceMap.at('p')) &= ~(squareToBit(m.dest.substr(0, 1) + "5"));
    } else if (m.piece == 'p' && m.dest == enPassent) {
        thePosition.at(pieceMap.at('P')) &= ~(squareToBit(m.dest.substr(0, 1) + "4"));
    }

    turn = !turn;
    enPassent = m.enPassentSquare;
    for (auto& i : m.castleRights) {
        if (i.second == false) {
            castleRights.at(i.first) = i.second;
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
    thePosition = OthePosition;
    whitePieces = OwhitePieces;
    blackPieces = OblackPieces;

    turn = Oturn;
    castleRights = OcastleRights;
    enPassent = OenPassent;
    halfMove = OhalfMove;
    fullMove = OfullMove;    
}

bool Position::whiteInCheck() {
    for (int i = 6; i <= 10; ++i) {
        for (auto& j : bbToSquares(thePosition.at(i))) {
            for (auto& k : checkingSquares(intMap.at(i), j)) {
                if (thePosition.at(pieceMap.at('K')) & squareToBit(k.substr(2, 2))) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Position::blackInCheck() {
    for (int i = 0; i <= 4; ++i) {
        for (auto& j : bbToSquares(thePosition.at(i))) {
            for (auto& k : checkingSquares(intMap.at(i), j)) {
                if (thePosition.at(pieceMap.at('k')) & squareToBit(k.substr(2, 2))) {
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

int Position::countBits(U64 num) const {
    int c = 0;
    while (num != 0) {
        c += num & 1ULL;
        num <<= 1;
    }
    return c;
}

int Position::materialCount() const {
    int white = 0;
    int black = 0;
    std::map<char, int> values = std::map<char, int>{
        std::pair<char, int>('P', 1),
        std::pair<char, int>('N', 3),
        std::pair<char, int>('B', 3),
        std::pair<char, int>('R', 5),
        std::pair<char, int>('Q', 9),
        std::pair<char, int>('p', 1),
        std::pair<char, int>('n', 3),
        std::pair<char, int>('b', 3),
        std::pair<char, int>('r', 5),
        std::pair<char, int>('q', 9)
    };

    for (int i = 0; i <= 4; ++i) {
        white += bbToSquares(thePosition.at(i)).size() * values.at(intMap.at(i));
    }
    for (int i = 6; i <= 10; ++i) {
        black += bbToSquares(thePosition.at(i)).size() * values.at(intMap.at(i));
    }
    return white - black;
}

void Position::updateBoards() {
    whitePieces = thePosition.at(0) | thePosition.at(1) |
                  thePosition.at(2) | thePosition.at(3) |
                  thePosition.at(4) | thePosition.at(5);

    blackPieces = thePosition.at(6) | thePosition.at(7) |
                  thePosition.at(8) | thePosition.at(9) |
                  thePosition.at(10) | thePosition.at(11);
}

void Position::resetOriginal() {
    OthePosition = thePosition;
    OwhitePieces = whitePieces;
    OblackPieces = blackPieces;

    Oturn = turn;
    OcastleRights = castleRights;
    OenPassent = enPassent;
    OhalfMove = halfMove;
    OfullMove = fullMove;
}

}
