#ifndef PROJECT_ANDLESTICK_HART_MAINDIALOG_H
#define PROJECT_ANDLESTICK_HART_MAINDIALOG_H

#include <QDialog>

class QLabel;

class QLineEdit;

class QPushButton;

class QDateTimeEdit;

class MainDialog : public QDialog {
Q_OBJECT
public:
    MainDialog(QWidget *parent = 0);

private slots:

    void findClicked();

    void enableFindButton(const QString &text);

private:
    QLabel *label_CompanyName;
    QLineEdit *CompanyName;
    QPushButton *graphButton;
    QLabel *label_dateFrom;
    QLabel *label_dateTo;
    QDateTimeEdit *dateFrom;
    QDateTimeEdit *dateTo;
};

#endif //PROJECT_ANDLESTICK_HART_MAINDIALOG_H
