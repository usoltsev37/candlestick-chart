#include "styleset.h"
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
double DataGrouping::str_to_timestamp(std::string date) {
    QDateTime tmp_date_time = QDateTime::fromString(QString::fromStdString(date),
                                                    "yyyy-MM-dd hh:mm:ss");
    double timestamp = tmp_date_time.toTime_t();
    return timestamp;
}

void DataGrouping::compress_by_n_days() {
    for (size_t i = 1; i < candle_vec.size();) {
        tmp_open = candle_vec[i - 1].open;
        double lowest = candle_vec[i - 1].low;
        double highest = candle_vec[i - 1].high;

        while (unix_time_cnt < 86400 * number_of_days) { //unix время в секундах, в дне 86400 секунд
            if (i >= candle_vec.size()) break;
            if (lowest > candle_vec[i].low)
                lowest = candle_vec[i].low;
            if (highest < candle_vec[i].high)
                highest = candle_vec[i].high;

            if (unix_time_cnt + str_to_timestamp(candle_vec[i].begin_time)
                - str_to_timestamp(candle_vec[i - 1].end_time) >= 86400 * number_of_days)
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


double chartwindow::str_to_timestamp(std::string date) {
    QDateTime tmp_date_time = QDateTime::fromString(QString::fromStdString(date),
                                                    "yyyy-MM-dd hh:mm:ss");
    double timestamp = tmp_date_time.toTime_t();
    return timestamp;
}

chartwindow::chartwindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::chartwindow) {
    ui->setupUi(this);
    this->setWindowTitle(tr("Charts"));
    setMinimumSize(800, 600);
    setMaximumSize(800, 600);

    QTimer *theme_timer = new QTimer();
    theme_timer->start(100);

    //добавляем в вектор кнопки(потом это понадобится)(убрать)
    buttons.push_back(ui->pushButton_0);
    buttons.push_back(ui->pushButton_2);

    connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));

    //всё это вынесется потом в отдельный метод
    themewin = new themewindow();
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_pushButton2_clicked()));
    connect(theme_timer, SIGNAL(timeout()), this, SLOT(theme_change()));
}

chartwindow::~chartwindow() {
    delete ui;
}

void chartwindow::fill(Model model) {

    QtCharts::QCandlestickSeries * acmeSeries = new QtCharts::QCandlestickSeries();
    acmeSeries->setName(tr("Candles"));
    acmeSeries->setIncreasingColor(QColor(Qt::green));
    acmeSeries->setDecreasingColor(QColor(Qt::red));

    const size_t size_of_data = model.get_size();
    ModelData tmp;
    for (std::size_t i = 0; i < size_of_data - 1; i++) {
        tmp.low = model.get_data_byIndex(i).low;
        tmp.high = model.get_data_byIndex(i).high;
        tmp.open = model.get_data_byIndex(i).open;
        tmp.close = model.get_data_byIndex(i).close;
        tmp.end_time = model.get_data_byIndex(i).end_time;
        tmp.begin_time = model.get_data_byIndex(i).begin_time;
        data.push_back(tmp);
    }

    DataGrouping *convey = new DataGrouping(data, 2);
    //2 -- параметр группировки, должен быть не константным
    convey->compress_by_n_days();
    QStringList categories;

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
    QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
    //axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, acmeSeries);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QtCharts::QChartView(chart);
    chartView->setRubberBand(QtCharts::QChartView::HorizontalRubberBand);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFrame);//важно
}

void chartwindow::on_pushButton_clicked() {
    themewin->close();
    chartwindow::close();
}

void chartwindow::on_pushButton2_clicked() {
    themewin->show();
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
            chartView->chart()->scroll(10,
                                       0);//здесь можно не константы, а интовую или дабловую переменную на чуствительность
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

void chartwindow::theme_change() {
    QString inp = themewin->get_theme_name();
    QPalette pal_1(palette());
    if (inp == "Default") {
        this->setStyleSheet(styleset::getWindowStyleSheet());
        for (auto butt : buttons) {
            butt->setStyleSheet("background-color: #454545; ");
        }
    } else if (inp == "Dark") {
        this->setAutoFillBackground(true);
        pal_1.setColor(QPalette::Window, Qt::black);
        this->setPalette(pal_1);
        for (auto butt : buttons) {
            butt->setStyleSheet(styleset::getCloseStyleSheet());
        }
    } else if (inp == "Red") {
        this->setAutoFillBackground(true);
        pal_1.setColor(QPalette::Window, Qt::red);
        this->setPalette(pal_1);
        for (auto butt : buttons) {
            butt->setStyleSheet(styleset::getCloseStyleSheet());
        }
    }
}
