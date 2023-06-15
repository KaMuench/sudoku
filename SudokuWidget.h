//
// Created by muenc on 12/06/2023.
//

#ifndef AUFGABE_X_SUDOKUWIDGET_H
#define AUFGABE_X_SUDOKUWIDGET_H


#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QMenuBar>
#include <bits/stdc++.h>
#include "SudokuCell.h"
#include "GameMenuWidget.h"

class SudokuWidget : public QWidget {
Q_OBJECT            //This macro enables feature provided by Qt's meta object system, necessary for using signals
public:
    explicit SudokuWidget(QWidget *parent = nullptr, GameMode mode = GameMode::EASY);

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    QGridLayout *gridLayout;
    const int gridSize = 9;
    SudokuCell *gridCells[9][9];
    SudokuCell *previousCell = nullptr;
    QMenuBar *menuBar = nullptr;
    GameMode currentMode;
    bool hintsEnabled = true;

    void setUpGrid();

    void setUpMenu();

    bool isPossible(int x, int y, int value);

    bool checkPlausibility();

    void fillSudoku();

    bool backtracking(int row, int col, int block);

    void updateHints();

    void removeValues(int count);

    bool isAmbiguous();

    bool arrayContains(int array[81][2], int x, int y, int amount);

signals:

    void gameFinished();

private slots:

    void easy();

    void medium();

    void hard();

    void enableHints();

    void disableHints();
};


#endif //AUFGABE_X_SUDOKUWIDGET_H
