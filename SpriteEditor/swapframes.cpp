#include "swapframes.h"
#include "ui_swapframes.h"

SwapFrames::SwapFrames(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SwapFrames)
{
    ui->setupUi(this);
    connect(ui->resetButton, &QPushButton::clicked, this, &SwapFrames::reset);
    connect(ui->confirmButton, &QPushButton::clicked, this, &SwapFrames::confirm);
    connect(ui->cancelButton, &QPushButton::clicked, this, &SwapFrames::cancel);
}

SwapFrames::~SwapFrames()
{
    delete ui;
}

void SwapFrames::setButtonNumbers(int buttonNumber)
{
    if(!isLCDSet)
    {
        ui->LCDDisplay1->display(buttonNumber);
        swapPostion1 = buttonNumber;
        isLCDSet = true;
        return;
    }

    if(!isLCD2Set)
    {
        ui->LCDDisplay2->display(buttonNumber);
        swapPostion2 = buttonNumber;
        isLCD2Set = true;
        ui->confirmButton->setEnabled(true);
        return;
    }
}

void SwapFrames::reset()
{
    ui->LCDDisplay1->display(RESET_NUMBER);
    swapPostion1 = RESET_NUMBER;
    isLCDSet = false;

    ui->LCDDisplay2->display(RESET_NUMBER);
    swapPostion2 = RESET_NUMBER;
    isLCD2Set = false;

    ui->confirmButton->setEnabled(false);
}

void SwapFrames::confirm()
{
    this->close();
    emit restoreButtons(true);
    emit swapFrames(swapPostion1, swapPostion2);
    reset();
}

void SwapFrames::cancel()
{
    this->close();
    emit cancelled();
    reset();
}
