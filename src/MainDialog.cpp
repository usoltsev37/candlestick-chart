#include "MainDialog.h"

#include <QtGui>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

MainDialog::MainDialog(QWidget *parent) : QDialog(parent) {
    label = new QLabel(tr("Company: "));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);
    graphButton = new QPushButton("&Draw");
    graphButton->setDefault(true);
    graphButton->setEnabled(false);

    connect(lineEdit, SIGNAL(textChanged(
                                     const QString &)),
            this, SLOT(enableFindButton(
                               const QString &)));
    connect(graphButton, SIGNAL(clicked()), this, SLOT(findClicked()));

    QHBoxLayout *leftLayout = new QHBoxLayout;
    leftLayout->addWidget(label);
    leftLayout->addWidget(lineEdit);
    leftLayout->addStretch();                  // растяжка вниз
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(graphButton);
    rightLayout->addStretch();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Find"));
    setFixedHeight(sizeHint().height());    // фиксирует высоту
}

void MainDialog::findClicked() {
    QString text = lineEdit->text();
}

void MainDialog::enableFindButton(const QString &text) {
    graphButton->setEnabled(!text.isEmpty()); // сделать так, чтобы graph не работал при пустом периоде
}
