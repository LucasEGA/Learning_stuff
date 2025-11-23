#ifndef UNTITLED_TIKTAKTO_H
#define UNTITLED_TIKTAKTO_H

#include <iostream>
#include <utility>

using namespace std;

class Matrix3x3 {
public:
    Matrix3x3() : n(1) {            // start at turn 1
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                data[i][j] = 0;
    }

    // try to place at (x, y)
    bool foo(int x, int y) {
        if (x < 0 || x >= 3 || y < 0 || y >= 3) {
            std::cout << "Out of range!\n";
            return false;
        }

        if (data[x][y] != 0) {
            std::cout << "Field already taken!\n";
            return false;
        }

        if (n % 2 == 0)
            data[x][y] = -1;   // player 2
        else
            data[x][y] = 1;    // player 1

        n++;
        return true;
    }

    void print() const {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++)
                std::cout << data[i][j] << " ";
            std::cout << "\n";
        }
    }

    int checkWin() const {
        for (int i = 0; i < 3; i++) {
            int sum = data[i][0] + data[i][1] + data[i][2];
            if (sum == 3)  return 1;
            if (sum == -3) return -1;
        }

        for (int j = 0; j < 3; j++) {
            int sum = data[0][j] + data[1][j] + data[2][j];
            if (sum == 3)  return 1;
            if (sum == -3) return -1;
        }

        int d1 = data[0][0] + data[1][1] + data[2][2];
        if (d1 == 3)  return 1;
        if (d1 == -3) return -1;

        int d2 = data[0][2] + data[1][1] + data[2][0];
        if (d2 == 3)  return 1;
        if (d2 == -3) return -1;

        return 0;
    }

    void input() {
        int a, b;

        cout << "Type first number: ";
        cin >> a;

        cout << "Type second number: ";
        cin >> b;

        cout << "You entered: " << a << " and " << b << "\n";
    }

    bool isEmpty(int x, int y) {
        return data[x][y] == 0;
    }

    int evaluateMove(int x, int y) {
        if (data[x][y] != 0) return -100000;

        int score = 0;

        if (x == 1 && y == 1) score += 2;

        if ((x == 0 || x == 2) && (y == 0 || y == 2)) score += 3;

        if (!((x == 1 && y == 1) ||
              ((x == 0 || x == 2) && (y == 0 || y == 2))))
            score += 1;

        data[x][y] = -1;
        if (checkWin() == -1)
            score += 100;
        data[x][y] = 0;

        data[x][y] = 1;
        if (checkWin() == 1)
            score += 50;
        data[x][y] = 0;

        return score;
    }

    std::pair<int,int> bestBotMove() {
        int bestScore = -1000000;
        int bestX = -1, bestY = -1;

        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                if (data[x][y] == 0) {
                    int s = evaluateMove(x, y);
                    if (s > bestScore) {
                        bestScore = s;
                        bestX = x;
                        bestY = y;
                    }
                }
            }
        }
        return {bestX, bestY};
    }

private:
    int data[3][3];
    int n;
};

#endif // UNTITLED_TIKTAKTO_H
