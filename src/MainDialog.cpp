#include "MainDialog.h"

MainDialog::MainDialog(QWidget *parent)
        : QDialog(parent) {
    graphButton_ = new QPushButton(tr("&Draw"));
    graphButton_->setEnabled(true);
    showButton_ = new QPushButton(tr("&SHOW"));
    showButton_->setDefault(true);
    showButton_->setEnabled(true); //
    labelDateFrom_ = new QLabel(tr("Date From: "));
    labelDateTo_ = new QLabel(tr("Date To: "));
    dateFrom_ = new QDateTimeEdit(QDate(2020, 02, 01));
    dateTo_ = new QDateTimeEdit(QDate::currentDate());
    dateTo_->setMaximumDate(QDate::currentDate());
    dateTo_->setMinimumDate(dateFrom_->date());
    dateFrom_->setDisplayFormat("yyyy.MM.dd");
    dateTo_->setDisplayFormat("yyyy.MM.dd");

    comboBox = new QComboBox;
    labelInstrumentName_ = new QLabel(tr("Instrument Name: "));
    labelInstrumentName_->setBuddy(comboBox);

    chwi = new chartwindow; //добавил

    connect(comboBox, SIGNAL(currentIndexChanged(
                                     const QString &)),
            this, SLOT(enableShowButton(
                               const QString &)));
    connect(graphButton_, SIGNAL(clicked()), this, SLOT(findClicked()));
//    connect(showButton_, SIGNAL(clicked()), this, SLOT(showClicked()));

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(labelInstrumentName_);
    topLeftLayout->addWidget(comboBox);

    QHBoxLayout *midLeftLayout = new QHBoxLayout;
    midLeftLayout->addWidget(labelDateFrom_);
    midLeftLayout->addWidget(dateFrom_);
    QHBoxLayout *bottomLeftLayout = new QHBoxLayout;
    bottomLeftLayout->addWidget(labelDateTo_);
    bottomLeftLayout->addWidget(dateTo_);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addLayout(midLeftLayout);
    leftLayout->addLayout(bottomLeftLayout);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(graphButton_);
    rightLayout->addWidget(showButton_);
    rightLayout->addStretch();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Сandlestick Сhart"));
    setFixedHeight(sizeHint().height());
    loader.do_all_instrument_request(); // TODO
}

//void MainDialog::showClicked() {
//    std::string s = "https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities/SiU1/candles.json";
//    char cstr[s.size() + 1];
//    s.copy(cstr, s.size() + 1);
//    cstr[s.size()] = '\0';
//    std::cout << s << '\n';
//    QUrl url = QUrl(cstr);
//    loader.manager = new QNetworkAccessManager();
//    QObject::connect(loader.manager, SIGNAL(finished(QNetworkReply * )),
//                     this, SLOT(anotherRequest(QNetworkReply * )));
//    loader.request.setUrl(url);
//    loader.manager->get(loader.request);
//}


void MainDialog::findClicked() { // Влад: findClicked + managerFinished вынести в отдельный метод в load
    //добавил
    QTimer* tmr = new QTimer();
    loader.do_one_instrument_request(tmr);
    connect(tmr, &QTimer::isActive, this, [this]() {
       std::cout << "Time is okay\n";
    });
//    QObject::connect(loader.timer, SIGNAL(timeout()),
//    QTimer::singleShot(0, &loader, SLOT(one_instrument_request()));
//    QObject::connect(loader.timer, SIGNAL(timeout()), SLOT(show_graph()));
//    chwi->fill(loader.mm.get_bt(), loader.mm.get_et(), loader.mm.get_op(),
//            loader.mm.get_cl(), loader.mm.get_hi(), loader.mm.get_lo());
//    chwi->show();
}

//void MainDialog::anotherRequest(QNetworkReply *reply) {
//    if (reply->error()) {
//        qDebug() << reply->errorString();
//        reply->deleteLater();
//        return;
//    }
//    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
//    QJsonObject jsonObj = document.object();
//    QJsonValue value = jsonObj.value("candles");
//    QJsonArray dataObj = value.toObject().value("data").toArray();
//    loader.mm.set_fields(dataObj, ONE_INSTRUMENT);
//    std::cout << loader.mm;
//}

void MainDialog::enableShowButton(const QString &text) {
    showButton_->setEnabled(text != "-");
}

void MainDialog::show_graph() {
    std::cout << "I WAS HERE AT LEAST\n";
    chwi->fill(loader.mm.get_bt(), loader.mm.get_et(), loader.mm.get_op(),
            loader.mm.get_cl(), loader.mm.get_hi(), loader.mm.get_lo());
    chwi->show();
}

//void MainDialog::slotTimerAlarm() {
//
//}


//void MainDialog::make_request() {
//    loader.manager = new QNetworkAccessManager(); // чтобы из констктора можно было сразу заполнять массив list_of_futures
//    QObject::connect(loader.manager, SIGNAL(finished(QNetworkReply * )),
//                     this, SLOT(managerFinished(QNetworkReply * )));
//    company = comboBox->currentText().toStdString();
//    loader.set_url("https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities.json");
//    loader.request.setUrl(loader.get_url());
//    loader.manager->get(loader.request);
//}
