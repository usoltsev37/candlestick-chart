#include "themewindow.h"
#include "ui_themewindow.h"
#include "styleset.h"
#include <QTime>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QPalette>
#include <QStandardItemModel>
#include <QToolButton>
#include <QMouseEvent>

//полно плохого копипаста, но доработка планируется на лучшие времена
themewindow::themewindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::themewindow),
        m_leftMouseButtonPressed(None) {

    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);//минус рамка
    this->setStyleSheet(styleset::getWindowStyleSheet());
    this->setMouseTracking(true);//отслеживаем курсор

    ui->label->setText("theme change");
    ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    ui->max_btn->setStyleSheet(styleset::getMaximizeStyleSheet());
    ui->cls_btn->setStyleSheet(styleset::getCloseStyleSheet());
    ui->min_btn->setStyleSheet(styleset::getMinimizeStyleSheet());
    ui->pushButton_2->setStyleSheet(styleset::getPlayStyleSheet());
    ui->Test_button->setStyleSheet(styleset::getMenuStyleSheet());
    ui->pushButton_2->setCursor(Qt::PointingHandCursor);
    ui->Test_button->setCursor(Qt::PointingHandCursor);

    connect(ui->min_btn, &QToolButton::clicked, this, &QWidget::showMinimized);
    connect(ui->max_btn, &QToolButton::clicked, [this]() {
        if (this->isMaximized()) {
            ui->max_btn->setStyleSheet(styleset::getMaximizeStyleSheet());
            this->layout()->setMargin(9);
            this->showNormal();
        } else {
            ui->max_btn->setStyleSheet(styleset::getRestoreStyleSheet());
            this->layout()->setMargin(0);
            this->showMaximized();
        }
    });

    connect(ui->cls_btn, SIGNAL(clicked()), this, SLOT(on_buttonclick()));
    connect(ui->Test_button, SIGNAL(clicked()), this, SLOT(on_Test_button_clicked));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_pushButton_2_clicked));

}

themewindow::~themewindow() {
    delete ui;
}


QPoint themewindow::previousPosition() const {
    return m_previousPosition;
}

void themewindow::setPreviousPosition(QPoint previousPosition) {
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);

}


void themewindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // Определяем, в какой области произошёл клик
        m_leftMouseButtonPressed = checkResizableField(event);
        setPreviousPosition(event->pos()); // и устанавливаем позицию клика
    }
    return QWidget::mousePressEvent(event);
}

void themewindow::mouseReleaseEvent(QMouseEvent *event) {
    //отпускаем лкм сбрасываем клик
    if (event->button() == Qt::LeftButton) {
        m_leftMouseButtonPressed = None;
    }
    return QWidget::mouseReleaseEvent(event);
}


void themewindow::mouseMoveEvent(QMouseEvent *event) {
    // При перемещении мыши, проверяем статус нажатия левой кнопки мыши
    switch (m_leftMouseButtonPressed) {
        case Move: {
            // При этом проверяем, не максимизировано ли окно
            if (isMaximized()) {
                /* При перемещении из максимизированного состояния
                Необходимо вернуть окно в нормальное состояние и установить стили кнопки
                А также путём нехитрых вычислений пересчитать позицию окна,
                чтобы оно оказалось под курсором*/
                ui->max_btn->setStyleSheet(styleset::getMaximizeStyleSheet());
                this->layout()->setMargin(9);
                auto part = event->screenPos().x() / width();
                this->showNormal();
                auto offsetX = width() * part;
                setGeometry(event->screenPos().x() - offsetX, 0, width(), height());
                setPreviousPosition(QPoint(offsetX, event->y()));
            } else {
                // Если окно не максимизировано, то просто перемещаем его относительно
                // последней запомненной позиции, пока не отпустим кнопку мыши
                auto dx = event->x() - m_previousPosition.x();
                auto dy = event->y() - m_previousPosition.y();
                setGeometry(x() + dx, y() + dy, width(), height());
            }
            break;
        }
        case Top: {
            // Для изменения размеров также проверяем на максимизацию
            // поскольку мы же не можем изменить размеры у максимизированного окна
            if (!isMaximized()) {
                auto dy = event->y() - m_previousPosition.y();
                setGeometry(x(), y() + dy, width(), height() - dy);
            }
            break;
        }
        case Bottom: {
            if (!isMaximized()) {
                auto dy = event->y() - m_previousPosition.y();
                setGeometry(x(), y(), width(), height() + dy);
                setPreviousPosition(event->pos());
            }
            break;
        }
        case Left: {
            if (!isMaximized()) {
                auto dx = event->x() - m_previousPosition.x();
                setGeometry(x() + dx, y(), width() - dx, height());
            }
            break;
        }
        case Right: {
            if (!isMaximized()) {
                auto dx = event->x() - m_previousPosition.x();
                setGeometry(x(), y(), width() + dx, height());
                setPreviousPosition(event->pos());
            }
            break;
        }
        default:
            // Если курсор перемещается по окну без зажатой кнопки,
            // то просто отслеживаем в какой области он находится
            // и изменяем его курсор
            checkResizableField(event);
            break;
    }
    return QWidget::mouseMoveEvent(event);
}

