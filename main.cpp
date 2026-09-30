#include <QApplication>
#include "mainwindow.h"

int main(int argc, char **argv) {
    QApplication application(argc, argv);

    MainWindow window;
    window.resize(500, 600);
    window.show();

    return QApplication::exec();
}
