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
            }
            if (i == 1 && j % 2 == 0)
            {
                squares[i][j].setPiece(BLACK);
            }
            if ((i == 5 || i == 7) && j % 2 == 0)
            {
                squares[i][j].setPiece(WHITE);
            }
            if (i == 6 && j % 2 != 0)
            {
                squares[i][j].setPiece(WHITE);
            }
        }
    }
}

Turn Board::checkTurn(Piece p){
    if (p == BLACK || p == BLACK_KING) {
        return BLACK_TURN;
    }
    else {
        return WHITE_TURN;
    }
}

bool Square::turnToPlay(Turn turn)
{
    if (turn == BLACK_TURN && piece == BLACK || turn == WHITE_TURN && piece == WHITE)
    {
        return true;
    }
    else
    {
        return false;
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
            if (newX == x + 2 || newX = x - 2)
            {
                if (newY == y + 2 || newY == y - 2)
                {
                    Square captureSquare = getSquare((newX + x) / 2, (newY + y) / 2);
                    if (captureSquare.getPiece == EMPTY)
                    {
                        cout << "You can't move there!" << endl;
                        continue;
                    }
                    setSquarePiece(square.getPiece(), newX, newY);
                    setSquarePiece(EMPTY, (newX + x) / 2, (newY + y) / 2);
                    setSquarePiece(EMPTY, x, y);
                    toggleTurn();
                }
            }
            cout << "You can't move there!" << endl;
            continue;
        }
    }
}

vector<Board> Board::findMoves()
{
    vector<Board> moves;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; i < 8; ++j)
        {
            if (squares[i][j].turnToPlay(turn))
            {
                if (i + 1 <= 7 && j + 1 <= 7) {
                    if (squares[i + 1][j + 1].getPiece() == EMPTY)
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(squares[i][j].getPiece(), i + 1, j + 1);
                        newBoard.setSquarePiece(EMPTY, i, j);
                        moves.push_back(newBoard);
                    }
                }
                
                if (i - 1 >= 0 && j + 1 <= 7) {
                    if (squares[i - 1][j + 1].getPiece() == EMPTY)
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(squares[i][j].getPiece(), i - 1, j + 1);
                        newBoard.setSquarePiece(EMPTY, i, j);
                        moves.push_back(newBoard);
                    }
                }

                if (i + 1 <= 7 && j - 1 >= 0) {
                    if (squares[i + 1][j - 1].getPiece() == EMPTY)
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(squares[i][j].getPiece(), i + 1, j - 1);
                        newBoard.setSquarePiece(EMPTY, i, j);
                        moves.push_back(newBoard);
                    }
                }

                if (i - 1 >= 0 && j - 1 >= 0) {
                    if (squares[i - 1][j - 1].getPiece() == EMPTY)
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(squares[i][j].getPiece(), i - 1, j - 1);
                        newBoard.setSquarePiece(EMPTY, i, j);
                        moves.push_back(newBoard);
                    }
                }
            }
        }
    }
    return moves;
}

