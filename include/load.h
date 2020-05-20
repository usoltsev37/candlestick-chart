#ifndef LOAD_H
#define LOAD_H

#include "model.h"
#include <QObject>
#include <QNetworkRequest>
#include <string>

//#include "candlesFwd.h"
//#include "MainDialog.h"

//class Model;
class QUrl;

class QDateTimeEdit;

class QNetworkRequest;

class QNetworkReply;

class QNetworkReply;

class QObject;

class QJsonObject;

class QJsonDocument;

class QComboBox;

class QTimer;

class QNetworkAccessManager;

enum RequestType {
    ALL_INSTRUMENTS = 1,
    ONE_INSTRUMENT = 2,
    INTERVALS = 3
};


class load : public QObject {
Q_OBJECT
public:
    load() = default; // del useless M
    std::string date_to_string(QDateTimeEdit *date); // static M
//    void set_url(std::string str, QDateTimeEdit *dateFrom, QDateTimeEdit *dateTo); // Влад, можно сюда &str?????

    void set_all_instrument_url();

    QUrl get_all_instrument_url() const;

    QUrl get_one_instrument_url() const;

    void do_all_instrument_request();

    void do_one_instrument_request(QTimer *timer);

    void set_one_instrument_url();

    QNetworkAccessManager *manager;
    QNetworkRequest request;
    Model mm;
    QTimer *timer;
    QComboBox *comboBox;
public slots:

    void managerFinished(QNetworkReply *reply); // завести сигнал, его повешать на слот в MD M
    void anotherRequest(QNetworkReply *reply);

private:
    QUrl all_instrument_url;
    QUrl one_instrument_url;
    std::size_t start = 0;
};

#endif // LOAD_H
