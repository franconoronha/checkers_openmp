vector<Board> Board::findCaptures() {
    vector<Board> captures;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (squares[i][j].turnToPlay(turn))
            {
                const int moveSet[8] = {1, 1, -1, 1, 1, -1, -1, -1};
                const int captureSet[8] = {2, 2, -2, 2, 2, -2, -2, -2};
                for (int i = 0; i < 8; i + 2) {
                    if(i + moveSet[i] < 0 || i + moveSet[i] > 7 || j + moveSet[i + 1] < 0 || j + moveSet[i + 1] > 7)
                        continue;
                    if(i + captureSet[i] < 0 || i + captureSet[i] > 7 || j + captureSet[i + 1] < 0 || j + captureSet[i + 1] > 7)
                        continue;

                    if (!squares[i + moveSet[i]][j + moveSet[i + 1]].turnToPlay() && squares[i + captureSet[i]][j + captureSet[i + 1]].isEmpty()){
                        Board newBoard = Board(*this);
                        newBoard.setSquarePiece(squares[i][j].getPiece(), i + captureSet[i], j + captureSet[i + 1]);
                        newBoard.setSquarePiece(EMPTY, i, j);
                        newBoard.setSquarePiece(EMPTY, i + moveSet[i], j + moveSet[i + 1]);
                        Square s = newBoard.getSquare(i + captureSet[i], j + captureSet[i + 1]);
                        if (s.getPiece() == WHITE && j + captureSet[i + 1] == 7)
                            s.promote();
                        else if (s.getPiece() == BLACK && j + captureSet[i + 1] == 0)
                            s.promote();
                        captures.push_back(newBoard);
                    }
                }
            }
        }
    }
    return captures;
}