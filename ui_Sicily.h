/********************************************************************************
** Form generated from reading UI file 'Sicily.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SICILY_H
#define UI_SICILY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Sicily
{
public:
    QWidget *centralWidget;
    QLabel *sicily;
    QLabel *eye;

    void setupUi(QMainWindow *Sicily)
    {
        if (Sicily->objectName().isEmpty())
            Sicily->setObjectName(QStringLiteral("Sicily"));
        Sicily->setEnabled(true);
        Sicily->resize(300, 450);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Sicily->sizePolicy().hasHeightForWidth());
        Sicily->setSizePolicy(sizePolicy);
        Sicily->setMinimumSize(QSize(300, 450));
        Sicily->setMaximumSize(QSize(300, 450));
        Sicily->setToolTipDuration(-1);
        Sicily->setAnimated(false);
        Sicily->setUnifiedTitleAndToolBarOnMac(false);
        centralWidget = new QWidget(Sicily);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sicily = new QLabel(centralWidget);
        sicily->setObjectName(QStringLiteral("sicily"));
        sicily->setGeometry(QRect(0, 0, 300, 450));
        sicily->setMouseTracking(false);
        sicily->setScaledContents(false);
        sicily->setWordWrap(false);
        eye = new QLabel(centralWidget);
        eye->setObjectName(QStringLiteral("eye"));
        eye->setGeometry(QRect(0, 0, 300, 450));
        Sicily->setCentralWidget(centralWidget);

        retranslateUi(Sicily);

        QMetaObject::connectSlotsByName(Sicily);
    } // setupUi

    void retranslateUi(QMainWindow *Sicily)
    {
        Sicily->setWindowTitle(QApplication::translate("Sicily", "Sicily Chan", 0));
    } // retranslateUi

};

namespace Ui {
    class Sicily: public Ui_Sicily {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SICILY_H
