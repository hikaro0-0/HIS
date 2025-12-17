#include "C:/Users/User/Desktop/HIS/include/MainWindow.h"
#include "C:/Users/User/Desktop/HIS/include/Guest.h"
#include "C:/Users/User/Desktop/HIS/include/exceptions.h"
#include "C:/Users/User/Desktop/HIS/include/DeluxeRoom.h"
#include "C:/Users/User/Desktop/HIS/include/SuiteRoom.h"
#include "C:/Users/User/Desktop/HIS/include/WelcomeWindow.h"
#include "C:/Users/User/Desktop/HIS/include/RoomSearchDialog.h"
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QHeaderView>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QFileDialog>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QMessageBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QPushButton>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QHBoxLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QWidget>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QComboBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QDialog>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QVBoxLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QTextEdit>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QString>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtGui/QColor>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QTimer>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QPropertyAnimation>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QGraphicsEffect>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtGui/QPainter>
#include <ctime>

MainWindow::MainWindow(QWidget* parent, HotelSystem* sharedSystem)
    : QMainWindow(parent), hotelSystem(nullptr), ownsHotelSystem(false) {

    if (sharedSystem) {
        hotelSystem = sharedSystem;
        ownsHotelSystem = false;
    } else {
        hotelSystem = new HotelSystem("Отель");
        ownsHotelSystem = true;
        
        try {
            hotelSystem->loadFromFile("hotel_data_total");
        }
        catch (const FileException& e) {
        }
        catch (const std::exception& e) {
        }
    }

    setupUI();
    updateRoomTable();
    updateStatistics();
}

MainWindow::~MainWindow() {
    if (hotelSystem) {
        try {
            hotelSystem->saveToFile("hotel_data_total");
        } catch (...) {
        }
        
        if (ownsHotelSystem) {
            delete hotelSystem;
            hotelSystem = nullptr;
        }
    }
}

