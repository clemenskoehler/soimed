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
    // Erzeuge ein Objekt vom Typ QImage
    QImage image(512,512, QImage::Format_RGB32);

    image.fill(qRgb(0,0,0));

    // Schreibe einen roten Pixel
    image.setPixel(50, 100, qRgb(255, 0, 0));

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(image));
    repaint();
}
