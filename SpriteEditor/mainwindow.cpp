#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QShortcut>
#include <QKeySequence>

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowTitle("qt&&theFunc(bunch) Pixel Editor");
    ui->setupUi(this);

    // File Menu
    connect(ui->actionSave, &QAction::triggered, &model, &Model::saveFrames);
    connect(ui->actionLoad, &QAction::triggered, &model, &Model::openJSON);
    connect(ui->actionNew, &QAction::triggered, &model, &Model::newSpriteProject);
    connect(ui->actionPNG, &QAction::triggered, &model, &Model::savePNG);
    connect(ui->actionJPG, &QAction::triggered, &model, &Model::saveJPG);
    connect(ui->actionTileset, &QAction::triggered, &model, &Model::saveTileset);

    connect(&model, &Model::sendNewSpriteProjectRequest, ui->canvas, &Canvas::receiveNewSpriteProjectRequest);
    connect(ui->canvas, &Canvas::removeAllFrames, &model, &Model::clearFrames);
    
    //actions
    connect(ui->actionUndo, &QAction::triggered, &model, &Model::undo);
    connect(ui->actionRedo, &QAction::triggered, &model, &Model::redo);
    connect(ui->actionToggle_Gridlines, &QAction::triggered, ui->tools, &Tools::gridButtonClicked);
    connect(ui->actionToggle_Overlay, &QAction::triggered, ui->tools, &Tools::overlayButtonClicked);

    connect(ui->framesView, &FramesView::duplicateFrameAt, &model, &Model::duplicateFrame);
    connect(ui->framesView, &FramesView::newFrame, ui->canvas, &Canvas::changeFrame);
    connect(ui->framesView, &FramesView::swapFramePositions, &model, &Model::swapFrameOrder);
    connect(ui->framesView, &FramesView::changeOrdering, &model, &Model::changeFrameOrder);
    connect(ui->framesView, &FramesView::deleteFrameAt, &model, &Model::receiveDeleteFrameAt);
    connect(ui->framesView, &FramesView::clearFrames, &model, &Model::clearFrames);

    connect(ui->framesView, &FramesView::addAFrame, &model, &Model::createNewFrame);
    connect(&model, &Model::sendCurrentTool, ui->canvas, &Canvas::receiveCurrentTool);
    connect(ui->canvas, &Canvas::sendFrameSize, &model, &Model::receiveFrameSize);
    connect(ui->canvas, &Canvas::createFirstFrame, &model, &Model::createNewFrame);
    connect(&model, &Model::sendFrames, ui->canvas, &Canvas::receiveFrames);
    connect(&model, &Model::sendFrames, ui->preview, &Preview::receiveFrames);
    connect(ui->palette, &Palette::sendSelectedColor, &model, &Model::updateSelectedColor);
    connect(&model, &Model::sendSelectedColor, ui->canvas, &Canvas::receiveSelectedColor);
    connect(&model, &Model::sendSelectedColor, ui->tools, &Tools::updateCurrentColorButton);
    connect(&model, &Model::sendFrameSize, ui->tools, &Tools::updateFrameCount);
    connect(ui->preview, &Preview::sendFPS, ui->tools, &Tools::updateFPS);
    connect(ui->preview, &Preview::sendFPS, &model, &Model::updateFPS);

    connect(&model, &Model::updatePreviewSpeed, ui->preview, &Preview::fpsSlotUpdated);
    connect(ui->palette, &Palette::sendSelectedColor, &model, &Model::updateSelectedColor);
    connect(ui->tools, &Tools::sendSelectedTool, &model, &Model::updateCurrentTool);
    connect(ui->tools, &Tools::toggleGrid, ui->canvas, &Canvas::toggleGrid);
    connect(ui->tools, &Tools::toggleOverlay, ui->canvas, &Canvas::toggleOverlay);
    connect(ui->tools, &Tools::updateBrushSize, ui->canvas, &Canvas::setBrushSize);
    connect(ui->tools, &Tools::sendColor, &model, &Model::updateSelectedColor);


    connect(ui->canvas, &Canvas::canvasUpdated, ui->framesView, &FramesView::receiveCanvasUpdate);
    connect(ui->canvas, &Canvas::enableFrameViewButtons, ui->framesView, &FramesView::enableStartButtons);
    connect(ui->framesView, &FramesView::setEnabledCanvas, ui->canvas, &Canvas::setEnabledCanvas);
    connect(&model, &Model::sendDuplicateFrames, ui->canvas, &Canvas::receiveDuplicateFrames);
    connect(&model, &Model::sendInsertFrames, ui->canvas, &Canvas::receiveInsertFrames);
    connect(&model, &Model::sendSwapFrames, ui->canvas, &Canvas::receiveSwapFrames);
    connect(&model, &Model::sendNewFrameSize, ui->canvas, &Canvas::receiveNewFrameSize);

    connect(&model, &Model::sendDuplicateFramesToPreview, ui->preview, &Preview::receiveFrames);
    connect(&model, &Model::sendInsertFramesToPreview, ui->preview, &Preview::receiveFrames);
    connect(&model, &Model::sendSwapFramesToPreview, ui->preview, &Preview::receiveFrames);

    connect(ui->canvas, &Canvas::sendStroke, &model, &Model::addToHistory);
    connect(&model, &Model::sendBackwardStroke, ui->canvas, &Canvas::undo);
    connect(&model, &Model::sendForwardStroke, ui->canvas, &Canvas::redo);
      
    connect(&model, &Model::sendSaveFileDialog, this, &MainWindow::openSaveFileDialog);
    connect(this, &MainWindow::sendSSPFileName, &model, &Model::createJSON);

    connect(&model, &Model::sendPNGFileDialog, this, &MainWindow::openPNGFileDialog);
    connect(&model, &Model::sendJPGFileDialog, this, &MainWindow::openJPGFileDialog);
    connect(&model, &Model::sendTilesetFileDialog, this, &MainWindow::openTilesetFileDialog);
    connect(this, &MainWindow::sendImageFileName, &model, &Model::createImage);
    connect(this, &MainWindow::sendTilesetFileName, &model, &Model::createTileset);


    //opening json file
    connect(&model, &Model::sendOpenFileDialog, this, &MainWindow::openJSONFileDialog);
    connect(this, &MainWindow::sendJSONFileName, &model, &Model::parseJSON);


    //menu bar connections
    connect(ui->tools, &Tools::togglePreviewPlay, ui->preview, &Preview::togglePreviewAnimation);
    //Shortcuts

    ui->actionUndo->setShortcut(QKeySequence::Undo);
    ui->actionRedo->setShortcut(QKeySequence::Redo);
    ui->actionLoad->setShortcut(QKeySequence::Open);
    ui->actionSave->setShortcut(QKeySequence::Save);
    const char* saveAsSequence = "Ctrl+Shift+S";
    ui->actionSave_as->setShortcut(QKeySequence(saveAsSequence));
    ui->actionNew->setShortcut(QKeySequence::New);
    ui->actionHelp->setShortcut(QKeySequence::HelpContents);
    ui->actionToggle_Gridlines->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    ui->actionToggle_Overlay->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
}

void MainWindow::openJSONFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath(), tr("Sprite Project File (*.ssp)"));

    if (fileName != "")
    {
        emit sendJSONFileName(fileName);
    }
}

void MainWindow::openSaveFileDialog()
{

//    if (model->fileToSaveTo == "")
//    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project"), ".ssp", tr("Sprite Project File (*.ssp)"));
//    }

    if (fileName != "")
    {
        emit sendSSPFileName(fileName);
    }

}

void MainWindow::openPNGFileDialog()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), ".png", tr("Portable Network Graphics (*.png)"));
    if (fileName != "")
    {
        emit sendImageFileName(fileName);
    }
}

void MainWindow::openJPGFileDialog()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), ".jpg", tr("Joint Photographic Experts Group (*.jpg)"));
    if (fileName != "")
    {
        emit sendImageFileName(fileName);
    }
}


void MainWindow::openTilesetFileDialog()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), ".png", tr("Portable Network Graphics (*.png)"));
    if (fileName != "")
    {
        emit sendTilesetFileName(fileName);
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}

