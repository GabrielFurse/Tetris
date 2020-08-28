#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parameters.h"
#include "piece.h"
#include <windows.h>

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <random>
#include <QTimer>
#include <QRandomGenerator>
#include <QGraphicsProxyWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      currentPiece(new Piece),
      nextPiece(new Piece),
      scene (new QGraphicsScene),
      graphicsView (new QGraphicsView),
      timer(new QTimer),
      rowFlashTimer(new QTimer),
      playButton( new QPushButton)
{
    ui->setupUi(this);
    graphicsView->setStyleSheet("Background: transparent; border: none");
    //initialize board matrix to zero

    for (int i = 0; i < gameHeight; i++) {
        for (int j=0 ; j<gameWidth; j++) {
            board[i][j] = {0, gameBackgroundColor};
            nextPieceBoard[i][j] = {0,Qt::white};
        }
    }
    drawBoard();

    // create a scene and add it your view
    graphicsView->setScene(scene);

    ui->verticalLayout->insertWidget(0, graphicsView);
    ui->verticalLayout->setAlignment(graphicsView, Qt::AlignCenter);

    QSizePolicy policy;
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    policy.setHorizontalPolicy(QSizePolicy::Preferred);
    graphicsView->setSizePolicy(policy);
    ui->label->setFont(gameFont);
    ui->label->setStyleSheet("QLabel {background-color: #e3dac9; border: 3px solid black;}");
    ui->label->setAlignment(Qt::AlignCenter);

    playButton->setText("Play");
    playButton->setFont(gameFont);
    playButton->setStyleSheet(
            "QPushButton:pressed {background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(60, 186, 162, 255), stop:1 rgba(98, 211, 162, 255))}"
            "QPushButton {background-color: #e3dac9; border: 3px solid black;}"
            "QPushButton:disabled {background-color: rgb(170, 170, 127)}"
                );
    connect(playButton, &QPushButton::clicked, this ,&MainWindow::startNewGame);

    ui->verticalLayout->insertWidget(0, playButton, Qt::AlignHCenter);
    ui->verticalLayout->setAlignment(playButton, Qt::AlignHCenter);




    connect(timer, &QTimer::timeout, this, &MainWindow::playCycle);
    connect(rowFlashTimer, &QTimer::timeout, this, &MainWindow::flashRows);

    gen.seed(QRandomGenerator::global()->generate());
    distrib = std::uniform_int_distribution<>(0,6);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete currentPiece;
    delete nextPiece;
    delete timer;
    delete playButton;
    delete rowFlashTimer;
}

void MainWindow::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    QRectF x = scene->itemsBoundingRect();
    QRect y = graphicsView->geometry();


    double widthratio = x.width() / y.width();
    double heightratio = x.height() / y.height();
    double width = 0;
    if(widthratio > heightratio){
       width = y.width();
    }
    else {
        width = (y.height() /2) ;
        qDebug() << width;

    }
    playButton->setMaximumWidth(width);
    playButton->setMinimumWidth(width);
    ui->label->setMaximumWidth(width);
    ui->label->setMinimumWidth(width);


}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if(!timer->isActive())
    {
        event->ignore();
        return;
    }
    if(event->key() == Qt::Key_S)
    {
        qDebug() << "s";
        currentPiece->moveDown(board);
        drawBoard();
    }

    else if(event->key() == Qt::Key_D)
    {
        qDebug() << "d";
        currentPiece->moveRight(board);
        drawBoard();
    }

    else if(event->key() == Qt::Key_A)
    {
        qDebug() << "a";
        currentPiece->moveLeft(board);
        drawBoard();
    }

    else if(event->key() == Qt::Key_R)
    {
        qDebug() << "rotate";
        currentPiece->rotate(board);
        qDebug() << checkIfLegal(board);

        drawBoard();
    }

    else if(event->key() == Qt::Key_N)
    {
        count += 1;
        currentPiece->reset(Piece::I);
        if(currentPiece->addToBoard(board))
        {
            qDebug() << "game over";
        }
        drawBoard();
    }
    else event->ignore();
}

QRect MainWindow::rectangle(int i, int j){
    return QRect(100*i, 100*j, 100,100);
}

