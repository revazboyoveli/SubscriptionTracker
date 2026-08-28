/********************************************************************************
** Form generated from reading UI file 'SubscriptionTrackerGUI.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUBSCRIPTIONTRACKERGUI_H
#define UI_SUBSCRIPTIONTRACKERGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SubscriptionTrackerGUIClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SubscriptionTrackerGUIClass)
    {
        if (SubscriptionTrackerGUIClass->objectName().isEmpty())
            SubscriptionTrackerGUIClass->setObjectName("SubscriptionTrackerGUIClass");
        SubscriptionTrackerGUIClass->resize(600, 400);
        menuBar = new QMenuBar(SubscriptionTrackerGUIClass);
        menuBar->setObjectName("menuBar");
        SubscriptionTrackerGUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SubscriptionTrackerGUIClass);
        mainToolBar->setObjectName("mainToolBar");
        SubscriptionTrackerGUIClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(SubscriptionTrackerGUIClass);
        centralWidget->setObjectName("centralWidget");
        SubscriptionTrackerGUIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SubscriptionTrackerGUIClass);
        statusBar->setObjectName("statusBar");
        SubscriptionTrackerGUIClass->setStatusBar(statusBar);

        retranslateUi(SubscriptionTrackerGUIClass);

        QMetaObject::connectSlotsByName(SubscriptionTrackerGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *SubscriptionTrackerGUIClass)
    {
        SubscriptionTrackerGUIClass->setWindowTitle(QCoreApplication::translate("SubscriptionTrackerGUIClass", "SubscriptionTrackerGUI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SubscriptionTrackerGUIClass: public Ui_SubscriptionTrackerGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBSCRIPTIONTRACKERGUI_H
