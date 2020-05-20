#include "ui_chartwindow.h"
#include "chartwindow.h"
#include <QStyleFactory>
#include <QWidget>
#include <QMdiArea>
#include <QPushButton>
#include <QTimer>
#include <QtCharts/QCandlestickModelMapper>
#include <QtCharts/QCandlestickLegendMarker>
#include <QtCharts/QCandlestickSet>
#include <QtCharts/QCandlestickSeries>
#include <QDateTime>

#include <assert.h>
#include <iostream>

//копия аналогичной функции из класса chartwindow, наверное, стоит сделать её глобальной
double DataGrouping::str_to_timestamp(const std::string &date) const {
    QDateTime tmp_date_time = QDateTime::fromString(QString::fromStdString(date),
                                                    "yyyy-MM-dd hh:mm:ss");
    double timestamp = tmp_date_time.toTime_t();
    return timestamp;
}

void DataGrouping::compress_by_n_days() {
    constexpr size_t seconds_per_day = 86400;
    size_t seconds_for_period = seconds_per_day * number_of_days;

    for (size_t i = 1; i < candle_vec.size();) {
        tmp_open = candle_vec[i - 1].open;
        double lowest = candle_vec[i - 1].low;
        double highest = candle_vec[i - 1].high;

        while (unix_time_cnt < seconds_for_period) { //unix время в секундах, в дне 86400 секунд
            if (i >= candle_vec.size()) break;
            if (lowest > candle_vec[i].low)
                lowest = candle_vec[i].low;
            if (highest < candle_vec[i].high)
                highest = candle_vec[i].high;

            if (unix_time_cnt + str_to_timestamp(candle_vec[i].begin_time)
                - str_to_timestamp(candle_vec[i - 1].end_time) >= seconds_for_period)
                //предвидим закрытие
                tmp_close = candle_vec[i].close;

            unix_time_cnt = unix_time_cnt + str_to_timestamp(candle_vec[i].begin_time)
                            - str_to_timestamp(candle_vec[i - 1].end_time);

            i++;
        }

        Candle day_n_candle(tmp_open, highest, lowest,
                            tmp_close, str_to_timestamp(candle_vec[i].begin_time));

        result.push_back(day_n_candle);
        unix_time_cnt = 0;
    }

}


double chartwindow::str_to_timestamp(const std::string &date) const {
    QDateTime tmp_date_time = QDateTime::fromString(QString::fromStdString(date),
                                                    "yyyy-MM-dd hh:mm:ss");
    double timestamp = tmp_date_time.toTime_t();
    return timestamp;
}

void chartwindow::one_day_reload() {
    grouping_coefficient = 1;
    need_to_change = true;
}

void chartwindow::two_days_reload() {
    grouping_coefficient = 2;
    need_to_change = true;
}

void chartwindow::three_days_reload() {
    grouping_coefficient = 3;
    need_to_change = true;
}

void chartwindow::week_reload() {
    grouping_coefficient = 7;
    need_to_change = true;
}

void chartwindow::month_reload() {
    grouping_coefficient = 30;
    need_to_change = true;
}

chartwindow::chartwindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::chartwindow) {
    ui->setupUi(this);
    this->setWindowTitle(tr("Charts"));
    setMinimumSize(700, 600);
    setMaximumSize(700, 600);

    //надо не дать кнопкам нажиматься, пока перезагружается график, это многое ломает, ещё подумаю над этим
    connect(ui->one_day_button, SIGNAL(clicked()), this, SLOT(one_day_reload()));
    connect(ui->two_days_button, SIGNAL(clicked()), this, SLOT(two_days_reload()));
    connect(ui->three_days_button, SIGNAL(clicked()), this, SLOT(three_days_reload()));
    connect(ui->week_button, SIGNAL(clicked()), this, SLOT(week_reload()));
    connect(ui->month_button, SIGNAL(clicked()), this, SLOT(month_reload()));
}

chartwindow::~chartwindow() {
    delete ui;
}


