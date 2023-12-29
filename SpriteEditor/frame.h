/*
 Assignment 8 - Sprite Editor

 Team:      Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac
 Date:      11/15/2023
 Course:    CS 3505, University of Utah, School of Computing
 Copyright: CS 3505 and Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac - This work may not be copied for use in Academic Coursework.

 We, Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac, certify that we wrote this code from scratch and
 did not copy it in part or whole from another source. All references used in the completion of the assignments are cited in my README file.

 File Contents

    This file is used for creating Frame objects that hold the a pixel image, pixel size, and id for the frame.
*/
#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <QImage>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonArray>

/// @brief Frame class
class Frame
{
public:

    /// @brief Constructs a Frame Object
    /// @param frameSize - the size of the pixels in the frame.
    /// @param frameId - the size of the frame ID
    Frame(QSize frameSize, int frameId);

    /// @brief A copy constructor for the Frame constructor
    /// @param other - a reference to a frame object
    Frame(const Frame& other);

    /// @brief Sets the color of a pixel at (x,y) in the image held by Frame object
    /// @param x - the x coordinate of pixel
    /// @param y - the why coordinate of pixel
    /// @param pixelColor - the color of pixel as QColor object
    void setFramePixel(int x, int y, QColor pixelColor);


    /// @brief Gets the image held by Frame object
    /// @return A reference of the image
    QImage& getImage();

    /// @brief Gets the size of the Frame object (its width and length in pixel)
    /// @return A QSize object holding the width and length of the Frame
    QSize getFrameSize();

    /// @brief Gets the Frame's unique ID
    /// @return An int of the Frame's unique ID
    int getFrameId();

    /// @brief Sets the Frame object's image to another image
    /// @param copyImage - a QImage object that the Frame will now hold
    void setImage(QImage copyImage);

    /// @brief Clears the image of the Frame object by setting the image to transparent
    void clearImage();

    /// @brief Used for JSON serialization. Serializes the attributes of the Frame object including:
    /// the pixels and its color (as separate ARGB valuess) and the size of the Frame object.
    /// @return A QJsonObject that holds the serialized information of one Frame Object
    QJsonObject serialize();

private:

    /// @brief frameId - holds the Frame object's unique ID
    int frameId;

    /// @brief frameSize - holds the Frame object's size (width and height in pixels)
    QSize frameSize;

    /// @brief image - holds the image (pixel art) of the Frame object
    QImage image;
};

#endif // FRAME_H
