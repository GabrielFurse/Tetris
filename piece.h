#ifndef PIECE_H
#define PIECE_H
#include "parameters.h"
#include <array>
#include <QKeyEvent>

class Piece
{
public:
    Piece();

    enum TETRIMINO{
        I,
        J,
        L,
        O,
        S,
        T,
        Z
    };

    boardArray getBoard() {return pieceBoard;};
    void setBoard(boardArray newboard) {pieceBoard = newboard;};
    void reset(TETRIMINO shape = I);

    void removeFromBoard(boardArray &board);
    bool addToBoard(boardArray &board);

    bool moveDown(boardArray &board);
    void moveRight(boardArray &board);
    void moveLeft(boardArray &board);
    void rotate(boardArray &board);

    int lowestBlock();
    std::pair<float, float> calculateCentre();
    QColor currentColor();
    void setColor(QColor color);


private:
    boardArray pieceBoard;
    std::pair<float, float> centre;
    float width;
    float height;
    QColor color;

};

#endif // PIECE_H
