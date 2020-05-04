#include "load.h"

std::string load::date_to_string(QDateTimeEdit *date) {
    std::string s = date->text().toStdString();
    for (int i = 0; i < s.length(); ++i)
        if (s[i] == '.')
            s[i] = '-';
    return s;
}
void load::set_url(std::string str) {
    char cstr[str.size() + 1];
    str.copy(cstr, str.size() + 1);
    cstr[str.size()] = '\0';
    url = QUrl(cstr);
}


QUrl load::get_url() {
    return url;
}

void load::set_url(std::string str, QDateTimeEdit *dateFrom, QDateTimeEdit *dateTo) {
    str += "?from=" + date_to_string(dateFrom) + "&till=" + date_to_string(dateTo) + "&start=" + std::to_string(start);
    start += 500;
    std::cout << str << '\n';
    char cstr[str.size() + 1];
    str.copy(cstr, str.size() + 1);
    cstr[str.size()] = '\0';
    url = QUrl(cstr);
}

void load::do_all_instrument_request() {
    manager = new QNetworkAccessManager();

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
}

void load::do_one_instrument_request(QTimer* timer) {
    std::string s = "https://iss.moex.com/iss/engines/futures/markets/forts/boards/RFUD/securities/SiU1/candles.json";
    char cstr[s.size() + 1];
    s.copy(cstr, s.size() + 1);
    cstr[s.size()] = '\0';
    std::cout << s << '\n';
    QUrl url = QUrl(cstr);
    manager = new QNetworkAccessManager();
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
    timer->start(1000);
}
