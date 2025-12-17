#include "C:/Users/User/Desktop/HIS/include/RoomSearchDialog.h"
#include "C:/Users/User/Desktop/HIS/include/DeluxeRoom.h"
#include "C:/Users/User/Desktop/HIS/include/SuiteRoom.h"
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QHeaderView>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtGui/QColor>

RoomSearchDialog::RoomSearchDialog(HotelSystem* hotelSystem, QWidget* parent)
    : QDialog(parent), hotelSystem(hotelSystem) {
    setupUI();
    setWindowTitle("Поиск комнаты");
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
    setModal(true);
    setMinimumSize(900, 600);
    resize(1000, 700);
    updateResultsTable();
}

RoomSearchDialog::~RoomSearchDialog() {
}

void RoomSearchDialog::setupUI() {
    setStyleSheet(
        "QDialog {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "        stop:0 #f8f9fa, stop:1 #e9ecef);"
        "}"
    );
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    QWidget* contentWidget = new QWidget(this);
    contentWidget->setStyleSheet("QWidget { background: transparent; }");
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(20);
    contentLayout->setContentsMargins(25, 25, 25, 25);
    
    QGroupBox* searchGroup = new QGroupBox("Критерии поиска", this);
    searchGroup->setStyleSheet(
        "QGroupBox {"
        "    background: white;"
        "    border-radius: 12px;"
        "    border: 1px solid #dee2e6;"
        "    font-weight: bold;"
        "    font-size: 14pt;"
        "    padding-top: 15px;"
        "    margin-top: 10px;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 15px;"
        "    padding: 0 5px;"
        "    color: #212529;"
        "}"
    );
    QVBoxLayout* searchLayout = new QVBoxLayout();
    searchLayout->setSpacing(10);
    searchLayout->setContentsMargins(15, 20, 15, 15);
    
    searchWiFiCheckBox = new QCheckBox("WiFi", this);
    searchWiFiCheckBox->setStyleSheet(
        "QCheckBox {"
        "    color: #495057;"
        "    font-size: 10pt;"
        "    spacing: 8px;"
        "}"
        "QCheckBox::indicator {"
        "    width: 18px;"
        "    height: 18px;"
        "    border: 2px solid #dee2e6;"
        "    border-radius: 4px;"
        "    background: white;"
        "}"
        "QCheckBox::indicator:checked {"
        "    background-color: #27ae60;"
        "    border-color: #27ae60;"
        "}"
    );
    
    searchMiniBarCheckBox = new QCheckBox("Мини-бар", this);
    searchMiniBarCheckBox->setStyleSheet(searchWiFiCheckBox->styleSheet());
    
    searchBalconyCheckBox = new QCheckBox("Балкон", this);
    searchBalconyCheckBox->setStyleSheet(searchWiFiCheckBox->styleSheet());
    
    searchJacuzziCheckBox = new QCheckBox("Джакузи", this);
    searchJacuzziCheckBox->setStyleSheet(searchWiFiCheckBox->styleSheet());
    
    QHBoxLayout* bedLayout = new QHBoxLayout();
    QLabel* bedLabel = new QLabel("Кроватей:", this);
    bedLabel->setStyleSheet("QLabel { color: #495057; font-size: 10pt; }");
    bedLayout->addWidget(bedLabel);
    searchBedCountSpinBox = new QSpinBox(this);
    searchBedCountSpinBox->setMinimum(0);
    searchBedCountSpinBox->setMaximum(10);
    searchBedCountSpinBox->setValue(0);
    searchBedCountSpinBox->setSpecialValueText("Любое");
    searchBedCountSpinBox->setStyleSheet(
        "QSpinBox {"
        "    background: white;"
        "    border: 1px solid #dee2e6;"
        "    border-radius: 4px;"
        "    padding: 5px;"
        "    font-size: 10pt;"
        "    min-width: 100px;"
        "}"
        "QSpinBox:hover {"
        "    border-color: #27ae60;"
        "}"
        "QSpinBox:focus {"
        "    border-color: #27ae60;"
        "}"
    );
    bedLayout->addWidget(searchBedCountSpinBox);
    bedLayout->addStretch();
    
    QHBoxLayout* priceLayout = new QHBoxLayout();
    QLabel* priceLabel = new QLabel("Цена:", this);
    priceLabel->setStyleSheet("QLabel { color: #495057; font-size: 10pt; }");
    priceLayout->addWidget(priceLabel);
    searchPriceMinSpinBox = new QDoubleSpinBox(this);
    searchPriceMinSpinBox->setMinimum(0);
    searchPriceMinSpinBox->setMaximum(10000);
    searchPriceMinSpinBox->setValue(0);
    searchPriceMinSpinBox->setSuffix(" $");
    searchPriceMinSpinBox->setStyleSheet(
        "QDoubleSpinBox {"
        "    background: white;"
        "    border: 1px solid #dee2e6;"
        "    border-radius: 4px;"
        "    padding: 5px;"
        "    font-size: 10pt;"
        "    min-width: 100px;"
        "}"
        "QDoubleSpinBox:hover {"
        "    border-color: #27ae60;"
        "}"
        "QDoubleSpinBox:focus {"
        "    border-color: #27ae60;"
        "}"
    );
    searchPriceMaxSpinBox = new QDoubleSpinBox(this);
    searchPriceMaxSpinBox->setMinimum(0);
    searchPriceMaxSpinBox->setMaximum(10000);
    searchPriceMaxSpinBox->setValue(10000);
    searchPriceMaxSpinBox->setSuffix(" $");
    searchPriceMaxSpinBox->setStyleSheet(searchPriceMinSpinBox->styleSheet());
    priceLayout->addWidget(searchPriceMinSpinBox);
    QLabel* dashLabel = new QLabel("-", this);
    dashLabel->setStyleSheet("QLabel { color: #495057; font-size: 10pt; padding: 0 5px; }");
    priceLayout->addWidget(dashLabel);
    priceLayout->addWidget(searchPriceMaxSpinBox);
    priceLayout->addStretch();
    
    clearSearchButton = new QPushButton("Очистить поиск", this);
    clearSearchButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #95a5a6;"
        "    color: white;"
        "    font-size: 9pt;"
        "    font-weight: bold;"
        "    padding: 6px 14px;"
        "    border: none;"
        "    border-radius: 4px;"
        "    min-width: 90px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #7f8c8d;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #6c7a7b;"
        "}"
    );
    clearSearchButton->setCursor(Qt::PointingHandCursor);
    
    searchLayout->addWidget(searchWiFiCheckBox);
    searchLayout->addWidget(searchMiniBarCheckBox);
    searchLayout->addWidget(searchBalconyCheckBox);
    searchLayout->addWidget(searchJacuzziCheckBox);
    searchLayout->addLayout(bedLayout);
    searchLayout->addLayout(priceLayout);
    searchLayout->addWidget(clearSearchButton);
    
    searchGroup->setLayout(searchLayout);
    contentLayout->addWidget(searchGroup);
    
    QGroupBox* resultsGroup = new QGroupBox("Результаты поиска", this);
    resultsGroup->setStyleSheet(
        "QGroupBox {"
        "    background: white;"
        "    border-radius: 12px;"
        "    border: 1px solid #dee2e6;"
        "    font-weight: bold;"
        "    font-size: 14pt;"
        "    padding-top: 15px;"
        "    margin-top: 10px;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 15px;"
        "    padding: 0 5px;"
        "    color: #212529;"
        "}"
    );
    QVBoxLayout* resultsGroupLayout = new QVBoxLayout();
    resultsGroupLayout->setContentsMargins(15, 20, 15, 15);
    
    resultsTable = new QTableWidget(this);
    resultsTable->setColumnCount(6);
    resultsTable->setHorizontalHeaderLabels(QStringList() << "№" << "Тип" << "Статус"
        << "Гость" << "Цена/ночь" << "Кровати");
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultsTable->horizontalHeader()->setStretchLastSection(true);
    resultsTable->setAlternatingRowColors(true);
    resultsTable->setStyleSheet(
        "QTableWidget {"
        "    background: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    gridline-color: #e9ecef;"
        "    font-size: 9pt;"
        "}"
        "QTableWidget::item {"
        "    padding: 5px;"
        "    border: none;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #d5f4e6;"
        "    color: #212529;"
        "}"
        "QHeaderView::section {"
        "    background-color: #f8f9fa;"
        "    color: #212529;"
        "    font-weight: bold;"
        "    font-size: 10pt;"
        "    padding: 8px;"
        "    border: none;"
        "    border-bottom: 2px solid #dee2e6;"
        "}"
    );
    resultsGroupLayout->addWidget(resultsTable);
    resultsGroup->setLayout(resultsGroupLayout);
    contentLayout->addWidget(resultsGroup, 1);
    
    mainLayout->addWidget(contentWidget, 1);
    
    QWidget* footerWidget = new QWidget(this);
    footerWidget->setStyleSheet(
        "QWidget {"
        "    background: white;"
        "    padding: 15px;"
        "    border-top: 2px solid #e9ecef;"
        "}"
    );
    footerWidget->setFixedHeight(70);
    
    QHBoxLayout* footerLayout = new QHBoxLayout(footerWidget);
    footerLayout->setContentsMargins(20, 0, 20, 0);
    footerLayout->addStretch();
    
    QPushButton* closeButton = new QPushButton("Закрыть", footerWidget);
    closeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #27ae60;"
        "    color: white;"
        "    font-size: 12pt;"
        "    font-weight: bold;"
        "    padding: 10px 30px;"
        "    border: none;"
        "    border-radius: 6px;"
        "    min-width: 150px;"
        "    min-height: 35px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #229954;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1e8449;"
        "}"
    );
    closeButton->setCursor(Qt::PointingHandCursor);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    footerLayout->addWidget(closeButton);
    
    mainLayout->addWidget(footerWidget);
    
    connect(searchWiFiCheckBox, &QCheckBox::toggled, this, &RoomSearchDialog::onSearchCriteriaChanged);
    connect(searchMiniBarCheckBox, &QCheckBox::toggled, this, &RoomSearchDialog::onSearchCriteriaChanged);
    connect(searchBalconyCheckBox, &QCheckBox::toggled, this, &RoomSearchDialog::onSearchCriteriaChanged);
    connect(searchJacuzziCheckBox, &QCheckBox::toggled, this, &RoomSearchDialog::onSearchCriteriaChanged);
    connect(searchBedCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &RoomSearchDialog::onSearchCriteriaChanged);
    connect(searchPriceMinSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &RoomSearchDialog::onSearchCriteriaChanged);
    connect(searchPriceMaxSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &RoomSearchDialog::onSearchCriteriaChanged);
    connect(clearSearchButton, &QPushButton::clicked, this, &RoomSearchDialog::onClearSearch);
}

