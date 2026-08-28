#include "SubscriptionTrackerGUI.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QFont>
#include <QFontDatabase>
#include <QGridLayout>
#include <QScrollArea>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QIcon>
#include <QSystemTrayIcon>
#include <algorithm>
#include <QTimer>

SubscriptionTrackerGUI::SubscriptionTrackerGUI(QWidget* parent)
    : QMainWindow(parent)
{
    setupUI();
    applyStyles();
    loadTableData(manager.getAllSubs());
    QTimer::singleShot(1000, this, &SubscriptionTrackerGUI::checkUpcomingNotifications);
}

void SubscriptionTrackerGUI::setupUI() {
    setWindowIcon(QIcon("app_icon.png"));
    setWindowIcon(QIcon("app_icon.png"));
    setWindowTitle("Subscription Tracker");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralRoot");


    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(48, 48, 48, 32);
    mainLayout->setSpacing(24);

    mainLayout->addWidget(buildHeader());
    mainLayout->addSpacing(16);
    mainLayout->addWidget(buildControlsBar());
    mainLayout->addWidget(buildTableCard(), 1);
    mainLayout->addWidget(buildQuickAddCard());
    mainLayout->addSpacing(8);
    mainLayout->addWidget(buildFooter());

    setCentralWidget(centralWidget);
    resize(920, 860);

    connect(saveButton, &QPushButton::clicked, this, &SubscriptionTrackerGUI::onSaveClicked);
    connect(clearButton, &QPushButton::clicked, this, &SubscriptionTrackerGUI::onClearClicked);
    connect(deleteButton, &QPushButton::clicked, this, &SubscriptionTrackerGUI::onDeleteClicked);
    connect(searchInput, &QLineEdit::textChanged, this, &SubscriptionTrackerGUI::onSearchChanged);
}

QLabel* SubscriptionTrackerGUI::makeLabel(const QString& text, const QString& objectName) {
    QLabel* label = new QLabel(text, this);
    label->setObjectName(objectName);
    return label;
}

QWidget* SubscriptionTrackerGUI::buildHeader() {
    QWidget* header = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(header);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(24);

    QVBoxLayout* left = new QVBoxLayout();
    left->setSpacing(6);

    left->addWidget(makeLabel("TRACK YOUR SUBSCRIPTIONS", "eyebrow"));

    totalAmountLabel = makeLabel("$0.00 /mo", "totalPrice");
    left->addWidget(totalAmountLabel);

    subCountLabel = makeLabel("Tracking 0 recurring services.", "bodyMuted");
    subCountLabel->setWordWrap(true);
    subCountLabel->setMaximumWidth(360);
    left->addWidget(subCountLabel);

    layout->addLayout(left);
    layout->addStretch();

    QFrame* divider = new QFrame(this);
    divider->setObjectName("vDivider");
    divider->setFrameShape(QFrame::VLine);
    divider->setFixedWidth(1);
    divider->setFixedHeight(48);
    layout->addWidget(divider, 0, Qt::AlignBottom);

    QVBoxLayout* right = new QVBoxLayout();
    right->setSpacing(4);
    right->addStretch();
    right->addWidget(makeLabel("NEXT PAYMENT", "eyebrowSmall"));
    nextPaymentLabel = makeLabel("No subscriptions", "bodyStrong");
    right->addWidget(nextPaymentLabel);

    layout->addLayout(right);
    return header;
}

QWidget* SubscriptionTrackerGUI::buildControlsBar() {
    QWidget* bar = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(bar);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(12);

    searchInput = new QLineEdit(this);
    searchInput->setObjectName("searchField");
    searchInput->setPlaceholderText("Search subscriptions...");
    searchInput->setFixedHeight(38);
    searchInput->setMaximumWidth(340);
    searchInput->setClearButtonEnabled(true);

    deleteButton = new QPushButton("Delete Selected", this);
    deleteButton->setObjectName("deleteBtn");
    deleteButton->setFixedHeight(38);
    deleteButton->setCursor(Qt::PointingHandCursor);

    layout->addWidget(searchInput);
    layout->addStretch();
    layout->addWidget(deleteButton);
    return bar;
}

