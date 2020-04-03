#include "MainDialog.h"

#include <QtGui>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDateTimeEdit>

MainDialog::MainDialog(QWidget *parent) : QDialog(parent) {
    label_CompanyName = new QLabel(tr("Company: "));
    CompanyName = new QLineEdit;
    label_CompanyName->setBuddy(CompanyName);
    graphButton = new QPushButton("&Draw");
    graphButton->setDefault(true);
    graphButton->setEnabled(false);
    label_dateFrom = new QLabel(tr("Date From: "));
    label_dateTo = new QLabel(tr("Date To: "));
    dateFrom = new QDateTimeEdit(QDate(2020, 02, 01));
    dateTo = new QDateTimeEdit(QDate::currentDate());
    dateTo->setMaximumDate(QDate::currentDate());
    dateTo->setMinimumDate(dateFrom->date());
    // dateFrom->setMaximumDate(dateTo->date());
    dateFrom->setDisplayFormat("yyyy.MM.dd");
    dateTo->setDisplayFormat("yyyy.MM.dd");

    connect(CompanyName, SIGNAL(textChanged(
                                        const QString &)),
            this, SLOT(enableFindButton(
                               const QString &)));
    connect(graphButton, SIGNAL(clicked()), this, SLOT(findClicked()));

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label_CompanyName);
    topLeftLayout->addWidget(CompanyName);

    QHBoxLayout *midLeftLayout = new QHBoxLayout;
    midLeftLayout->addWidget(label_dateFrom);
    midLeftLayout->addWidget(dateFrom);
    QHBoxLayout *bottomLeftLayout = new QHBoxLayout;
    bottomLeftLayout->addWidget(label_dateTo);
    bottomLeftLayout->addWidget(dateTo);
    //bottomLeftLayout->addStretch();                  // растяжка вниз

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addLayout(midLeftLayout);
    leftLayout->addLayout(bottomLeftLayout);

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
    QString text = CompanyName->text();
}

void MainDialog::enableFindButton(const QString &text) {
    graphButton->setEnabled(!text.isEmpty()); // сделать так, чтобы graph не работал при пустом периоде
}
