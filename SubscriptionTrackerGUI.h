#pragma once

#ifndef SUBSCRIPTIONTRACKERGUI_H
#define SUBSCRIPTIONTRACKERGUI_H

#include <QtWidgets/QMainWindow>
#include <QTableWidget> 
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QWidget>

#include "SubscriptionManager.h"

class SubscriptionTrackerGUI : public QMainWindow
{
    Q_OBJECT

private:
    double usdRate = 2.70;
    double eurRate = 2.90;
    SubscriptionManager manager;

    QLabel* totalAmountLabel;
    QLabel* subCountLabel;
    QLabel* nextPaymentLabel;

    QLineEdit* searchInput;
    QTableWidget* tableWidget;

    QLineEdit* nameInput;
    QLineEdit* priceInput;
    QComboBox* currencyInput;
    QComboBox* intervalInput;
    QLineEdit* dayInput;

    QPushButton* saveButton;
    QPushButton* clearButton;
    QPushButton* deleteButton;

    void setupUI();
    void applyStyles();

    QWidget* buildHeader();
    QWidget* buildControlsBar();
    QWidget* buildTableCard();
    QWidget* buildQuickAddCard();
    QWidget* buildFooter();

    QLabel* makeLabel(const QString& text, const QString& objectName);
    QWidget* makeField(const QString& caption, QWidget* field);

    QWidget* makeNameCell(const QString& name);
    QWidget* makePriceCell(double price, const QString& currency);

    void updateHeaderSummary();
    void loadTableData(const vector<Sub>& data);

    static QString currencySymbol(const QString& currency);
    static QString formatMoney(double price, const QString& currency);
    static QString initialOf(const QString& name);
    static QString nextPaymentText(int billingDay);

private slots:
    void onSaveClicked();
    void onClearClicked();
    void onDeleteClicked();
    void onSearchChanged(const QString& text);
    void checkUpcomingNotifications();

public:
    SubscriptionTrackerGUI(QWidget* parent = nullptr);
    ~SubscriptionTrackerGUI() = default;
};

#endif
