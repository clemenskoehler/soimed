#include "widget.h"
#include "ui_widget.h"
#include "math.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

// // Erzeuge ein Objekt vom Typ QImage
// QImage image(512,512, QImage::Format_RGB32);

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , windowing_center(0)
    , windowing_width(800)
{
    ui->setupUi(this);
    connect(ui->pushButton_load8BitFile, SIGNAL(clicked()), this, SLOT(load8BitFile()));
    connect(ui->pushButton_load12BitFile, SIGNAL(clicked()), this, SLOT(load12BitFile()));


    connect(ui->horizontalSlider_center, &QSlider::valueChanged, this, &Widget::onCenterSliderValueChanged);
    connect(ui->horizontalSlider_width, &QSlider::valueChanged, this, &Widget::onWidthSliderValueChanged);


    // Speicher der Größe 512*512 reservieren
    m_pImageData = new short[512*512];

    image = new QImage(512,512,QImage::Format_RGB32);
    // Initialisiere das Bild mit schwarzem Hintergrund
    image->fill(qRgb(0,0,0));

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(*image));
    repaint();

    ui->horizontalSlider_center->setValue(windowing_center);
    ui->horizontalSlider_width->setValue(windowing_width);

}

Widget::~Widget()
{
    delete ui;
    delete[] m_pImageData;
    delete image;
}

void Widget::load8BitFile()
{
    char imageData[512 * 512];

    // Open file dialog
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image", "./", "Raw Image Files (*.raw)");

    QFile dataFile(imagePath);
    if (!dataFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Failed to open file");
        return;
    }

    int iFileSize = dataFile.size();
    if (iFileSize > sizeof(imageData)) {
        QMessageBox::warning(this, "Warning", "File is too large and cannot be loaded.");
        dataFile.close();
        return;
    }

    int iNumberBytesRead = dataFile.read(imageData, sizeof(imageData));
    if (iNumberBytesRead < 0) {
        QMessageBox::critical(this, "Error", "Error reading the file");
        dataFile.close();
        return;
    }

    dataFile.close();

    // Update the image with the windowing function (which now uses class variables center and width)
    updateImage(imageData);
}

void Widget::load12BitFile()
{
    short imageData[512 * 512];

    // Open file dialog
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image", "./", "Raw Image Files (*.raw)");

    QFile dataFile(imagePath);
    if (!dataFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Failed to open file");
        return;
    }

    int iFileSize = dataFile.size();
    if (iFileSize > sizeof(imageData)) {
        QMessageBox::warning(this, "Warning", "File is too large and cannot be loaded.");
        dataFile.close();
        return;
    }

    int iNumberBytesRead = dataFile.read((char*)imageData, sizeof(imageData) * sizeof(short));
    if (iNumberBytesRead < 0) {
        QMessageBox::critical(this, "Error", "Error reading the file");
        dataFile.close();
        return;
    }

    dataFile.close();

    // Update the image with the windowing function (which now uses class variables center and width)
    updateImage(imageData);
}

void Widget::updateImage(char* imageData)
{
    // Update the image with the windowing function (which now uses class variables center and width)
    for (int y = 0; y < 512; ++y) {
        for (int x = 0; x < 512; ++x) {
            int index = y * 512 + x;
            if(index < sizeof(imageData)) {
                int grayValue = windowing(imageData[index], windowing_center, windowing_width);
                image->setPixel(x, y, qRgb(grayValue, grayValue, grayValue));
            }
        }
    }

    // Update the label to show the new image
    ui->label_image->setPixmap(QPixmap::fromImage(*image));
    repaint();
}

void Widget::updateImage(short* imageData)
{
    // Update the image with the windowing function (which now uses class variables center and width)
    for (int y = 0; y < 512; ++y) {
        for (int x = 0; x < 512; ++x) {
            int index = y * 512 + x;

            if(index < sizeof(imageData)) {
                int grayValue = windowing(imageData[index], windowing_center, windowing_width);
                image->setPixel(x, y, qRgb(grayValue, grayValue, grayValue));
            }
        }
    }

    // Update the label to show the new image
    ui->label_image->setPixmap(QPixmap::fromImage(*image));
    repaint();
}

int Widget::windowing(int HU_value, int center, int width)
{
    int grayValue;
    int start = center - (width / 2);
    int end = center + (width / 2);

    if (HU_value < start) {
        return 0;
    }
    if (HU_value > end) {
        return 255;
    }

    grayValue = static_cast<int>((HU_value - start) * 255.0 / width);
    return grayValue;
}

void Widget::onCenterSliderValueChanged(int value)
{
    windowing_center = value;
    updateImage(m_pImageData);  // Update the image with the new center value
}

void Widget::onWidthSliderValueChanged(int value)
{
    windowing_width = value;
    updateImage(m_pImageData);  // Update the image with the new width value
}