QWidget* SubscriptionTrackerGUI::buildTableCard() {
    QFrame* card = new QFrame(this);
    card->setObjectName("tableCard");

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);

    tableWidget = new QTableWidget(this);
    tableWidget->setObjectName("subsTable");
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels(
        QStringList{ "SERVICE NAME", "INTERVAL", "BILLING", "PRICE" });

    QHeaderView* hh = tableWidget->horizontalHeader();
    hh->setSectionResizeMode(0, QHeaderView::Stretch);
    hh->setSectionResizeMode(1, QHeaderView::Fixed);
    hh->setSectionResizeMode(2, QHeaderView::Fixed);
    hh->setSectionResizeMode(3, QHeaderView::Fixed);
    tableWidget->setColumnWidth(1, 130);
    tableWidget->setColumnWidth(2, 130);
    tableWidget->setColumnWidth(3, 140);
    hh->setHighlightSections(false);
    hh->setFixedHeight(40);

    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setDefaultSectionSize(58);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setShowGrid(false);
    tableWidget->setFocusPolicy(Qt::NoFocus);
    tableWidget->setAlternatingRowColors(false);
    tableWidget->setFrameShape(QFrame::NoFrame);
    tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    layout->addWidget(tableWidget);
    return card;
}

QWidget* SubscriptionTrackerGUI::makeField(const QString& caption, QWidget* field) {
    QWidget* wrap = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(wrap);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);
    layout->addWidget(makeLabel(caption, "fieldCaption"));
    field->setFixedHeight(36);
    layout->addWidget(field);
    return wrap;
}

QWidget* SubscriptionTrackerGUI::buildQuickAddCard() {
    QFrame* card = new QFrame(this);
    card->setObjectName("cardFrame");

    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(24, 22, 24, 22);
    cardLayout->setSpacing(20);

    cardLayout->addWidget(makeLabel("Quick Add", "cardTitle"));

    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Netflix");

    priceInput = new QLineEdit(this);
    priceInput->setPlaceholderText("9.99");

    currencyInput = new QComboBox(this);
    currencyInput->addItems(QStringList{ "USD", "EUR", "GEL" });

    intervalInput = new QComboBox(this);
    intervalInput->addItems(QStringList{ "Monthly", "Annual" });

    dayInput = new QLineEdit(this);
    dayInput->setPlaceholderText("24");

    QHBoxLayout* fields = new QHBoxLayout();
    fields->setSpacing(16);
    fields->addWidget(makeField("SERVICE", nameInput), 2);
    fields->addWidget(makeField("PRICE", priceInput), 1);
    fields->addWidget(makeField("CURRENCY", currencyInput), 1);
    fields->addWidget(makeField("INTERVAL", intervalInput), 1);
    fields->addWidget(makeField("DUE DATE", dayInput), 1);
    cardLayout->addLayout(fields);

    clearButton = new QPushButton("Clear", this);
    clearButton->setObjectName("textBtn");
    clearButton->setCursor(Qt::PointingHandCursor);
    clearButton->setFixedHeight(36);

    saveButton = new QPushButton("Save Subscription", this);
    saveButton->setObjectName("primaryBtn");
    saveButton->setCursor(Qt::PointingHandCursor);
    saveButton->setFixedHeight(36);

    QHBoxLayout* actions = new QHBoxLayout();
    actions->setSpacing(12);
    actions->addStretch();
    actions->addWidget(clearButton);
    actions->addWidget(saveButton);
    cardLayout->addLayout(actions);

    return card;
}

