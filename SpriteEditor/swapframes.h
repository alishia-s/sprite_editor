#ifndef SWAPFRAMES_H
#define SWAPFRAMES_H

#include <QMainWindow>

namespace Ui {
class SwapFrames;
}

class SwapFrames : public QMainWindow
{
    Q_OBJECT

public:
    explicit SwapFrames(QWidget *parent = nullptr);
    ~SwapFrames();

public slots:
    void setButtonNumbers(int buttonNumber);

signals:
    void swapFrames(int swapPostion1, int swapPostion2);
    void restoreButtons(bool restore);
    void cancelled();

private:
    Ui::SwapFrames *ui;

    bool isLCDSet = false;
    bool isLCD2Set = false;
    int swapPostion1;
    int swapPostion2;
    int RESET_NUMBER = 0;

    void confirm();
    void reset();
    void cancel();
};

#endif // SWAPFRAMES_H