void chartwindow::fill(const Model &model) {

    const size_t size_of_data = model.get_size();
    ModelData tmp;
    for (std::size_t i = 0; i < size_of_data - 1; i++) {
        tmp.low = model.get_data_by_index(i).low;
        tmp.high = model.get_data_by_index(i).high;
        tmp.open = model.get_data_by_index(i).open;
        tmp.close = model.get_data_by_index(i).close;
        tmp.end_time = model.get_data_by_index(i).end_time;
        tmp.begin_time = model.get_data_by_index(i).begin_time;
        data.push_back(std::move(tmp));
    }
}


void chartwindow::chart_load() {
    try {
        if (is_loaded == false) {
//        if(acmeSeries != nullptr) delete acmeSeries;
//        if(chart != nullptr) delete chart;
            acmeSeries = new QtCharts::QCandlestickSeries();
            acmeSeries->setName(tr("Candles"));
            acmeSeries->setIncreasingColor(QColor(Qt::green));
            acmeSeries->setDecreasingColor(QColor(Qt::red));

            convey = new DataGrouping(data, grouping_coefficient);
            convey->compress_by_n_days();

            for (auto candle : convey->result) {
                const QDateTime dt = QDateTime::fromTime_t(candle.timestamp);
                const QString textdate = dt.toString(Qt::TextDate);
                categories << textdate;
                acmeSeries->append(candle.candlestickSet);
            }
            acmeSeries->setBodyOutlineVisible(true);
            chart = new QtCharts::QChart();
            chart->addSeries(acmeSeries);
            chart->setTitle(" ");
            chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);


            axis = new QtCharts::QBarCategoryAxis();
            axis->append(categories);
            chart->createDefaultAxes();

            auto theme = chart->ChartThemeBrownSand;
            chart->setTheme(theme);

            chart->setAxisX(axis, acmeSeries);

            //    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis;
            //    axisY->setTitleText(" ");
            //    chart->addAxis(axisY, Qt::AlignLeft);
            chart->legend()->setVisible(true);
            chart->legend()->setAlignment(Qt::AlignBottom);

            chartView = new QtCharts::QChartView(chart);
            chartView->setRubberBand(QtCharts::QChartView::HorizontalRubberBand);
            chartView->setRenderHint(QPainter::Antialiasing);
            chartView->setParent(ui->horizontalFrame);//прикрепляем
            chartView->show();

            convey->clear();
            need_to_change = false;
            is_loaded = true;
        } else chart_reload();
    } catch (const std::bad_alloc& e) {
        std::cerr << "chart_load() failed!\n";
    }
}


void chartwindow::chart_reload() {


    convey->fill(data, grouping_coefficient);
    convey->compress_by_n_days();

    categories.clear();
    axis->clear();
    acmeSeries->clear();
    chart->removeSeries(acmeSeries);

    for (auto candle : convey->result) {
        const QDateTime dt = QDateTime::fromTime_t(candle.timestamp);
        const QString textdate = dt.toString(Qt::TextDate);
        categories << textdate;
        acmeSeries->append(candle.candlestickSet);
    }

    //acmeSeries->setBodyOutlineVisible(true);
    chart->addSeries(acmeSeries);
    axis->append(categories);
    //axis->append(categories);
    chart->setAxisX(axis, acmeSeries);

    chartView->repaint();

    convey->clear();
    need_to_change = false;
}


void chartwindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Plus:
            chartView->chart()->zoomIn();
            break;
        case Qt::Key_Minus:
            chartView->chart()->zoomOut();
            break;
        case Qt::Key_D:
            chartView->chart()->scroll(10, 0);//здесь можно не константы, а интовую
            // или дабловую переменную на чуствительность
            break;
        case Qt::Key_A:
            chartView->chart()->scroll(-10, 0);
            break;
        case Qt::Key_W:
            chartView->chart()->scroll(0, 10);
            break;
        case Qt::Key_S:
            chartView->chart()->scroll(0, -10);
            break;
    }
}

