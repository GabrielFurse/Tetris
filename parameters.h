#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <array>
#include <QColor>
#include <QFont>

const int gameWidth = 10;
const int gameHeight = 20;
inline int gameMoveTime(int score){
    return std::max(100, 1000 - score/2);
}
    typedef std::array<std::array<std::pair<int, QColor> , gameWidth>, gameHeight> boardArray;
const QColor gameBackgroundColor = QColor::fromCmyk(0,1,4,4);
const QFont gameFont = {"Times", 20};
#endif // PARAMETERS_H
