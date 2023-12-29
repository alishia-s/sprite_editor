#include "frame.h"

Frame::Frame(QSize frameSize, int frameId) : frameId(frameId), frameSize(frameSize)
{
    image = QImage(frameSize, QImage::Format_ARGB32);
    image.fill(QColor(Qt::transparent));
}

Frame::Frame(const Frame& other)
{
    image = QImage(other.image);
    frameSize = other.frameSize;
    frameId = other.frameId;
}

void Frame::setFramePixel(int x, int y, QColor pixelColor)
{
    image.setPixelColor(x, y, pixelColor);
}

QImage& Frame::getImage()
{
    return image;
}

QSize Frame::getFrameSize()
{
    return frameSize;
}

int Frame::getFrameId()
{
    return frameId;
}

void Frame::setImage(QImage copyImage)
{
    image = copyImage;
}

QJsonObject Frame::serialize()
{
    QJsonObject frameJson;

    //Frames will always be square, thus size = frame width or height
    int size = frameSize.width();

    frameJson["pixelWidth"] = size;

    frameJson["pixelHeight"] = size;

    QJsonArray imageJson;

    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            QJsonObject pixel;

            pixel["x"] = x;

            pixel["y"] = y;

            //This is adding the color of each pixel to JSON object
            pixel["Red"] = image.pixelColor(x, y).red();

            pixel["Green"] = image.pixelColor(x, y).green();

            pixel["Blue"] = image.pixelColor(x, y).blue();

            pixel["Alpha"] = image.pixelColor(x, y).alpha();

            imageJson.push_back(pixel);
        }
    }

    frameJson["pixels"] = imageJson;

    return frameJson;
}

void Frame::clearImage()
{
    image.fill(Qt::transparent);
}
