#include "order.h"
#include "ui_order.h"

order::order(FramesView* _framesView, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::order)
{
    framesView = _framesView;
    ui->setupUi(this);
    connect(ui->resetButton, &QPushButton::clicked, this, &order::resetClicked);
    connect(ui->confirmButton, &QPushButton::clicked, this, &order::confirmClicked);
    connect(this, &order::swapFrames, framesView, &FramesView::swapFrames);
}

order::~order()
{
    delete ui;
}

void order::takeButton(int buttonPositon)
{
    if(!isLCDSet)
    {
        ui->LCD->display(buttonPositon);
        swapPostionOne = buttonPositon;
        isLCDSet = true;
        return;
    }
    if(!isLCD2Set)
    {
        ui->LCD2->display(buttonPositon);
        swapPostionTwo = buttonPositon;
        isLCD2Set = true;
        return;
    }
}

void order::resetClicked()
{
    ui->LCD->display(0);
    ui->LCD2->display(0);
    isLCDSet = false;
    isLCD2Set = false;
}

void order::confirmClicked()
{
    emit swapFrames(swapPostionOne, swapPostionTwo);
}
