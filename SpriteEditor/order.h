#ifndef ORDER_H
#define ORDER_H

#include <QMainWindow>
#include <QVector>
#include "model.h"

namespace Ui {
class order;
}

class order : public QMainWindow
{
    Q_OBJECT

public:
    explicit order(Model& model, QWidget *parent = nullptr);
    ~order();

public slots:
    void takeButton(int);
    void resetClicked();
    void confirmClicked();
signals:
    void swapFrames(int, int);
private:
    Ui::order *ui;
    Model& model;
    bool isLCDSet = false;
    bool isLCD2Set = false;
    int swapPostionOne;
    int swapPostionTwo;

};

#endif // ORDER_H
