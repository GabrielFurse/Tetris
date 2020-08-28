#include "piece.h"
#include "parameters.h"
#include "mainwindow.h"

#include <QDebug>

Piece::Piece()
{
    reset(T);

}

void Piece::reset(TETRIMINO shape){
    for(int i=0; i < gameHeight; i++){
        for(int j=0; j < gameWidth; j++){
            pieceBoard[i][j].first = 0;
        }
    }

    switch(shape){
    case I :
        width = 4;
        height = 1;
        color = {0,255,255,200};
        pieceBoard[1][2] = {1,color};
        pieceBoard[1][3] = {1,color};
        pieceBoard[1][4] = {1,color};
        pieceBoard[1][5] = {1,color};
        centre = {1.5,3.5};
        break;

    case S :
        color = {114,203,59,200};
        pieceBoard[0][4] = {1,color};
        pieceBoard[0][5] = {1,color};
        pieceBoard[1][3] = {1,color};
        pieceBoard[1][4] = {1,color};
        centre = {1,4};
        height = 2;
        width =3;
        break;

    case J :
        color = {3,65,174,200};
        pieceBoard[0][3] = {1,color};
        pieceBoard[1][3] = {1,color};
        pieceBoard[1][4] = {1,color};
        pieceBoard[1][5] = {1,color};
        centre = {1,4};
        height = 2;
        width = 3;
        break;

    case L :
        color = {255,151,28,200};
        pieceBoard[0][5] = {1,color};
        pieceBoard[1][3] = {1,color};
        pieceBoard[1][4] = {1,color};
        pieceBoard[1][5] = {1,color};
        centre = {1,4};
        height = 2;
        width = 3;
        break;

    case O :
        color = {255,213,0,200};
        pieceBoard[0][4] = {1,color};
        pieceBoard[0][5] = {1,color};
        pieceBoard[1][4] = {1,color};
        pieceBoard[1][5] = {1,color};
        centre = {0.5,4.5};
        height = 2;
        width = 2;
        break;

    case T:
        color = {153,50,204,200};
        pieceBoard[0][4] = {1,color};
        pieceBoard[1][3] = {1,color};
        pieceBoard[1][4] = {1,color};
        pieceBoard[1][5] = {1,color};
        centre = {1,4};
        height = 2;
        width = 3;
        break;

    case Z:
        color = {255,50,19,200};
        pieceBoard[0][3] = {1,color};
        pieceBoard[0][4] = {1,color};
        pieceBoard[1][4] = {1,color};
        pieceBoard[1][5] = {1,color};
        centre = {1,4};
        height = 2;
        width = 3;
        break;

    }
}

void Piece::removeFromBoard(boardArray &board){
    for(int i=0; i < gameHeight; i++){
        for(int j=0; j < gameWidth; j++){
            if(board[i][j].first == pieceBoard[i][j].first)
            {
                board[i][j] = {0,0};
            }
        }
    }
}

bool Piece::addToBoard(boardArray &board){
    //returns true if move allowed
    for(int i=0; i < gameHeight; i++){
        bool positionTaken = false;
        for(int j=0; j < gameWidth; j++){
            QColor oldcolor = board[i][j].second;

            if(pieceBoard[i][j].first == 1)
            {
                board[i][j].first += pieceBoard[i][j].first;
                board[i][j].second = pieceBoard[i][j].second;
            }
            if(board[i][j].first == 2)
            {
                positionTaken = true;
                board[i][j].first = 1;
                board[i][j].second = oldcolor;
            }
        if(positionTaken) return false;
        }
    }
    return true;
}
bool Piece::moveDown(boardArray &board){
    //returns true if move allowed, else returns false
    boardArray oldBoard = board;
    boardArray oldPieceBoard = pieceBoard;
    removeFromBoard(board);
    //check if at bottom
    int lowest = lowestBlock();
    if(lowest == (gameHeight -1))
    {
        board = oldBoard;
        return false;
    }
    //check if blocked below
    for(int j =0; j< gameHeight; j++){
        for(int i=0; i < gameWidth; i++){
            if((pieceBoard[j][i].first == 1) && (board[j + 1][i].first == 1))
            {
                board = oldBoard;
                return false;
            }
        }
    }
    //do move
    centre.first += 1;
    for(int i=1; i < gameHeight; i++){
        for(int j=0; j< gameWidth; j++){


                pieceBoard[i][j].first += oldPieceBoard[i-1][j].first;
                pieceBoard[i][j].first -= oldPieceBoard[i][j].first;
        }
    }
    for(int i=0; i<gameWidth; i++){
        pieceBoard[0][i].first -= oldPieceBoard[0][i].first;
    }
    setColor(color);
    addToBoard(board);
    return true;
}

