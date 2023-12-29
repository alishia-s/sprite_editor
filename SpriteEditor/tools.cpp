#include "tools.h"
#include "ui_tools.h"

Tools::Tools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tools),
    selectedTool(Model::Tool::brush),
    gridOn(true),
    overlayOn(true),
    previewToggle(true)
{
    ui->setupUi(this);

    connect(ui->mouseButton, &QPushButton::clicked, this, &Tools::toolButtonClicked);
    connect(ui->brushButton, &QPushButton::clicked, this, &Tools::toolButtonClicked);
    connect(ui->rectangleButton, &QPushButton::clicked, this, &Tools::toolButtonClicked);
    connect(ui->circleButton, &QPushButton::clicked, this, &Tools::toolButtonClicked);
    connect(ui->eraserButton, &QPushButton::clicked, this, &Tools::toolButtonClicked);
    connect(ui->gridButton, &QPushButton::clicked, this, &Tools::gridButtonClicked);
    connect(ui->overlayButton, &QPushButton::clicked, this, &Tools::overlayButtonClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &Tools::toolButtonClicked);
    connect(ui->brushSize, &QSpinBox::valueChanged, this, &Tools::sendBrushSize);
    connect(ui->currentColorButton, &QPushButton::clicked, this, &Tools::pullUpColorSelector);


    //preview play and puase
    connect(ui->togglePreviewButton, &QPushButton::clicked, this, &Tools::togglePreviewButtonClicked);

    ui->mouseButton->setIcon(QIcon(QPixmap(":/images/cursor.png")));
    ui->eraserButton->setIcon(QIcon(QPixmap(":/images/eraser.png")));
    ui->brushButton->setIcon(QIcon(QPixmap(":/images/pen.png")));
    ui->gridButton->setIcon(QIcon(QPixmap(":/images/gridCancelIcon.png")));
    ui->rectangleButton->setIcon(QIcon(QPixmap(":/images/rectangular-shape-outline.png")));
    ui->circleButton->setIcon(QIcon(QPixmap(":/images/dry-clean.png")));
    ui->clearButton->setIcon(QIcon(QPixmap(":/images/broom.png")));
    ui->togglePreviewButton->setIcon(QIcon(QPixmap(":/images/playPausePreviewBtn.png")));
    updateCurrentColorButton(QColor("black"));

    ui->brushSize->clearFocus();
    ui->overlayButton->setIcon(QIcon(QPixmap(":/images/overlayCancelIcon.png")));
}

void Tools::togglePreviewButtonClicked()
{
    previewToggle = !previewToggle;
    emit togglePreviewPlay(previewToggle);

}

void Tools::toolButtonClicked()
{
    QString toolName = sender()->objectName();
    if(toolName.startsWith("mouse"))
    {
        selectedTool = Model::Tool::mouse;
    }
    if(toolName.startsWith("brush"))
    {
        selectedTool = Model::Tool::brush;
    }
    if(toolName.startsWith("rectangle"))
    {
        selectedTool = Model::Tool::rectangle;
    }
    if(toolName.startsWith("circle"))
    {
        selectedTool = Model::Tool::circle;
    }
    if(toolName.startsWith("eraser"))
    {
        selectedTool = Model::Tool::eraser;
    }
    if(toolName.startsWith("clear"))
    {
       selectedTool = Model::Tool::clear;
    }

    emit sendSelectedTool(selectedTool);
}

void Tools::gridButtonClicked()
{
    gridOn = !gridOn;
    emit toggleGrid(gridOn);
    if(gridOn)
    {
        ui->gridButton->setIcon(QIcon(":/images/gridCancelIcon.png"));
    }
    else
    {
        ui->gridButton->setIcon(QIcon(":/images/gridIcon.png"));
    }
}

void Tools::overlayButtonClicked()
{
    overlayOn = !overlayOn;
    emit toggleOverlay(overlayOn);
    if(overlayOn)
    {
        ui->overlayButton->setIcon(QIcon(":/images/overlayCancelIcon.png"));
    }
    else
    {
        ui->overlayButton->setIcon(QIcon(":/images/overlayIcon.png"));
    }
}

void Tools::updateCurrentColorButton(QColor color)
{
    QPixmap icon(89,89);
    icon.fill(color);
    QIcon colorIcon(icon);

    ui->currentColorButton->setIconSize(QSize(89,89));
    ui->currentColorButton->setIcon(colorIcon);
}

void Tools::sendBrushSize(int size)
{
    emit updateBrushSize(size);
}

void Tools::pullUpColorSelector()
{
    QColor newColor = QColorDialog::getColor(Qt::white, this);
    updateCurrentColorButton(newColor);
    emit sendColor(newColor);
}

void Tools::updateFPS(int FPS)
{
    ui->fpsSpeed->setText(QString::number(FPS));
}

void Tools::updateFrameCount(int frameCount)
{
    ui->numberOfFrames->setText(QString::number(frameCount));
}

Tools::~Tools()
{
    delete ui;
}
