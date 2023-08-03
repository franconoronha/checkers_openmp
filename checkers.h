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
public:
    void setPiece(Piece newPiece) { piece = newPiece; }
    Piece getPiece() { return piece; }
    bool isEmpty() { return piece == EMPTY; }
    bool turnToPlay(Turn turn);
    Square(Piece newPiece)
    {
        piece = newPiece;
    };
    Square(const Square& s) {
        piece = s.piece;
    }
    Square()
    {
        piece = EMPTY;
    }
    void promote() { piece = (piece == WHITE) ? WHITE_KING : BLACK_KING; }
};

class Board
{
    Square squares[8][8];
    bool gameOver;
    Turn turn;
public:
    bool whiteVictory = false;
    bool blackVictory = false;
    int whitePieces;
    int blackPieces;
    Board()
    {
        gameOver = false;
        turn = WHITE_TURN;
        whitePieces = 12;
        blackPieces = 12;
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
        whitePieces = b.whitePieces;
        blackPieces = b.blackPieces;
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
    void endGame();
    void copySquares(Board b);
    vector<Board> findMoves();
    vector<Board> findCaptures();
    vector<Board> findMovesAndCaptures();
    Board captureChain();
    vector<Board> findSquareCaptures(int x, int y);
    vector<Board> findSquareMoves(int x, int y);
};