QWidget* SubscriptionTrackerGUI::buildFooter() {
    QWidget* footer = new QWidget(this);
    QVBoxLayout* outer = new QVBoxLayout(footer);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(16);

    QFrame* line = new QFrame(this);
    line->setObjectName("hDivider");
    line->setFrameShape(QFrame::HLine);
    line->setFixedHeight(1);
    outer->addWidget(line);

    QHBoxLayout* row = new QHBoxLayout();
    row->setContentsMargins(0, 0, 0, 0);
    row->addStretch();
    row->addWidget(makeLabel("dkele25@freeuni.edu.ge", "footerBrand"));
    outer->addLayout(row);

    return footer;
}

void SubscriptionTrackerGUI::applyStyles() {
    const QString style = R"(
        /* ---------- surfaces ---------- */
        QMainWindow, #centralRoot {
            background-color: #09090b;
        }
        QWidget {
            color: #a1a1aa;
            font-family: 'Instrument Sans', 'Segoe UI', sans-serif;
            font-size: 13px;
        }

        /* ---------- typography ---------- */
        #eyebrow {
            color: #71717a;
            font-family: 'JetBrains Mono', Consolas, monospace;
            font-size: 11px;
            font-weight: bold;
            letter-spacing: 2px;
        }
        #eyebrowSmall {
            color: #71717a;
            font-family: 'JetBrains Mono', Consolas, monospace;
            font-size: 10px;
            font-weight: bold;
            letter-spacing: 1px;
        }
        #totalPrice {
            color: #f4f4f5;
            font-size: 44px;
            font-weight: 500;
            font-style: italic;
        }
        #bodyMuted {
            color: #a1a1aa;
            font-size: 13px;
        }
        #bodyStrong {
            color: #e4e4e7;
            font-size: 13px;
            font-weight: 600;
        }
        #vDivider, #hDivider {
            background-color: #27272a;
            border: none;
            max-height: 1px;
        }
        #vDivider {
            max-height: 48px;
            max-width: 1px;
        }
        #footerMono {
            color: #52525b;
            font-family: 'JetBrains Mono', Consolas, monospace;
            font-size: 10px;
            letter-spacing: 2px;
        }
        #footerBrand {
            color: #71717a;
            font-size: 10px;
            font-weight: bold;
            letter-spacing: 3px;
        }

        /* ---------- inputs ---------- */
        QLineEdit {
            background-color: #09090b;
            border: 1px solid #27272a;
            border-radius: 6px;
            padding: 6px 12px;
            color: #f4f4f5;
            selection-background-color: #3f3f46;
        }
        QLineEdit:hover { border: 1px solid #3f3f46; }
        QLineEdit:focus { border: 1px solid #52525b; }
        QLineEdit::placeholder { color: #52525b; }

        #searchField {
            background-color: rgba(24, 24, 27, 160);
            padding-left: 14px;
        }

        QComboBox {
            background-color: #09090b;
            border: 1px solid #27272a;
            border-radius: 6px;
            padding: 6px 12px;
            color: #f4f4f5;
        }
        QComboBox:hover { border: 1px solid #3f3f46; }
        QComboBox:focus { border: 1px solid #52525b; }
        QComboBox::drop-down { border: none; width: 22px; }
        QComboBox QAbstractItemView {
            background-color: #18181b;
            border: 1px solid #27272a;
            color: #e4e4e7;
            selection-background-color: #27272a;
            outline: none;
            padding: 4px;
        }

        #fieldCaption {
            color: #71717a;
            font-family: 'JetBrains Mono', Consolas, monospace;
            font-size: 10px;
            font-weight: bold;
            letter-spacing: 1px;
        }

        /* ---------- table ---------- */
        #tableCard {
            background-color: rgba(24, 24, 27, 90);
            border: 1px solid #1c1c1f;
            border-radius: 12px;
        }
        QTableWidget#subsTable {
            background-color: transparent;
            border: none;
            outline: none;
        }
        QHeaderView::section {
            background-color: transparent;
            color: #71717a;
            font-family: 'JetBrains Mono', Consolas, monospace;
            font-size: 10px;
            font-weight: bold;
            letter-spacing: 1px;
            border: none;
            border-bottom: 1px solid #232326;
            padding: 10px 20px;
            text-align: left;
        }
        QTableWidget#subsTable::item {
            border-bottom: 1px solid #1c1c1f;
            padding: 0px 20px;
            color: #a1a1aa;
        }
        QTableWidget#subsTable::item:selected {
            background-color: #1c1c20;
            color: #f4f4f5;
        }
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #27272a;
            border-radius: 4px;
            min-height: 32px;
        }
        QScrollBar::handle:vertical:hover { background: #3f3f46; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }

        /* rich cell widgets */
        #avatarTile {
            background-color: #27272a;
            border-radius: 4px;
            color: #a1a1aa;
            font-family: 'JetBrains Mono', Consolas, monospace;
            font-size: 11px;
            font-weight: bold;
        }
        #cellName {
            color: #f4f4f5;
            font-size: 13px;
            font-weight: 600;
        }
        #cellPrice {
            color: #f4f4f5;
            font-size: 13px;
            font-weight: 600;
        }
        #cellCurrency {
            color: #52525b;
            font-size: 10px;
        }

        /* ---------- quick add card ---------- */
        #cardFrame {
            background-color: rgba(24, 24, 27, 120);
            border: 1px solid #232326;
            border-radius: 12px;
        }
        #cardTitle {
            color: #f4f4f5;
            font-size: 14px;
            font-weight: 600;
        }

        /* ---------- buttons ---------- */
        #primaryBtn {
            background-color: #27272a;
            color: #e4e4e7;
            border: 1px solid #3f3f46;
            border-radius: 6px;
            padding: 6px 22px;
            font-size: 13px;
            font-weight: 600;
        }
        #primaryBtn:hover { background-color: #3f3f46; }
        #primaryBtn:pressed { background-color: #232326; }

        #textBtn {
            background-color: transparent;
            color: #71717a;
            border: none;
            padding: 6px 16px;
            font-size: 13px;
        }
        #textBtn:hover { color: #e4e4e7; }

        #deleteBtn {
            background-color: transparent;
            color: #a1a1aa;
            border: 1px solid #27272a;
            border-radius: 6px;
            padding: 6px 18px;
            font-size: 13px;
            font-weight: 600;
        }
        #deleteBtn:hover {
            background-color: #2a1416;
            border: 1px solid #4c1d1f;
            color: #f87171;
        }

        QToolTip {
            background-color: #18181b;
            color: #e4e4e7;
            border: 1px solid #27272a;
            padding: 4px 8px;
        }
    )";

    this->setStyleSheet(style);
}

