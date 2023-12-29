#ifndef INSERTFRAMES_H
#define INSERTFRAMES_H

#include <QMainWindow>

namespace Ui {
class InsertFrames;
}

class InsertFrames : public QMainWindow
{
    Q_OBJECT

public:
    explicit InsertFrames(QWidget *parent = nullptr);
    ~InsertFrames();

public slots:
    void setInsertButton(int buttonNumber, int sizeOfFrames);

signals:
    void insertFrame(int oldFrameIndex, int newFrameIndex);
    void restoreButtons(bool restore);
    void cancelled();

private:
    Ui::InsertFrames *ui;

    bool isLCDSet = false;
    bool isLCD2Set = false;
    int framePosition;
    int insertPosition;
    int RESET_NUMBER = 0;

    void confirm();
    void reset();
    void cancel();
    void getInput(int index);
};

#endif // INSERTFRAMES_H
