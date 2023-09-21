#include <iostream>
#include <vector>
#include "checkers.h"
#include <omp.h>
#include <mpi.h>mpicc

using namespace std;

Board minimax(Board b, int depth, bool maximizingPlayer, int alpha, int beta)
{
    int numProcs;
    int myRank;

    if (depth == 0 || b.isGameOver())
    {
        return b;
    }

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0)
    {
        cout << "  The number of processes is " << numProcs << endl;
    }

    Board bestMove;
    if (maximizingPlayer)
    {

        vector<Board> moves = b.findMovesAndCaptures();
        bestMove = moves[0];
        int bestScore = -1000;
        int size = moves.size();
        bool flag = false;
        for (int i = 0; i < size; ++i)
        {
            if (flag)
                continue;
            Board newBoard = minimax(moves[i], depth - 1, false, alpha, beta);
            int score = newBoard.evaluate();
            if (score > bestScore)
            {
                bestScore = score;
                bestMove = moves[i];
            }
            if (score > alpha)
            {
                alpha = score;
            }
            if (beta <= alpha)
            {
                flag = true;
            }
        }
    }
    else
    {
        vector<Board> moves = b.findMovesAndCaptures();
        bestMove = moves[0];
        int bestScore = 1000;
        int size = moves.size();
        bool flag = false;
        for (int i = 0; i < size; ++i)
        {
            if (flag)
                continue;
            Board newBoard = minimax(moves[i], depth - 1, true, alpha, beta);
            int score = newBoard.evaluate();
            if (score < bestScore)
            {
                bestScore = score;
                bestMove = moves[i];
            }
            if (score < beta)
            {
                beta = score;
            }
            if (beta <= alpha)
            {
                flag = true;
            }
        }
    }

    MPI_Finalize();
    return bestMove;
}

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
    gameOver = false;
    turn = WHITE_TURN;
    whitePieces = 12;
    blackPieces = 12;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            squares[i][j].setPiece(EMPTY);
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
    if ((turn == BLACK_TURN && piece == BLACK) || (turn == WHITE_TURN && piece == WHITE) || (turn == BLACK_TURN && piece == BLACK_KING) || (turn == WHITE_TURN && piece == WHITE_KING))
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

bool oppositeSquare(Square s1, Square s2)
{
    if ((s1.getPiece() == BLACK && s2.getPiece() == WHITE) || (s1.getPiece() == WHITE && s2.getPiece() == BLACK))
    {
        return true;
    }
    else if (s1.getPiece() == BLACK_KING && s2.getPiece() == WHITE_KING)
    {
        return true;
    }
    else if (s1.getPiece() == WHITE_KING && s2.getPiece() == BLACK_KING)
    {
        return true;
    }
    else if (s1.getPiece() == BLACK_KING && s2.getPiece() == WHITE)
    {
        return true;
    }
    else if (s1.getPiece() == WHITE_KING && s2.getPiece() == BLACK)
    {
        return true;
    }
    else if (s1.getPiece() == BLACK && s2.getPiece() == WHITE_KING)
    {
        return true;
    }
    else if (s1.getPiece() == WHITE && s2.getPiece() == BLACK_KING)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Board::playGame(int max_depth)
{
    bool firstAImove = false;
    auto start = MPI_Wtime();
    auto end = MPI_Wtime();
    while (!isGameOver())
    {
        if (firstAImove)
        {
            auto duration = end - start;
            cout << "Computer took " << duration << " seconds to make a move" << endl;
            auto duration_seq = end_seq - start_seq;
            cout << "Computer took " << duration_seq << " seconds to make a move (sequentially)" << endl;
        }

        if (whitePieces == 0 || blackPieces == 0)
        {
            endGame();
            break;
        }

        vector<Board> allCaptures = findCaptures();
        vector<Board> allMoves = findMoves();
        if (allMoves.size() == 0 && allCaptures.size() == 0)
        {
            endGame();
            break;
        }
        if (turn == WHITE_TURN)
        {
            cout << "White's (x) turn" << endl;
        }
        else
        {
            cout << "Black's (o) turn" << endl;
        }
        if (turn == WHITE_TURN)
        {
            printBoard();
            int x, y, moveIndex;
            cout << "Enter the x coordinate of the piece you want to move: ";
            cin >> x;
            cout << "Enter the y coordinate of the piece you want to move: ";
            cin >> y;

            if (x < 0 || x > 7 || y < 0 || y > 7)
            {
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

            vector<Board> squareCaptures = findSquareCaptures(x, y);
            for (int i = 0; i < squareCaptures.size(); ++i)
            {
                squareCaptures[i] = squareCaptures[i].captureChain();
            }
            if (allCaptures.size() > 0 && squareCaptures.size() == 0)
            {
                clearScreen();
                cout << "You have to capture a piece!" << endl;
                continue;
            }

            if (squareCaptures.size() == 0)
            {
                vector<Board> moves = findSquareMoves(x, y);
                if (moves.size() == 0)
                {
                    clearScreen();
                    cout << "There are no moves for that piece!" << endl;
                    continue;
                }

                for (int i = 0; i < moves.size(); ++i)
                {
                    moves[i].printBoard();
                    cout << "Move " << i << endl;
                }

                cout << "Enter the number of the move you want to make: ";
                cin >> moveIndex;
                if (moveIndex < 0 || moveIndex >= moves.size())
                {
                    clearScreen();
                    cout << "Invalid move!" << endl;
                    continue;
                }
                copySquares(moves[moveIndex]);
            }
            else
            {
                for (int i = 0; i < squareCaptures.size(); ++i)
                {
                    squareCaptures[i].printBoard();
                    cout << "Move " << i << endl;
                }

                cout << "Enter the number of the move you want to make: ";
                cin >> moveIndex;

                if (moveIndex < 0 || moveIndex >= squareCaptures.size())
                {
                    clearScreen();
                    cout << "Invalid move!" << endl;
                    continue;
                }
                copySquares(squareCaptures[moveIndex]);
            }
        }
        else
        {
            start = MPI_Wtime();
            Board bestMove = minimax(*this, max_depth, true, -1000, 1000);
            copySquares(bestMove);
            end = MPI_Wtime();

            firstAImove = true;
        }
        toggleTurn();
        clearScreen();
    }
}

void Board::copySquares(Board b)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            squares[i][j].setPiece(b.squares[i][j].getPiece());
        }
    }
    gameOver = b.gameOver;
    turn = b.turn;
    whitePieces = b.whitePieces;
    blackPieces = b.blackPieces;
}

