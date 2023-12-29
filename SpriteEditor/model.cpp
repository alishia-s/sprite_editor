#include "model.h"
#include <QColor>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>

Model::Model(QObject *parent)
    : QObject{parent}
{
}

Frame& Model::getCurrentFrame()
{
    return *frames[selectedFrameId];
}

void Model::updateCurrentTool(Model::Tool tool)
{
    emit sendCurrentTool(tool);
}

void Model::updateSelectedFrameId(int frameId)
{
    emit sendCurrentFrame(*frames[frameId]);
}

void Model::updateSelectedColor(QColor color) {
    selectedColor = color;
    emit sendSelectedColor(color);
}

void Model::receiveFrameSize(QSize frameSize)
{
    this->frameSize = frameSize;
}

void Model::duplicateFrame(int frameId)
{
    // duplicating image, not same ID
    Frame * dupe = new Frame(frames[frameId]->getFrameSize(), QDateTime::currentMSecsSinceEpoch());
    dupe->setImage(frames[frameId]->getImage().copy());
    frames.insert(frameId+1, 1, dupe);

    //set current image as the current image
    selectedFrameId=dupe->getFrameId();
    emit sendDuplicateFrames(frames, frameId+1);
    emit sendDuplicateFramesToPreview(frames);
    emit sendFrameSize(frames.size());
}

void Model::createNewFrame()
{
    Frame * newF = new Frame(frameSize, QDateTime::currentMSecsSinceEpoch());
    frames.push_back(newF);
    selectedFrameId = newF->getFrameId();
    emit sendFrames(frames);
    emit sendFrameSize(frames.size());
}

void Model::receiveDeleteFrameAt(int frameLoc)
{
    //reset preview to beginning to avoid
    frames.removeAt(frameLoc);
    emit sendFrames(frames);
    emit sendFrameSize(frames.size());
}


void Model::deleteLastFrame()
{
    for(int i = 0; i < frames.size(); i++)
    {
        if(selectedFrameId == frames[i]->getFrameId())
        {
            frames.removeAt(i);
            selectedFrameId = frames[i - 1]->getFrameId();
            emit sendFrames(frames);
            return;
        }
    }
}

void Model::addToHistory(Frame* frameToAdd, bool backwards)
{
    if(backwards)
    {
        backwardStrokes.push(frameToAdd);
        return;
    }
    forwardStrokes.push(frameToAdd);
}

void Model::swapFrameOrder(int swapPostion1, int swapPostion2)
{
    // Swap
    Frame* temp = frames[swapPostion1];
    frames[swapPostion1] = frames[swapPostion2];
    frames[swapPostion2] = temp;
    selectedFrameId = frames[swapPostion1]->getFrameId();

    emit sendSwapFrames(frames, swapPostion1, swapPostion2);
    emit sendSwapFramesToPreview(frames);
}

void Model::changeFrameOrder(int oldFrameIndex, int newFrameIndex)
{
    // Insert
    Frame* temp = frames[oldFrameIndex];
    frames.remove(oldFrameIndex);
    frames.insert(newFrameIndex, temp);
    selectedFrameId = newFrameIndex;

    emit sendInsertFrames(frames, newFrameIndex);
    emit sendInsertFramesToPreview(frames);
}

void Model::clearFrames()
{
    frames.clear();
    createNewFrame();
    emit sendInsertFrames(frames, 0);
    emit sendInsertFramesToPreview(frames);
}

void Model::saveFrames(bool isTriggered)
{
    emit sendSaveFileDialog();
}

void Model::openJSON(bool isTriggered)
{
    emit sendOpenFileDialog();
}

void Model::newSpriteProject()
{
    clearFrames();
    emit sendNewSpriteProjectRequest();
}

void Model::savePNG(bool isTriggered)
{
    emit sendPNGFileDialog();
}

void Model::saveJPG(bool isTriggered)
{
    emit sendJPGFileDialog();
}

void Model::saveTileset(bool isTriggered)
{
    emit sendTilesetFileDialog();
}

