#include "palette.h"
#include "ui_palette.h"

Palette::Palette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Palette)
{
    ui->setupUi(this);

    buttonColors[QString("redButton")] = QColor(qRgb(255, 0, 0));
    buttonColors[QString("orangeButton")] = QColor(qRgb(255, 170, 0));
    buttonColors[QString("yellowButton")] = QColor(qRgb(255, 255, 0));
    buttonColors[QString("greenButton")] = QColor(qRgb(0, 255, 0));
    buttonColors[QString("blueButton")] = QColor(qRgb(0, 0, 255));
    buttonColors[QString("purpleButton")] = QColor(qRgb(127, 0, 255));
    buttonColors[QString("pinkButton")] = QColor(qRgb(255, 0, 255));
    buttonColors[QString("grayButton")] = QColor(qRgb(128, 128, 128));
    buttonColors[QString("whiteButton")] = QColor(qRgb(255, 255, 255));
    buttonColors[QString("blackButton")] = QColor(qRgb(0, 0, 0));
    buttonColors[QString("customColor_1")]= Qt::white;
    buttonColors[QString("customColor_2")]= Qt::white;
    buttonColors[QString("customColor_3")]= Qt::white;
    buttonColors[QString("customColor_4")]= Qt::white;
    buttonColors[QString("customColor_5")]= Qt::white;
    buttonColors[QString("customColor_6")]= Qt::white;
    buttonColors[QString("customColor_7")]= Qt::white;
    buttonColors[QString("customColor_8")]= Qt::white;
    buttonColors[QString("customColor_9")]= Qt::white;

    connect(ui->redButton, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->orangeButton, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->yellowButton, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->greenButton, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->blueButton, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->purpleButton, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->pinkButton, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->grayButton, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->whiteButton, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->blackButton, &QPushButton::clicked, this, &Palette::paletteButtonClicked);

    connect(ui->customColor_1, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->customColor_2, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->customColor_3, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->customColor_4, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->customColor_5, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->customColor_6, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->customColor_7, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->customColor_8, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->customColor_9, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
    connect(ui->customColor_10, &QPushButton::clicked, this, &Palette::paletteButtonClicked);
}

void Palette::paletteButtonClicked()
{
    if(QString(sender()->objectName()).startsWith("custom"))
    {
        QColor newColor =QColorDialog::getColor(buttonColors[QString(sender()->objectName())], this);
        int r = newColor.red();
        int g = newColor.green();
        int b = newColor.blue();
        qobject_cast<QPushButton*>(sender())->setStyleSheet(QString("QPushButton {background-color: rgb(") + QString::number(r) + "," + QString::number(g) + "," + QString::number(b) + ");}");
        buttonColors[QString(sender()->objectName())] = newColor;
    }
    emit sendSelectedColor(buttonColors[QString(sender()->objectName())]);
}

Palette::~Palette()
{
    delete ui;
}
