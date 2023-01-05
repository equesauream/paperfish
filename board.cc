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
    if (n == 1) {
        int sum = 0;
        for (int i = 0; i < 12; ++i) {
            if ((i <= 5 && current.turn == true) ||
                (i >= 6 && current.turn == false)) continue;
            for (auto& square : current.bbToSquares(current.thePosition.at(i))) {
                for (auto& j : current.validMoves(Position::intMap.at(i), square)) {
                    move(j);
                    //std::cout << current << '\n';
                    unmove();
                }
                sum += current.validMoves(Position::intMap.at(i), square).size();
            }
        }
        return sum;
    } else {
        int sum = 0;
        for (int i = 0; i < 12; ++i) {
            if ((i <= 5 && current.turn == true) ||
                (i >= 6 && current.turn == false)) continue;
            for (auto& square : current.bbToSquares(current.thePosition.at(i))) {
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
        for (auto& square : current.bbToSquares(current.thePosition.at(i))) {
            for (auto& m : current.validMoves(Position::intMap.at(i), square)) {
                move(m);
                std::cout << m << " " << perft(n - 1) << '\n';
                unmove();
            }
        }
    }
}

}
