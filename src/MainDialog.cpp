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
    graphButton_->setDefault(false);
    graphButton_->setEnabled(true);
    TEMP_ = new QPushButton("&TEMP");
    TEMP_->setEnabled(true);
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
    connect(TEMP_, SIGNAL(clicked()), this, SLOT(tempClicked()));
    // добавь здесь connect к слоту, которому ты хотел подключить

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
    rightLayout->addWidget(TEMP_);
    rightLayout->addStretch();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Сandlestick Сhart"));
    setFixedHeight(sizeHint().height());    // фиксирует высоту
}

void MainDialog::tempClicked() {
    std::string s = "https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities/SiZ0/candles.json";
    char cstr[s.size() + 1];
    s.copy(cstr, s.size() + 1);
    cstr[s.size()] = '\0';
    std::cout << s << '\n';
    QUrl url = QUrl(cstr);
    manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(anotherRequest(QNetworkReply*)));
    request.setUrl(url);
    manager->get(request);
}


void MainDialog::findClicked() {
    manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(managerFinished(QNetworkReply*)));
    QString text = CompanyName_->text();
    company = CompanyName_->text().toStdString();
    load loader;
    loader.set_url("https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities.json");
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
    Model my_model(dataObj, dataObj.size());
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
    Model my_model(dataObj);
    std::cout << my_model;
}

void MainDialog::enableFindButton(const QString &text) {
    graphButton_->setEnabled(!text.isEmpty()); // сделать так, чтобы graph не работал при пустом периоде
    // TODO период никогда не был пустым
}
