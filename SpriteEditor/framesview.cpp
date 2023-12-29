#include "framesview.h"
#include "ui_framesview.h"
#include "swapframes.h"
#include "insertframes.h"
#include <QString>

FramesView::FramesView(QWidget *parent):
    QWidget(parent),
    ui(new Ui::FramesView)
{
    previousNumOfFrames=1;

    buttonName = "";

    SIZE = QSize(90,90);

    /// Seting up the FramesView UI
    ui->setupUi(this);

    ui->framesView->setWidgetResizable(true);

    /// Connect methods for all buttons
    connect(ui->addFrameBtn, &QPushButton::clicked, this, &FramesView::addFrameButtonClicked);
    connect(ui->deleteFrameBtn, &QPushButton::clicked, this, &FramesView::deleteAFrameDisplay);
    connect(ui->swapBtn, &QPushButton::clicked, this, &FramesView::openOrderFramesWindow);
    connect(ui->duplicateBtn, &QPushButton::clicked, this, &FramesView::duplicateFrame);
    connect(ui->insertBtn, &QPushButton::clicked, this, &FramesView::openInsertFramesWindow);
    connect(ui->clearBtn, &QPushButton::clicked, this, &FramesView::clearFramesAction);

    /// Setting up the object to swap two frames and its connections
    orderFrames = new SwapFrames();
    connect(orderFrames, &SwapFrames::swapFrames, this, &FramesView::swapFrames);
    connect(orderFrames, &SwapFrames::restoreButtons, this, &FramesView::setEnabledButtons);
    connect(orderFrames, &SwapFrames::cancelled, this, &FramesView::cancelledSwap);
    connect(this, &FramesView::sendSwapNumber, orderFrames, &SwapFrames::setButtonNumbers);

    /// Setting up the object to change a Frame's position in the FramesView UI and its connections
    insertFrames = new InsertFrames();
    connect(this, &FramesView::sendInsertNumber, insertFrames, &InsertFrames::setInsertButton);
    connect(insertFrames, &InsertFrames::restoreButtons, this, &FramesView::setEnabledButtons);
    connect(insertFrames, &InsertFrames::cancelled, this, &FramesView::cancelledInsert);
    connect(insertFrames, &InsertFrames::insertFrame, this, &FramesView::insertFrame);
}

void FramesView::duplicateFrame()
{
    /// If the user hasn't selected a Frame to duplicate, it will duplicate the latest Frame.
    if (buttonName=="")
    {
        emit duplicateFrameAt(buttons.size()-1);
    }
    else
    {
        emit duplicateFrameAt(buttonName.toInt());
    }
}

void FramesView::clearFramesAction()
{
    previousNumOfFrames = 1;

    buttonName = "";

    emit clearFrames();
}

void FramesView::enableStartButtons()
{
    ui->addFrameBtn->setEnabled(true);

    ui->swapBtn->setEnabled(true);

    ui->duplicateBtn->setEnabled(true);

    ui->insertBtn->setEnabled(true);

    ui->clearBtn->setEnabled(true);
}

void FramesView::setEnabledButtons(bool enable)
{
    ui->addFrameBtn->setEnabled(enable);

    ui->deleteFrameBtn->setEnabled(enable);

    ui->swapBtn->setEnabled(enable);

    ui->duplicateBtn->setEnabled(enable);

    ui->insertBtn->setEnabled(enable);

    ui->clearBtn->setEnabled(enable);

    emit setEnabledCanvas(enable);
}

void FramesView::addFrameButtonClicked()
{
    emit addAFrame();
}

void FramesView::deleteAFrameDisplay()
{
    /// If a user hasn't selected a Frame to delete, it will delete the latest one
    if (buttonName == "")
    {
        int location = buttons.size() - 1;

        emit deleteFrameAt(location);
    }
    else
    {
        int buttonPos = buttonName.toInt();

        emit deleteFrameAt(buttonPos);
    }
}

FramesView::~FramesView()
{
    for (QPushButton * button : buttons)
    {
        button->deleteLater();
    }
    delete ui;
}

