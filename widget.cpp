#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton_Pixel, SIGNAL(clicked()), this, SLOT(MalePixel()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::MalePixel()
{
    ui->pushButton_Pixel->setText("Funktioniert");
}
