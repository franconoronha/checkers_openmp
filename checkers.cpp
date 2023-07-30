#include <iostream>
#include <vector>
#include "checkers.h"

using namespace std;

void clearScreen()
{
    cout << "\033[2J\033[1;1H";
}

void Board::printBoard()
{
    cout << " |0|1|2|3|4|5|6|7|" << endl;
    cout << "-----------------" << endl;
    for (int i = 0; i < 8; ++i)
    {
        cout << i << "|";
        for (int j = 0; j < 8; ++j)
        {
            switch (squares[i][j].getPiece())
            {
            case WHITE:
                cout << "x ";
                break;
            case BLACK:
                cout << "o ";
                break;
            case WHITE_KING:
                cout << "X ";
                break;
            case BLACK_KING:
                cout << "O ";
                break;
            case EMPTY:
                cout << "- ";
                break;
            }
        }
        cout << endl;
    }
}

void Board::clearBoard()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if ((i == 0 || i == 2) && j % 2 != 0)
            {
                squares[i][j].setPiece(BLACK);
                blackPieces.push_back(squares[i][j]);
            }
            if (i == 1 && j % 2 == 0)
            {
                squares[i][j].setPiece(BLACK);
                blackPieces.push_back(squares[i][j]);
            }
            if ((i == 5 || i == 7) && j % 2 == 0)
            {
                squares[i][j].setPiece(WHITE);
                whitePieces.push_back(squares[i][j]);
            }
            if (i == 6 && j % 2 != 0)
            {
                squares[i][j].setPiece(WHITE);
                whitePieces.push_back(squares[i][j]);
            }
        }
    }
}

void Board::toggleTurn()
{
    if (turn == BLACK_TURN)
    {
        turn = WHITE_TURN;
    }
    else
    {
        turn = BLACK_TURN;
    }
}

void Board::playGame()
{
    while (!isGameOver())
    {
        printBoard();
        cout << "Enter the x coordinate of the piece you want to move: ";
        int x;
        cin >> x;
        cout << "Enter the y coordinate of the piece you want to move: ";
        int y;
        cin >> y;

        Square square = getSquare(x, y);
        if (square.getPiece() == EMPTY)
        {
            cout << "There is no piece there!" << endl;
            continue;
        }

        if (getTurn() == BLACK_TURN && square.getPiece() != BLACK || getTurn() == WHITE_TURN && square.getPiece() != WHITE)
        {
            cout << "You can't move that piece!" << endl;
            continue;
        }

        cout << "Enter the x coordinate of the square you want to move to: ";
        int newX;
        cin >> newX;
        cout << "Enter the y coordinate of the square you want to move to: ";
        int newY;
        cin >> newY;

        Square newSquare = getSquare(newX, newY);
        if (newSquare.getPiece() != EMPTY)
        {
            cout << "There is already a piece there!" << endl;
            continue;
        }
        if (newX == x + 1 || newX == x - 1)
        {
            if (newY == y + 1 || newY == y - 1)
            {
                setSquarePiece(square.getPiece(), newX, newY); // tem que ser por função de board, se não não muda sem criar um novo square;
                setSquarePiece(EMPTY, x, y);                   // a não ser que tudo isso ocorra dentro de board?
                toggleTurn();
            }
            else
            {
                cout << "You can't move there!" << endl;
                continue;
            }
        }
        else
        {
            cout << "You can't move there!" << endl;
            continue;
        }
    }
}