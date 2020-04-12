#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <iostream>
#include "MainDialog.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainDialog dialog;
    dialog.show();

    return app.exec();
}

