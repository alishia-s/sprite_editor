#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include "model.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>

namespace Ui {
class Canvas;
}

class Canvas : public QWidget
{
    Q_OBJECT

private:
    Frame * currentFrame;
    Frame * previousFrame;
    Frame shapeOverlay;
    QVector<Frame*> frames;
    bool dragging;
    bool sizeSelected;
    QImage gridImage;
    QSize frameSize;
    int pixelSize;
    QColor selectedColor;
    Model::Tool currentTool = Model::Tool::brush;
    bool overlayOn;
    QPoint mouseLocation;
    int brushSize = 1;

public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();

    void displayCurrentFrame();
    /// \brief Displays the options to select a frame size.
    void displayFrameSizeOptions();
    void displayOverlay();
    void mousePressEvent(QMouseEvent* clicked) override;
    void mouseMoveEvent(QMouseEvent* moved) override;
    void mouseReleaseEvent(QMouseEvent* released) override;

public slots:
    void receiveFrames(QVector<Frame*> frames);
    void receiveCurrentTool(Model::Tool currentTool);
    void receiveFrameSize();
    void receiveNewFrameSize(QSize newSize);
    void receiveFrameSizeInput();
    void receiveSelectedColor(QColor selectedColor);
    void toggleGrid(bool on);
    void toggleOverlay(bool on);
    void changeFrame(int framePos);
    void setEnabledCanvas(bool enable);
    void receiveDuplicateFrames(QVector<Frame*> frames, int framePos);
    void receiveInsertFrames(QVector<Frame*> frames, int newIndex);
    void receiveSwapFrames(QVector<Frame*> frames, int swap1, int swap2);
    void undo(int index, Frame* lastFrame);
    void redo(int index, Frame* lastFrame);

    /// \brief Clears the current sprite and redisplays the option to pick a canvas size when new is clicked.
    void receiveNewSpriteProjectRequest();
    void setBrushSize(int size);

signals:
    void sendFrameSize(QSize size);
    void createFirstFrame();
    void enableFrameViewButtons();
    void canvasUpdated(QVector<Frame*> frames, int currentlySelected);
    void sendStroke(Frame* lastStroke, bool backwards);
    /// \brief Sends a signal to model to clear all frames once submit frame size is pressed.
    void removeAllFrames();

private:
    Ui::Canvas *ui;
    void handleShapeClick(QMouseEvent* clicked);
    void handleShapeDragging(QMouseEvent* moved);

    void drawRectangle(QPoint endPos, Frame& toDrawOn);
    void drawCircle(QPoint endPos, Frame& toDrawOn);
    void modifyCanvas(QMouseEvent* mouse, QColor color);
};

#endif // CANVAS_H
