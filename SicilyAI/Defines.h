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

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdio>

using namespace std;

struct SicilyConnect{
    bool update;
    int boxLife;
    char text[512];
};

#endif