QString SubscriptionTrackerGUI::currencySymbol(const QString& currency) {
    const QString c = currency.trimmed().toUpper();
    if (c == "USD") return "$";
    if (c == "EUR") return QString::fromUtf8("\u20AC");
    if (c == "GEL") return QString::fromUtf8("\u20BE");
    return QString();
}

QString SubscriptionTrackerGUI::formatMoney(double price, const QString& currency) {
    return currencySymbol(currency) + QString::number(price, 'f', 2);
}

QString SubscriptionTrackerGUI::initialOf(const QString& name) {
    const QString trimmed = name.trimmed();
    if (trimmed.isEmpty()) return "?";
    return trimmed.left(1).toUpper();
}

QString SubscriptionTrackerGUI::nextPaymentText(int billingDay) {
    if (billingDay < 1 || billingDay > 31) return "-";

    const QDate today = QDate::currentDate();
    int day = qMin(billingDay, today.daysInMonth());
    QDate candidate(today.year(), today.month(), day);

    if (candidate < today) {
        QDate next = today.addMonths(1);
        candidate = QDate(next.year(), next.month(),
            qMin(billingDay, next.daysInMonth()));
    }
    return candidate.toString("MMM d");
}

QWidget* SubscriptionTrackerGUI::makeNameCell(const QString& name) {
    QWidget* cell = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(cell);
    layout->setContentsMargins(20, 0, 12, 0);
    layout->setSpacing(12);

    QLabel* avatar = new QLabel(initialOf(name));
    avatar->setObjectName("avatarTile");
    avatar->setFixedSize(32, 32);
    avatar->setAlignment(Qt::AlignCenter);

    QLabel* label = new QLabel(name);
    label->setObjectName("cellName");

    layout->addWidget(avatar);
    layout->addWidget(label);
    layout->addStretch();
    return cell;
}

