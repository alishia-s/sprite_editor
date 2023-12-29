#include "preview.h"
#include "ui_preview.h"

Preview::Preview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Preview),
    toggled(true)
{
    ui->setupUi(this);
    ui->fpsSlider->setMinimum(FPS_MIN);
    ui->fpsSlider->setMaximum(FPS_MAX);
    imagesWidth = ui->imageHolder->width();
    connect(ui->fpsSlider, &QSlider::sliderMoved, this, &Preview::fpsSlotUpdated);

    displayImages();
}

Preview::~Preview()
{
    delete ui;
}

void Preview::fpsSlotUpdated(int fpsUpdated)
{
    framesPerSecond = fpsUpdated;
    //to do :ask luis if updateFPS should be framesperSecond?
    //what is he using this for?
    ui->fpsSlider->setSliderPosition(fpsUpdated);
    emit sendFPS(framesPerSecond);
}

void Preview::receiveFrames(QVector<Frame*> frames)
{
    currentFrames = frames;
}

void Preview::togglePreviewAnimation(bool _toggle)
{
    toggled = _toggle;
    if(toggled){
        int time = previewPeriod();
        QTimer::singleShot(time, this, &Preview::displayImages);
    }
}

void Preview::runDisplayImages()
{


}

void Preview::displayImages()
{

    if(currentFrames.size())
    {

        if(frameToDrawIndex < currentFrames.size()) {
            QImage image = (currentFrames)[frameToDrawIndex]->getImage();
            image = image.scaled(imagesWidth, imagesWidth);
            ui->imageHolder->setPixmap(QPixmap::fromImage(image));
        }

        if(++frameToDrawIndex >= currentFrames.size()) {
            //when we sense we've deleted, frameToDrawIndex = 0 (restarts the preview)
            frameToDrawIndex = 0;
        }
    }
    int time = previewPeriod();

    if(toggled)
    {
        QTimer::singleShot(time, this, &Preview::displayImages);
    }

}

int Preview::previewPeriod()
{
    float period = (1.0 / framesPerSecond) * 1000;
    return int(period);
}

