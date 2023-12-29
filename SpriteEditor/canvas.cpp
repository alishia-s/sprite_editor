#include "canvas.h"
#include "ui_canvas.h"
#include <QMouseEvent>
#include <QtMath>

Canvas::Canvas(QWidget *parent) :
    QWidget(parent),
    previousFrame(nullptr),
    shapeOverlay(QSize(-1,-1), 0),
    dragging(false),
    sizeSelected(false),
    selectedColor(Qt::black),
    overlayOn(true),
    ui(new Ui::Canvas)
{
    ui->setupUi(this);
    ui->frameSizeComboBox->addItem("8 x 8 px");
    ui->frameSizeComboBox->addItem("16 x 16 px");
    ui->frameSizeComboBox->addItem("32 x 32 px");

    ui->frameSizeComboBox->setStyleSheet("");
    ui->frameSizeButton->setStyleSheet("");
    ui->frameSizeLabel->setStyleSheet("");

    displayFrameSizeOptions();

}

void Canvas::displayFrameSizeOptions()
{
    ui->frameSizeComboBox->setVisible(true);
    ui->frameSizeButton->setVisible(true);
    ui->frameSizeLabel->setVisible(true);

    ui->grid->setVisible(false);
    ui->overlay->setVisible(true);
    ui->shapeOverlay->setVisible(true);

    connect(ui->frameSizeComboBox,
            &QComboBox::activated,
            this,
            &Canvas::receiveFrameSizeInput);

    connect(ui->frameSizeButton,
            &QPushButton::clicked,
            this,
            &Canvas::receiveFrameSize);
}

void Canvas::receiveNewSpriteProjectRequest()
{
    // set the current tool to the mouse so that user's can't draw on canvas
    receiveCurrentTool(Model::Tool::mouse);
    // clear the onion image
    if(previousFrame != nullptr)
    {
        previousFrame->setImage(currentFrame->getImage());
        displayOverlay();
    }

    // redisplay the option to pick frame size
    displayFrameSizeOptions();
}

void Canvas::setBrushSize(int size)
{
    brushSize = size;
}

void Canvas::receiveDuplicateFrames(QVector<Frame*> _frames, int framePos){
    frames = _frames;
    currentFrame = frames[framePos];
    displayCurrentFrame();
    emit canvasUpdated(frames, currentFrame->getFrameId());
}

void Canvas::receiveInsertFrames(QVector<Frame*> _frames, int newIndex){
    frames = _frames;
    currentFrame = frames[newIndex];
    displayCurrentFrame();

    if(previousFrame != nullptr)
    {
        previousFrame->setImage(currentFrame->getImage());
        displayOverlay();
    }
    for(int i = 0; i < frames.size(); i++)
    {
        emit canvasUpdated(frames, frames[i]->getFrameId());
    }
}

void Canvas::receiveSwapFrames(QVector<Frame*> _frames, int swap1, int swap2)
{
    frames = _frames;
    currentFrame = frames[swap2];
    displayCurrentFrame();

    if(previousFrame != nullptr)
    {
        previousFrame->setImage(currentFrame->getImage());
        displayOverlay();
    }

    emit canvasUpdated(frames, frames[swap1]->getFrameId());
    emit canvasUpdated(frames, currentFrame->getFrameId());
}

void Canvas::receiveFrames(QVector<Frame*> _frames){
    frames = _frames;
    currentFrame = frames.last();
    if(frames.size() > 1)
    {
        previousFrame = frames[frames.size()-2];
        displayOverlay();
    }
    displayCurrentFrame();
}

void Canvas::changeFrame(int framePos)
{

    currentFrame = (frames)[framePos];
    if(framePos != 0)
    {
        previousFrame = (frames)[framePos -1];
        displayOverlay();
    }
    displayCurrentFrame();
}
void Canvas::displayCurrentFrame()
{
    ui->label->setScaledContents(true);
    ui->label->setPixmap(QPixmap::fromImage(currentFrame->getImage().scaled(2056, 2056)));
    ui->shapeOverlay->setScaledContents(true);
    ui->shapeOverlay->setPixmap(QPixmap::fromImage(shapeOverlay.getImage().scaled(2056,2056)));
    emit canvasUpdated(frames, currentFrame->getFrameId());

}

void Canvas::displayOverlay()
{
    if(overlayOn)
    {
        QImage overlay(previousFrame->getImage());
        for(int x = 0; x < overlay.width(); x++)
        {
            for(int y = 0; y < overlay.height(); y++)
            {
                QColor pixel = overlay.pixelColor(x,y);
                if(pixel.alpha() != 0)
                {
                    pixel.setAlpha(50);
                    overlay.setPixel(x,y, pixel.rgba());
                }

            }
        }
        ui->overlay->setScaledContents(true);
        ui->overlay->setPixmap(QPixmap::fromImage(overlay.scaled(2056,2056)));
    }
}