void MainWindow::setupUI() {
    setWindowTitle("Система управления отелем");
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
    setMinimumSize(1400, 900);
    resize(1400, 900);
    
    setStyleSheet(
        "QMainWindow {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "        stop:0 #f8f9fa, stop:1 #e9ecef);"
        "}"
    );

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setStyleSheet("QWidget { background: transparent; }");
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    QVBoxLayout* leftLayout = new QVBoxLayout();

    QGroupBox* roomGroup = new QGroupBox("Список номеров", this);
    roomGroup->setStyleSheet(
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
    QVBoxLayout* roomLayout = new QVBoxLayout(roomGroup);
    roomLayout->setSpacing(10);
    roomLayout->setContentsMargins(15, 20, 15, 15);
    
    QHBoxLayout* filterLayout = new QHBoxLayout();
    QLabel* filterLabel = new QLabel("Фильтр:", this);
    filterLabel->setStyleSheet("QLabel { color: #495057; font-weight: bold; font-size: 10pt; }");
    filterLayout->addWidget(filterLabel);
    
    filterButtonGroup = new QButtonGroup(this);
    filterAllButton = new QRadioButton("Все номера", this);
    filterAllButton->setStyleSheet(
        "QRadioButton {"
        "    color: #495057;"
        "    font-size: 10pt;"
        "    spacing: 8px;"
        "}"
        "QRadioButton::indicator {"
        "    width: 18px;"
        "    height: 18px;"
        "    border: 2px solid #dee2e6;"
        "    border-radius: 9px;"
        "    background: white;"
        "}"
        "QRadioButton::indicator:checked {"
        "    background-color: #27ae60;"
        "    border-color: #27ae60;"
        "}"
    );
    filterBookedButton = new QRadioButton("Бронь", this);
    filterBookedButton->setStyleSheet(filterAllButton->styleSheet());
    filterOccupiedButton = new QRadioButton("Заселён", this);
    filterOccupiedButton->setStyleSheet(filterAllButton->styleSheet());
    
    filterAllButton->setChecked(true);
    currentFilter = std::nullopt;
    
    filterButtonGroup->addButton(filterAllButton, 0);
    filterButtonGroup->addButton(filterBookedButton, 1);
    filterButtonGroup->addButton(filterOccupiedButton, 2);
    
    filterLayout->addWidget(filterAllButton);
    filterLayout->addWidget(filterBookedButton);
    filterLayout->addWidget(filterOccupiedButton);
    filterLayout->addStretch();
    
    connect(filterButtonGroup, QOverload<int>::of(&QButtonGroup::idClicked), this, &MainWindow::onFilterChanged);

    roomLayout->addLayout(filterLayout);

    roomTable = new QTableWidget(this);
    roomTable->setColumnCount(6);
    roomTable->setHorizontalHeaderLabels(QStringList() << "Тип" << "Статус"
        << "Гость" << "Цена/ночь" << "Кровати" << "Действия");
    roomTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    roomTable->setSelectionMode(QAbstractItemView::SingleSelection);
    roomTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    roomTable->setAlternatingRowColors(true);
    roomTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    roomTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    roomTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    roomTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    roomTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    roomTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    roomTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
    
    roomTable->setColumnWidth(0, 150);
    roomTable->setColumnWidth(1, 100);
    roomTable->setColumnWidth(2, 230);
    roomTable->setColumnWidth(3, 90);
    roomTable->setColumnWidth(4, 75);
    roomTable->setColumnWidth(5, 280);
    
    roomTable->horizontalHeader()->setMinimumSectionSize(35);
    roomTable->verticalHeader()->setDefaultSectionSize(40);
    roomTable->setStyleSheet(
        "QTableWidget {"
        "    background: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    gridline-color: #e9ecef;"
        "    font-size: 9pt;"
        "}"
        "QTableWidget::item {"
        "    padding: 8px 5px;"
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

    roomLayout->addWidget(roomTable);

    leftLayout->addWidget(roomGroup);
    
    clearButton = new QPushButton("Очистить данные", this);
    clearButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #e74c3c;"
        "    color: white;"
        "    font-weight: bold;"
        "    font-size: 9pt;"
        "    padding: 6px 14px;"
        "    border: none;"
        "    border-radius: 4px;"
        "    min-width: 90px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #c0392b;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #a93226;"
        "}"
    );
    clearButton->setCursor(Qt::PointingHandCursor);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearData);
    
    leftLayout->addWidget(clearButton, 0, Qt::AlignLeft);

    QVBoxLayout* rightLayout = new QVBoxLayout();

    QGroupBox* statsGroup = new QGroupBox("Статистика", this);
    statsGroup->setStyleSheet(
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
    QVBoxLayout* statsLayout = new QVBoxLayout(statsGroup);
    statsLayout->setContentsMargins(15, 20, 15, 15);

    statisticsLabel = new QLabel(this);
    statisticsLabel->setWordWrap(true);
    statisticsLabel->setStyleSheet("QLabel { background: transparent; padding: 0px; }");
    statsLayout->addWidget(statisticsLabel);
    rightLayout->addWidget(statsGroup);

    QPushButton* searchButton = new QPushButton("Поиск комнаты", this);
    searchButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #3498db;"
        "    color: white;"
        "    font-size: 11pt;"
        "    font-weight: bold;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 6px;"
        "    min-width: 150px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #21618c;"
        "}"
    );
    searchButton->setCursor(Qt::PointingHandCursor);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchRooms);
    rightLayout->addWidget(searchButton);
    
    rightLayout->addStretch();
    
    bookRoomButton = new QPushButton("Забронировать номер", this);
    bookRoomButton->setStyleSheet(
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
    bookRoomButton->setCursor(Qt::PointingHandCursor);
    connect(bookRoomButton, &QPushButton::clicked, this, &MainWindow::onBookRoom);
    
    rightLayout->addWidget(bookRoomButton, 0, Qt::AlignRight);

    mainLayout->addLayout(leftLayout, 2);
    mainLayout->addLayout(rightLayout, 1);
}