void RoomSearchDialog::onSearchCriteriaChanged() {
    updateResultsTable();
}

void RoomSearchDialog::onClearSearch() {
    searchWiFiCheckBox->setChecked(false);
    searchMiniBarCheckBox->setChecked(false);
    searchBalconyCheckBox->setChecked(false);
    searchJacuzziCheckBox->setChecked(false);
    searchBedCountSpinBox->setValue(0);
    searchPriceMinSpinBox->setValue(0);
    searchPriceMaxSpinBox->setValue(10000);
    updateResultsTable();
}

void RoomSearchDialog::updateResultsTable() {
    resultsTable->setRowCount(0);

    for (const auto& roomPtr : *hotelSystem) {
        if (!roomPtr) continue;

        const Room* room = roomPtr.get();
        
        if (!matchesSearchCriteria(room)) {
            continue;
        }
        
        int row = resultsTable->rowCount();
        resultsTable->insertRow(row);

        resultsTable->setItem(row, 0, new QTableWidgetItem(QString::number(room->getRoomNumber())));
        resultsTable->setItem(row, 1, new QTableWidgetItem(getRoomTypeString(room)));
        resultsTable->setItem(row, 2, new QTableWidgetItem(getStatusString(room->getStatus())));
        const Guest* guest = room->getGuest();
        resultsTable->setItem(row, 3, new QTableWidgetItem(
            guest ? QString::fromStdString(guest->toShortString()) : "-"));
        resultsTable->setItem(row, 4, new QTableWidgetItem("$" + QString::number(static_cast<int>(room->getPricePerDay()))));
        resultsTable->setItem(row, 5, new QTableWidgetItem(QString::number(room->getBedCount())));

        QColor color;
        switch (room->getStatus()) {
        case RoomStatus::AVAILABLE:
            color = QColor(200, 255, 200);
            break;
        case RoomStatus::BOOKED:
            color = QColor(255, 255, 200);
            break;
        case RoomStatus::OCCUPIED:
            color = QColor(255, 200, 200);
            break;
        default:
            color = QColor(235, 235, 235);
            break;
        }

        for (int col = 0; col < 6; ++col) {
            if (QTableWidgetItem* item = resultsTable->item(row, col)) {
                item->setBackground(color);
            }
        }
    }

    resultsTable->resizeColumnsToContents();
}