QWidget* SubscriptionTrackerGUI::makePriceCell(double price, const QString& currency) {
    QWidget* cell = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(cell);
    layout->setContentsMargins(12, 0, 20, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignVCenter);

    QLabel* amount = new QLabel(formatMoney(price, currency));
    amount->setObjectName("cellPrice");
    amount->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QLabel* code = new QLabel(currency.toUpper());
    code->setObjectName("cellCurrency");
    code->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    layout->addWidget(amount);
    layout->addWidget(code);
    return cell;
}

void SubscriptionTrackerGUI::updateHeaderSummary() {
    const auto& subs = manager.getAllSubs();

    double totalGEL = 0.0;
    for (const auto& sub : subs) {
        QString curr = QString::fromStdString(sub.currency).trimmed().toUpper();

        if (curr == "USD") {
            totalGEL += sub.price * 2.7;
        }
        else if (curr == "EUR") {
            totalGEL += sub.price * 2.9;
        }
        else {
            totalGEL += sub.price;
        }
    }

    totalAmountLabel->setText(QString::fromUtf8("\u20BE") + QString::number(totalGEL, 'f', 2) + " /mo");

    if (subs.empty()) {
        subCountLabel->setText("Tracking 0 recurring services.");
        nextPaymentLabel->setText("No subscriptions");
        return;
    }

    vector<string> seen;
    for (const auto& sub : subs) {
        bool found = false;
        for (const auto& c : seen) {
            if (c == sub.currency) { found = true; break; }
        }
        if (!found) seen.push_back(sub.currency);
    }

    subCountLabel->setText(
        QString("Tracking %1 recurring service commitments across %2 %3.")
        .arg(static_cast<int>(subs.size()))
        .arg(static_cast<int>(seen.size()))
        .arg(seen.size() == 1 ? "currency" : "currencies"));

    QDate today = QDate::currentDate();
    const Sub* soonestSub = nullptr;
    int minDaysLeft = 999999;

    for (const auto& sub : subs) {
        int day = std::min(sub.billingDay, today.daysInMonth());
        QDate dueDate(today.year(), today.month(), day);

        if (dueDate < today) {
            QDate nextMonth = today.addMonths(1);
            dueDate = QDate(nextMonth.year(), nextMonth.month(), std::min(sub.billingDay, nextMonth.daysInMonth()));
        }

        int daysLeft = static_cast<int>(today.daysTo(dueDate));
        if (daysLeft < minDaysLeft) {
            minDaysLeft = daysLeft;
            soonestSub = &sub;
        }
    }

    if (soonestSub) {
        nextPaymentLabel->setText(
            QString("%1  \u2022  %2")
            .arg(QString::fromStdString(soonestSub->name))
            .arg(nextPaymentText(soonestSub->billingDay)));
    }
}

void SubscriptionTrackerGUI::loadTableData(const vector<Sub>& data) {
    tableWidget->clearContents();
    tableWidget->setRowCount(0);

    for (size_t i = 0; i < data.size(); i++) {
        const int row = static_cast<int>(i);
        tableWidget->insertRow(row);

        const QString name = QString::fromStdString(data[i].name);
        const QString currency = QString::fromStdString(data[i].currency);

        QTableWidgetItem* nameItem = new QTableWidgetItem();
        nameItem->setData(Qt::UserRole, name);
        tableWidget->setItem(row, 0, nameItem);
        tableWidget->setCellWidget(row, 0, makeNameCell(name));

        QTableWidgetItem* intervalItem = new QTableWidgetItem("Monthly");
        intervalItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        tableWidget->setItem(row, 1, intervalItem);

        QTableWidgetItem* billingItem = new QTableWidgetItem(nextPaymentText(data[i].billingDay));
        billingItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        tableWidget->setItem(row, 2, billingItem);

        QTableWidgetItem* priceItem = new QTableWidgetItem();
        tableWidget->setItem(row, 3, priceItem);
        tableWidget->setCellWidget(row, 3, makePriceCell(data[i].price, currency));
    }

    updateHeaderSummary();
}

