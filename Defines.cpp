#include "Defines.h"

QDir directoryOf(const QString &subdir)
{
    QDir dir(QApplication::applicationDirPath());

#if defined(Q_OS_WIN)
    if (dir.dirName().toLower() == "debug"
            || dir.dirName().toLower() == "release"
            || dir.dirName().toLower() == "bin")
        dir.cdUp();
#elif defined(Q_OS_MAC)
    if (dir.dirName() == "MacOS"){
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
    }
#endif
    dir.cd(subdir);
    return dir;
}

QString GetFileDir(QString file){
    return directoryOf("").absoluteFilePath(file);
}

bool IsLeapYear(int year){
        return (year%400==0||(year%100!=0&&year%4==0));
}
int YearHaveDay(int year){
        if (IsLeapYear(year))return 366;
            return 365;
}
int MonthHaveDay(int month,bool leap){
        if (month==4||month==6||month==9||month==11)return 30;
            if (month==2)return leap?29:28;
                return 31;
}

Date GetDate(int offsetHour){
    int t = time(0) + offsetHour * 60 *60;
    Date d;
    d.second = t % 60;
    t /= 60;
    d.minute = t % 60;
    t /= 60;
    d.hour = t % 24;
    t /= 24;
    //从1970年1月1日开始偏移日期
    d.year = 1970;
    d.month = 1;
    d.day = 1;
    int temp;
    while(t>=(temp=YearHaveDay(d.year))){t-=temp;d.year++;}
    while(t>=(temp=MonthHaveDay(d.month,IsLeapYear(d.year)))){t-=temp;d.month++;}
    d.day += t;
    return d;
}

int GetClock(){
    time_t ltime1;
    struct TIMEB tstruct1;
    ftime (&tstruct1);            // start time ms
    time (&ltime1);               // start time s
    return ltime1 * 1000 + tstruct1.millitm;
}
