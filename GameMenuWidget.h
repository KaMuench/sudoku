#ifndef GAMEMENUWIDGET_H
#define GAMEMENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTimer> // Include QTimer

enum GameMode {
    EASY, MEDIUM, HARD
};

#include "SudokuWidget.h"
class GameMenuWidget : public QWidget {
Q_OBJECT

public:
    explicit GameMenuWidget(QWidget *parent = nullptr);
    void showMenu();

private slots:

    void startGame(GameMode mode);

private:
    QWidget *sWidget;
    QVBoxLayout *layout = nullptr;
};

#endif // GAMEMENUWIDGET_H
