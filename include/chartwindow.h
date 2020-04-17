#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QWidget>
#include <QMdiArea>
#include <QPushButton>
#include <QTimer>
#include <QtCharts/QCandlestickModelMapper>
#include <QtCharts/QCandlestickLegendMarker>
#include <QtCharts/QCandlestickSet>
#include <QtCharts/QCandlestickSeries>
#include <QDateTime>
#include "model.h"

#include <vector>

#include "view.h"
#include "themewindow.h"

struct Candle{
    QtCharts::QCandlestickSet *candlestickSet;
    qreal open;
    qreal high;
    qreal low;
    qreal close;
    qreal timestamp;

    //TODO перенести конструктор в срр

    Candle(double op, double hi, double lo, double clo, double timest){
        candlestickSet = new QtCharts::QCandlestickSet();
        open = op;
        high = hi;
        low = lo;
        close = clo;
        timestamp = timest;
        candlestickSet->setOpen(open);
        candlestickSet->setHigh(high);
        candlestickSet->setLow(low);
        candlestickSet->setClose(close);
        candlestickSet->setTimestamp(timestamp);
    }
};


namespace Ui {
class chartwindow;
}


class chartwindow : public QWidget
{
    Q_OBJECT

public:
    explicit chartwindow(QWidget *parent = nullptr);
    ~chartwindow();
    double str_to_timestamp(std::string date);

    std::vector<std::string> begin_time;
    std::vector<std::string> end_time;
    std::vector<double> open;
    std::vector<double> closse;
    std::vector<double> high;
    std::vector<double> low;

    std::vector<ModelData> data; // это дожно быть private ->data_
    void fill(Model &model);

private slots:
    // Слоты от кнопок главного окна
    void on_pushButton_clicked();
    void on_pushButton2_clicked();

    void theme_change();//process

    //void on_help_button_clicked();

private:
    Ui::chartwindow *ui;
    std::vector<QPushButton*> buttons;
    themewindow* themewin;
    QtCharts::QChart *chart;
    QtCharts::QChartView *chartView;
protected:
     void keyPressEvent(QKeyEvent *event);
};

#endif // CHARTWINDOW_H
