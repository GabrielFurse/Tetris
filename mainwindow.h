#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <random>
#include <QRandomGenerator>
#include <QPushButton>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include "parameters.h"
#include "piece.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    boardArray getBoard() {return board;};
    void setBoard(boardArray newboard) {board = newboard;};

    QRect rectangle(int i, int j);
    void drawBoard();

    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void playGame();
    std::vector<int> checkIfRowFilled();
    void moveRows(std::vector<int> indexes);

    static bool checkIfLegal(boardArray board);
    void changeCurrentPiece();


private slots:
    void playCycle();
    void emptyFilledRows();
    void flashRows();
    void startNewGame();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    boardArray board;
    boardArray nextPieceBoard;
    Piece *currentPiece;
    Piece *nextPiece;
    QGraphicsScene *scene;
    QGraphicsView *graphicsView;
    int shape = 0;
    int count = 0;
    QTimer *timer;
    QTimer *rowFlashTimer;
    QPushButton *playButton;
    QGraphicsTextItem *text;

    int flashes = 0;
    bool alive = true;
    bool down = true;
    int score = 0;

    std::mt19937 gen; //mersenne twister engine seeded from QRandomGenerator. (random_device was deterministic on this device)
    std::uniform_int_distribution<> distrib;


};
#endif // MAINWINDOW_H
