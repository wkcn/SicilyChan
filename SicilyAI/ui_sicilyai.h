/********************************************************************************
** Form generated from reading UI file 'sicilyai.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SICILYAI_H
#define UI_SICILYAI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SicilyAI
{
public:
    QWidget *centralWidget;
    QTextEdit *textEdit;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SicilyAI)
    {
        if (SicilyAI->objectName().isEmpty())
            SicilyAI->setObjectName(QStringLiteral("SicilyAI"));
        SicilyAI->resize(400, 300);
        centralWidget = new QWidget(SicilyAI);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(40, 100, 301, 61));
        SicilyAI->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SicilyAI);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        SicilyAI->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SicilyAI);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SicilyAI->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SicilyAI);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SicilyAI->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());

        retranslateUi(SicilyAI);

        QMetaObject::connectSlotsByName(SicilyAI);
    } // setupUi

    void retranslateUi(QMainWindow *SicilyAI)
    {
        SicilyAI->setWindowTitle(QApplication::translate("SicilyAI", "SicilyAI", 0));
        menu->setTitle(QApplication::translate("SicilyAI", "\350\217\234\345\215\225", 0));
    } // retranslateUi

};

namespace Ui {
    class SicilyAI: public Ui_SicilyAI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SICILYAI_H
