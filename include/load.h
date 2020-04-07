#ifndef LOAD_H
#define LOAD_H

#include "model.h"
#include <string>
#include <QUrl>
#include <QDateTimeEdit>
#include <iostream>

class load {
public:
    load() = default;
    std::string date_to_string(QDateTimeEdit* date);
    void set_url(QString company, QDateTimeEdit* dateFrom, QDateTimeEdit* dateTo);
    QUrl get_url();
    void do_request();
private:
    QUrl url;
};

#endif // LOAD_H