QWidget* MainWindow::createActionsWidget(const Room* room) {
    QWidget* actionsWidget = new QWidget();
    QHBoxLayout* actionsLayout = new QHBoxLayout(actionsWidget);
    actionsLayout->setContentsMargins(2, 2, 2, 2);
    actionsLayout->setSpacing(3);

    int roomNumber = room->getRoomNumber();
    
    QPushButton* detailsBtn = new QPushButton("Детали", actionsWidget);
        detailsBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: #95a5a6;"
            "    color: white;"
            "    font-size: 8pt;"
            "    font-weight: bold;"
            "    padding: 4px 10px;"
            "    border: none;"
            "    border-radius: 3px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #7f8c8d;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #6c7a7b;"
            "}"
        );
        detailsBtn->setCursor(Qt::PointingHandCursor);
    connect(detailsBtn, &QPushButton::clicked, [this, roomNumber]() {
        const Room* room = hotelSystem->findRoom(roomNumber);
        if (room) {
            showRoomDetailsDialog(room);
        }
    });
    actionsLayout->addWidget(detailsBtn);
    
    switch (room->getStatus()) {
    case RoomStatus::AVAILABLE: {
        QPushButton* bookBtn = new QPushButton("Забронировать", actionsWidget);
        bookBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: #27ae60;"
            "    color: white;"
            "    font-size: 8pt;"
            "    font-weight: bold;"
            "    padding: 4px 10px;"
            "    border: none;"
            "    border-radius: 3px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #229954;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #1e8449;"
            "}"
        );
        bookBtn->setCursor(Qt::PointingHandCursor);
        connect(bookBtn, &QPushButton::clicked, [this, roomNumber]() {
            const Room* room = hotelSystem->findRoom(roomNumber);
            if (room) {
            for (int i = 0; i < roomTable->rowCount(); ++i) {
                    QTableWidgetItem* typeItem = roomTable->item(i, 0);
                    if (typeItem && typeItem->text() == getRoomTypeString(room)) {
                    roomTable->selectRow(i);
                    onBookRoom();
                    break;
                    }
                }
            }
        });
        actionsLayout->addWidget(bookBtn);
        break;
    }
    case RoomStatus::BOOKED: {
        QPushButton* checkInBtn = new QPushButton("Заселить", actionsWidget);
        checkInBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: #3498db;"
            "    color: white;"
            "    font-size: 8pt;"
            "    font-weight: bold;"
            "    padding: 4px 10px;"
            "    border: none;"
            "    border-radius: 3px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #2980b9;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #21618c;"
            "}"
        );
        checkInBtn->setCursor(Qt::PointingHandCursor);
        connect(checkInBtn, &QPushButton::clicked, [this, roomNumber]() {
            const Room* room = hotelSystem->findRoom(roomNumber);
            if (room) {
            for (int i = 0; i < roomTable->rowCount(); ++i) {
                    QTableWidgetItem* typeItem = roomTable->item(i, 0);
                    if (typeItem && typeItem->text() == getRoomTypeString(room)) {
                    roomTable->selectRow(i);
                    onCheckIn();
                    break;
                    }
                }
            }
        });
        actionsLayout->addWidget(checkInBtn);
        
        QPushButton* cancelBtn = new QPushButton("Отменить", actionsWidget);
        cancelBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: #e74c3c;"
            "    color: white;"
            "    font-size: 8pt;"
            "    font-weight: bold;"
            "    padding: 4px 10px;"
            "    border: none;"
            "    border-radius: 3px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #c0392b;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #a93226;"
            "}"
        );
        cancelBtn->setCursor(Qt::PointingHandCursor);
        connect(cancelBtn, &QPushButton::clicked, [this, roomNumber]() {
            const Room* room = hotelSystem->findRoom(roomNumber);
            if (room) {
            for (int i = 0; i < roomTable->rowCount(); ++i) {
                    QTableWidgetItem* typeItem = roomTable->item(i, 0);
                    if (typeItem && typeItem->text() == getRoomTypeString(room)) {
                    roomTable->selectRow(i);
                    onCancelBooking();
                    break;
                    }
                }
            }
        });
        actionsLayout->addWidget(cancelBtn);
        break;
    }
    case RoomStatus::OCCUPIED: {
        QPushButton* checkOutBtn = new QPushButton("Выселить", actionsWidget);
        checkOutBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: #f39c12;"
            "    color: white;"
            "    font-size: 8pt;"
            "    font-weight: bold;"
            "    padding: 4px 10px;"
            "    border: none;"
            "    border-radius: 3px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #e67e22;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #d68910;"
            "}"
        );
        checkOutBtn->setCursor(Qt::PointingHandCursor);
        connect(checkOutBtn, &QPushButton::clicked, [this, roomNumber]() {
            const Room* room = hotelSystem->findRoom(roomNumber);
            if (room) {
            for (int i = 0; i < roomTable->rowCount(); ++i) {
                    QTableWidgetItem* typeItem = roomTable->item(i, 0);
                    if (typeItem && typeItem->text() == getRoomTypeString(room)) {
                    roomTable->selectRow(i);
                    onCheckOut();
                    break;
                    }
                }
            }
        });
        actionsLayout->addWidget(checkOutBtn);
        break;
    }
    }
    
    actionsLayout->addStretch();
    return actionsWidget;
}

