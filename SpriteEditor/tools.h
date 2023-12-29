#ifndef TOOLS_H
#define TOOLS_H

#include <QWidget>
#include <QColorDialog>
#include "model.h"

namespace Ui {
class Tools;
}

class Tools : public QWidget
{
    Q_OBJECT

public:
    explicit Tools(QWidget *parent = nullptr);
    ~Tools();

public slots:
    void toolButtonClicked();
    void gridButtonClicked();
    void overlayButtonClicked();
    void updateCurrentColorButton(QColor color);
    void togglePreviewButtonClicked();
    void sendBrushSize(int size);
    void pullUpColorSelector();
    void updateFPS(int FPS);
    void updateFrameCount(int frameCount);

signals:
    void sendSelectedTool(Model::Tool selectedTool);
    void toggleGrid(bool on);
    void toggleOverlay(bool on);
    void togglePreviewPlay(bool toggle);
    void updateBrushSize(int size);
    void sendColor(QColor color);

private:
    Ui::Tools *ui;
    Model::Tool selectedTool;
    bool gridOn;
    bool overlayOn;
    bool previewToggle;
};

#endif // TOOLS_H
