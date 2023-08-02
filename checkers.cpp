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

bool Square::turnToPlay(Turn turn)
{
    if ((turn == BLACK_TURN && piece == BLACK) || (turn == WHITE_TURN && piece == WHITE))
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

bool oppositeSquare(Square s1, Square s2) {
    if ((s1.getPiece() == BLACK && s2.getPiece() == WHITE) || (s1.getPiece() == WHITE && s2.getPiece() == BLACK))
        return true;
    else 
        return false;
}

void Board::playGame()
{
    while (!isGameOver())
    {
        if (turn == WHITE_TURN) {
            cout << "White's (x) turn" << endl;
        } else {
            cout << "Black's (o) turn" << endl;
        }
        printBoard();
        int x, y, moveIndex;
        cout << "Enter the x coordinate of the piece you want to move: ";
        cin >> x;
        cout << "Enter the y coordinate of the piece you want to move: ";
        cin >> y;

        if (x < 0 || x > 7 || y < 0 || y > 7) {
            clearScreen();
            cout << "Invalid coordinates!" << endl;
            continue;
        }

        Square square = getSquare(x, y);
        if (square.getPiece() == EMPTY)
        {
            clearScreen();
            cout << "There is no piece there!" << endl;
            continue;
        }

        if (!square.turnToPlay(turn))
        {
            clearScreen();
            cout << "You can't move that piece!" << endl;
            continue;
        }

        vector<Board> allCaptures = findCaptures();
        vector<Board> captures = findSquareCaptures(x, y);
        for (int i = 0; i < captures.size(); ++i) {
            captures[i] = captures[i].captureChain();
        }
        if (allCaptures.size() > 0 && captures.size() == 0) {
            clearScreen();
            cout << "You have to capture a piece!" << endl;
            continue;
        }
        if (captures.size() == 0) {
            vector<Board> moves = findSquareMoves(x, y);
            if (moves.size() == 0) {
                clearScreen();
                cout << "There are no moves for that piece!" << endl;
                continue;
            }

            for (int i = 0; i < moves.size(); ++i) {
                moves[i].printBoard();
                cout << "Move " << i << endl;
            }

            cout << "Enter the number of the move you want to make: ";
            cin >> moveIndex;

            if (moveIndex < 0 || moveIndex >= moves.size()) {
                clearScreen();
                cout << "Invalid move!" << endl;
                continue;
            }
            copySquares(moves[moveIndex]); 
        } else {
            for (int i = 0; i < captures.size(); ++i) {
                captures[i].printBoard();
                cout << "Move " << i << endl;
            }

            cout << "Enter the number of the move you want to make: ";
            cin >> moveIndex;

            if (moveIndex < 0 || moveIndex >= captures.size()) {
                clearScreen();
                cout << "Invalid move!" << endl;
                continue;
            }
            copySquares(captures[moveIndex]); 
        }
        toggleTurn();
        clearScreen();
    }
}

void Board::copySquares(Board b) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            squares[i][j].setPiece(b.squares[i][j].getPiece());
        }
    }
    gameOver = b.gameOver;
    turn = b.turn;
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
                if (turn == BLACK_TURN) {
                    if (i + 1 <= 7 && j + 1 <= 7) {
                        if (squares[i + 1][j + 1].getPiece() == EMPTY)
                        {
                            Board newBoard = Board(*this);
                            newBoard.setSquarePiece(squares[i][j].getPiece(), i + 1, j + 1);
                            newBoard.setSquarePiece(EMPTY, i, j);
                            if (i + 1 == 7 && newBoard.squares[i + 1][j + 1].getPiece() == BLACK)
                                newBoard.getSquare(i + 1, j + 1).promote();
                            moves.push_back(newBoard);
                        }
                    }
                
                    if (i + 1 <= 7 && j - 1 >= 0) {
                        if (squares[i + 1][j - 1].getPiece() == EMPTY)
                        {
                            Board newBoard = Board(*this);
                            newBoard.setSquarePiece(squares[i][j].getPiece(), i + 1, j - 1);
                            newBoard.setSquarePiece(EMPTY, i, j);
                            if (i + 1 == 7 && newBoard.squares[i + 1][j - 1].getPiece() == BLACK)
                                newBoard.squares[i + 1][j - 1].promote();
                            moves.push_back(newBoard);
                        }
                    }
                } else {
                    if (i - 1 >= 0 && j + 1 <= 7) {
                        if (squares[i - 1][j + 1].getPiece() == EMPTY)
                        {
                            Board newBoard = Board(*this);
                            newBoard.setSquarePiece(squares[i][j].getPiece(), i - 1, j + 1);
                            newBoard.setSquarePiece(EMPTY, i, j);
                            if (i - 1 == 0 && newBoard.squares[i - 1][j + 1].getPiece() == WHITE)
                                newBoard.squares[i - 1][j + 1].promote();
                            moves.push_back(newBoard);
                        }
                    }

                    if (i - 1 >= 0 && j - 1 >= 0) {
                        if (squares[i - 1][j - 1].getPiece() == EMPTY)
                        {
                            Board newBoard = Board(*this);
                            newBoard.setSquarePiece(squares[i][j].getPiece(), i - 1, j - 1);
                            newBoard.setSquarePiece(EMPTY, i, j);
                            if (i - 1 == 0 && newBoard.squares[i - 1][j - 1].getPiece() == WHITE)
                                newBoard.squares[i - 1][j - 1].promote();
                            moves.push_back(newBoard);
                        }
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
        for (int j = 0; j < 8; ++j)
        {
            if (squares[i][j].turnToPlay(turn))
            {
                if (i + 1 <= 7 && j + 1 <= 7 && i + 2 <= 7 && j + 2 <= 7) {
                // se a peça na diagonal não for a vez (time oposto), e a seguinte estiver vazia.
                    if (oppositeSquare(squares[i][j], squares[i + 1][j + 1]) && squares[i + 2][j + 2].getPiece() == EMPTY)
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(squares[i][j].getPiece(), i + 2, j + 2);
                        newBoard.setSquarePiece(EMPTY, i, j);
                        newBoard.setSquarePiece(EMPTY, i + 1, j + 1);
                        captures.push_back(newBoard);
                    }
                }
                if (i - 1 >= 0 && j + 1 <= 7 && i - 2 >= 0 && j + 2 <= 7) {
                    if (oppositeSquare(squares[i][j], squares[i - 1][j + 1]) && squares[i - 2][j + 2].getPiece() == EMPTY)
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(squares[i][j].getPiece(), i - 2, j + 2);
                        newBoard.setSquarePiece(EMPTY, i, j);
                        newBoard.setSquarePiece(EMPTY, i - 1, j + 1);
                        captures.push_back(newBoard);
                    }
                }

                if (i + 1 <= 7 && j - 1 >= 0 && i + 2 <= 7 && j - 2 >= 0) {
                    if (oppositeSquare(squares[i][j], squares[i + 1][j - 1]) && squares[i + 2][j - 2].getPiece() == EMPTY)
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(squares[i][j].getPiece(), i + 2, j - 2);
                        newBoard.setSquarePiece(EMPTY, i, j);
                        newBoard.setSquarePiece(EMPTY, i + 1, j - 1);
                        captures.push_back(newBoard);
                    }
                }

                if (i - 1 >= 0 && j - 1 >= 0 && i - 2 >= 0 && j - 2 >= 0) {
                    if (oppositeSquare(squares[i][j], squares[i - 1][j - 1]) && squares[i - 2][j - 2].getPiece() == EMPTY)
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
        if (turn == BLACK_TURN) {
            if (x + 1 <= 7 && y + 1 <= 7) {
                if (squares[x + 1][y + 1].isEmpty())
                {
                    Board newBoard = Board(*this);
                    newBoard.setSquarePiece(square.getPiece(), x + 1, y + 1);
                    newBoard.setSquarePiece(EMPTY, x, y);
                    if (x + 1 == 7 && newBoard.squares[x + 1][y + 1].getPiece() == BLACK)
                        newBoard.squares[x + 1][y + 1].promote();
                    moves.push_back(newBoard);
                }
            }

            if (x + 1 <= 7 && y - 1 >= 0) {
                if (squares[x + 1][y - 1].isEmpty())
                {
                    Board newBoard = Board(*this);
                    newBoard.setSquarePiece(square.getPiece(), x + 1, y - 1);
                    newBoard.setSquarePiece(EMPTY, x, y);
                    if (x + 1 == 7 && newBoard.squares[x + 1][y - 1].getPiece() == BLACK)
                        newBoard.squares[x + 1][y - 1].promote();
                    moves.push_back(newBoard);
                }
            }
        } else {
            if (x - 1 >= 0 && y + 1 <= 7) {
                if (squares[x - 1][y + 1].isEmpty())
                {
                    Board newBoard = Board(*this);
                    newBoard.setSquarePiece(square.getPiece(), x - 1, y + 1);
                    newBoard.setSquarePiece(EMPTY, x, y);
                    if (x - 1 == 0 && newBoard.squares[x - 1][y + 1].getPiece() == WHITE)
                        newBoard.squares[x - 1][y + 1].promote();
                    moves.push_back(newBoard);
                }
            }

            if (x - 1 >= 0 && y - 1 >= 0) {
                if (squares[x - 1][y - 1].isEmpty())
                {
                    Board newBoard = Board(*this);
                    newBoard.setSquarePiece(square.getPiece(), x - 1, y - 1);
                    newBoard.setSquarePiece(EMPTY, x, y);
                    if (x - 1 == 0 && newBoard.squares[x - 1][y - 1].getPiece() == WHITE)
                        newBoard.squares[x - 1][y - 1].promote();
                    moves.push_back(newBoard);
                }
            }
        }
        

    }
    cout << moves.size() << endl;
    return moves;
}

