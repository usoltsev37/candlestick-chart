#ifndef THEMEWINDOW_H
#define THEMEWINDOW_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QPalette>
#include <QStandardItemModel>
#include <QToolButton>
#include <QMouseEvent>

namespace Ui {
class themewindow;
}

class themewindow : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPoint previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)
    enum MouseType {
            None = 0,
            Top,
            Bottom,
            Left,
            Right,
            Move
        };

public:
    explicit themewindow(QWidget *parent = nullptr);
    ~themewindow();
    QPoint previousPosition() const;
    void change_button();
    QString get_theme_name();

public slots:
    void setPreviousPosition(QPoint previousPosition);

signals:
    void previousPositionChanged(QPoint previousPosition);

private slots:
    void on_buttonclick();

    void on_Test_button_clicked();

    void on_pushButton_2_clicked();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::themewindow *ui;
    QTimer *timer;
    QString current_theme = "Default";

    MouseType m_leftMouseButtonPressed;
    QPoint m_previousPosition;
    MouseType checkResizableField(QMouseEvent *event);
};

#endif // THEMEWINDOW_H
