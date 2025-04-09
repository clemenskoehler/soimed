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

private slots:
    void MalePixel();
    void MaleLinieH();
    void MaleLinieV();
    void MaleKreuz();
    void MaleSchraeg();
    void FillRot();
    void MaleQuadrat();
    void MaleFarbverlauf();
    void MaleKreis();
    void MaleSmiley();
};


#endif // WIDGET_H
