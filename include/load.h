#ifndef LOAD_H
#define LOAD_H

#include "model.h"
#include <string>
#include <QUrl>
#include <QDateTimeEdit>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <iostream>

enum RequestType {
    ALL_INSTRUMENTS = 1,
    ONE_INSTRUMENT = 2,
    INTERVALS = 3
};


class load {
public:
    load() = default;
    std::string date_to_string(QDateTimeEdit* date);
    void set_url(std::string str);
    void set_url(std::string str, QDateTimeEdit* dateFrom, QDateTimeEdit* dateTo);
    QUrl get_url();
    void do_request();
    bool finish_load;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
private:
    QUrl url;
    std::size_t start = 0;
};

#endif // LOAD_H
