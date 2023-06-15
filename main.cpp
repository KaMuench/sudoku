#include <QApplication>
#include "GameMenuWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    GameMenuWidget gameMenuWidget;
    gameMenuWidget.setFixedSize(550,550);
    gameMenuWidget.show();
    return a.exec();
}
