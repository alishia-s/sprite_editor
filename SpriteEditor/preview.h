#ifndef PREVIEW_H
#define PREVIEW_H

#include <QWidget>
#include <QVector>
#include "frame.h"
#include <QThread>
#include <QTimer>


namespace Ui {
class Preview;
}

class Preview : public QWidget
{
    Q_OBJECT
    int framesPerSecond = 1;
    int FPS_MIN = 1;
    int FPS_MAX = 144;
    int imagesWidth;
    QVector<Frame*> currentFrames;
//    QVector<QImage> imageCopies;
    int frameToDrawIndex = 0;

public:
    explicit Preview(QWidget *parent = nullptr);
    ~Preview();

private:
    Ui::Preview *ui;

    void runDisplayImages();
    void displayImages();
    int previewPeriod();

    bool toggled;


public slots:
    void fpsSlotUpdated(int);
    void receiveFrames(QVector<Frame*> frames);
    void togglePreviewAnimation(bool toggle);

//    void receiveImages(QVector<QImage> images);

signals:
    void sendFPS(int FPS);

};

#endif // PREVIEW_H
