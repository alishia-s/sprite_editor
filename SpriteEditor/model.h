#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "frame.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <QStack>
#include <QPainter>

class Model : public QObject
{
    Q_OBJECT
public:
    /// Constructor
    explicit Model(QObject *parent = nullptr);

    /// \brief An enumerator of Tools so that they can easily be accessed.
    enum class Tool {mouse, brush, rectangle, circle, eraser, clear};

    // Methods
    /// \brief Returns the current vector of frames.
    QVector<Frame*> getFrames();

    /// \brief Returns the current tool.
    Tool getCurrentTool();

    /// \brief Returns the current frame.
    Frame& getCurrentFrame();

    /// \brief Sets the Id of the current frame.
    void setCurrentFrameId();

    /// \brief The default filename when saving a file.
    QString fileToSaveTo ="";

    int framesPerSecond = 0;

    ~Model();

public slots:

    /// \brief Adds to the vector a new frame, and then it will send the entire vector of frames to FramesView
    void createNewFrame();

    /// \brief Deletes the last frame on the vector of frames.
    void deleteLastFrame();

    /// \brief Updates the current tool being used in the canvas.
    /// \param tool The tool that will be set as the selected tool.
    void updateCurrentTool(Tool tool);

    /// \brief Updates the current color being used in the canvas.
    /// \param color The color that will be set as the selected color.
    void updateSelectedColor(QColor color);

    /// \brief updateSelectedFrameId
    /// \param frameId The index of the frame being updated.
    void updateSelectedFrameId(int frameId);

    /// \brief Duplicates the current selected frame.
    /// \param frameId The index of the frame being duplicated.
    void duplicateFrame(int frameId);

    /// \brief
    /// \param frameSize
    void receiveFrameSize(QSize frameSize);

    /// \brief
    /// \param frameLoc
    void receiveDeleteFrameAt(int frameLoc);

    /// \brief
    /// \param swapPostion1
    /// \param swapPostion2
    void swapFrameOrder(int swapPostion1, int swapPostion2);

    /// \brief
    /// \param oldFrameIndex
    /// \param newFrameIndex
    void changeFrameOrder(int oldFrameIndex, int newFrameIndex);

    /// \brief
    /// \param frameToAdd
    /// \param backwards
    void addToHistory(Frame* frameToAdd, bool backwards);

    /// \brief
    void clearFrames();

    /// \brief
    /// \param filename
    void createJSON(QString filename);

    /// \brief
    /// \param filename
    void createImage(QString filename);

    /// \brief
    /// \param filename
    void createTileset(QString filename);

    /// \brief
    /// \param filename
    void parseJSON(QString filename);

    //actions
    
    /// \brief
    /// \param isTriggered
    void undo(bool isTriggered);

    /// \brief
    /// \param isTriggered
    void redo(bool isTriggered);

    /// \brief Clears the frames and tells the canvas to re-ask for frame size
    void newSpriteProject();
    
    /// \brief
    /// \param isTriggered
    void saveFrames(bool isTriggered);

    /// \brief
    /// \param isTriggered
    void openJSON(bool isTriggered);

    /// \brief
    /// \param isTriggered
    void savePNG(bool isTriggered);

    /// \brief
    /// \param isTriggered
    void saveJPG(bool isTriggered);

    /// \brief
    /// \param isTriggered
    void saveTileset(bool isTriggered);

    void updateFPS(int framesPerSecond);

signals:
    void sendCurrentFrame(Frame& frame);
    void sendFrames(QVector<Frame*> &frames);
    void sendCurrentTool(Tool currentTool);
    void sendSelectedColor(QColor color);
    void sendDuplicateFrames(QVector<Frame*> &frames, int framePos);
    void sendDuplicateFramesToPreview(QVector<Frame*> &frames);
    void sendInsertFrames(QVector<Frame*> frames, int newFrameIndex);
    void sendInsertFramesToPreview(QVector<Frame*> &frames);
    void sendSwapFrames(QVector<Frame*> frames, int swap1, int swap2);
    void sendSwapFramesToPreview(QVector<Frame*> &frames);
    void sendFrameSize(int size);

    void sendSaveFileDialog();
    void sendOpenFileDialog();
    void sendPNGFileDialog();
    void sendJPGFileDialog();
    void sendTilesetFileDialog();
    void sendNewFrameSize(QSize size);

    void sendBackwardStroke(int index, Frame* lastFrame);
    void sendForwardStroke(int index, Frame* lastUndo);

    void sendNewSpriteProjectRequest();
    void updatePreviewSpeed(int previewSpeed);

private:
    /// \brief
    int selectedFrameId;

    /// \brief
    QColor selectedColor;
    
    /// \brief
    Tool currentTool;
    
    /// \brief
    QSize frameSize;
    
    /// \brief
    //previous history (undo)
    QStack<Frame*> backwardStrokes;
    
    /// \brief
    //forward history (redo)
    QStack<Frame*> forwardStrokes;
    
    /// \brief
    QVector<Frame*> frames;


};

#endif // MODEL_H
