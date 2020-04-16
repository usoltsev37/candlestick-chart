#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"
#include "MainDialog.h"
#include <vector>
#include <iostream>
#include <string>
#include <QDateTimeEdit>


class controller {
public:
    controller();

    void deb_() {
        //qDebug(company.toLatin1());

        std::string s = company.toStdString();
        std::cout << s << '\n';
        /*
        QDateTime from = dateFrom->dateTime();
        QString b = from.toString ("yyyy.MM.dd");
        std::string m = b.toStdString();
        std::cout << m << '\n';
        */
    }

    std::string get_url();    // по company выдаёт url
    std::size_t request_for_load(); // "url" -> load.cpp - "data" -> model.cpp - data.push_back(model) - index
    void request_for_view(std::size_t index);  //  index - &model.pop() -> view.cpp - view

    void show_dialog();
private:
    QString company;
    QDateTimeEdit *dateFrom;
    QDateTimeEdit *dateTo;
    std::string url;
    std::vector<Model> data;
    MainDialog *dialog;
};

#endif // CONTROLLER_H
