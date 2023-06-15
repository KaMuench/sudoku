//
// Created by muenc on 12/06/2023.
//

#include "SudokuWidget.h"

SudokuWidget::SudokuWidget(QWidget *parent, GameMode mode ) : QWidget(parent) {
    currentMode=mode;
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(2);

    setUpMenu();
    setUpGrid();
    setLayout(gridLayout);
    setStyleSheet("QWidget { background-color: black}");
    setWindowTitle("Sudoku Grid");

    switch (currentMode) {
        case GameMode::EASY:
            easy();
            break;
        case GameMode::MEDIUM:
            medium();
            break;
        case GameMode::HARD:
            hard();
            break;
    }
}

void SudokuWidget::mousePressEvent(QMouseEvent *event) {
    //This is not a static method but a virtual method of the QWidget class
    //Since we override this method of the QWidget class we have to call the base
    //class implementation of this method.
    //Like in JAVA: super.baseClassMethod();
    QWidget::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        SudokuCell *clickedCell = dynamic_cast<SudokuCell *>(childAt(
                event->pos()));     //Cast child at position xy of SudokuWidget from QWidget to QLabel
        if (clickedCell != nullptr) {
            // Reset color for the previously clicked cell (if any)
            if (previousCell != nullptr && previousCell != clickedCell) {
                previousCell->updateBackground();
            }
            //Assign color for clicked cell
            previousCell = clickedCell;
            clickedCell->setCellColor("yellow");
        }
    }

}

void SudokuWidget::keyPressEvent(QKeyEvent *event) {
    if (previousCell != nullptr) {
        QString text = event->text();
        //If pressed key is a digit
        if (!text.isEmpty() && text.at(0).isDigit()) {
            previousCell->setValue(text, hintsEnabled);
            //Update whether entered values are correct or not
            //If all values are entered and correct invoke gameFinished();
            if(checkPlausibility()) {
                emit gameFinished();
            }
            updateHints();

        }
    }
}

void SudokuWidget::setUpGrid() {
    for (int row = 0; row < gridSize; ++row) {
        for (int col = 0; col < gridSize; ++col) {
            gridCells[row][col] = new SudokuCell(this, row, col);
            gridCells[row][col]->setCellColor("white");
            gridCells[row][col]->setAlignment(Qt::AlignCenter);
            gridLayout->addWidget(gridCells[row][col], row, col);
        }
    }
}

void SudokuWidget::setUpMenu() {
    menuBar = new QMenuBar(this);
    menuBar->setStyleSheet("QWidget {background-color: #999999}");
    layout()->setMenuBar(menuBar);

    // Create the "NewGame" menu
    QMenu *fileMenu = new QMenu("New Game");
    QMenu *toggleHint = new QMenu("Toogle Hint");
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(toggleHint);
    fileMenu->setStyleSheet("QWidget {background-color: grey}");
    toggleHint->setStyleSheet("QWidget {background-color: grey}");

    // Create the actions
    QAction *newGameEasy = new QAction("Easy");
    QAction *newGameMedium = new QAction("Medium");
    QAction *newGameHard = new QAction("Hard");
    QAction *enableHints = new QAction("Enable");
    QAction *disableHints = new QAction("Disable");
    connect(newGameEasy, &QAction::triggered, this, &SudokuWidget::easy);
    connect(newGameMedium, &QAction::triggered, this, &SudokuWidget::medium);
    connect(newGameHard, &QAction::triggered, this, &SudokuWidget::hard);
    connect(enableHints, &QAction::triggered, this, &SudokuWidget::enableHints);
    connect(disableHints, &QAction::triggered, this, &SudokuWidget::disableHints);
    fileMenu->addAction(newGameEasy);
    fileMenu->addAction(newGameMedium);
    fileMenu->addAction(newGameHard);
    toggleHint->addAction(enableHints);
    toggleHint->addAction(disableHints);

}


bool SudokuWidget::isPossible(int col, int row, int value) {
    if (!value) return true;
    int block = (col / 3) + (row / 3) * 3;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            //If i and j mach the col and row of the currently checked value, skip
            if (i == row && j == col) continue;
            //If the compared value is not the same as the currently checked value, skip
            if (gridCells[i][j]->getVal() != value) continue;
            //If the compared value either is at the same row or col, or in the same block return: not possible -> false
            if (i == row || j == col || ((j / 3) + (i / 3) * 3) == block) return false;
        }
    }
    return true;
}

bool SudokuWidget::checkPlausibility() {
    //Check whether this cell has a legal value, and set colors  accordingly
    int filled = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            //Check whether the value in a cell is right
            if(isPossible(j, i, gridCells[i][j]->getVal())) {
                gridCells[i][j]->setLegal(true);
                //Count filled cells, to check whether game is finished
                if(gridCells[i][j]->getVal() != 0) filled ++;
            } else {
                gridCells[i][j]->setLegal(false);
            }
        }
    }
    //If game over due to all cells are filled with correct values, return true
    return filled == 81;
}

