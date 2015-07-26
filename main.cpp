#include "Sicily.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font  = a.font();
    font.setPointSize(12);
    a.setFont(font);
    a.setWindowIcon(QIcon("ico.png"));  //貌似必须要在根目录
    Sicily w;
    w.show();
    int ret = a.exec();
    return ret;
}
