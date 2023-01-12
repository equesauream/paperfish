#include <iostream>
#include <sstream>
#include <string>

#include "board.h"
#include "position.h"
#include "table.h"
#include "move.h"

using namespace engine;

std::string spaceSplit(std::string& line) {
    if (line.find(" ") == std::string::npos) {
        std::string put = line;
        line = "";
        return put;
    }

    std::string put = line.substr(0, line.find(" "));
    line = line.substr(line.find(" ") + 1);
    return put;
}

int main() {
    //initZobristTables();
    std::string flag;
    
    Board game = Board();
    while (true) {
        std::string line, first;
        std::getline(std::cin, line);
        first = spaceSplit(line);
        if (first == "isready") {
            // engine should respond with readyok without stopping search
            std::cout << "readyok" << std::endl;
        } else if (first == "setoption") {
            std::string next = spaceSplit(line), 
                        name = spaceSplit(line), 
                        val  = spaceSplit(line);
            // next is a filler string
            // name stores the name
            // val stores the value


        } else if (first == "register") {
            std::string tag = spaceSplit(line), 
                        val = spaceSplit(line);
            while (tag != "") {
                if (tag == "later") {

                } else if (tag == "name") {

                } else if (tag == "code") {

                }
                tag = spaceSplit(line);
                val = spaceSplit(line);
            }
        } else if (first == "register") {
            // reset game/board state
        } else if (first == "position") {
            std::string next = spaceSplit(line);
            if (next == "fen") {
                std::string 
                    a = spaceSplit(line),
                    b = spaceSplit(line),
                    c = spaceSplit(line),
                    d = spaceSplit(line),
                    e = spaceSplit(line),
                    f = spaceSplit(line);
                // initialize game with FEN
                game = Board(a + " " + b + " " + " " + c + " " + d + " " + e + " " + f);
            } else if (next == "startpos") {
                // initialize game from starting posiiton
                game = Board();
            }
            std::string rest = spaceSplit(line);
            if (rest == "")
                continue;
            
            std::string move = spaceSplit(line);
            while (move != "") {
                // apply move to game
                game.move(game.current.parseString(move));
                move = spaceSplit(line);
            }
        } else if (first == "go") {
            std::string next = spaceSplit(line);
            if (next == "searchmoves") {
                flag = next;
                // take in searchmoves
                continue;
            } else if (next == "ponder") {

            } else if (next == "wtime") {

            } else if (next == "btime") {

            } else if (next == "winc") {

            } else if (next == "binc") {

            } else if (next == "movestogo") {

            } else if (next == "depth") {

            } else if (next == "nodes") {

            } else if (next == "mate") {

            } else if (next == "movetime") {

            } else if (next == "infinite") {
                // search until the "stop" command

                continue;
            }

            std::cout << "bestmove " << game.current.legalMoves().at(0) << '\n';
        } else if (first == "stop") {
            // stop calculating
            if (flag == "searchmoves") {
                std::cout << "bestmove " << game.current.legalMoves().at(0) << '\n';
                flag = "";
            } else if (flag == "infinite") {
                std::cout << "bestmove " << game.current.legalMoves().at(0) << '\n';
                flag = "";
            }
        } else if (first == "ponderhit") {
            // the user played the move the engine is pondering on
            // switch from pondering to normal thinking
        } else if (first == "quit") {
            return 0;
        } else if (first == "d") {
            std::cout << game.current;
        }

        if (first == "uci") {
            std::cout << "id name Paperfish 1.0\nid author Frank Wang" << std::endl; 
            std::cout << "uciok" << std::endl;
        }
    }
}

