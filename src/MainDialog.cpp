#include "MainDialog.h"
#include "load.h"

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
    graphButton_->setEnabled(true);
    showButton_ = new QPushButton("&SHOW");
    graphButton_->setDefault(true);
    showButton_->setEnabled(false);
    labelDateFrom_ = new QLabel(tr("Date From: "));
    labelDateTo_ = new QLabel(tr("Date To: "));
    dateFrom_ = new QDateTimeEdit(QDate(2020, 02, 01));
    dateTo_ = new QDateTimeEdit(QDate::currentDate());
    dateTo_->setMaximumDate(QDate::currentDate());
    dateTo_->setMinimumDate(dateFrom_->date());
    // dateFrom->setMaximumDate(dateTo->date());
    dateFrom_->setDisplayFormat("yyyy.MM.dd");
    dateTo_->setDisplayFormat("yyyy.MM.dd");

    comboBox = new QComboBox;

    std::vector<std::string> list_of_{"YNDX", "ABRD", "JETBAINS", "GOOGLE", "MAILRU", // убрать
                                      "SBERBANK", "TINKOFF", "VTB", "HSE", "MSU"};

    comboBox->addItem("-");
    for(size_t i = 0; i < list_of_.size(); i++) {
        comboBox->addItem(QString::fromUtf8(list_of_[i].c_str()));
    }

    connect(comboBox, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(enableShowButton(const QString &)));

    //connect(CompanyName_, SIGNAL(textChanged(
    //                                    const QString &)),
    //       this, SLOT(enableFindButton(
    //                          const QString &)));
    connect(graphButton_, SIGNAL(clicked()), this, SLOT(findClicked()));
    connect(showButton_, SIGNAL(clicked()), this, SLOT(showClicked()));
    // добавь здесь connect к слоту, которому ты хотел подключить

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(labelCompanyName_);
    topLeftLayout->addWidget(CompanyName_);
    topLeftLayout->addWidget(comboBox); // пока без коннекта

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
    rightLayout->addWidget(showButton_);
    rightLayout->addStretch();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Сandlestick Сhart"));
    setFixedHeight(sizeHint().height());    // фиксирует высоту
}

void MainDialog::findClicked() { // Влад: findClicked + managerFinished вынести в отдельный метод в load,
    manager = new QNetworkAccessManager(); // чтобы из констктора можно было сразу заполнять массив list_of_futures
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(managerFinished(QNetworkReply*)));
    QString text = CompanyName_->text();
    company = CompanyName_->text().toStdString();
    std::string s = "https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities.json"; // const?
    loader.set_url(s);
    request.setUrl(loader.get_url());
    manager->get(request);
}

void MainDialog::showClicked() {
//    load loader;
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
