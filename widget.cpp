#include "widget.h"
#include "ui_widget.h"
#include "math.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSignalBlocker>

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


    connect(ui->center_horizontalSlider, &QSlider::valueChanged, this, &Widget::onCenterValueChanged);
    connect(ui->center_spinBox, &QSpinBox::valueChanged, this, &Widget::onCenterValueChanged);
    connect(ui->width_dial, &QDial::valueChanged, this, &Widget::onWidthValueChanged);
    connect(ui->width_spinBox, &QSpinBox::valueChanged, this, &Widget::onWidthValueChanged);


    // Speicher der Größe 512*512 reservieren
    m_pImageData = new short[512*512];

    image = new QImage(512,512,QImage::Format_RGB32);
    // Initialisiere das Bild mit schwarzem Hintergrund
    image->fill(qRgb(0,0,0));

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(*image));
    repaint();

    ui->center_horizontalSlider->setValue(windowing_center);
    ui->width_dial->setValue(windowing_width);

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
    // if (iFileSize > sizeof(imageData)) {
    //     QMessageBox::warning(this, "Warning", "File is too large and cannot be loaded.");
    //     dataFile.close();
    //     return;
    // }

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
    // Open file dialog
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image", "./", "Raw Image Files (*.raw)");

    QFile dataFile(imagePath);
    if (!dataFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Failed to open file");
        return;
    }

    int iFileSize = dataFile.size();
    if (iFileSize > 512*512*sizeof(short)) {
        QMessageBox::warning(this, "Warning", "File is too large and cannot be loaded.");
        dataFile.close();
        return;
    }

    int iNumberBytesRead = dataFile.read((char*)m_pImageData, 512 * 512 * sizeof(short));
    if (iNumberBytesRead < 0) {
        QMessageBox::critical(this, "Error", "Error reading the file");
        dataFile.close();
        return;
    }

    dataFile.close();

    // Update the image with the windowing function (which now uses class variables center and width)
    // updateImage(m_pImageData);
    updateImage(m_pImageData);
}

void Widget::updateImage(char* imageData)
{
    // Update the image with the windowing function (which now uses class variables center and width)
    for (int y = 0; y < 512; ++y) {
        for (int x = 0; x < 512; ++x) {
            int index = y * 512 + x;
            if(index < 512*512) {
                int grayValue = imageData[index];
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

            if(index < 512*512) {
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

void Widget::onCenterValueChanged(int value)
{
    setWindowingCenter(value);

    QSignalBlocker blockerSpinBox(ui->center_spinBox);
    ui->center_spinBox->setValue(value);

    QSignalBlocker blockerSlider(ui->center_horizontalSlider);
    ui->center_horizontalSlider->setValue(value);

    updateImage(m_pImageData);  // Update the image with the new center value
}

void Widget::onWidthValueChanged(int value)
{
    setWindowingWidth(value);

    QSignalBlocker blockerSpinBox(ui->width_spinBox);
    ui->width_spinBox->setValue(value);

    QSignalBlocker blockerSlider(ui->width_dial);
    ui->width_dial->setValue(value);

    updateImage(m_pImageData);  // Update the image with the new width value
}

void Widget::setWindowingCenter(int value) {
    windowing_center = value;
}

void Widget::setWindowingWidth(int value) {
    windowing_width = value;
}