themewindow::MouseType themewindow::checkResizableField(QMouseEvent *event) {
    QPointF position = event->screenPos();  // Определяем позицию курсора на экране
    qreal x = this->x();                    // координаты окна приложения, ...
    qreal y = this->y();                    // ... то есть координату левого верхнего угла окна
    qreal width = this->width();            // А также ширину ...
    qreal height = this->height();          // ... и высоту окна

    // Определяем области, в которых может находиться курсор мыши
    // По ним будет определён статус клика
    QRectF rectTop(x + 9, y, width - 18, 7);
    QRectF rectBottom(x + 9, y + height - 7, width - 18, 7);
    QRectF rectLeft(x, y + 9, 7, height - 18);
    QRectF rectRight(x + width - 7, y + 9, 7, height - 18);
    QRectF rectInterface(x + 9, y + 9, width - 18, height - 18);

    // И в зависимости от области, в которой находится курсор
    // устанавливаем внешний вид курсора и возвращаем его статус
    if (rectTop.contains(position)) {
        setCursor(Qt::SizeVerCursor);
        return Top;
    } else if (rectBottom.contains(position)) {
        setCursor(Qt::SizeVerCursor);
        return Bottom;
    } else if (rectLeft.contains(position)) {
        setCursor(Qt::SizeHorCursor);
        return Left;
    } else if (rectRight.contains(position)) {
        setCursor(Qt::SizeHorCursor);
        return Right;
    } else if (rectInterface.contains(position)) {
        setCursor(QCursor());
        return Move;
    } else {
        setCursor(QCursor());
        return None;
    }
}

void themewindow::on_buttonclick() {
    themewindow::close();

}


void themewindow::on_Test_button_clicked() {
    QPalette pal_1(palette());
    if (ui->comboBox->currentText() == "Default") {
        pal_1.setColor(QPalette::Window, QColor(53, 53, 53));
        pal_1.setColor(QPalette::WindowText, Qt::white);
        pal_1.setColor(QPalette::Base, QColor(25, 25, 25));
        pal_1.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        pal_1.setColor(QPalette::ToolTipBase, Qt::white);
        pal_1.setColor(QPalette::ToolTipText, Qt::white);
        pal_1.setColor(QPalette::Text, Qt::white);
        pal_1.setColor(QPalette::Button, QColor(53, 53, 53));
        pal_1.setColor(QPalette::ButtonText, Qt::white);
        pal_1.setColor(QPalette::BrightText, Qt::red);
        pal_1.setColor(QPalette::Link, QColor(42, 130, 218));
        pal_1.setColor(QPalette::Highlight, QColor(42, 130, 218));
        pal_1.setColor(QPalette::HighlightedText, Qt::black);
        this->setPalette(pal_1);
        ui->Test_button->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 87, 59);"));
    } else if (ui->comboBox->currentText() == "Dark") {
        this->setPalette(style()->standardPalette());
        ui->Test_button->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 190, 132);"));
    } else if (ui->comboBox->currentText() == "Red") {
        this->setAutoFillBackground(true);
        pal_1.setColor(QPalette::Background, Qt::red);
        this->setPalette(pal_1);
        ui->Test_button->setStyleSheet(QString::fromUtf8("background-color: rgb(220, 250, 150);"));
    }
}

QString themewindow::get_theme_name() {
    return current_theme;
}

void themewindow::on_pushButton_2_clicked() {
    QString wanted = ui->comboBox->currentText();
    current_theme = wanted;
}
