#ifndef SICILYAI_H
#define SICILYAI_H

#include "Defines.h"

namespace Ui {
class SicilyAI;
}

class SicilyAI : public QMainWindow
{
    Q_OBJECT

public:
    explicit SicilyAI(QWidget *parent = 0);
    ~SicilyAI();
private slots:
    void timerUpDate();
    void checkTime();

private:
    Ui::SicilyAI *ui;
    QSharedMemory sharedMem;
    SicilyConnect *sicilyConnect;
};

#endif // SICILYAI_H