bool RoomSearchDialog::matchesSearchCriteria(const Room* room) const {
    if (!room) return false;
    
    if (searchWiFiCheckBox->isChecked() && !room->getHasWiFi()) {
        return false;
    }
    
    if (searchBedCountSpinBox->value() > 0 && room->getBedCount() != searchBedCountSpinBox->value()) {
        return false;
    }
    
    double price = room->getPricePerDay();
    if (price < searchPriceMinSpinBox->value() || price > searchPriceMaxSpinBox->value()) {
        return false;
    }
    
    bool hasMiniBar = false;
    bool hasBalcony = false;
    bool hasJacuzzi = false;
    
    if (auto dr = dynamic_cast<const DeluxeRoom*>(room)) {
        hasMiniBar = dr->getHasMiniBar();
        hasBalcony = dr->getHasBalcony();
    }
    
    if (auto sr = dynamic_cast<const SuiteRoom*>(room)) {
        hasMiniBar = sr->getHasMiniBar();
        hasBalcony = sr->getHasBalcony();
        hasJacuzzi = sr->getHasJacuzzi();
    }
    
    if (searchMiniBarCheckBox->isChecked() && !hasMiniBar) {
        return false;
    }
    
    if (searchBalconyCheckBox->isChecked() && !hasBalcony) {
        return false;
    }
    
    if (searchJacuzziCheckBox->isChecked() && !hasJacuzzi) {
        return false;
    }
    
    return true;
}

