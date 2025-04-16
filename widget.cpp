#include "widget.h"
#include "ui_widget.h"
#include "math.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton_openFile, SIGNAL(clicked()), this, SLOT(LoadFile()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::LoadFile()
{
    // Erzeuge ein Objekt vom Typ QImage
    QImage image(512,512, QImage::Format_RGB32);

    // Initialisiere das Bild mit schwarzem Hintergrund
    image.fill(qRgb(0,0,0));

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
                image.setPixel(x, y, qRgb(grayValue, grayValue, grayValue));
            }
        }
    }

    // Bild auf Benutzeroberflaeche anzeigen
    ui->label_image->setPixmap(QPixmap::fromImage(image));
    repaint();
}
