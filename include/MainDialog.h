#ifndef PROJECT_ANDLESTICK_HART_MAINDIALOG_H
#define PROJECT_ANDLESTICK_HART_MAINDIALOG_H

#include <QDialog>
#include <string>
#include <iostream>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QComboBox>
#include "model.h"
#include "load.h"

class QLabel;

class QPushButton;

class QDateTimeEdit;

class MainDialog : public QDialog {
    Q_OBJECT
public:
    MainDialog(QString &company, QDateTimeEdit *dateFrom, QDateTimeEdit *dateTo, QWidget *parent = 0);

private slots:
    void showClicked();
    void findClicked();
    void managerFinished(QNetworkReply *reply); // cлот, выполняемый при завершении запроса
    void anotherRequest(QNetworkReply *reply);
    void enableShowButton(const QString &text);

private:
    Model mm;
    load loader;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QString &company_;
    std::string company = "";
    QLabel *labelCompanyName_;
    QPushButton *graphButton_;
    QPushButton *showButton_;
    QLabel *labelDateFrom_;
    QLabel *labelDateTo_;
    QDateTimeEdit *dateFrom_;
    QDateTimeEdit *dateTo_;
    QComboBox *comboBox;
};

#endif //PROJECT_ANDLESTICK_HART_MAINDIALOG_H
