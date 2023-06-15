//
// Created by muenc on 14/06/2023.
//

#include "GameMenuWidget.h"

GameMenuWidget::GameMenuWidget(QWidget *parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);
    QString style = "QWidget {background-color: grey; width: 200px; height: 50px}";
    setStyleSheet("QWidget {background-color: #999999}");

    QPushButton* easyButton = new QPushButton("Easy");
    QPushButton* mediumButton = new QPushButton("Medium");
    QPushButton* hardButton = new QPushButton("Hard");

    easyButton->setStyleSheet(style);
    mediumButton->setStyleSheet(style);
    hardButton->setStyleSheet(style);


    connect(easyButton, &QPushButton::clicked, this, [this](){ startGame(GameMode::EASY); });
    connect(mediumButton, &QPushButton::clicked, this, [this](){ startGame(GameMode::MEDIUM); });
    connect(hardButton, &QPushButton::clicked, this, [this](){ startGame(GameMode::HARD); });

    layout->addWidget(easyButton);
    layout->addWidget(mediumButton);
    layout->addWidget(hardButton);

    layout->setSpacing(0);                                          // Adjust the spacing between buttons
    layout->setContentsMargins(40, 0, 40, 0);  // Adjust the margins around the buttons


    setLayout(layout);
    setWindowTitle("Game Menu");
}

void GameMenuWidget::startGame(GameMode mode) {
    sWidget = new SudokuWidget(parentWidget(),mode);
    sWidget->setFixedSize(550,550);
    //Callback method gets invoked after finishing the game, opens a dialog box
    connect(dynamic_cast<SudokuWidget*>(sWidget), &SudokuWidget::gameFinished, this, [this]() {
        QMessageBox::information(this, "Game Over", "Congratulations! You completed the game.");
        sWidget->close();   //Close the Sudoku window
        showMenu();
    });
    sWidget->show();
    hide(); // Close the GameMenuWidget when the SudokuWidget is shown
}

void GameMenuWidget::showMenu() {
    sWidget->deleteLater();     // Delete SudokuWidget object, after finishing the game and returning to menu
    layout->setEnabled(true);   // Enable the GameMenu layout
    show();                     // Show the GameMenuWidget
}