QString RoomSearchDialog::getStatusString(RoomStatus status) {
    switch (status) {
    case RoomStatus::AVAILABLE:
        return "Доступен";
    case RoomStatus::BOOKED:
        return "Забронирован";
    case RoomStatus::OCCUPIED:
        return "Занят";
    default:
        return "Неизвестно";
    }
}

QString RoomSearchDialog::getRoomTypeString(const Room* room) {
    if (!room) return "Неизвестно";
    
    int beds = room->getBedCount();
    
    bool hasMiniBar = false;
    bool hasBalcony = false;
    bool hasJacuzzi = false;
    
    if (auto dr = dynamic_cast<const DeluxeRoom*>(room)) {
        hasMiniBar = dr->getHasMiniBar();
        hasBalcony = dr->getHasBalcony();
    }
    
    if (auto sr = dynamic_cast<const SuiteRoom*>(room)) {
        hasMiniBar = sr->getHasMiniBar();
        hasBalcony = sr->getHasBalcony();
        hasJacuzzi = sr->getHasJacuzzi();
    }
    
    if (beds == 4) {
        return "8-местный";
    } else if (beds == 3) {
        return "6-местный";
    } else if (beds == 2 && !hasMiniBar && !hasBalcony) {
        return "4-местный женский";
    } else if (beds == 1 && !hasMiniBar && !hasBalcony) {
        return "Твин Джуниор";
    } else if (beds == 1 && hasMiniBar && hasBalcony && !hasJacuzzi) {
        double price = room->getPricePerDay();
        if (price <= 25) {
            return "Двухместный Эконом";
        } else if (price <= 28) {
            return "Loft Double";
        } else {
            return "Стандартный Дабл";
        }
    } else if (beds == 2 && hasMiniBar && hasBalcony && hasJacuzzi) {
        return "Семейный";
    }
    
    return QString::fromStdString(room->getRoomType());
}