void Model::parseJSON(QString fileName)
{
    QTextStream ts(stdout);
    QJsonParseError parseError;
    QJsonDocument jsonDocument;
    {
        QFile fin(fileName);
        fin.open(QIODevice::ReadOnly);
        QByteArray ba2 = fin.readAll();
        jsonDocument = QJsonDocument::fromJson(ba2, &parseError);
        fin.close();
    }

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Parse error at" << parseError.offset << ":" << parseError.errorString();
    }

    clearFrames();
    QJsonObject framesObject = jsonDocument.object();
    int previewSpeed = framesObject.value("previewSpeed").toInt();

    emit updatePreviewSpeed(previewSpeed);

    for (int i = 0; i < framesObject.value("numberOfFrames").toInt(); i++)
    {
        QJsonObject frame = framesObject.value("frame" + QString::number(i)).toObject();
        double width = frame.value("pixelWidth").toDouble();
        double height = frame.value("pixelHeight").toDouble();
        QJsonArray pixelArr = frame.value("pixels").toArray();

        Frame * parsedFrame =  new Frame(QSize(width, height), QDateTime::currentMSecsSinceEpoch());

        for (int j = 0; j < width * height; j++)
        {
            QJsonObject pixelObj = pixelArr[j].toObject();
            int red =  pixelObj.value("Red").toInt();
            int green = pixelObj.value("Green").toInt();
            int blue = pixelObj.value("Blue").toInt();
            int alpha = pixelObj.value("Alpha").toInt();

            int x = pixelObj.value("x").toInt();
            int y = pixelObj.value("y").toInt();

            QColor color(red, green, blue, alpha);
            parsedFrame->setFramePixel(x, y, color);
        }

        //TODO: bad code... discuss with team
        if( i == 0)
        {
            Frame * formerFirst;
            formerFirst = frames[0];
            frames.removeFirst();
            delete formerFirst;

            emit sendNewFrameSize(QSize(width, height));
        }
        frames.push_back(parsedFrame);
        emit sendFrames(frames);
    }
}

void Model::undo(bool isTriggered)
{
    if(!backwardStrokes.isEmpty())
    {
        Frame* lastFrame = backwardStrokes.pop();
        int index = 0;
        for(int i = 0; i < frames.size(); i++)
        {
            if (frames[i]->getFrameId() == lastFrame->getFrameId())
            {
                forwardStrokes.push(frames[i]);
                frames[i] = lastFrame;
                index = i;
            }
        }
        emit sendBackwardStroke(index, lastFrame);
        emit sendFrames(frames);
    }
}

void Model::createJSON(QString fileName)
{
    QJsonObject canvas;
    canvas["numberOfFrames"] =  frames.size();
    canvas["previewSpeed"] = framesPerSecond;

    for (int i = 0; i < frames.size(); i++)
    {
        Frame * current = frames[i];
        QString name = "frame" + QString::number(i);
        canvas[name] = current->serialize();
    }

    QByteArray ba = QJsonDocument(canvas).toJson();
    {
        QFile fout(fileName);
        fout.open(QIODevice::WriteOnly);
        fout.write(ba);
    }
}

void Model::createImage(QString filename)
{
    int index =0;
    for(int i =0; i < frames.size(); i++)
    {
        if(selectedFrameId == frames[i]->getFrameId())
        {
            index = i;
        }
    }

    QImage toSave = QImage(frames[index]->getImage().scaled(QSize(512,512)));
    toSave.save(filename);
}

void Model::createTileset(QString fileName)
{
    QImage tileset = QImage(QSize(frameSize.width()*frames.size(), frameSize.height()), QImage::Format_ARGB32);
    tileset.fill(QColor(Qt::transparent));
    int x = 0;
    int frameCount = 0;
    for(auto& frame : frames)
    {
        QImage frameImage = frame->getImage();
        for(int i = x; i < x+frameSize.width(); i++)
        {
            for(int y = 0; y < frameSize.height(); y++)
            {
                tileset.setPixelColor(i, y, frameImage.pixelColor(i - frameCount*frameSize.width(), y));
            }
        }
        x += frameSize.width();
        frameCount++;
    }

    tileset = QImage(tileset.scaled(QSize (512*frames.size(), 512)));
    tileset.save(fileName);
}



void Model::redo(bool isTriggered)
{
    if(!forwardStrokes.isEmpty())
    {
        Frame* undoneFrame = forwardStrokes.pop();
        int index = 0;
        for(int i = 0; i < frames.size(); i++)
        {
            if (frames[i]->getFrameId() == undoneFrame->getFrameId())
            {
                backwardStrokes.push(frames[i]);
                frames[i] = undoneFrame;
                index = i;
                break;
            }
        }
        emit sendForwardStroke(index, undoneFrame);
        emit sendFrames(frames);
    }
}

void Model::updateFPS(int _framesPerSecond)
{
    framesPerSecond = _framesPerSecond;
}

Model::~Model()
{
    for(Frame * frame :frames)
    {
        delete frame;
    }
}
