#include "board.h"
#include "position.h"
#include "move.h"

namespace engine {

Board::Board() {}

Board::Board(const std::string& FEN) {
    current = Position(FEN);
    history = std::vector<Position>();
}

Board::~Board() {}

void Board::move(const Move& m) {
    history.push_back(current); 
    Position newBoard = current;
    newBoard.move(m);
    newBoard.resetOriginal();
    current = newBoard;
}

void Board::unmove() {
    current = history.back();
    history.pop_back();
}

int Board::perft(int n) {
    if (n == 0) {
        return 1;
    } else {
        int sum = 0;
        for (int i = 0; i < 12; ++i) {
            if ((i <= 5 && current.turn == true) ||
                (i >= 6 && current.turn == false)) continue;
            for (auto& square : bbToSquares(current.thePosition.at(i))) {
                for (auto& m : current.validMoves(Position::intMap.at(i), square)) {
                    move(m);
                    sum += perft(n - 1);
                    unmove();
                }
            }
        }
        return sum;
    }
}

void Board::perftDivide(int n) {
    if (n == 1) {
        const auto& x = current.legalMoves();
        for (auto& i : x) {
            std::cout << i << '\n';
        }
        return;
    }
    for (int i = 0; i < 12; ++i) {
        if ((i <= 5 && current.turn == true) ||
            (i >= 6 && current.turn == false)) continue;
        for (auto& square : bbToSquares(current.thePosition.at(i))) {
            for (auto& m : current.validMoves(Position::intMap.at(i), square)) {
                move(m);
                std::cout << m << " " << perft(n - 1) << '\n';
                unmove();
            }
        }
    }
}

int Board::materialCount() const {
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
        white += bbToSquares(current.thePosition.at(i)).size() * values.at(Position::intMap.at(i));
    }
    for (int i = 6; i <= 10; ++i) {
        black += bbToSquares(current.thePosition.at(i)).size() * values.at(Position::intMap.at(i));
    }
    return white - black;
}

}
