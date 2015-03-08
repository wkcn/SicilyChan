#include "Sicily.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Sicily w;
    /*
    QDesktopWidget *desktopWidgt = QApplication::desktop();
    QRect deskRect = desktopWidgt->availableGeometry();
    int width = deskRect.width();
    int height = deskRect.height();

    w.move(width-300,height-440-350);
    */
    w.show();
    int ret = a.exec();
    /*
    if (ret == 773) {
        qDebug("RUn");
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        return 0;
    }
    */
    return ret;
}
