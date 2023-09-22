#include <iostream>
#include <string>
#include <vector>
#include "checkers.h"

using namespace std;

int main()
{
    Board board = Board();
    string again;
    do
    {
        board.clearBoard();
        board.playGame(5);
        cout << "Do you want to play again? (y for yes, anything else for no)";
        cin >> again;
    } while (again.compare("y") == 0);

    return 0;
}