vector<Board> Board::findMoves()
{
    vector<Board> moves;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (squares[i][j].turnToPlay(turn))
            {
                if (squares[i][j].getPiece() == WHITE_KING || squares[i][j].getPiece() == BLACK_KING)
                {
                    for (int ii = 0; ii < 4; ++ii)
                    {
                        int xDir, yDir;
                        switch (ii)
                        {
                        case 0:
                            xDir = 1;
                            yDir = 1;
                            break;
                        case 1:
                            xDir = 1;
                            yDir = -1;
                            break;
                        case 2:
                            xDir = -1;
                            yDir = 1;
                            break;
                        case 3:
                            xDir = -1;
                            yDir = -1;
                            break;
                        }
                        for (int jj = 1; jj < 8; ++jj)
                        {
                            int x2 = i + jj * xDir;
                            int y2 = j + jj * yDir;
                            if (x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7)
                            {
                                break;
                            }
                            if (squares[x2][y2].isEmpty())
                            {
                                Board newBoard = Board(*this);
                                newBoard.setSquarePiece(squares[i][j].getPiece(), x2, y2);
                                newBoard.setSquarePiece(EMPTY, i, j);
                                moves.push_back(newBoard);
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if (turn == BLACK_TURN)
                    {
                        if (i + 1 <= 7 && j + 1 <= 7)
                        {
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

                        if (i + 1 <= 7 && j - 1 >= 0)
                        {
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
                    }
                    else
                    {
                        if (i - 1 >= 0 && j + 1 <= 7)
                        {
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

                        if (i - 1 >= 0 && j - 1 >= 0)
                        {
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
    }
    return moves;
}

vector<Board> Board::findMovesAndCaptures()
{
    vector<Board> movesAndCaptures = findCaptures();
    if (movesAndCaptures.size() > 0)
    {
        for (int i = 0; i < movesAndCaptures.size(); ++i)
        {
            movesAndCaptures[i] = movesAndCaptures[i].captureChain();
        }
    }
    if (movesAndCaptures.size() == 0)
    {
        movesAndCaptures = findMoves();
    }
    return movesAndCaptures;
}

Board Board::captureChain()
{
    vector<Board> captures = findCaptures();
    if (captures.size() == 0)
    {
        return *this;
    }
    else
    {
        Board newBoard = captures[0].captureChain();
        return newBoard;
    }
}

vector<Board> Board::findSquareMoves(int x, int y)
{
    vector<Board> moves;
    Square square = getSquare(x, y);
    if (square.turnToPlay(turn))
    {
        if (square.getPiece() == WHITE_KING || square.getPiece() == BLACK_KING)
        {
            for (int i = 0; i < 4; ++i)
            {
                int xDir, yDir;
                switch (i)
                {
                case 0:
                    xDir = 1;
                    yDir = 1;
                    break;
                case 1:
                    xDir = 1;
                    yDir = -1;
                    break;
                case 2:
                    xDir = -1;
                    yDir = 1;
                    break;
                case 3:
                    xDir = -1;
                    yDir = -1;
                    break;
                }
                for (int j = 1; j < 8; ++j)
                {
                    int x2 = x + j * xDir;
                    int y2 = y + j * yDir;
                    if (x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7)
                    {
                        break;
                    }
                    if (squares[x2][y2].isEmpty())
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(square.getPiece(), x2, y2);
                        newBoard.setSquarePiece(EMPTY, x, y);
                        moves.push_back(newBoard);
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            if (turn == BLACK_TURN)
            {
                if (x + 1 <= 7 && y + 1 <= 7)
                {
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

                if (x + 1 <= 7 && y - 1 >= 0)
                {
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
            }
            else
            {
                if (x - 1 >= 0 && y + 1 <= 7)
                {
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

                if (x - 1 >= 0 && y - 1 >= 0)
                {
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
    }
    return moves;
}

vector<Board> Board::findCaptures()
{
    vector<Board> captures;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            vector<Board> squareCaptures = findSquareCaptures(i, j);
            captures.insert(captures.end(), squareCaptures.begin(), squareCaptures.end());
        }
    }
    return captures;
}

vector<Board> Board::findSquareCaptures(int x, int y)
{
    vector<Board> captures;
    Square square = getSquare(x, y);
    if (square.turnToPlay(turn))
    {
        const int moveSet[8] = {1, 1, -1, 1, 1, -1, -1, -1};
        if (square.getPiece() == WHITE_KING || square.getPiece() == BLACK_KING)
        {
            for (int i = 0; i < 8; i + 2)
            {
                for (int j = 1; j < 8; ++j)
                {
                    int x2 = x + j * moveSet[i];
                    int y2 = y + j * moveSet[i + 1];
                    int x3 = x2 + moveSet[i];
                    int y3 = y2 + moveSet[i + 1];
                    if (x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7 || x3 < 0 || x3 > 7 || y3 < 0 || y3 > 7)
                    {
                        break;
                    }
                    if (squares[x2][y2].isEmpty())
                    { // segue até achar uma peça adversaria
                        continue;
                    }
                    if (oppositeSquare(square, squares[x2][y2]) && squares[x3][y3].isEmpty())
                    {
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(square.getPiece(), x3, y3);
                        newBoard.setSquarePiece(EMPTY, x, y);
                        newBoard.setSquarePiece(EMPTY, x2, y2);
                        if (square.getPiece() == WHITE_KING)
                        {
                            newBoard.blackPieces -= 1;
                        }
                        else
                        {
                            newBoard.whitePieces -= 1;
                        }
                        captures.insert(captures.end(), newBoard);
                        break;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            const int captureSet[8] = {2, 2, -2, 2, 2, -2, -2, -2};
            for (int i = 0; i < 8; i += 2)
            {
                if (x + moveSet[i] < 0 || x + moveSet[i] > 7 || y + moveSet[i + 1] < 0 || y + moveSet[i + 1] > 7)
                    continue;
                if (x + captureSet[i] < 0 || x + captureSet[i] > 7 || y + captureSet[i + 1] < 0 || y + captureSet[i + 1] > 7)
                    continue;
                if (oppositeSquare(square, squares[x + moveSet[i]][y + moveSet[i + 1]]) && squares[x + captureSet[i]][y + captureSet[i + 1]].isEmpty())
                {
                    Board newBoard = Board(*this);
                    newBoard.setSquarePiece(square.getPiece(), x + captureSet[i], y + captureSet[i + 1]);
                    newBoard.setSquarePiece(EMPTY, x, y);
                    newBoard.setSquarePiece(EMPTY, x + moveSet[i], y + moveSet[i + 1]);
                    Square s = newBoard.getSquare(x + captureSet[i], y + captureSet[i + 1]);
                    if (s.getPiece() == WHITE && y + captureSet[i + 1] == 7)
                        s.promote();
                    else if (s.getPiece() == BLACK && y + captureSet[i + 1] == 0)
                        s.promote();
                    captures.insert(captures.end(), newBoard);
                }
            }
        }
    }
    return captures;
}

void Board::endGame()
{
    gameOver = true;
    cout << "Game over!" << endl;
    printBoard();
    if (turn == WHITE_TURN)
    {
        blackVictory = true;
        cout << "Black wins!" << endl;
    }
    else
    {
        whiteVictory = true;
        cout << "White wins!" << endl;
    }
}

int Board::evaluate()
{
    int value = 0;
    if (whiteVictory)
    {
        value = -1000;
    }
    else if (blackVictory)
    {
        value = 1000;
    }
    else
    {
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (squares[i][j].getPiece() == WHITE)
                {
                    value -= 1;
                }
                else if (squares[i][j].getPiece() == BLACK)
                {
                    value += 1;
                }
                else if (squares[i][j].getPiece() == WHITE_KING)
                {
                    value -= 2;
                }
                else if (squares[i][j].getPiece() == BLACK_KING)
                {
                    value += 2;
                }
            }
        }
    }
    return value;
}