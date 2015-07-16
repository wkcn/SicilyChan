#ifndef DEFINES_H
#define  DEFINES_H
#define _CRT_SECURE_NO_WARNINGS
#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QPainter>
#include <QApplication>
#include <QSharedMemory>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QProcess>
#include <QClipboard>
#include <QThread>
#include <QBitmap>

//#include <Python.h>

#include <sys/timeb.h>
#if defined(WIN32)
# define  TIMEB    _timeb
# define  ftime    _ftime
//windows 的编码问题
#pragma execution_character_set("utf-8")
#else
#define TIMEB timeb
#endif

#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <cmath>
#include <fstream>
#include <map>

using namespace std;

struct SicilyConnect{
    bool update;
    int boxLife;
    char text[512];
};

QDir directoryOf(const QString &subdir);
QString GetQDir(QString file);
string GetDataDir(string file);
QString GetFileDir(QString file);

bool IsLeapYear(int year);
int YearHaveDay(int year);
int MonthHaveDay(int month,bool leap);

struct Date{
    int year,month,day;
    int hour,minute,second;
};

Date GetDate(int offsetHour = 0);

int GetClock();

#endif
