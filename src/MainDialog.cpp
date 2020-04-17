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

    chwi = new chartwindow; //добавил

    connect(comboBox, SIGNAL(currentIndexChanged(
                                     const QString &)),
            this, SLOT(enableShowButton(
                               const QString &)));
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
    make_request();
}

void MainDialog::showClicked() {
    std::string s = "https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities/SiZ0/candles.json";
    char cstr[s.size() + 1];
    s.copy(cstr, s.size() + 1);
    cstr[s.size()] = '\0';
    std::cout << s << '\n';
    QUrl url = QUrl(cstr);
    manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply * )),
                     this, SLOT(anotherRequest(QNetworkReply * )));
    request.setUrl(url);
    manager->get(request);
}


void MainDialog::findClicked() { // Влад: findClicked + managerFinished вынести в отдельный метод в load
    //добавил
    chwi->fill(mm.get_bt(), mm.get_et(), mm.get_op(), mm.get_cl(), mm.get_hi(), mm.get_lo());
    chwi->show();
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
    fill_combobox();
}

void MainDialog::anotherRequest(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObj = document.object();
    QJsonValue value = jsonObj.value("candles");
    QJsonArray dataObj = value.toObject().value("data").toArray();
    mm.set_fields(dataObj, ONE_INSTRUMENT);
    std::cout << mm;
}

void MainDialog::enableShowButton(const QString &text) {
    showButton_->setEnabled(text != "-");
}

void MainDialog::fill_combobox() {
    comboBox->addItem("-");
    std::size_t size = mm.get_number_of_instruments();
    for (int i = 0; i < size; ++i) {
        comboBox->addItem(QString::fromUtf8(mm.get_future_name(i).c_str()));
    }
}

void MainDialog::make_request() {
    manager = new QNetworkAccessManager(); // чтобы из констктора можно было сразу заполнять массив list_of_futures
    QObject::connect(manager, SIGNAL(finished(QNetworkReply * )),
                     this, SLOT(managerFinished(QNetworkReply * )));
    company = comboBox->currentText().toStdString();
    loader.set_url("https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities.json");
    request.setUrl(loader.get_url());
    manager->get(request);
}