vector<Board> Board::findCaptures() {
    vector<Board> captures;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; i < 8; ++j)
        {
            if (squares[i][j].turnToPlay(turn))
            {
                if (i + 1 <= 7 && j + 1 <= 7 && i + 2 <= 7 && j + 2 <= 7) {
                // se a peça na diagonal não for a vez (time oposto), e a seguinte estiver vazia.
                    if (!squares[i + 1][j + 1].turnToPlay(turn) && squares[i + 2][j + 2].getPiece() == EMPTY)
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(squares[i][j].getPiece(), i + 2, j + 2);
                        newBoard.setSquarePiece(EMPTY, i, j);
                        newBoard.setSquarePiece(EMPTY, i + 1, j + 1);
                        captures.push_back(newBoard);
                    }
                }
                if (i - 1 >= 0 && j + 1 <= 7 && i - 2 >= 0 && j + 2 <= 7) {
                    if (squares[i - 1][j + 1].turnToPlay(turn) && squares[i - 2][j + 2].getPiece() == EMPTY)
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(squares[i][j].getPiece(), i - 2, j + 2);
                        newBoard.setSquarePiece(EMPTY, i, j);
                        newBoard.setSquarePiece(EMPTY, i - 1, j + 1);
                        captures.push_back(newBoard);
                    }
                }

                if (i + 1 <= 7 && j - 1 >= 0 && i + 2 <= 7 && j - 2 >= 0) {
                    if (squares[i + 1][j - 1].turnToPlay(turn) && squares[i + 2][j - 2].getPiece() == EMPTY)
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(squares[i][j].getPiece(), i + 2, j - 2);
                        newBoard.setSquarePiece(EMPTY, i, j);
                        newBoard.setSquarePiece(EMPTY, i + 1, j - 1);
                        captures.push_back(newBoard);
                    }
                }

                if (i - 1 >= 0 && j - 1 >= 0 && i - 2 >= 0 && j - 2 >= 0) {
                if (squares[i - 1][j - 1].turnToPlay(turn) && squares[i - 2][j - 2].getPiece() == EMPTY)
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(squares[i][j].getPiece(), i - 2, j - 2);
                        newBoard.setSquarePiece(EMPTY, i, j);
                        newBoard.setSquarePiece(EMPTY, i - 1, j - 1);
                        captures.push_back(newBoard);
                    }
                }
            }
        }
    }
    return captures;
}

vector<Board> Board::findMovesAndCaptures() {
    vector<Board> movesAndCaptures;
    vector<Board> captures = findCaptures();
    movesAndCaptures.insert(movesAndCaptures.end(), captures.begin(), captures.end());
    if (captures.size() == 0) {
        vector<Board> moves = findMoves();
        movesAndCaptures.insert(movesAndCaptures.end(), moves.begin(), moves.end());
    }
    return movesAndCaptures;
}

Board Board::captureChain() {
    vector<Board> captures = findCaptures();
    if (captures.size() == 0) {
        return *this;
    }
    else {
        Board newBoard = captures[0].captureChain();
        return newBoard;
    }
}

vector<Board> Board::findSquareMoves(int x, int y) {
    vector<Board> moves;
    Square square = getSquare(x, y);
    if (square.turnToPlay(turn))
    {
        if (x + 1 <= 7 && y + 1 <= 7) {
            if (squares[x + 1][y + 1].getPiece() == EMPTY)
            {
                Board newBoard = Board(*this);
                newBoard.setSquarePiece(square.getPiece(), x + 1, y + 1);
                newBoard.setSquarePiece(EMPTY, x, y);
                moves.push_back(newBoard);
            }
        }
        
        if (x - 1 >= 0 && y + 1 <= 7) {
            if (squares[x - 1][y + 1].getPiece() == EMPTY)
            {
                Board newBoard = Board(*this);
                newBoard.setSquarePiece(square.getPiece(), x - 1, y + 1);
                newBoard.setSquarePiece(EMPTY, x, y);
                moves.push_back(newBoard);
            }
        }

        if (x + 1 <= 7 && y - 1 >= 0) {
            if (squares[x + 1][y - 1].getPiece() == EMPTY)
            {
                Board newBoard = Board(*this);
                newBoard.setSquarePiece(square.getPiece(), x + 1, y - 1);
                newBoard.setSquarePiece(EMPTY, x, y);
                moves.push_back(newBoard);
            }
        }

        if (x - 1 >= 0 && y - 1 >= 0) {
            if (squares[x - 1][y - 1].getPiece() == EMPTY)
            {
                Board newBoard = Board(*this);
                newBoard.setSquarePiece(square.getPiece(), x - 1, y - 1);
                newBoard.setSquarePiece(EMPTY, x, y);
                moves.push_back(newBoard);
            }
        }
    }
    return moves;
}