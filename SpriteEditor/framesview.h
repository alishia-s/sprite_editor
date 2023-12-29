/*
 Assignment 8 - Sprite Editor

 Team:      Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac
 Date:      11/15/2023
 Course:    CS 3505, University of Utah, School of Computing
 Copyright: CS 3505 and Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac - This work may not be copied for use in Academic Coursework.

 We, Aimal Esa, Alishia Seo, Ashley Lujan, James Donaldson, Luis Alvarado-Labarca, Madelyn Sudac, certify that we wrote this code from scratch and
 did not copy it in part or whole from another source. All references used in the completion of the assignments are cited in my README file.

 File Contents

    This header file is used to manage the UI that holds all the Frames or pixel/sprite art the user is working on.
    This UI is interactable.
*/

#ifndef FRAMESVIEW_H
#define FRAMESVIEW_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QSize>
#include <QLayout>
#include "frame.h"

class SwapFrames;

class InsertFrames;

namespace Ui
{
class FramesView;
}

/// @brief The FramesView class inherits from the QWidget class
class FramesView : public QWidget
{
    Q_OBJECT

public:

    /// @brief This is the constructor for the FramesView
    /// @param parent - A pointer to the parent QWidget class, which is the MainWindow
    explicit FramesView(QWidget *parent = nullptr);

    /// Destructor for FramesView object
    ~FramesView();

public slots:

    /// @brief Adds a new Frame to edit when '+' button is clicked
    void addFrameButtonClicked();

    /// @brief Deletes a Frame when the '-' button is clicked
    void deleteAFrameDisplay();

    /// @brief receives a QVector of Frame pointers and the frameId of the Frame object currently
    ///        being edited by the user
    /// @param frames - A QVector of Frame pointers
    /// @param currentEdit - the unique frameId of the Frame the user is currently working on
    void receiveCanvasUpdate(QVector<Frame*> frames, int currentEdit);

    /// @brief Enables the buttons for user interaction once the user starts the Sprite Editor
    void enableStartButtons();

    /// @brief Opens the dialog window to reorder Frames
    void openOrderFramesWindow();

    /// @brief Opens the dialog window to insert a frame in a new location
    void openInsertFramesWindow();

    /// @brief Swaps the position of two user-selected Frames
    /// @param swapPosition1 - the first Frame the user wants to swap
    /// @param swapPosition2 - the second Frame the user wants to swap
    void swapFrames(int swapPostion1, int swapPostion2);

    /// @brief When the user does not want to swap two Frames, it will cancel the swap
    void cancelledSwap();

    /// @brief When the user does not want to change the Frame's position, it will cancel
    ///        the insert
    void cancelledInsert();

    /// @brief Will change the position of one frame and shift all the other Frames over by one
    /// @param oldFrameIndex - the user-selected Frame's old position in the FramesView UI
    /// @param newFrameIndex - the user-selected Frame's new position in the FramesView UI
    void insertFrame(int oldFrameIndex, int newFrameIndex);

    /// @brief Clears all of the Frames from the FramesView UI
    void clearFramesAction();

    /// @brief Responds to button clicks attached to FramesView UI
    void buttonClicked();

    /// @brief Duplicates a frame that is currently being worked on or selected
    void duplicateFrame();

private:

    /// @brief A pointer to the UI
    Ui::FramesView *ui;

    /// @brief A QVector of QPushButton pointers that exist in the FramesView UI (to hold the frames)
    QVector<QPushButton*> buttons;

    /// @brief A QString that holds the name of the Frame button that is selected
    QString buttonName;

    /// @brief A constant QSize that sets the QPushButtons and QIcon sizes
    QSize SIZE;

    /// @brief The number of frames previously in QVector of Frame pointers
    int previousNumOfFrames;

    /// @brief A pointer to the SwapFrames object
    SwapFrames* orderFrames;

    /// @brief A pointer to the InsertFrames object
    InsertFrames* insertFrames;

    ///Methods:

    /// @brief Adds a duplicate of the currently selected frame to the FramesView UI
    /// @param frames - A QVector of Frame pointers that contain all the Frames the user is working on
    /// @param newButtonPosition - the position of where the duplicate Frame should go in the UI
    void addDuplicate(QVector<Frame*> frames, int newButtonPosition);

    /// @brief Displays all the Frames the user is working on
    /// @param frames - A QVector of Frame pointers
    void displayFrames(QVector<Frame*> frames);

    /// @brief Finds the index of a frame based on its frame ID
    /// @param frames - A QVector of Frame pointers
    /// @param currentFrame - The unique frame ID
    int findIndex(QVector<Frame*> frames, int currentFrame);

    /// @brief Updates the icon of the button used to represent all the Frames
    /// @param button - The button that will hold the image of frame
    /// @param frame - The frame that needs to be added or its image needs to be updated in UI
    void updateButtonBackground(QPushButton* button, Frame* frame);

    /// @brief Sends the name of the button (which is the Frame's unique ID) to swap its position
    ///        with another frame
    /// @param senderObj - The button
    void swapAction(QObject* senderObj);

    /// @brief Sends the name of the button (which is the Frame's unique ID) to change its posiiton
    ///        in the UI
    /// @param senderObj - The button
    void insertAction(QObject* senderObj);

    /// @brief Sets the frameSelected field as the button's name (which is the Frame's unique ID)
    /// @param senderObj - The butotn
    void selectButtonAction(QObject* senderObj);

    /// @brief Enables buttons for user interaction
    /// @param enable - A boolean that determines if the buttons will be enabled or not
    void setEnabledButtons(bool enable);

signals:

    /// @brief Sends button position of the Frame that was selected
    /// @param buttonPosition - Position of button
    void newFrame(int buttonPosition);

    /// @brief Sends a signal that a Frame was added
    void addAFrame();

    /// @brief Sends the button position that should be deleted
    /// @param buttonPosition - Position of button
    void deleteFrameAt(int buttonPosition);

    /// @brief Sends the button position of the Frame that needs to be swapped
    /// @param buttonPosition - Position of button
    void sendSwapNumber(int buttonPosition);

    /// @brief Sends the button positon of the
    /// @param buttonPosition - Position of button
    /// @param sizeOfFrames - the size of the QVector of Frame Pointers
    void sendInsertNumber(int buttonPosition, int sizeOfFrames);

    /// @brief Swaps position of two Frames
    /// @param swapPosition1 - the position of the first Frame
    /// @param swapPosition2 - the position of the second Frame
    void swapFramePositions(int swapPostion1, int swapPostion2);

    /// @brief Tells canvas where the buttons to FramesView UI are enabled
    /// @param enable - Boolean of whether the buttons are enabled or not
    void setEnabledCanvas(bool enable);

    /// @brief Duplicates a Frame
    /// @param index - The index of the button that holds the Frame that the user wants to duplicate
    void duplicateFrameAt(int index);

    /// @brief Changes the position of one Frame
    /// @param oldFrameIndex - the Frame's old position
    /// @param newFrameIndex = the Frame's new position
    void changeOrdering(int oldFrameIndex, int newFrameIndex);

    /// @brief Clears all the Frames
    void clearFrames();
};
#endif // FRAMESVIEW_H
