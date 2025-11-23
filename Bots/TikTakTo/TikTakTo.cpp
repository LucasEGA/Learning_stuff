#include <iostream>
#include "Tiktakto.h"

int main() {
    Matrix3x3 board;

    std::cout << "Welcome to TicTacToe!\n";
    std::cout << "You are player 1 (value 1). Bot is player 2 (value -1).\n";
    std::cout << "Enter positions as: row col  (each from 0 to 2)\n\n";

    while (true) {
        board.print();
        std::cout << "\n";

        // --- check if someone already won (should not happen at start, but safe) ---
        int winner = board.checkWin();
        if (winner == 1) {
            std::cout << "Player 1 (you) wins!\n";
            break;
        } else if (winner == -1) {
            std::cout << "Player 2 (bot) wins!\n";
            break;
        }

        // === HUMAN MOVE ===
        int x, y;
        bool moved = false;
        while (!moved) {
            std::cout << "Your move (row col): ";
            std::cin >> x >> y;

            if (!std::cin) {
                std::cout << "Invalid input. Exiting.\n";
                return 0;
            }

            moved = board.foo(x, y);   // will place 1 on odd turns
            if (!moved) {
                std::cout << "Invalid move, try again.\n";
            }
        }

        // check if human just won
        winner = board.checkWin();
        if (winner == 1) {
            board.print();
            std::cout << "Player 1 (you) wins!\n";
            break;
        }

        // === BOT MOVE ===
        std::pair<int,int> botMove = board.bestBotMove();
        int bx = botMove.first;
        int by = botMove.second;

        // no move left -> draw
        if (bx == -1 || by == -1) {
            board.print();
            std::cout << "It's a draw!\n";
            break;
        }

        std::cout << "Bot plays at: " << bx << " " << by << "\n";
        board.foo(bx, by);   // on even turns foo() places -1

        // check if bot just won
        winner = board.checkWin();
        if (winner == -1) {
            board.print();
            std::cout << "Player 2 (bot) wins!\n";
            break;
        }

        // loop continues until win or draw
    }

    std::cout << "Game over.\n";
    return 0;
}
