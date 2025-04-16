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
{
    ui->setupUi(this);
    connect(ui->pushButton_load8BitFile, SIGNAL(clicked()), this, SLOT(load8BitFile()));
    connect(ui->pushButton_load12BitFile, SIGNAL(clicked()), this, SLOT(load12BitFile()));

    // Speicher der Größe 512*512 reservieren
    m_pImageData = new short[512*512];

    image = new QImage(512,512,QImage::Format_RGB32);
    // Initialisiere das Bild mit schwarzem Hintergrund
    image->fill(qRgb(0,0,0));

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(*image));
    repaint();

}

Widget::~Widget()
{
    delete ui;
    delete[] m_pImageData;
    delete image;
}

void Widget::load8BitFile()
{
    // Speicher (Array) für Bilddaten reservieren/bereitstellen
    char imageData[512*512];

    // Datei auswählen
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image", "./", "Raw Image Files (*.raw)");

    // Datei öffnen
    QFile dataFile(imagePath);
    bool bFileOpen = dataFile.open(QIODevice::ReadOnly);

    if (!bFileOpen) {
        QMessageBox::critical(this, "ACHTUNG", "Datei konnte nicht geöffnet werden");
        return;
    }

    // Überprüfen der Dateigröße
    int iFileSize = dataFile.size();

    if (iFileSize > sizeof(imageData)) {
        QMessageBox::warning(this, "Warnung", "Die Datei ist zu groß und kann nicht geladen werden.");
        dataFile.close();
        return;
    }

    // Bilddaten in Array einlesen und Anzahl der gelesenen Bytes speichern
    int iNumberBytesRead = dataFile.read(imageData, sizeof(imageData));

    if (iNumberBytesRead < 0) {
        QMessageBox::critical(this, "Fehler", "Fehler beim Lesen der Datei");
        dataFile.close();
        return;
    }

    // Datei schließen
    dataFile.close();

    // Setze Inhalt des Arrays Pixel für Pixel in das Bild
    for (int y = 0; y < 512; ++y) {
        for (int x = 0; x < 512; ++x) {
            int index = y * 512 + x;

            if (index < iNumberBytesRead) {
                int grayValue = imageData[index];
                image->setPixel(x, y, qRgb(grayValue, grayValue, grayValue));
            }
        }
    }

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(*image));
    repaint();
}

void Widget::load12BitFile()
{
    // Datei auswählen
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image", "./", "Raw Image Files (*.raw)");

    // Datei öffnen
    QFile dataFile(imagePath);
    bool bFileOpen = dataFile.open(QIODevice::ReadOnly);

    if (!bFileOpen) {
        QMessageBox::critical(this, "ACHTUNG", "Datei konnte nicht geöffnet werden");
        return;
    }

    // Überprüfen der Dateigröße
    int iFileSize = dataFile.size();

    if (iFileSize > 512*512*sizeof(short)) {
        QMessageBox::warning(this, "Warnung", "Die Datei ist zu groß und kann nicht geladen werden.");
        dataFile.close();
        return;
    }

    // Bilddaten in Array einlesen und Anzahl der gelesenen Bytes speichern
    int iNumberBytesRead = dataFile.read((char*)m_pImageData, 512*512*sizeof(short));

    if (iNumberBytesRead < 0) {
        QMessageBox::critical(this, "Fehler", "Fehler beim Lesen der Datei");
        dataFile.close();
        return;
    }

    // Datei schließen
    dataFile.close();

    // Setze Inhalt des Arrays Pixel für Pixel in das Bild
    for (int y = 0; y < 512; ++y) {
        for (int x = 0; x < 512; ++x) {
            int index = y * 512 + x;

            if (index < iNumberBytesRead) {
                int grayValue = windowing(m_pImageData[index], 0, 800);
                image->setPixel(x, y, qRgb(grayValue, grayValue, grayValue));
            }
        }
    }

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(*image));
    repaint();
}

int Widget::windowing(int HU_value, int center, int width) {
    int grayValue;

    int start = center-(width/2);
    int end = center+(width/2);

    // Fensterung berechnen
    if(HU_value < start) {
        return 0;
    }

    if(HU_value > end) {
        return 255;
    }

    grayValue = static_cast<int>((HU_value - start) * 255.0 / width);



    return grayValue;
}
