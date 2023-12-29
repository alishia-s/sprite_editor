#ifndef PALETTE_H
#define PALETTE_H

#include <QWidget>
#include <QColorDialog>

namespace Ui {
class Palette;
}

class Palette : public QWidget
{
    Q_OBJECT

public:
    explicit Palette(QWidget *parent = nullptr);
    ~Palette();

private:
    Ui::Palette *ui;
    QMap<QString, QColor> buttonColors;

public slots:
    void paletteButtonClicked();

signals:
    void sendSelectedColor(QColor color);
};

#endif // PALETTE_H
