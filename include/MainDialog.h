#ifndef PROJECT_ANDLESTICK_HART_MAINDIALOG_H
#define PROJECT_ANDLESTICK_HART_MAINDIALOG_H

#include "load.h"
#include "chartwindow.h"
#include <QDialog>

class load;

class QNetworkReply;
class QJsonDocument;
class QJsonObject;
class QJsonArray;
class QComboBox;
class QLabel;
class QPushButton;
class QHBoxLayout;
class QDateTimeEdit;

class MainDialog : public QDialog {
Q_OBJECT
public:
    MainDialog(QWidget *parent = nullptr);

private slots:
    void findClicked();
    void show_graph();
    void enableShowButton(const QString &text);
  
private:

    void set_QHBox();

    chartwindow *chartWindow; //добавил
    load loader;
    QLabel *labelInstrumentName_;
    QPushButton *loadButton_;
    QPushButton *showButton_;
    QLabel *labelDateFrom_;
    QLabel *labelDateTo_;
    QDateTimeEdit *dateFrom_;
    QDateTimeEdit *dateTo_;
    QComboBox *comboBox;

private slots:
    void scale_change();
};

#endif //PROJECT_ANDLESTICK_HART_MAINDIALOG_H