void SudokuWidget::fillSudoku() {

    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    int offsetCol = 0;
    int offsetArray = 0;
    int array[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::shuffle(array, array + 9, std::default_random_engine(0));

    //Fill first diagonal blocks
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 3; col++) {
            int i = array[col + offsetArray];
            QString string = QString::number(array[col + offsetArray]);
            gridCells[row][col + offsetCol]->setValue(QString::number(array[col + offsetArray]), hintsEnabled);
        }
        if (!((row + 1) % 3)) {
            offsetArray = 0;
            offsetCol += 3;
            std::shuffle(array, array + 9, std::default_random_engine(0));
        } else offsetArray += 3;
    }

    //Fill outer middle parts with backtracking
    backtracking(0, 3, 1);
}

bool SudokuWidget::backtracking(int row, int col, int block) {

    //Current cell is
    //Right side outside of current block
    if (!(col % 3) && ((row + 1) % 3) && ((row / 3) * 3 + (col / 3)) != block) {
        col -= 3;
        row += 1;
    }
        //Right side of block and end
    else if (!((col) % 3 || (row + 1) % 3)) {
        //Select which block to fill next
        switch (block) {
            case 1:
                block = 3;
                row = 3;
                col = 0;
                break;
            case 2:
                block = 6;
                row = 6;
                col = 0;
                break;
            case 3:
                block = 5;
                row = 3;
                col = 6;
                break;
            case 5:
                block = 7;
                row = 6;
                col = 3;
                break;
            case 6:
                return true;
            case 7:
                block = 2;
                row = 0;
                col = 6;
            default:
                break;
        }
    }


    //Loop through possible values 1-9
    for (int i = 1; i < 10; i++) {
        //If value is possible set value
        if (isPossible(col, row, i)) {
            gridCells[row][col]->setValue(QString::number(i), hintsEnabled);
            //Start backtracking for next col
            if (backtracking(row, col + 1, block)) {
                //If last row and col are reached, finish
                return true;
            } else gridCells[row][col]->setValue(QString::number(0), hintsEnabled);
        }
    }
    //If all values where looped through and did not work, go back to last backtracking call and search for next value
    return false;
}

void SudokuWidget::updateHints() {
    QString hint = "";
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            //If current cell is not empty skip hint update
            if (gridCells[i][j]->getVal()) continue;
            for (int k = 1; k < 10; k++) {
                if (isPossible(j, i, k)) hint.append(QString::number(k)+",");
            }
            hint.removeLast();
            gridCells[i][j]->setHint(hint, hintsEnabled);
            hint = "";
        }
    }
}

//Checks whether there are several possibilities to solve the sudoku
bool SudokuWidget::isAmbiguous() {
    int count=0;
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (gridCells[i][j]->getVal()) continue;
            for (int k = 1; k < 10; k++) {
                if(isPossible(j, i, k)) count++;
            }
            if(count<2) return false;
            count=0;
        }
    }
    return true;
}

//Removes a certain amount of values from the sudoku
void SudokuWidget::removeValues(int count) {
    int array[81][2];
    int amount = 0;
//    srand(time(NULL));
    QString value;
    int x = rand() % 9;
    int y = rand() % 9;
    while (count) {
        //Check whether the coordinates where already used, if so generate new ones
        while(arrayContains(array, x, y, amount)) {
            x = rand() % 9;
            y = rand() % 9;
        }
        //Add the new coordinates to the list, to remember them
        array[amount][0]=y;array[amount][1]=x;
        amount++;
        //Safe current cell value and remove it
        value=gridCells[y][x]->getQVal();
        gridCells[y][x]->resetValue();
        //If the sudoku is ambiguous now, put the value back and stop removing
        if(isAmbiguous()) {
            gridCells[y][x]->setValue(value, hintsEnabled);
            break;
        }
        count--;
    }
}

//Check whether the array contains the coordinates
bool SudokuWidget::arrayContains(int (*array)[2], int x, int y, int amount) {
    for(int i=0;i<amount;i++) {
        if(array[i][0] == y && array[i][1] == x) return true;
    }
    return false;
}


void SudokuWidget::easy() {
    fillSudoku();
    removeValues(25);
    updateHints();
}

void SudokuWidget::medium() {
    fillSudoku();
    removeValues(50);
    updateHints();
}

void SudokuWidget::hard() {
    fillSudoku();
    removeValues(75);
    updateHints();
}

void SudokuWidget::disableHints() {
    if(hintsEnabled) {
        hintsEnabled=false;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if(!gridCells[i][j]->getVal())gridCells[i][j]->setText("");
            }
        }
    }
}

void SudokuWidget::enableHints() {
    if(!hintsEnabled) {
        hintsEnabled = true;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if(!gridCells[i][j]->getVal())gridCells[i][j]->setText(gridCells[i][j]->getHint());
            }
        }
    }
}
