#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <iostream>
#include "MainDialog.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget widget;
    widget.resize(37, 37  * 37);
    widget.setWindowTitle("Start project!");

    QGridLayout *gridLayout = new QGridLayout(&widget);

    QLabel * label = new QLabel("Start project!");
    label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    gridLayout->addWidget(label);

    widget.show();

    MainDialog *dialog = new MainDialog;
    dialog->show();

    return app.exec();
}

