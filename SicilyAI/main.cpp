#include "sicilyai.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SicilyAI w;
    w.show();

    return a.exec();
}
