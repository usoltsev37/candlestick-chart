#include "load.h"
#include <QUrl>
#include <QDateTimeEdit>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtCore/QJsonObject>
#include <QJsonDocument>
#include <QtWidgets/QComboBox>
#include <QTimer>
#include <QJsonArray>
#include <iostream>

std::string load::date_to_string(QDateTimeEdit *date) {
    std::string s = date->text().toStdString();
    for(char &i : s)
        if (i == '.')
            i = '-';
    return s;
}

void load::set_all_instrument_url() {
    std::string new_url = "https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities.json";
    all_instrument_url = QUrl(new_url.c_str());
}

QUrl load::get_all_instrument_url() const {
    return all_instrument_url;
}


void load::do_all_instrument_request() {
    manager = new QNetworkAccessManager(this);

    QObject::connect(manager, SIGNAL(finished(QNetworkReply * )),
                     this, SLOT(managerFinished(QNetworkReply * )));

    set_all_instrument_url();
    request.setUrl(get_all_instrument_url());
    manager->get(request);
}

void load::managerFinished(QNetworkReply *reply) {
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
    comboBox->addItems(mm.get_list_of_futures());
}

void load::do_one_instrument_request(QTimer* timer) {
    set_one_instrument_url();
    manager = new QNetworkAccessManager(this);
    QObject::connect(manager, SIGNAL(finished(QNetworkReply * )),
                     this, SLOT(anotherRequest(QNetworkReply *)));
    request.setUrl(get_one_instrument_url());
    manager->get(request);
}

void load::anotherRequest(QNetworkReply *reply) {
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

    timer->start(1000); // Влад, давай сделаем магическую константу
}

QUrl load::get_one_instrument_url() const {
    return one_instrument_url;
}

void load::set_one_instrument_url() {
    std::string instrument_name = comboBox->currentText().toStdString();
    std::string new_url = "https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities/"
                    + instrument_name + "/candles.json";
    std::cerr << new_url << '\n';
    one_instrument_url = new_url.c_str();
}
