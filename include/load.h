#ifndef LOAD_H
#define LOAD_H

#include "model.h"
#include <string>
#include <QUrl>

class load {
public:
    load(std::string &url); // загружает данные по url в model
    void do_request();
private:
    QUrl url;
    Model model;
};

#endif // LOAD_H