void MainWindow::updateRoomTable() {
    roomTable->setRowCount(0);

    for (const auto& roomPtr : *hotelSystem) {
        if (!roomPtr) continue;

        const Room* room = roomPtr.get();
        
        if (currentFilter.has_value() && room->getStatus() != currentFilter.value()) {
            continue;
        }
        
        int row = roomTable->rowCount();
        roomTable->insertRow(row);
        roomTable->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(room->getRoomNumber())));

        QTableWidgetItem* item0 = new QTableWidgetItem(getRoomTypeString(room));
        item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item0->setToolTip(getRoomTypeString(room));
        roomTable->setItem(row, 0, item0);
        
        QTableWidgetItem* item1 = new QTableWidgetItem(getStatusString(room->getStatus()));
        item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        roomTable->setItem(row, 1, item1);
        
        const Guest* guest = room->getGuest();
        QString guestName = guest ? QString::fromStdString(guest->toShortString()) : "-";
        QTableWidgetItem* item2 = new QTableWidgetItem(guestName);
        item2->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item2->setToolTip(guestName);
        roomTable->setItem(row, 2, item2);
        
        QTableWidgetItem* item3 = new QTableWidgetItem("$" + QString::number(static_cast<int>(room->getPricePerDay())));
        item3->setTextAlignment(Qt::AlignCenter);
        roomTable->setItem(row, 3, item3);
        
        QTableWidgetItem* item4 = new QTableWidgetItem(QString::number(room->getBedCount()));
        item4->setTextAlignment(Qt::AlignCenter);
        roomTable->setItem(row, 4, item4);

        QWidget* actionsWidget = createActionsWidget(room);
        roomTable->setCellWidget(row, 5, actionsWidget);

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

        for (int col = 0; col < 5; ++col) {
            if (QTableWidgetItem* item = roomTable->item(row, col)) {
                item->setBackground(color);
            }
        }
    }
}

void MainWindow::updateStatistics() {
    QString hotelName = QString::fromStdString(hotelSystem->getHotelName());
    double revenue = hotelSystem->getTotalRevenue();
    double futureRevenue = hotelSystem->getFutureRevenue();
    
    QString stats = QString(
        "<div style='text-align: center; margin-bottom: 15px;'>"
        "<h2 style='color: #1a1a1a; margin: 0; font-size: 20pt; font-weight: bold;'>%1</h2>"
        "</div>"
        "<hr style='border: 2px solid #95a5a6; margin: 15px 0;'>"
        "<div style='font-size: 13pt; line-height: 2.0;'>"
        "<table style='width: 100%; border-collapse: collapse;'>"
        "<tr><td style='padding: 8px 0; color: #2c3e50; font-size: 13pt;'><b>Всего номеров:</b></td>"
        "<td style='text-align: right; padding: 8px 0; color: #1a1a1a; font-weight: bold; font-size: 13pt;'>%2</td></tr>"
        "<tr><td style='padding: 8px 0; color: #f39c12; font-size: 13pt;'><b>Забронировано:</b></td>"
        "<td style='text-align: right; padding: 8px 0; color: #f39c12; font-weight: bold; font-size: 13pt;'>%3</td></tr>"
        "<tr><td style='padding: 8px 0; color: #e74c3c; font-size: 13pt;'><b>Занято:</b></td>"
        "<td style='text-align: right; padding: 8px 0; color: #e74c3c; font-weight: bold; font-size: 13pt;'>%4</td></tr>"
        "</table>"
        "</div>"
        "<hr style='border: 2px solid #95a5a6; margin: 15px 0;'>"
        "<div style='text-align: center; margin-top: 15px;'>"
        "<div style='font-size: 14pt; color: #27ae60; font-weight: bold; margin-bottom: 5px;'>Общий доход</div>"
        "<div style='font-size: 18pt; color: #27ae60; font-weight: bold;'>$%5 <span style='font-size: 14pt; color: #7f8c8d;'>(+$%6)</span></div>"
        "</div>"
    )
        .arg(hotelName)
        .arg(hotelSystem->getTotalRooms())
        .arg(hotelSystem->getBookedRoomsCount())
        .arg(hotelSystem->getOccupiedRoomsCount())
        .arg(revenue, 0, 'f', 2)
        .arg(futureRevenue, 0, 'f', 2);
    
    statisticsLabel->setText(stats);
}

