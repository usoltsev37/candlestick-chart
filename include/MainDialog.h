#ifndef PROJECT_ANDLESTICK_HART_MAINDIALOG_H
#define PROJECT_ANDLESTICK_HART_MAINDIALOG_H

#include <QDialog>

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
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *graphButton;
};

#endif //PROJECT_ANDLESTICK_HART_MAINDIALOG_H
