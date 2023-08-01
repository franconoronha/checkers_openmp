#include <iostream>

using namespace std;

enum Piece
{
    WHITE,
    BLACK,
    WHITE_KING,
    BLACK_KING,
    EMPTY
};

enum Turn
{
    BLACK_TURN,
    WHITE_TURN
};

class Square
{
    Piece piece;
    int x, y;

public:
    void setPiece(Piece newPiece) { piece = newPiece; }
    Piece getPiece() { return piece; }
/*     void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    int getX() { return x; }
    int getY() { return y; } */
    bool isEmpty() { return piece == EMPTY; }
    bool turnToPlay(Turn turn);
    Square(Piece newPiece, int newX, int newY)
    {
        piece = newPiece;
        x = newX;
        y = newY;
    };
    Square(int newX, int newY)
    {
        piece = EMPTY;
        x = newX;
        y = newY;
    };
    Square(const Square& s) {
        piece = s.piece;
        x = s.x;
        y = s.y;
    }
    Square()
    {
        piece = EMPTY;
        x = 0;
        y = 0;
    }
};

class Board
{
    Square squares[8][8];
    bool gameOver;
    Turn turn;

public:
    Board()
    {
        gameOver = false;
        turn = WHITE_TURN;
    }
    Board(const Board& b) // Construtor de cópia
    {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                squares[i][j] = Square(b.squares[i][j]);
            }
        }
        gameOver = b.gameOver;
        turn = b.turn;
    }
    bool isGameOver() { return gameOver; }
    void setGameOver(bool gameState) { gameOver = gameState; }
    Turn getTurn() { return turn; }
    void setTurn(Turn newTurn) { turn = newTurn; }
    Square getSquare(int x, int y) { return squares[x][y]; }
    void setSquarePiece(Piece piece, int x, int y) { squares[x][y].setPiece(piece); }
    void setSquare(Square square, int x, int y) { squares[x][y] = square; }
    void clearBoard();
    void printBoard();
    void toggleTurn();
    void playGame();
    void playGame2();
    void copySquares(Board b);
    vector<Board> findMoves();
    vector<Board> findCaptures();
    vector<Board> findMovesAndCaptures();
    Board captureChain();
    vector<Board> findSquareMoves(int x, int y);
};