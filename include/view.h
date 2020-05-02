#ifndef VIEW_H
#define VIEW_H

#include <QtCharts/QCandlestickSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

#include "model.h"
#include <string>

class view {
public:
//    view(std::string chart_name, Model &model);

    class Candlestick_Chart { // TODO make base class
    public:
        explicit Candlestick_Chart(); // TODO set colours
        ~Candlestick_Chart();

        void load();

        void create_chart();

        void setAxes();

        void set_legend();

        void show();

    private:
        QColor IncreasingColor = Qt::green;
        QColor DecreasingColor = Qt::red;

        QtCharts::QCandlestickSeries *acmeSeries;
        QStringList categories;
        QtCharts::QChart *chart;
    };

private:
//    Model model;
    std::string chart_title;
};

#endif // VIEW_H