QString MainWindow::getStatusString(RoomStatus status) const {
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

QString MainWindow::getRoomTypeString(const Room* room) const {
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

void MainWindow::showRoomDetailsDialog(const Room* room) {
    if (!room) {
        return;
    }

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Детали номера " + QString::number(room->getRoomNumber()));
    dialog->setMinimumSize(400, 500);
    dialog->setStyleSheet(
        "QDialog {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "        stop:0 #f8f9fa, stop:1 #e9ecef);"
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    QTextEdit* infoText = new QTextEdit(dialog);
    infoText->setReadOnly(true);
    infoText->setStyleSheet(
        "QTextEdit {"
        "    background-color: white;"
        "    border: 1px solid #dee2e6;"
        "    border-radius: 8px;"
        "    padding: 15px;"
        "    font-size: 11pt;"
        "}"
    );

    QString info = QString("<b>Номер %1</b><br>").arg(room->getRoomNumber());
    info += QString("Тип: %1<br>").arg(getRoomTypeString(room));
    info += QString("Статус: %1<br>").arg(getStatusString(room->getStatus()));
    info += QString("Цена за ночь: $%1<br>").arg(static_cast<int>(room->getPricePerDay()));
    info += QString("Кровати: %1<br>").arg(room->getBedCount());
    
    RateType rateType = room->getRateType();
    QString rateName;
    switch (rateType) {
    case RateType::NonRefundable:
        rateName = "Невозвратный тариф";
        break;
    case RateType::FlexibleStart:
        rateName = "Гибкий старт";
        break;
    default:
        rateName = "Неизвестно";
        break;
    }
    info += QString("Тариф: %1<br>").arg(rateName);

    if (const Guest* guest = room->getGuest()) {
        info += QString("Гость: %1<br>").arg(QString::fromStdString(guest->getName()));
        if (guest->hasPhone()) {
            info += QString("Телефон: %1<br>").arg(QString::fromStdString(guest->getPhone()));
        }
    }
    if (room->getBookingTime() > 0) {
        info += QString("Время бронирования: %1<br>")
            .arg(QString::fromStdString(room->getBookingTimeString()));
    }
    
    if (room->getCheckInTime() > 0) {
        std::time_t checkInTime = room->getCheckInTime();
        std::tm* timeinfo = std::localtime(&checkInTime);
        if (timeinfo) {
            timeinfo->tm_hour = 14;
            timeinfo->tm_min = 0;
            timeinfo->tm_sec = 0;
            std::time_t checkInDate = std::mktime(timeinfo);
            info += QString("Дата заезда: %1<br>")
                .arg(QString::fromStdString(Room::timeToString(checkInDate)));
        }
    } else if (room->getPlannedCheckInTime() > 0) {
        info += QString("Дата заезда (план): %1<br>")
            .arg(QString::fromStdString(Room::timeToString(room->getPlannedCheckInTime())));
    } else if (room->getBookingTime() > 0 && room->getBookingDays() > 0) {
        std::time_t bookingTime = room->getBookingTime();
        std::tm* timeinfo = std::localtime(&bookingTime);
        if (timeinfo) {
            timeinfo->tm_mday += 1;
            timeinfo->tm_hour = 14;
            timeinfo->tm_min = 0;
            timeinfo->tm_sec = 0;
            std::time_t plannedCheckIn = std::mktime(timeinfo);
            info += QString("Дата заезда (план): %1<br>")
                .arg(QString::fromStdString(Room::timeToString(plannedCheckIn)));
        }
    }

    if (auto dr = dynamic_cast<const DeluxeRoom*>(room)) {
        info += QString("Мини-бар: %1<br>").arg(dr->getHasMiniBar() ? "Да" : "Нет");
        info += QString("Балкон: %1<br>").arg(dr->getHasBalcony() ? "Да" : "Нет");
    }

    if (auto sr = dynamic_cast<const SuiteRoom*>(room)) {
        info += QString("Мини-бар: %1<br>").arg(sr->getHasMiniBar() ? "Да" : "Нет");
        info += QString("Балкон: %1<br>").arg(sr->getHasBalcony() ? "Да" : "Нет");
        info += QString("Джакузи: %1<br>").arg(sr->getHasJacuzzi() ? "Да" : "Нет");
    }

    infoText->setHtml(info);
    layout->addWidget(infoText);

    QPushButton* closeButton = new QPushButton("Закрыть", dialog);
    closeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #27ae60;"
        "    color: white;"
        "    font-weight: bold;"
        "    font-size: 12pt;"
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
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeButton);

    dialog->exec();
    delete dialog;
}

void MainWindow::onFilterChanged() {
    int id = filterButtonGroup->checkedId();
    switch (id) {
    case 0:
        currentFilter = std::nullopt;
        break;
    case 1:
        currentFilter = RoomStatus::BOOKED;
        break;
    case 2:
        currentFilter = RoomStatus::OCCUPIED;
        break;
    }
    updateRoomTable();
}

void MainWindow::onSearchRooms() {
    RoomSearchDialog* dialog = new RoomSearchDialog(hotelSystem, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->exec();
}

void MainWindow::showError(const QString& message) {
    QLabel* notification = new QLabel(message, this);
    notification->setStyleSheet(
        "QLabel {"
        "    background-color: white;"
        "    color: #e74c3c;"
        "    padding: 15px 20px;"
        "    border-radius: 8px;"
        "    border: 2px solid #e74c3c;"
        "    font-size: 12pt;"
        "    font-weight: bold;"
        "    min-width: 300px;"
        "    max-width: 500px;"
        "}"
    );
    notification->setWordWrap(true);
    notification->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    notification->setAttribute(Qt::WA_DeleteOnClose);
    notification->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    notification->setAttribute(Qt::WA_TranslucentBackground);
    
    QPoint pos = mapToGlobal(QPoint(20, height() - notification->sizeHint().height() - 40));
    notification->move(pos);
    notification->resize(330, notification->sizeHint().height());
    notification->show();
    
    QTimer::singleShot(4000, notification, &QLabel::close);
}

void MainWindow::showSuccess(const QString& message) {
    QLabel* notification = new QLabel(message, this);
    notification->setStyleSheet(
        "QLabel {"
        "    background-color: white;"
        "    color: #27ae60;"
        "    padding: 15px 20px;"
        "    border-radius: 8px;"
        "    border: 2px solid #27ae60;"
        "    font-size: 12pt;"
        "    font-weight: bold;"
        "    min-width: 300px;"
        "    max-width: 500px;"
        "}"
    );
    notification->setWordWrap(true);
    notification->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    notification->setAttribute(Qt::WA_DeleteOnClose);
    notification->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    notification->setAttribute(Qt::WA_TranslucentBackground);
    
    QPoint pos = mapToGlobal(QPoint(20, height() - notification->sizeHint().height() - 40));
    notification->move(pos);
    notification->resize(330, notification->sizeHint().height());
    notification->show();
    
    QTimer::singleShot(3000, notification, &QLabel::close);
}

void MainWindow::onBookRoom() {
    try {
        hotelSystem->saveToFile("hotel_data_total");
    } catch (...) {
    }
    
    WelcomeWindow* welcomeWindow = new WelcomeWindow(this);
    welcomeWindow->setHotelSystem(hotelSystem);
    
    welcomeWindow->setAttribute(Qt::WA_DeleteOnClose, true);
    welcomeWindow->setWindowState(Qt::WindowMaximized);
    if (welcomeWindow->exec() == QDialog::Accepted) {
        hotelSystem = welcomeWindow->getHotelSystem();
        
        try {
            if (hotelSystem) {
                hotelSystem->loadFromFile("hotel_data_total");
            }
        } catch (...) {
        }
        updateRoomTable();
        updateStatistics();
    }
}

void MainWindow::onCheckIn() {
    int roomNumber = getSelectedRoomNumber();
    if (roomNumber <= 0) {
        showError("Пожалуйста, выберите номер из таблицы.");
        return;
    }

    try {
        hotelSystem->checkIn(roomNumber);
        showSuccess("Номер успешно заселен!");
        updateRoomTable();
        updateStatistics();
        autoSave();
    }
    catch (const BookingException& e) {
        showError(QString("Ошибка заселения: %1").arg(e.what()));
    }
    catch (const std::exception& e) {
        showError(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::onCheckOut() {
    int roomNumber = getSelectedRoomNumber();
    if (roomNumber <= 0) {
        showError("Пожалуйста, выберите номер из таблицы.");
        return;
    }

    try {
        if (hotelSystem->checkOut(roomNumber)) {
            bool removed = hotelSystem->removeRoom(roomNumber);
            if (removed) {
                showSuccess(QString("Номер %1 выселен и удален успешно!").arg(roomNumber));
            } else {
                showSuccess(QString("Номер %1 успешно выселен!").arg(roomNumber));
                showError("Предупреждение: Номер не может быть удален из системы.");
            }
            updateRoomTable();
            updateStatistics();
            autoSave();
        }
        else {
            showError("Невозможно выселить номер.");
        }
    }
    catch (const BookingException& e) {
        showError(QString("Ошибка выселения: %1").arg(e.what()));
    }
    catch (const std::exception& e) {
        showError(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::onCancelBooking() {
    int roomNumber = getSelectedRoomNumber();
    if (roomNumber <= 0) {
        showError("Пожалуйста, выберите номер из таблицы.");
        return;
    }

    try {
        if (hotelSystem->cancelBooking(roomNumber)) {
            bool removed = hotelSystem->removeRoom(roomNumber);
            if (removed) {
                showSuccess(QString("Бронирование номера %1 отменено и номер удален успешно!").arg(roomNumber));
            } else {
                showSuccess(QString("Бронирование номера %1 успешно отменено!").arg(roomNumber));
                showError("Предупреждение: Номер не может быть удален из системы.");
            }
            updateRoomTable();
            updateStatistics();
            autoSave();
        }
        else {
            showError("Невозможно отменить бронирование.");
        }
    }
    catch (const BookingException& e) {
        showError(QString("Ошибка отмены бронирования: %1").arg(e.what()));
    }
    catch (const std::exception& e) {
        showError(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::onDisplayAllRooms() {
    updateRoomTable();
    showSuccess("Все номера обновлены.");
}

void MainWindow::onDisplayBookedRooms() {
    roomTable->setRowCount(0);
    auto bookedRooms = hotelSystem->findRoomsByStatus(RoomStatus::BOOKED);

    for (const Room* room : bookedRooms) {
        int row = roomTable->rowCount();
        roomTable->insertRow(row);
        roomTable->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(room->getRoomNumber())));

        QTableWidgetItem* item0 = new QTableWidgetItem(getRoomTypeString(room));
        item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item0->setToolTip(getRoomTypeString(room));
        roomTable->setItem(row, 0, item0);
        
        QTableWidgetItem* item1 = new QTableWidgetItem(getStatusString(room->getStatus()));
        item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        roomTable->setItem(row, 1, item1);
        
        const Guest* guest = room->getGuest();
        QString guestName = guest ? QString::fromStdString(guest->toShortString()) : "-";
        QTableWidgetItem* item2 = new QTableWidgetItem(guestName);
        item2->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item2->setToolTip(guestName);
        roomTable->setItem(row, 2, item2);
        
        QTableWidgetItem* item3 = new QTableWidgetItem("$" + QString::number(static_cast<int>(room->getPricePerDay())));
        item3->setTextAlignment(Qt::AlignCenter);
        roomTable->setItem(row, 3, item3);
        
        QTableWidgetItem* item4 = new QTableWidgetItem(QString::number(room->getBedCount()));
        item4->setTextAlignment(Qt::AlignCenter);
        roomTable->setItem(row, 4, item4);

        QWidget* actionsWidget = createActionsWidget(room);
        roomTable->setCellWidget(row, 5, actionsWidget);

        for (int col = 0; col < 5; ++col) {
            if (QTableWidgetItem* item = roomTable->item(row, col)) {
                item->setBackground(QColor(255, 255, 200));
            }
        }
    }
}

void MainWindow::onDisplayOccupiedRooms() {
    roomTable->setRowCount(0);
    auto occupiedRooms = hotelSystem->findRoomsByStatus(RoomStatus::OCCUPIED);

    for (const Room* room : occupiedRooms) {
        int row = roomTable->rowCount();
        roomTable->insertRow(row);
        roomTable->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(room->getRoomNumber())));

        QTableWidgetItem* item0 = new QTableWidgetItem(getRoomTypeString(room));
        item0->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item0->setToolTip(getRoomTypeString(room));
        roomTable->setItem(row, 0, item0);
        
        QTableWidgetItem* item1 = new QTableWidgetItem(getStatusString(room->getStatus()));
        item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        roomTable->setItem(row, 1, item1);
        
        const Guest* guest = room->getGuest();
        QString guestName = guest ? QString::fromStdString(guest->toShortString()) : "-";
        QTableWidgetItem* item2 = new QTableWidgetItem(guestName);
        item2->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item2->setToolTip(guestName);
        roomTable->setItem(row, 2, item2);
        
        QTableWidgetItem* item3 = new QTableWidgetItem("$" + QString::number(static_cast<int>(room->getPricePerDay())));
        item3->setTextAlignment(Qt::AlignCenter);
        roomTable->setItem(row, 3, item3);
        
        QTableWidgetItem* item4 = new QTableWidgetItem(QString::number(room->getBedCount()));
        item4->setTextAlignment(Qt::AlignCenter);
        roomTable->setItem(row, 4, item4);

        QWidget* actionsWidget = createActionsWidget(room);
        roomTable->setCellWidget(row, 5, actionsWidget);

        for (int col = 0; col < 5; ++col) {
            if (QTableWidgetItem* item = roomTable->item(row, col)) {
                item->setBackground(QColor(255, 200, 200));
            }
        }
    }
}

void MainWindow::onShowStatistics() {
    QString hotelName = QString::fromStdString(hotelSystem->getHotelName());
    double revenue = hotelSystem->getTotalRevenue();
    double futureRevenue = hotelSystem->getFutureRevenue();
    
    QString stats = QString(
        "<div style='text-align: center; margin-bottom: 20px;'>"
        "<h2 style='color: #2c3e50; margin: 0; font-size: 24pt;'>%1</h2>"
        "</div>"
        "<hr style='border: 1px solid #bdc3c7; margin: 15px 0;'>"
        "<div style='font-size: 11pt; line-height: 1.8;'>"
        "<table style='width: 100%; border-collapse: collapse;'>"
        "<tr><td style='padding: 8px 0; color: #34495e;'><b>Всего номеров:</b></td>"
        "<td style='text-align: right; padding: 8px 0; color: #2c3e50; font-weight: bold;'>%2</td></tr>"
        "<tr><td style='padding: 8px 0; color: #f39c12;'><b>Забронировано:</b></td>"
        "<td style='text-align: right; padding: 8px 0; color: #f39c12; font-weight: bold;'>%3</td></tr>"
        "<tr><td style='padding: 8px 0; color: #e74c3c;'><b>Занято:</b></td>"
        "<td style='text-align: right; padding: 8px 0; color: #e74c3c; font-weight: bold;'>%4</td></tr>"
        "</table>"
        "</div>"
        "<hr style='border: 1px solid #bdc3c7; margin: 15px 0;'>"
        "<div style='text-align: center; margin-top: 15px;'>"
        "<div style='font-size: 14pt; color: #27ae60; font-weight: bold;'>Общий доход</div>"
        "<div style='font-size: 18pt; color: #27ae60; font-weight: bold; margin-top: 5px;'>$%5 <span style='font-size: 14pt; color: #7f8c8d;'>(+$%6)</span></div>"
        "</div>"
    )
        .arg(hotelName)
        .arg(hotelSystem->getTotalRooms())
        .arg(hotelSystem->getBookedRoomsCount())
        .arg(hotelSystem->getOccupiedRoomsCount())
        .arg(revenue, 0, 'f', 2)
        .arg(futureRevenue, 0, 'f', 2);
    
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Статистика отеля");
    msgBox.setText(stats);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void MainWindow::onClearData() {
    QMessageBox msgBox(QMessageBox::Warning, "Очистить все данные",
        "Это удалит все номера, гостей и сбросит доход до нуля.\n"
        "Это действие нельзя отменить!\n\n"
        "Вы уверены, что хотите продолжить?",
        QMessageBox::NoButton, this);
    QPushButton* yesButton = msgBox.addButton("Да", QMessageBox::YesRole);
    QPushButton* noButton = msgBox.addButton("Нет", QMessageBox::NoRole);
    msgBox.setDefaultButton(noButton);
    int ret = msgBox.exec();

    if (msgBox.clickedButton() == yesButton) {
        try {
            hotelSystem->clearAllData();
            showSuccess("Все данные успешно очищены!");
            updateRoomTable();
            updateStatistics();
            autoSave();
        }
        catch (const std::exception& e) {
            showError(QString("Ошибка при очистке данных: %1").arg(e.what()));
        }
    }
}



int MainWindow::getSelectedRoomNumber() const {
    QList<QTableWidgetItem*> selectedItems = roomTable->selectedItems();
    if (selectedItems.isEmpty()) {
        return 0;
    }

    int row = selectedItems.first()->row();
    
    QTableWidgetItem* typeItem = roomTable->item(row, 0);
    QTableWidgetItem* statusItem = roomTable->item(row, 1);
    QTableWidgetItem* guestItem = roomTable->item(row, 2);
    
    if (!typeItem || !statusItem || !guestItem) {
        return 0;
    }
    
    QString typeText = typeItem->text();
    QString statusText = statusItem->text();
    QString guestText = guestItem->text();
    
    for (const auto& roomPtr : *hotelSystem) {
        if (!roomPtr) continue;
        
        const Room* room = roomPtr.get();
        
        if (getRoomTypeString(room) == typeText &&
            getStatusString(room->getStatus()) == statusText) {
            const Guest* guest = room->getGuest();
            QString roomGuestName = guest ? QString::fromStdString(guest->toShortString()) : "-";
            
            if (roomGuestName == guestText) {
                return room->getRoomNumber();
        }
    }
    }
    
    return 0;
}

void MainWindow::autoSave() {
    try {
        hotelSystem->saveToFile("hotel_data_total");
    }
    catch (const FileException& e) {
    }
    catch (const std::exception& e) {
    }
}


