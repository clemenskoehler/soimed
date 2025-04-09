#include "widget.h"
#include "ui_widget.h"
#include "math.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton_pixel, SIGNAL(clicked()), this, SLOT(MalePixel()));
    connect(ui->pushButton_lineH, SIGNAL(clicked()), this, SLOT(MaleLinieH()));
    connect(ui->pushButton_lineV, SIGNAL(clicked()), this, SLOT(MaleLinieV()));
    connect(ui->pushButton_cross, SIGNAL(clicked()), this, SLOT(MaleKreuz()));
    connect(ui->pushButton_linefunction, SIGNAL(clicked()), this, SLOT(MaleSchraeg()));
    connect(ui->pushButton_fillRed, SIGNAL(clicked()), this, SLOT(FillRot()));
    connect(ui->pushButton_squareRim, SIGNAL(clicked()), this, SLOT(MaleQuadrat()));
    connect(ui->pushButton_farbverlauf, SIGNAL(clicked()), this, SLOT(MaleFarbverlauf()));
    connect(ui->pushButton_kreis, SIGNAL(clicked()), this, SLOT(MaleKreis()));
    connect(ui->pushButton_smiley, SIGNAL(clicked()), this, SLOT(MaleSmiley()));
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

void Widget::MaleLinieH()
{
    // Erzeuge ein Objekt vom Typ QImage
    QImage image(512,512, QImage::Format_RGB32);

    image.fill(qRgb(0,0,0));

    for (int x = 0; x < 300; ++x) {
        image.setPixel(x, 100, qRgb(255, 0, 0));
    }

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(image));
    repaint();
}

void Widget::MaleLinieV()
{
    // Erzeuge ein Objekt vom Typ QImage
    QImage image(512,512, QImage::Format_RGB32);

    image.fill(qRgb(0,0,0));

    for (int y = 0; y < 300; ++y) {
        image.setPixel(256, y, qRgb(255, 0, 0));
    }

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(image));
    repaint();
}

void Widget::MaleKreuz()
{
    // Erzeuge ein Objekt vom Typ QImage
    QImage image(512,512, QImage::Format_RGB32);

    image.fill(qRgb(0,0,0));

    // Schreibe einen roten Pixel
    for (int x = 256-30; x < 256+30; ++x) {
        image.setPixel(x, 256, qRgb(255, 0, 0));
    }
    for (int y = 256-30; y < 256+30; ++y) {
        image.setPixel(256, y, qRgb(255, 0, 0));
    }

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(image));
    repaint();
}

void Widget::MaleSchraeg()
{
    // Erzeuge ein Objekt vom Typ QImage
    QImage image(512,512, QImage::Format_RGB32);

    image.fill(qRgb(0,0,0));

    // Schreibe einen roten Pixel
    for (int x = 0; x < 512; ++x) {
            image.setPixel(x, 2*x, qRgb(100, 100, 255));
    }

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(image));
    repaint();
}

void Widget::FillRot()
{
    // Erzeuge ein Objekt vom Typ QImage
    QImage image(512,512, QImage::Format_RGB32);

    image.fill(qRgb(255,50,50));

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(image));
    repaint();
}

void Widget::MaleQuadrat()
{
    // Erzeuge ein Objekt vom Typ QImage
    QImage image(512,512, QImage::Format_RGB32);

    image.fill(qRgb(0,0,0));

    // Male den Rand
    for (int x = 256-52; x < 256+52; ++x) {
        for (int y = 256-52; y < 256+52; ++y) {
            image.setPixel(x, y, qRgb(100, 255, 100));
        }
    }

    // Fuelle die Flaeche
    for (int x = 256-50; x < 256+50; ++x) {
        for (int y = 256-50; y < 256+50; ++y) {
            image.setPixel(x, y, qRgb(255, 0, 0));
        }
    }


    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(image));
    repaint();
}

void Widget::MaleFarbverlauf()
{
    // Erzeuge ein Objekt vom Typ QImage
    QImage image(512,512, QImage::Format_RGB32);

    image.fill(qRgb(0,0,0));

    // Schreibe einen Farbverlauf
    for (int x = 0; x < 512; ++x) {
        for (int y = 0; y < 512; ++y) {
            image.setPixel(x, y, qRgb(x % 255, y % 255, (x*y)%255));
        }
    }

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(image));
    repaint();
}

void Widget::MaleKreis()
{
    // Erzeuge ein Objekt vom Typ QImage
    QImage image(512,512, QImage::Format_RGB32);

    image.fill(qRgb(0,0,0));

    // Schreibe einen roten Pixel
    for (int x = 0; x < 512; ++x) {
        for (int y = 0; y < 512; ++y) {
            if((pow(x-256,2.0) + pow(y-256,2.0)) < pow(100,2.0)) {
                image.setPixel(x, y, qRgb(255, 255, 0));
            }
        }
    }

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(image));
    repaint();
}

void Widget::MaleSmiley()
{
    // Erzeuge ein Objekt vom Typ QImage
    QImage image(512,512, QImage::Format_RGB32);

    image.fill(qRgb(0,0,0));

    // Rechtes Auge
    for (int x = 0; x < 512; ++x) {
        for (int y = 0; y < 512; ++y) {
            if((pow(x-256+100,2.0) + pow(y-256,2.0)) < pow(30,2.0)) {
                image.setPixel(x, y, qRgb(255, 255, 0));
            }
        }
    }

    // linkes Auge
    for (int x = 0; x < 512; ++x) {
        for (int y = 0; y < 512; ++y) {
            if((pow(x-256-100,2.0) + pow(y-256,2.0)) < pow(30,2.0)) {
                image.setPixel(x, y, qRgb(0, 255, 0));
            }
        }
    }

    // Mund
    for (int x = 0; x < 512; ++x) {
        for (int y = 0; y < 512; ++y) {
            if((pow(x-256,2.0) + pow(y-256+100,2.0)) < pow(30,2.0)) {
                image.setPixel(x, y, qRgb(0, y, 0));
            }
        }
    }

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(image));
    repaint();
}