void FramesView::receiveCanvasUpdate(QVector<Frame*> frames, int frameId)
{
    /// If the canvas is active and ready to use, the buttons are enabled
    setEnabledButtons(true);

    ui->deleteFrameBtn->setEnabled(frames.size() > 1);

    /// When a new Frame is added
    if (buttons.size() < (frames).size())
    {
        while(buttons.size() != frames.size())
        {
            int id = buttons.size();

            /// if the new item added was a duplicate from middle of frames
            if (findIndex(frames, frameId)!=buttons.size())
            {
                addDuplicate(frames, findIndex(frames, frameId));

                return;
            }
            /// If not, create a new button holder for the Frame
            QPushButton * temp = new QPushButton();

            temp->setObjectName(QString::number(id));

            temp->setIconSize(SIZE);

            temp->setFixedSize(SIZE);

            buttons.push_back(temp);

            connect(temp, &QPushButton::clicked, this, &FramesView::buttonClicked);

            ui->hlayout->addWidget(temp);

            updateButtonBackground(temp, frames[id]);
        }
    }
    /// If a Frame has been deleted
    else if (buttons.size() > (frames).size())
    {
        while (buttons.size() != frames.size())
        {
            QPushButton * former = nullptr;

            /// If no Frame was selected to be deleted, the last frame will be deleted
            if (buttonName == "")
            {
                int location = buttons.size() - 1;

                former = buttons[location ];

                buttons.removeLast();
            }
            else
            {
                int indexOfButton = buttonName.toInt();

                buttons[indexOfButton]->disconnect();

                former = buttons[indexOfButton];

                buttons.remove(indexOfButton);

                for (int i = indexOfButton; i < buttons.size(); i++)
                {
                    buttons[i]->setObjectName(QString::number(i));
                }
                buttonName = "";
            }
            former->deleteLater();
        }
    }
    /// If no frame has been deleted, update the selected Frame
    else
    {
        int buttonIndex = findIndex(frames, frameId);

        QPushButton* updateButton = (buttons)[buttonIndex];

        Frame * currentFrame = (frames)[buttonIndex];

        updateButtonBackground(updateButton, currentFrame);
    }
    previousNumOfFrames = frames.size();
}

void FramesView::addDuplicate(QVector<Frame*> frames, int newButtonPosition)
{
    QPushButton * temp = new QPushButton();

    temp->setObjectName(QString::number(newButtonPosition));

    temp->setIconSize(SIZE);

    temp->setFixedSize(SIZE);

    /// Adds this new duplicated Frame next to the Frame it was duplicated Frame and shifts
    /// all the other Frames after it
    for (int i = newButtonPosition; i < buttons.size(); i++)
    {
        buttons[i]->setObjectName(QString::number(i+1));
    }
    buttons.insert(newButtonPosition, 1, temp);

    updateButtonBackground(temp, frames[newButtonPosition+1]);

    connect(temp, &QPushButton::clicked, this, &FramesView::buttonClicked);

    ui->hlayout->insertWidget(newButtonPosition, temp);
}

int FramesView::findIndex(QVector<Frame*> frames, int currentFrame)
{
    int index = 0;

    for (Frame * frame: frames)
    {
        if (frame->getFrameId() == currentFrame)
        {
            return index;
        }
        index ++;
    }
    //shouldn't get to this point
    return 0;
}

void FramesView::updateButtonBackground(QPushButton * button, Frame * frame)
{
    QImage image = frame->getImage();

    image = image.scaled(SIZE);

    QIcon icon(QPixmap::fromImage(image));

    button->setIcon(icon);

    button->setIconSize(SIZE);

    button->setFixedSize(SIZE);
}

void FramesView::buttonClicked()
{
    QObject *senderObj = sender();

    if (orderFrames->isVisible())
    {
        swapAction(senderObj);
    }
    else if (insertFrames->isVisible())
    {
        insertAction(senderObj);
    }
    else
    {
        selectButtonAction(senderObj);
    }
}

void FramesView::selectButtonAction(QObject* senderObj)
{
    QString senderObjName = senderObj->objectName();

    buttonName = senderObjName;

    int buttonIndex = buttonName.toInt();

    emit newFrame(buttonIndex);
}

void FramesView::swapAction(QObject* senderObj)
{
    emit sendSwapNumber(senderObj->objectName().toInt());
}

void FramesView::insertAction(QObject* senderObj)
{
    emit sendInsertNumber(senderObj->objectName().toInt(), buttons.size());
}

void FramesView::openOrderFramesWindow()
{
    orderFrames->setGeometry(1200, 650, orderFrames->width(), orderFrames->height());

    orderFrames->show();

    setEnabledButtons(false);
}

void FramesView::openInsertFramesWindow()
{
    insertFrames->setGeometry(1200, 650, orderFrames->width(), orderFrames->height());

    insertFrames->show();

    setEnabledButtons(false);
}

void FramesView::insertFrame(int oldFrameIndex, int newFrameIndex)
{
    ui->deleteFrameBtn->setEnabled(previousNumOfFrames - 1);

    emit changeOrdering(oldFrameIndex, newFrameIndex);
}

void FramesView::swapFrames(int swapPostion1, int swapPostion2)
{
    ui->deleteFrameBtn->setEnabled(previousNumOfFrames - 1);

    emit swapFramePositions(swapPostion1, swapPostion2);
}

void FramesView::cancelledSwap()
{
    setEnabledButtons(true);

    ui->deleteFrameBtn->setEnabled(previousNumOfFrames - 1);
}

void FramesView::cancelledInsert()
{
    setEnabledButtons(true);

    ui->deleteFrameBtn->setEnabled(previousNumOfFrames - 1);
}