void SubscriptionTrackerGUI::onSaveClicked() {
    const string name = nameInput->text().trimmed().toStdString();
    const double price = priceInput->text().toDouble();
    const string currency = currencyInput->currentText().toStdString();
    const int day = dayInput->text().toInt();

    if (name.empty() || price <= 0 || day < 1 || day > 31) {
        QMessageBox::warning(this, "Invalid entry",
            "Enter a service name, a price above zero, and a billing day between 1 and 31.");
        return;
    }

    manager.addSub(Sub(name, price, currency, day));
    searchInput->clear();
    loadTableData(manager.getAllSubs());
    onClearClicked();
}

void SubscriptionTrackerGUI::onClearClicked() {
    nameInput->clear();
    priceInput->clear();
    dayInput->clear();
    currencyInput->setCurrentIndex(0);
    intervalInput->setCurrentIndex(0);
    nameInput->setFocus();
}

void SubscriptionTrackerGUI::onDeleteClicked() {
    const int currentRow = tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Nothing selected",
            "Select a subscription row first.");
        return;
    }

    QTableWidgetItem* item = tableWidget->item(currentRow, 0);
    const QString name = item ? item->data(Qt::UserRole).toString() : QString();

    const auto& subs = manager.getAllSubs();
    int realIndex = -1;
    for (size_t i = 0; i < subs.size(); i++) {
        if (QString::fromStdString(subs[i].name) == name) {
            realIndex = static_cast<int>(i);
            break;
        }
    }
    if (realIndex < 0) realIndex = currentRow;

    manager.deleteSub(realIndex);
    onSearchChanged(searchInput->text());
}

void SubscriptionTrackerGUI::onSearchChanged(const QString& text) {
    if (text.trimmed().isEmpty()) {
        loadTableData(manager.getAllSubs());
        return;
    }
    vector<Sub> filtered = manager.searchSub(text.toStdString());
    loadTableData(filtered);
}

void SubscriptionTrackerGUI::checkUpcomingNotifications() {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return;
    }

    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);

    if (!this->windowIcon().isNull()) {
        trayIcon->setIcon(this->windowIcon());
    }
    else {
        trayIcon->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    }

    trayIcon->show();

    QDate today = QDate::currentDate();

    for (const auto& sub : manager.getAllSubs()) {
        int day = std::min(sub.billingDay, today.daysInMonth());
        QDate dueDate(today.year(), today.month(), day);

        if (dueDate < today) {
            QDate nextMonth = today.addMonths(1);
            dueDate = QDate(nextMonth.year(), nextMonth.month(), std::min(sub.billingDay, nextMonth.daysInMonth()));
        }

        int daysLeft = static_cast<int>(today.daysTo(dueDate));

        if (daysLeft <= 3 && daysLeft >= 0) {
            QString msg;
            if (daysLeft == 0) {
                msg = QString("%1 payment is due today!").arg(QString::fromStdString(sub.name));
            }
            else if (daysLeft == 1) {
                msg = QString("%1 payment is due tomorrow!").arg(QString::fromStdString(sub.name));
            }
            else {
                msg = QString("%1 payment is due in %2 days!").arg(QString::fromStdString(sub.name)).arg(daysLeft);
            }

            trayIcon->showMessage("Subtrack Reminder", msg, QSystemTrayIcon::Information, 5000);
        }
    }
}