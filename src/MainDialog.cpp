#include "MainDialog.h"

MainDialog::MainDialog(QWidget *parent)
        : QDialog(parent) {
    graphButton_ = new QPushButton(tr("&Draw"));
    graphButton_->setEnabled(true);
    showButton_ = new QPushButton(tr("&SHOW"));
    showButton_->setDefault(true);
    showButton_->setEnabled(false);
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

    std::vector<std::string> list_of_{"SIZ0", "YNDX", "ABRD", "JETBAINS", "GOOGLE", "MAILRU", // HARDCODE
                                      "SBERBANK", "TINKOFF", "VTB", "HSE", "MSU"};

    comboBox->addItem("-");
    for(auto & future : list_of_) {
        comboBox->addItem(QString::fromUtf8(future.c_str()));
    }

    connect(comboBox, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(enableShowButton(const QString &)));
    connect(graphButton_, SIGNAL(clicked()), this, SLOT(findClicked()));
    connect(showButton_, SIGNAL(clicked()), this, SLOT(showClicked()));

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
}

void MainDialog::findClicked() {            // Влад: findClicked + managerFinished вынести в отдельный метод в load,
    manager = new QNetworkAccessManager();  // чтобы из констктора можно было сразу заполнять массив list_of_futures
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(managerFinished(QNetworkReply*)));
    company = comboBox->currentText().toStdString();
    std::string s = "https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities.json"; // const?
    loader.set_url(s);
    request.setUrl(loader.get_url());
    manager->get(request);
}

void MainDialog::showClicked() {            //load loader;
    std::string s = "https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities/"
                    + company + "/candles.json";
    manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(anotherRequest(QNetworkReply*)));
    loader.finish_load = false;
    loader.set_url(s, dateFrom_, dateTo_);
    request.setUrl(loader.get_url());
    manager->get(request);
}


void MainDialog::managerFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObj = document.object();
    QJsonValue value = jsonObj.value("securities");
    QJsonArray dataObj = value.toObject().value("data").toArray();
    mm.set_fields(dataObj, ALL_INSTRUMENTS);
}

void MainDialog::anotherRequest(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }
    while(!reply->isFinished())
        qApp->processEvents();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObj = document.object();
    QJsonValue value = jsonObj.value("candles");
    QJsonArray dataObj = value.toObject().value("data").toArray();
    std::cout << dataObj.size() << '\n';
    if (dataObj.size() < 500) {
        loader.finish_load = true;
    }
    mm.set_fields(dataObj, ONE_INSTRUMENT);
    std::cout << mm;
    reply->deleteLater();
}

void MainDialog::enableShowButton(const QString &text) {
    showButton_->setEnabled(text != "-");
    // TODO период никогда не был пустым
}
