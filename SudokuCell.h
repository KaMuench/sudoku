//
// Created by muenc on 13/06/2023.
//

#ifndef AUFGABE_X_SUDOKUCELL_H
#define AUFGABE_X_SUDOKUCELL_H

#include <QLabel>
#include <algorithm>
#include <iterator>
#include <string>

class SudokuCell : public QLabel {

public:
    explicit SudokuCell(QWidget *parent = nullptr, int row=0, int col=0);
    void setValue(QString value, bool showHint);
    void resetValue();
    void setHint(QString hints, bool updateLabel);
    void setCellColor(std::string color);
    void updateBackground();
    static QFont setHintFont();
    static QFont setValueFont();

    int getRow(){return row;}
    int getCol(){return col;}
    int getVal(){return value;}
    QString getHint(){return hint;}
    QString getQVal(){return text();}
    bool isLegal() {return legal;}
    void setLegal(bool legal) {
        this->legal=legal;
        legal? setCellColor("white"):setCellColor("red");
    }
private:
    QString hint = "";
    int value = 0;
    int row;
    int col;
    bool legal = true;
    static inline const QFont hintFont = setHintFont();
    static inline const QFont valueFont = setValueFont();
};
#endif //AUFGABE_X_SUDOKUCELL_H