void Piece::moveRight(boardArray &board){
    boardArray oldBoard = board;
    boardArray oldPieceBoard = pieceBoard;
    removeFromBoard(board);

    //check if blocked right
    for(int i=0; i<gameHeight; i++){

        if(pieceBoard[i][gameWidth -1].first == 1)
        {
            board = oldBoard;
            return;
        }

    }
    for(int i =0; i< gameHeight; i++){
        for(int j=0; j < gameWidth -1; j++){
            if((pieceBoard[i][j].first == 1) && (board[i][j+1].first == 1))
            {
                board = oldBoard;
                qDebug() << "blocked";
                return;
            }
        }
    }
    //do move
    centre.second += 1;
    for(int i=0; i < gameHeight; i++){
        for(int j=0; j< gameWidth; j++){
                pieceBoard[i][j].first += oldPieceBoard[i][j-1].first;
                pieceBoard[i][j].first -= oldPieceBoard[i][j].first;
        }
    }

    setColor(color);
    addToBoard(board);
}

void Piece::moveLeft(boardArray &board){
    boardArray oldBoard = board;
    boardArray oldPieceBoard = pieceBoard;
    removeFromBoard(board);

    //check if blocked left
    for(int i=0; i<gameHeight; i++){

        if(pieceBoard[i][0].first == 1)
        {
            board = oldBoard;
            qDebug() << "left";
            return;
        }

    }
    for(int i =0; i< gameHeight; i++){
        for(int j=1; j < gameWidth; j++){
            if((pieceBoard[i][j].first == 1) && (board[i][j-1].first == 1))
            {
                board = oldBoard;
                qDebug() << "blocked";
                return;
            }
        }
    }
    //do move
    centre.second -= 1;
    for(int i=0; i < gameHeight; i++){
        for(int j=0; j< gameWidth; j++){
                pieceBoard[i][j].first += oldPieceBoard[i][j+1].first;
                pieceBoard[i][j].first -= oldPieceBoard[i][j].first;
        }
    }
    setColor(color);
    addToBoard(board);
}

void Piece::rotate(boardArray &board){
    boardArray oldPieceBoard = pieceBoard;
    boardArray const noRotationPieceBoard = pieceBoard;
    boardArray const oldBoard = board;
    removeFromBoard(board);
    if(centre.first + width/2 - 0.5 > gameHeight -1)
    {
        board = oldBoard;
        qDebug() << "blocked by bottom";
        return;
    }
    //perform wall kick if needed. 0.5 shift in check needed as centres of rotation not always true centre of shape
    while(centre.second + (height/2) - 0.5 > gameWidth -1 )
    {
        moveLeft(oldPieceBoard);
        pieceBoard = oldPieceBoard;
    }

    while(centre.second - (height/2) + 0.5 < 0 )
    {
        moveRight(oldPieceBoard);
        pieceBoard = oldPieceBoard;
    }

    for(int i=0; i < gameHeight; i++){
        for(int j=0; j< gameWidth; j++){
            if(oldPieceBoard[i][j].first == 1)
            {
                float dy = i - centre.first;
                float dx = j - centre.second;

                pieceBoard[i][j].first -= 1;
                pieceBoard[centre.first + dx][centre.second - dy].first += 1;
            }
        }
    }
    setColor(color);
    if(!addToBoard(board))
    {
        //if desired spot is taken revert changes
        board = oldBoard;
        pieceBoard = noRotationPieceBoard;
        qDebug() << "no rotation";
        return;
    }

    //if rotation succesful, swap width and height
    std::swap(width, height);
}

int Piece::lowestBlock(){
    int ret = 0;
    for(int i=0; i < gameHeight; i++){
        for(int j=0; j< gameWidth; j++){
            if(pieceBoard[i][j].first == 1)
            {
                ret = i;
            }
        }
    }
    return ret;
}


std::pair<float, float> Piece::calculateCentre(){
    //unused
    float y = 0;
    float x = 0;
    int pieces = 0;
    for(int i=0; i < gameHeight; i++){
        for(int j=0; j< gameWidth; j++){
            if(pieceBoard[i][j].first ==1)
            {
                y += i;
                x += j;
                pieces += 1;
            }

        }
    }
    return {y/pieces, x/pieces};
}

QColor Piece::currentColor(){
    //works only if pieces are monochromatic
    for(int i=0; i < gameHeight; i++){
        for(int j=0; j< gameWidth; j++){
            if(pieceBoard[i][j].first == 1)
            {
                return pieceBoard[i][j].second;
            }

        }
    }
    return Qt::black;
}

void Piece::setColor(QColor color){
    for(int i=0; i < gameHeight; i++){
        for(int j=0; j< gameWidth; j++){
            if(pieceBoard[i][j].first == 1)
            {
                pieceBoard[i][j].second = color;
            }
            else pieceBoard[i][j].second = Qt::white;

        }
    }
}

