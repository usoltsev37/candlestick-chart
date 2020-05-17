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
    std::string date_to_string(QDateTimeEdit* date); // static M
    void set_url(const std::string &str);
    void set_url(std::string str, QDateTimeEdit* dateFrom, QDateTimeEdit* dateTo); // Влад, можно сюда &str?????
    QUrl get_url() const;
    void do_all_instrument_request();
    void do_one_instrument_request(QTimer* timer);
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    Model mm;
    QTimer *timer;
public slots:
    void managerFinished(QNetworkReply *reply); // завести сигнал, его повешать на слот в MD M
    void anotherRequest(QNetworkReply *reply);
private:
    QUrl url;
    std::size_t start = 0;
};

#endif // LOAD_H
