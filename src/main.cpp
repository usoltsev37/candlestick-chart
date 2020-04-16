#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <iostream>
#include "controller.h"
#include "view.h"
#include "chartwindow.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    controller contr;
    contr.show_dialog();

    //chartwindow tmp;
    //tmp.show();

    return app.exec();
}

