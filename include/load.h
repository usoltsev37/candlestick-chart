#ifndef LOAD_H
#define LOAD_H

#include "model.h"
#include <string>
#include <QUrl>
#include <QDateTimeEdit>
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
    QUrl get_url();
    void do_request();
private:
    QUrl url;
};

#endif // LOAD_H
