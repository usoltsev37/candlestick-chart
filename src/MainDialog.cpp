#include "MainDialog.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QComboBox>
#include <QtGui>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDateTimeEdit>

MainDialog::MainDialog(QWidget *parent)
        : QDialog(parent) {
    graphButton_ = new QPushButton(tr("&Draw"), this);
    graphButton_->setEnabled(true);
    showButton_ = new QPushButton(tr("&SHOW"), this);
    showButton_->setDefault(true);
    showButton_->setEnabled(true); //
    labelDateFrom_ = new QLabel(tr("Date From: "), this);
    labelDateTo_ = new QLabel(tr("Date To: "), this);
    dateFrom_ = new QDateTimeEdit(QDate(2020, 02, 01), this);
    dateTo_ = new QDateTimeEdit(QDate::currentDate(), this);
    dateTo_->setMaximumDate(QDate::currentDate());
    dateTo_->setMinimumDate(dateFrom_->date());
    dateFrom_->setDisplayFormat("yyyy.MM.dd");
    dateTo_->setDisplayFormat("yyyy.MM.dd");

    comboBox = new QComboBox(this);
    labelInstrumentName_ = new QLabel(tr("Instrument Name: "), this);
    labelInstrumentName_->setBuddy(comboBox);

    connect(comboBox, SIGNAL(currentIndexChanged(
                                     const QString &)),
            this, SLOT(enableShowButton(
                               const QString &)));
    connect(graphButton_, SIGNAL(clicked()), this, SLOT(findClicked()));
//    connect(showButton_, SIGNAL(clicked()), this, SLOT(showClicked()));

    QHBoxLayout *topLeftLayout = new QHBoxLayout();
    topLeftLayout->addWidget(labelInstrumentName_);
    topLeftLayout->addWidget(comboBox);

    QHBoxLayout *midLeftLayout = new QHBoxLayout();
    midLeftLayout->addWidget(labelDateFrom_);
    midLeftLayout->addWidget(dateFrom_);
    QHBoxLayout *bottomLeftLayout = new QHBoxLayout();
    bottomLeftLayout->addWidget(labelDateTo_);
    bottomLeftLayout->addWidget(dateTo_);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addLayout(midLeftLayout);
    leftLayout->addLayout(bottomLeftLayout);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(graphButton_);
    rightLayout->addWidget(showButton_);
    rightLayout->addStretch();
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Сandlestick Сhart"));
    setFixedHeight(sizeHint().height());
    loader.do_all_instrument_request();

    chwi = new chartwindow(); // TODO add this
}

void MainDialog::findClicked() {
    QTimer* tmr = new QTimer(this);
    loader.do_one_instrument_request(tmr);
    loader.timer = tmr;
    connect(tmr, &QTimer::timeout, this, [this]() {
        show_graph();
        this->loader.timer->deleteLater();
    });
}

void MainDialog::enableShowButton(const QString &text) {
    showButton_->setEnabled(text != "-");
}

void MainDialog::show_graph() {
    chwi->fill(loader.mm);
    chwi->show();
}
