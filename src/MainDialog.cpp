#include "MainDialog.h"

#include <QtGui>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDateTimeEdit>

MainDialog::MainDialog(QString &company, QDateTimeEdit *dateFrom, QDateTimeEdit *dateTo, QWidget *parent)
        : company_(company), dateFrom_(dateFrom), dateTo_(dateTo), QDialog(parent) {
    labelCompanyName_ = new QLabel(tr("Company: "));
    CompanyName_ = new QLineEdit;
    labelCompanyName_->setBuddy(CompanyName_);
    graphButton_ = new QPushButton("&Draw");
    graphButton_->setDefault(true);
    graphButton_->setEnabled(false);
    labelDateFrom_ = new QLabel(tr("Date From: "));
    labelDateTo_ = new QLabel(tr("Date To: "));
    dateFrom_ = new QDateTimeEdit(QDate(2020, 02, 01));
    dateTo_ = new QDateTimeEdit(QDate::currentDate());
    dateTo_->setMaximumDate(QDate::currentDate());
    dateTo_->setMinimumDate(dateFrom_->date());
    // dateFrom->setMaximumDate(dateTo->date());
    dateFrom_->setDisplayFormat("yyyy.MM.dd");
    dateTo_->setDisplayFormat("yyyy.MM.dd");

    connect(CompanyName_, SIGNAL(textChanged(
                                        const QString &)),
            this, SLOT(enableFindButton(
                               const QString &)));
    connect(graphButton_, SIGNAL(clicked()), this, SLOT(findClicked()));

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(labelCompanyName_);
    topLeftLayout->addWidget(CompanyName_);

    QHBoxLayout *midLeftLayout = new QHBoxLayout;
    midLeftLayout->addWidget(labelDateFrom_);
    midLeftLayout->addWidget(dateFrom_);
    QHBoxLayout *bottomLeftLayout = new QHBoxLayout;
    bottomLeftLayout->addWidget(labelDateTo_);
    bottomLeftLayout->addWidget(dateTo_);
    //bottomLeftLayout->addStretch();                  // растяжка вниз

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addLayout(midLeftLayout);
    leftLayout->addLayout(bottomLeftLayout);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(graphButton_);
    rightLayout->addStretch();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Сandlestick Сhart"));
    setFixedHeight(sizeHint().height());    // фиксирует высоту
}

void MainDialog::findClicked() {
    QString text = CompanyName_->text();
    company_ = CompanyName_->text();
}

void MainDialog::enableFindButton(const QString &text) {
    graphButton_->setEnabled(!text.isEmpty()); // сделать так, чтобы graph не работал при пустом периоде
                                             // TODO период никогда не был пустым
}
