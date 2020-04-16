
#include "ui_chartwindow.h"
#include "chartwindow.h"
#include <QStyleFactory>
#include "styleset.h"
#include <assert.h>

//пока так, потом раскидаю по методам в view

double  chartwindow::str_to_timestamp(std::string date){
    QDateTime tmp_date_time = QDateTime::fromString(QString::fromStdString(date), "yyyy-MM-dd hh:mm:ss");
    double timestamp = tmp_date_time.toTime_t();
    return  timestamp;
}

chartwindow::chartwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chartwindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Графики");
    setMinimumSize(800,600);
    setMaximumSize(800,600);

    QTimer* theme_timer = new QTimer();
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

chartwindow::~chartwindow()
{
    delete ui;
}

void chartwindow::fill(std::vector<std::string> bt, std::vector<std::string> et, std::vector<double> op, std::vector<double> cl, std::vector<double> hi, std::vector<double> lo){
    begin_time = bt;
    end_time = et;
    open = op;
    closse = cl;
    high = hi;
    low = lo;

    QtCharts::QCandlestickSeries *acmeSeries = new QtCharts::QCandlestickSeries();
    acmeSeries->setName("Свечи");
    acmeSeries->setIncreasingColor(QColor(Qt::green));
    acmeSeries->setDecreasingColor(QColor(Qt::red));


    //TODO заполнение
    QList<Candle> Candles; QStringList categories;
    for (size_t i = 15; i < 60; ++i){// ha-ha nice hardcode
        assert(!open.empty());
        Candle tmp(open[i], high[i], low[i], closse[i], str_to_timestamp(begin_time[i]));
        categories << QString::fromStdString(begin_time[i]);
        acmeSeries->append(tmp.candlestickSet);
    }

    acmeSeries->setBodyOutlineVisible(true);

    chart = new QtCharts::QChart();
    chart->addSeries(acmeSeries);
    chart->setTitle(" ");
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
    QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, acmeSeries);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QtCharts::QChartView(chart);
    chartView->setRubberBand(QtCharts::QChartView::HorizontalRubberBand);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFrame);//важно
}

void chartwindow::on_pushButton_clicked(){
    themewin->close();
    chartwindow::close();
}

void chartwindow::on_pushButton2_clicked(){
    themewin->show();
}

void chartwindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Plus:
        chartView->chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chartView->chart()->zoomOut();
        break;
    case Qt::Key_D:
        chartView->chart()->scroll(10,0);//здесь можно не константы, а интовую или дабловую переменную на чуствительность
        break;
    case Qt::Key_A:
        chartView->chart()->scroll(-10,0);
        break;
    case Qt::Key_W:
        chartView->chart()->scroll(0,10);
        break;
    case Qt::Key_S:
        chartView->chart()->scroll(0,-10);
        break;
    }
}

void chartwindow::theme_change(){
    QString inp = themewin->get_theme_name();
    QPalette pal_1(palette());
    if(inp == "Default"){
         this->setStyleSheet(styleset::getWindowStyleSheet());
         for(auto butt : buttons){
             butt->setStyleSheet("background-color: #454545; ");
         }
    }
    else if(inp == "Dark"){
        this->setAutoFillBackground(true);
        pal_1.setColor(QPalette::Window, Qt::black);
        this->setPalette(pal_1);
        for(auto butt : buttons){
            butt->setStyleSheet(styleset::getCloseStyleSheet());
        }
    }
    else if(inp == "Red"){
        this->setAutoFillBackground(true);
        pal_1.setColor(QPalette::Window, Qt::red);
        this->setPalette(pal_1);
        for(auto butt : buttons){
            butt->setStyleSheet(styleset::getCloseStyleSheet());
        }
    }
}
