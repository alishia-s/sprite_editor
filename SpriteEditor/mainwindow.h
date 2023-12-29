#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canvas.h"
#include "model.h"
#include "palette.h"
#include "tools.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model&, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendSSPFileName(QString filename);
    void sendJSONFileName(QString filename);
    void sendImageFileName(QString filename);
    void sendTilesetFileName(QString filename);

private slots:
    void openSaveFileDialog();
    void openJSONFileDialog();
    void openPNGFileDialog();
    void openJPGFileDialog();
    void openTilesetFileDialog();

private:
    Ui::MainWindow *ui;
    Canvas canvas;
    Palette palette;
    Tools tools;
};
#endif // MAINWINDOW_H
