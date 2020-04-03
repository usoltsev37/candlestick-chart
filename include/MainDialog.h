#ifndef PROJECT_ANDLESTICK_HART_MAINDIALOG_H
#define PROJECT_ANDLESTICK_HART_MAINDIALOG_H

#include <QDialog>
#include <QDateTimeEdit>

class QLabel;

class QLineEdit;

class QPushButton;

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
};

#endif //PROJECT_ANDLESTICK_HART_MAINDIALOG_H
