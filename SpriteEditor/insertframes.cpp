#include "insertframes.h"
#include "ui_insertframes.h"
#include <QString>

InsertFrames::InsertFrames(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InsertFrames)
{
    ui->setupUi(this);
    connect(ui->resetButton, &QPushButton::clicked, this, &InsertFrames::reset);
    connect(ui->confirmButton, &QPushButton::clicked, this, &InsertFrames::confirm);
    connect(ui->cancelButton, &QPushButton::clicked, this, &InsertFrames::cancel);

}

InsertFrames::~InsertFrames()
{
    delete ui;
}

void InsertFrames::setInsertButton(int buttonNumber, int sizeOfFrames)
{
    if(!isLCDSet)
    {
        ui->LCDDisplay->display(buttonNumber);
        framePosition = buttonNumber;
        isLCDSet = true;
        return;
    }

    if(!isLCD2Set)
    {
        ui->LCDDisplay2->display(buttonNumber);
        insertPosition = buttonNumber;
        isLCD2Set = true;
        ui->confirmButton->setEnabled(true);
    }
}

void InsertFrames::getInput(int index)
{
    insertPosition = index;
    ui->confirmButton->setEnabled(true);
}

void InsertFrames::reset()
{
    ui->LCDDisplay->display(RESET_NUMBER);
    framePosition = RESET_NUMBER;
    isLCDSet = false;

    ui->LCDDisplay2->display(RESET_NUMBER);
    insertPosition = RESET_NUMBER;
    isLCD2Set = false;

    ui->confirmButton->setEnabled(false);
}

void InsertFrames::confirm()
{
    this->close();
    emit restoreButtons(true);
    emit insertFrame(framePosition, insertPosition);
    reset();
}

void InsertFrames::cancel()
{
    this->close();
    emit cancelled();
    reset();
}
