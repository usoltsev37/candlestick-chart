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
    loadButton_ = new QPushButton(tr("&LOAD"), this);
    loadButton_->setDefault(true);
    loadButton_->setEnabled(true); //
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
    connect(loadButton_, SIGNAL(clicked()), this, SLOT(findClicked()));

    chartWindow = new chartwindow();

    set_QHBox();

    setWindowTitle(tr("Сandlestick Сhart"));
    setFixedHeight(sizeHint().height());
    loader.comboBox = comboBox;
    loader.do_all_instrument_request();
}

void MainDialog::set_QHBox() {
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(labelInstrumentName_);
    bottomLayout->addWidget(comboBox);
    bottomLayout->addWidget(labelDateFrom_);
    bottomLayout->addWidget(dateFrom_);
    bottomLayout->addWidget(labelDateTo_);
    bottomLayout->addWidget(dateTo_);
    bottomLayout->addWidget(loadButton_);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(chartWindow);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
}

void MainDialog::findClicked() {
    QTimer* tmr = new QTimer(this);
    loader.do_one_instrument_request(tmr);
    loader.timer = tmr;
    connect(tmr, &QTimer::timeout, this, [this]() {
        show_graph(); // убрать лямбду M
        this->loader.timer->deleteLater(); // не нужна // M
    });
}

void MainDialog::enableShowButton(const QString &text) {
    loadButton_->setEnabled(text != "-");
}

void MainDialog::show_graph() {

    chartWindow->fill(loader.mm);
    chartWindow->chart_load();

    QTimer *timer_for_buttons = new QTimer();
    timer_for_buttons->start(200);
    connect(timer_for_buttons, SIGNAL(timeout()), this, SLOT(scale_change()));

    //начальные периоды по месяцам -- это не очень красиво, загрузка кривая получается
    //chartWindow->month_reload();
}

void MainDialog::scale_change(){
    if(chartWindow->need_to_change){
        //chartWindow->close();
            chartWindow->chart_load();
        //chartWindow->show();
    }
}
