#ifndef PROJECT_ANDLESTICK_HART_MAINDIALOG_H
#define PROJECT_ANDLESTICK_HART_MAINDIALOG_H

#include <QDialog>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QComboBox>
#include <QtGui>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDateTimeEdit>
//#include "model.h"
#include "load.h"
#include "chartwindow.h"
//#include "candlesFwd.h"

class load;

class MainDialog : public QDialog {
    Q_OBJECT
public:
    MainDialog(QWidget *parent = 0);

private slots:
    void findClicked();
    void slotTimerAlarm();
    void enableShowButton(const QString &text);

private:
    chartwindow *chwi; //добавил
    load loader;
    QString company_;
    std::string company = "";
    QLabel *labelInstrumentName_;
    QPushButton *graphButton_;
    QPushButton *showButton_;
    QLabel *labelDateFrom_;
    QLabel *labelDateTo_;
    QDateTimeEdit *dateFrom_;
    QDateTimeEdit *dateTo_;
    QComboBox *comboBox;
    QTimer *timer;
};

#endif //PROJECT_ANDLESTICK_HART_MAINDIALOG_H