void MainWindow::drawBoard(){

    scene->clear();

    for (int i = 0; i < gameWidth; i++) {
        for (int j=0 ; j<gameHeight; j++) {
            if(board[j][i].first == 1)
            {
                scene->addRect(rectangle(i,j), QPen(Qt::NoPen), QBrush(board[j][i].second));
            }
            else  scene->addRect(rectangle(i,j), QPen(Qt::NoPen), QBrush(gameBackgroundColor));
            scene->addRect(rectangle(i,j));
        }   
    }

    ui->label->setText("Score: " + QString::number(score));


    for (int i = 2; i < 6; i++) {
        for (int j=0 ; j<3; j++) {
            if(nextPieceBoard[j][i].first == 1)
            {
                scene->addRect(rectangle(i,j -3), QPen(Qt::NoPen), QBrush(nextPieceBoard[j][i].second));
                scene->addRect(rectangle(i,j-3));
            }


        }
    }
    scene->addLine(QLine(0,-400,1000,-400), QPen(Qt::NoPen));
}

std::vector<int> MainWindow::checkIfRowFilled(){
    std::vector<int> indexes = {};
    for(int i=0; i < gameHeight; i++){
        int sum =0;
        for(auto x : board[i]){ sum += x.first;}
        if(sum == gameWidth )
        {
            indexes.push_back(i);
        }
    }
    return indexes;
}

void MainWindow::moveRows(std::vector<int> indexes){
    for(auto x : indexes){
        for(int k = x; k > 0; k--){
            std::swap(board[k], board[k-1]);
        }
    }
}

bool MainWindow::checkIfLegal(boardArray board){
//returns false if game state is illegal, else returns true. only used for debugging
    for (int i = 0; i < gameWidth; i++) {
        for (int j=0 ; j<gameHeight; j++) {
            if(!( (board[j][i].first == 1) | (board[j][i].first == 0) ))
            {
                qDebug() << j << i << board[j][i].first;
                return false;
            }
         }
    }
    return true;
}



void MainWindow::startNewGame(){
    score = 0;
    alive = true;
    timer->stop();
    rowFlashTimer->stop();
    //initialize board matrix to zero
    for (int i = 0; i < gameHeight; i++) {
        for (int j=0 ; j<gameWidth; j++) {
            board[i][j] = {0, gameBackgroundColor};
            nextPieceBoard[i][j] = {0, gameBackgroundColor};
        }
    }
    currentPiece->reset(static_cast<Piece::TETRIMINO>(distrib(gen)));
    shape = distrib(gen);
    nextPiece->reset(static_cast<Piece::TETRIMINO>(shape));
    nextPiece->addToBoard(nextPieceBoard);
    currentPiece->addToBoard(board);
    drawBoard();
    timer->start(gameMoveTime(score));
}

void MainWindow::flashRows(){
    qDebug() << "flash Rows";
    rowFlashTimer->stop();
    std::vector<int> indexes = checkIfRowFilled();
    if(int(indexes.size()) == 0){
        if(!down)
        {
            int x = shape;
            shape = distrib(gen);
            int y = shape;
            nextPiece->removeFromBoard(nextPieceBoard);
            currentPiece->reset(static_cast<Piece::TETRIMINO>(x));
            nextPiece->reset(static_cast<Piece::TETRIMINO>(y));
            nextPiece->addToBoard(nextPieceBoard);
            alive = currentPiece->addToBoard(board);
            drawBoard();
            qDebug() << "reset and add to board";
        }
        timer->start(gameMoveTime(score));
        flashes = 0;
        qDebug() << "nothing filled";
        return;
    }

    std::vector<std::array<std::pair<int, QColor>,gameWidth>> values;
    for(auto x : indexes)
    {
        values.push_back(board[x]);
        board[x].fill({0,gameBackgroundColor});
    }
    if(flashes%2 == 0) drawBoard();
    for(int i=0; i < int(indexes.size()); i++ )
    {
        board[indexes[i]] = values[i];
    }
    if(flashes%2 == 1) drawBoard();

    flashes += 1;
    if(flashes > 5)
    {
        emptyFilledRows();
        flashes = 0;
    }
    rowFlashTimer->start(50);
}

void MainWindow::emptyFilledRows(){
    std::vector<int> indexes = checkIfRowFilled();
    switch(int(indexes.size())){
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
        break;
    case 3:
        score += 500;
        break;
    case 4:
        score += 800;
        break;
    }
    if(indexes.size() != 0)
    {
        for(auto x : indexes)
        {
            board[x].fill({0,gameBackgroundColor});
        }
        moveRows(indexes);

    }
}

void MainWindow::playCycle(){
    timer->stop();
    qDebug() << "playcycle";
    down = currentPiece->moveDown(board);


    drawBoard();
    if(alive)
    {
        if(!down){
            flashRows();
        }


        else timer->start(gameMoveTime(score));
    }
}

void MainWindow::on_pushButton_clicked()
{
    startNewGame();
}
