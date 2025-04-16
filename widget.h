#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QImage* image;
    short* m_pImageData;
    int windowing(int HU_value, int center, int width);
    void updateImage(char* imageData);
    void updateImage(short* imageData);
    int windowing_center;
    int windowing_width;


private slots:
    void load8BitFile();
    void load12BitFile();
    void onCenterSliderValueChanged(int value);
    void onWidthSliderValueChanged(int value);
};


#endif // WIDGET_H