void Canvas::modifyCanvas(QMouseEvent* clicked, QColor color)
{
    for(int i = 0; i < brushSize; i++)
    {
        for(int j = 0; j < brushSize; j++)
        {
            currentFrame->setFramePixel(clicked->pos().x()/pixelSize + i, clicked->pos().y()/pixelSize + j, color);
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent* clicked)
{
    if(sizeSelected)
    {
        switch(currentTool)
        {
        case(Model::Tool::brush):
            dragging = true;
            emit sendStroke(new Frame(*currentFrame), true);
            modifyCanvas(clicked, selectedColor);
            displayCurrentFrame();
            break;
        case(Model::Tool::eraser):
            dragging = true;
            modifyCanvas(clicked, Qt::transparent);
            displayCurrentFrame();
            break;
        case(Model::Tool::clear):
            currentFrame->clearImage();
            displayCurrentFrame();
            break;
        case(Model::Tool::rectangle):
            handleShapeClick(clicked);
            break;
        case(Model::Tool::circle):
            handleShapeClick(clicked);
            break;
        case(Model::Tool::mouse):
            break;
        default:
            break;
        }


    }
}

void Canvas::drawCircle(QPoint endPos, Frame& toDrawOn)
{
    shapeOverlay.clearImage();

    int xRadius = (endPos.x() - mouseLocation.x())/2;
    int yRadius = (endPos.y() - mouseLocation.y())/2;
    int centerX = xRadius + mouseLocation.x();
    int centerY = yRadius + mouseLocation.y();
    QPoint center = QPoint(((endPos.x()-mouseLocation.x())/2) + mouseLocation.x(), ((endPos.y() - mouseLocation.y())/2) + mouseLocation.y());

    for (double angle = 0.01; angle < 6.28 ; angle +=0.01)
    {
        toDrawOn.setFramePixel(xRadius*qCos(angle) + centerX, yRadius*qSin(angle) + centerY, selectedColor);
    }
}

void Canvas::handleShapeClick(QMouseEvent* clicked)
{
    dragging = true;
    mouseLocation.setX(clicked->pos().x()/pixelSize);
    mouseLocation.setY(clicked->pos().y()/pixelSize);
    displayCurrentFrame();
}

void Canvas::handleShapeDragging(QMouseEvent* moved)
{
    QPoint endingMouseLocation(moved->pos().x()/pixelSize, moved->pos().y()/pixelSize);

    if(dragging && currentTool == Model::Tool::rectangle)
    {
        drawRectangle(endingMouseLocation, shapeOverlay);
    }
    if(dragging && currentTool == Model::Tool::circle)
    {
        drawCircle(endingMouseLocation, shapeOverlay);
    }

    displayCurrentFrame();

}

void Canvas::drawRectangle(QPoint endPos, Frame& toDrawOn)
{
    shapeOverlay.clearImage();
    //Moving left.
    if(endPos.x() <= mouseLocation.x())
    {
        //Moving up.
        if(endPos.y() <= mouseLocation.y())
        {
            for(int x = mouseLocation.x(); x >= endPos.x(); x--)
            {
                for(int y = mouseLocation.y(); y >= endPos.y(); y--)
                {
                    if(x == mouseLocation.x() || y == mouseLocation.y() || x == endPos.x() || y == endPos.y())
                    {
                        toDrawOn.setFramePixel(x, y, selectedColor);
                    }
                }
            }

        }
        //Moving down.
        else if (endPos.y() >= mouseLocation.y())
        {
            for(int x = mouseLocation.x(); x >= endPos.x(); x--)
            {
                for(int y = mouseLocation.y(); y <= endPos.y(); y++)
                {
                    if(x == mouseLocation.x() || y == mouseLocation.y() || x == endPos.x() || y == endPos.y())
                    {
                        toDrawOn.setFramePixel(x, y, selectedColor);
                    }
                }
            }

        }
    }
    //Moving right.
    else if (endPos.x() >= mouseLocation.x())
    {
        //Moving up.
        if(endPos.y() <= mouseLocation.y())
        {
            for(int x = mouseLocation.x(); x <= endPos.x(); x++)
            {

                for(int y = mouseLocation.y(); y >= endPos.y(); y--)
                {
                    if(x == mouseLocation.x() || y == mouseLocation.y() || x == endPos.x() || y == endPos.y())
                    {
                        toDrawOn.setFramePixel(x, y, selectedColor);
                    }
                }
            }
        }
        //Moving down.
        else if (endPos.y() >= mouseLocation.y())
        {
            for(int x = mouseLocation.x(); x <= endPos.x(); x++)
            {
                for(int y = mouseLocation.y(); y <= endPos.y(); y++)
                {
                    if(x == mouseLocation.x() || y == mouseLocation.y() || x == endPos.x() || y == endPos.y())
                    {
                        toDrawOn.setFramePixel(x, y, selectedColor);
                    }
                }
            }
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent* moved)
{
    if(dragging)
    {
        switch(currentTool)
        {
            case(Model::Tool::brush):
                modifyCanvas(moved, selectedColor);
                displayCurrentFrame();
                break;
            case(Model::Tool::eraser):
                modifyCanvas(moved, Qt::transparent);
                displayCurrentFrame();
                break;
            case(Model::Tool::rectangle):
                handleShapeDragging(moved);
                break;
            case(Model::Tool::circle):
                handleShapeDragging(moved);
                break;
            case(Model::Tool::mouse):
                break;
            default:
                break;
        }
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent* released)
{
    if(sizeSelected)
    {
        dragging = false;
        QPoint endingMouseLocation(released->pos().x()/pixelSize, released->pos().y()/pixelSize);
        if(currentTool == Model::Tool::rectangle)
        {
            emit sendStroke(new Frame(*currentFrame), true);
            drawRectangle(endingMouseLocation, *currentFrame);
        }
        if(currentTool==Model::Tool::circle)
        {
            emit sendStroke(new Frame(*currentFrame), true);
            drawCircle(endingMouseLocation, *currentFrame);
        }

        displayCurrentFrame();
    }
}

//Slots
void Canvas::receiveCurrentTool(Model::Tool currentTool)
{
    if(sizeSelected)
    {
        this->currentTool = currentTool;
        switch(currentTool)
        {
        case (Model::Tool::mouse):
            setCursor(Qt::ArrowCursor);
            break;
        case(Model::Tool::brush):
            setCursor(Qt::CrossCursor);
            break;
        case(Model::Tool::rectangle):
            setCursor(Qt::CrossCursor);
            break;
        case(Model::Tool::clear):
        {
            setCursor(QCursor(QPixmap(":/images/broomCursor.png")));
            break;
        }
        case(Model::Tool::circle):
            setCursor(Qt::CrossCursor);
            break;
        case(Model::Tool::eraser):
            setCursor(QCursor(QPixmap(":/images/eraserCursor.png")));
            break;
        }
    }
}

void Canvas::receiveSelectedColor(QColor selectedColor)
{
    this->selectedColor = selectedColor;
}


void Canvas::receiveNewFrameSize(QSize newSize)
{
    this->frameSize = newSize;
    switch (frameSize.width())
    {
    case 8:
        gridImage = QImage(":/images/grid8x8.png");
        break;
    case 16:
        gridImage = QImage(":/images/grid16x16.png");
        break;
    case 32:
        gridImage = QImage(":/images/grid32x32.png");
        break;
    }

    pixelSize = 512/frameSize.height();
    emit sendFrameSize(frameSize);
    emit enableFrameViewButtons();
    sizeSelected = true;
    ui->frameSizeButton->setVisible(false);
    ui->frameSizeComboBox->setVisible(false);
    ui->frameSizeLabel->setVisible(false);
    receiveCurrentTool(Model::Tool::brush);
    ui->grid->setPixmap(QPixmap::fromImage(gridImage));
    ui->grid->setVisible(true);
}


void Canvas::receiveFrameSize()
{
    int sizeNum = ui->frameSizeComboBox->currentIndex();
    switch (sizeNum)
    {
        case 0:
            this->frameSize = QSize(8,8);
            gridImage = QImage(":/images/grid8x8.png");
            break;
        case 1:
            this->frameSize = QSize(16,16);
            gridImage = QImage(":/images/grid16x16.png");
            break;
        case 2:
            this->frameSize = QSize(32,32);
            gridImage = QImage(":/images/grid32x32.png");
            break;
    }

    pixelSize = 512/frameSize.height();
    emit sendFrameSize(frameSize);
    emit removeAllFrames();
    emit enableFrameViewButtons();
    sizeSelected = true;
    ui->frameSizeButton->setVisible(false);
    ui->frameSizeComboBox->setVisible(false);
    ui->frameSizeLabel->setVisible(false);
    receiveCurrentTool(Model::Tool::brush);
    ui->grid->setPixmap(QPixmap::fromImage(gridImage));
    shapeOverlay = Frame(frameSize, -1);
    ui->grid->setVisible(true);
}

void Canvas::receiveFrameSizeInput()
{
    update();
}

void Canvas::toggleGrid(bool on)
{
    ui->grid->setVisible(on);
}

void Canvas::setEnabledCanvas(bool enable)
{
    this->setEnabled(enable);
}

void Canvas::toggleOverlay(bool on)
{
    if(overlayOn)
    {
        ui->overlay->setVisible(on);
    }
}

void Canvas::undo(int index, Frame* lastFrame)
{
    frames[index] = lastFrame;
    currentFrame = lastFrame;
    displayCurrentFrame();
}

void Canvas::redo(int index, Frame* undoneFrame)
{
    frames[index] = undoneFrame;
    currentFrame = undoneFrame;
    displayCurrentFrame();
}

Canvas::~Canvas()
{
    delete ui;
}



