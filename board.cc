#include "board.h"
#include "position.h"
#include "move.h"
#include "table.h"

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

void Board::searchmove(const Move& m, int n) {
    move(m);
    searchMoves(n);
    unmove();
}

// search n moves or until quiet
void Board::searchMoves(int n) {
    Move best;
    int bestScore = -100000;
    for (const auto& m : current.legalMoves()) {
        move(m);
        int score = alphabetaMax(n, -100000, 100000);
        if (score > bestScore) {
            best = m;
            bestScore = score;
        }
        unmove();
    }
    at(current) = TTInfo(best, bestScore, n);
}

int Board::alphabetaMax(int depth, int alpha, int beta) {
    if (depth == 0)
        return quiesce(alpha, beta);
        //return current.heurVal();
    
    for(const auto& m : current.legalMoves()) {
        current.move(m);
        int score = alphabetaMin(depth - 1, alpha, beta);
        current.unmove();
        if (score >= beta)    
            return beta; // fail hard beta-cutoff
        if (alpha < score)
            alpha = score;
    }
    return alpha;
}

int Board::alphabetaMin(int depth, int alpha, int beta) {
    if (depth == 0)
        return -quiesce(alpha, beta);
        //return current.heurVal();
    
    for(const auto& m : current.legalMoves()) {
        current.move(m);
        int score = alphabetaMax(depth - 1, alpha, beta);
        current.unmove();
        if (score <= alpha)
            return alpha; // fail hard alpha-cutoff
        if (alpha < beta)
            beta = score;
    }
    return beta;
}

int Board::quiesce(int alpha, int beta) {
    int standPat = current.heurVal();
    if (standPat >= beta)
        return beta;
    if (alpha < standPat)
        alpha = standPat;

    for (const auto& m : current.legalMoves()) {
        move(m);
        if (isQuiet()) {
            unmove();
            continue;
        }
        int score = -quiesce(-beta, -alpha);
        unmove();
        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    return alpha;
}

bool Board::isQuiet() {
    // last move resulted in check
    if (current.blackInCheck() || current.whiteInCheck()) 
        return false;
    
    // last move was capture
    if (countBits(history.back().whitePieces | history.back().blackPieces) != 
        countBits(current.whitePieces | current.blackPieces))
        return false;

    // last move resulted in large material swing
    if (history.back().materialCount() - current.materialCount() <= -50 ||
        history.back().materialCount() - current.materialCount() >= 50)
        return false;
    
    return true;
}

int Board::perft(int n) {
    if (n == 1) {
        return current.legalMoves().size();
    } else {
        int sum = 0;
        for (const auto& i : current.legalMoves()) {
            move(i);
            sum += perft(n - 1);
            unmove();
        }
        return sum;
    }
}

void Board::perftDivide(int n) {
    if (n == 1) {
        for (const auto& i : current.legalMoves()) {
            std::cout << i << '\n';
        }
        return;
    }
    for (const auto& i : current.legalMoves()) {
        move(i);
        std::cout << i << " : " << perft(n - 1) << std::endl;
        unmove();
    }
}

}
