//
// Created by muenc on 13/06/2023.
//
#include "SudokuCell.h"

SudokuCell::SudokuCell(QWidget *parent, int row, int col) : QLabel(parent) {
    setWordWrap(true);
    setFixedSize(53,53);
    this->row =row;
    this->col =col;
}

void SudokuCell::setValue(QString value, bool showHint) {
    this->value = std::stoi(value.toStdString());
    if(this->value) {
        setText(value);
        setFont(valueFont);
        setAlignment(Qt::AlignCenter);
    }
    else setHint(hint, showHint);
}

void SudokuCell::setHint(QString hint, bool updateLabel) {
    setAlignment(Qt::AlignHCenter);
    setFont(hintFont);
    this->hint = "<" + hint + ">";
    if(updateLabel)setText("<" + hint + ">");
    else setText(" ");
}

void SudokuCell::setCellColor(std::string color) {
    QString styleSheet = "QWidget {background-color: ";
    styleSheet += color +";}";
    if(((col + 1) % 3) == 0) styleSheet += "QWidget {margin-right: 3px}";
    if(((row + 1) % 3) == 0) styleSheet += "QWidget {margin-bottom: 3px}";
    this->setStyleSheet(styleSheet);
}

void SudokuCell::updateBackground() {
    if(this->legal) setCellColor("white");
    else setCellColor("red");
}

void SudokuCell::resetValue() {
    this->value = 0;
    setText(" ");
}

QFont SudokuCell::setHintFont() {
    QFont font;
    font.setPointSize(8);
    font.setBold(false);
    return font;
}

QFont SudokuCell::setValueFont() {
    QFont font;
    font.setPointSize(25);
    font.setBold(true);
    return font;
}
