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
#include "model.h"

class QLabel;

class QLineEdit;

class QPushButton;

class QDateTimeEdit;

class MainDialog : public QDialog {
    Q_OBJECT
public:
    MainDialog(QString &company, QDateTimeEdit *dateFrom, QDateTimeEdit *dateTo, QWidget *parent = 0);

private slots:

            void findClicked();
    void managerFinished(QNetworkReply *reply); // cлот, выполняемый при завершении запроса
    void enableFindButton(const QString &text);

private:
    Model mm;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QString &company_;
    QLabel *labelCompanyName_;
    QLineEdit *CompanyName_;
    QPushButton *graphButton_;
    QLabel *labelDateFrom_;
    QLabel *labelDateTo_;
    QDateTimeEdit *dateFrom_;
    QDateTimeEdit *dateTo_;
};

#endif //PROJECT_ANDLESTICK_HART_MAINDIALOG_H
