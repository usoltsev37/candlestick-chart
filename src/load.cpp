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

void load::set_url(const std::string &str) {
    char cstr[str.size() + 1];
    str.copy(cstr, str.size() + 1);
    cstr[str.size()] = '\0';
    url = QUrl(cstr);
}

QUrl load::get_url() const {
    return url;
}

void load::set_url(std::string str, QDateTimeEdit *dateFrom, QDateTimeEdit *dateTo) {
    str += "?from=" + date_to_string(dateFrom) + "&till=" + date_to_string(dateTo)
            + "&start=" + std::to_string(start);
    start += 500; // Влад, давай сделаем магическую константу
    std::cout << str << '\n';
    set_url(str); //Влад, чтобы не дублировать код, хотя на первый взгляд какая-то "рекурсивная фукция" может переименовать?
}

void load::do_all_instrument_request() {
    manager = new QNetworkAccessManager(this);

    QObject::connect(manager, SIGNAL(finished(QNetworkReply * )),
                     this, SLOT(managerFinished(QNetworkReply * )));

    set_url("https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities.json");
    request.setUrl(get_url());
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
//    QStringList sequence_len = QStringList() << tr("1") << tr("2") << tr("3") << tr("4") << tr("5");
    comboBox->addItems(mm.get_list_of_futures());
}

void load::do_one_instrument_request(QTimer* timer) {
    std::string s = "https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities/SiU1/candles.json";
    char cstr[s.size() + 1];       //
    s.copy(cstr, s.size() + 1); // Влад, может просто вынести эти 3 строчки в функцию? И вроде этот метод называется c_str?
    cstr[s.size()] = '\0';         //
    std::cout << s << '\n';
    QUrl url = QUrl(cstr);
    manager = new QNetworkAccessManager(this);
    QObject::connect(manager, SIGNAL(finished(QNetworkReply * )),
                     this, SLOT(anotherRequest(QNetworkReply *)));
    request.setUrl(url);
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
