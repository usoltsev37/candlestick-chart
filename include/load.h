#ifndef LOAD_H
#define LOAD_H


#include <string>
#include <QUrl>
#include <QDateTimeEdit>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QJsonDocument>
#include <iostream>
#include <QtWidgets/QComboBox>
#include <QTimer>
#include "model.h"
//#include "candlesFwd.h"
//#include "MainDialog.h"

//class Model;

enum RequestType {
    ALL_INSTRUMENTS = 1,
    ONE_INSTRUMENT = 2,
    INTERVALS = 3
};


class load : public QObject {
Q_OBJECT
public:
    load() = default;
    std::string date_to_string(QDateTimeEdit* date);
    void set_url(std::string str);
    void set_url(std::string str, QDateTimeEdit* dateFrom, QDateTimeEdit* dateTo);
    QUrl get_url();
    void do_all_instrument_request();
    void do_one_instrument_request(QTimer* timer);
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    Model mm;
    QTimer *timer;
public slots:
    void managerFinished(QNetworkReply *reply);
    void anotherRequest(QNetworkReply *reply);
    void foo();
private:
    QUrl url;
    std::size_t start = 0;
};

#endif // LOAD_H