vector<Board> Board::findSquareCaptures(int x, int y) {
    vector<Board> captures;
    Square square = getSquare(x, y);
    if (square.turnToPlay(turn))
    {
        if (x + 1 <= 7 && y + 1 <= 7 && x + 2 <= 7 && y + 2 <= 7) {
            if (oppositeSquare(square, squares[x + 1][y + 1]) && squares[x + 2][y + 2].isEmpty())
            {
                Board newBoard = Board(*this);
                newBoard.setSquarePiece(square.getPiece(), x + 2, y + 2);
                newBoard.setSquarePiece(EMPTY, x, y);
                newBoard.setSquarePiece(EMPTY, x + 1, y + 1);
                if (x + 2 == 7 && newBoard.squares[x + 2][y + 2].getPiece() == BLACK)
                    newBoard.squares[x + 2][y + 2].promote();
                captures.push_back(newBoard);
            }
        }
        if (x - 1 >= 0 && y + 1 <= 7 && x - 2 >= 0 && y + 2 <= 7) {
            if (oppositeSquare(square, squares[x - 1][y + 1]) && squares[x - 2][y + 2].isEmpty())
            {
                Board newBoard = Board(*this);
                newBoard.setSquarePiece(square.getPiece(), x - 2, y + 2);
                newBoard.setSquarePiece(EMPTY, x, y);
                newBoard.setSquarePiece(EMPTY, x - 1, y + 1);
                if(x - 2 == 0 && newBoard.squares[x - 2][y + 2].getPiece() == WHITE)
                    newBoard.squares[x - 2][y + 2].promote();
                captures.push_back(newBoard);
            }
        }

        if (x + 1 <= 7 && y - 1 >= 0 && x + 2 <= 7 && y - 2 >= 0) {
            if (oppositeSquare(square, squares[x + 1][y - 1]) && squares[x + 2][y - 2].isEmpty())
            {
                Board newBoard = Board(*this);
                newBoard.setSquarePiece(square.getPiece(), x + 2, y - 2);
                newBoard.setSquarePiece(EMPTY, x, y);
                newBoard.setSquarePiece(EMPTY, x + 1, y - 1);
                if(x + 2 == 7 && newBoard.squares[x + 2][y - 2].getPiece() == BLACK)
                    newBoard.squares[x + 2][y - 2].promote();
                captures.push_back(newBoard);
            }
        }

        if (x - 1 >= 0 && y - 1 >= 0 && x - 2 >= 0 && y - 2 >= 0) {
            if (oppositeSquare(square, squares[x - 1][y - 1]) && squares[x - 2][y - 2].isEmpty())
            {
                Board newBoard = Board(*this);
                newBoard.setSquarePiece(square.getPiece(), x - 2, y - 2);
                newBoard.setSquarePiece(EMPTY, x, y);
                newBoard.setSquarePiece(EMPTY, x - 1, y - 1);
                if(x - 2 == 0 && newBoard.squares[x - 2][y - 2].getPiece() == WHITE)
                    newBoard.squares[x - 2][y - 2].promote();
                captures.push_back(newBoard);
            }
        }
    }
    return captures;
}





