#include "C:/Users/User/Desktop/HIS/include/WelcomeWindow.h"
#include "C:/Users/User/Desktop/HIS/include/MainWindow.h"
#include "C:/Users/User/Desktop/HIS/include/exceptions.h"
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QApplication>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QFrame>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QGridLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QSize>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QTimer>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QDate>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QDateTime>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QTime>
#include <ctime>

WelcomeWindow::WelcomeWindow(QWidget* parent)
    : QDialog(parent), hotelSystem(nullptr), ownsHotelSystem(true), cardsLayout(nullptr) {
    hotelSystem = new HotelSystem("Отель");
    
    try {
        hotelSystem->loadFromFile("hotel_data_total");
    }
    catch (const FileException& e) {
    }
    catch (const std::exception& e) {
    }
    
    setupUI();
    updateRoomCards();
}

void WelcomeWindow::setHotelSystem(HotelSystem* system) {
    if (hotelSystem && ownsHotelSystem && hotelSystem != system) {
        delete hotelSystem;
    }
    hotelSystem = system;
    ownsHotelSystem = (parent() != nullptr) ? false : true;
    if (hotelSystem) {
        updateRoomCards();
    }
}

HotelSystem* WelcomeWindow::getHotelSystem() const {
    return hotelSystem;
}

WelcomeWindow::~WelcomeWindow() {
    if (ownsHotelSystem) {
        delete hotelSystem;
    }
}

void WelcomeWindow::setupUI() {
    setWindowTitle("Отель - Забронируй свой уютный номер");
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
    setMinimumSize(1000, 600);
    resize(1100, 700);
    
    setStyleSheet(
        "QDialog {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "        stop:0 #f8f9fa, stop:1 #e9ecef);"
        "}"
    );
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    cardsContainer = new QWidget(this);
    cardsContainer->setStyleSheet("QWidget { background: transparent; }");
    
    cardsLayout = new QGridLayout(cardsContainer);
    cardsLayout->setSpacing(20);
    cardsLayout->setContentsMargins(25, 25, 25, 25);
    
    mainLayout->addWidget(cardsContainer, 1);
    
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
    
    QLabel* featuresLabel = new QLabel(
        "✓ Время заезда 14:00, выезда 11:00  |  "
        "✓ Центр города  |  "
        "✓ Кофейня с завтраками  |  "
        "✓ 24/7",
        footerWidget
    );
    featuresLabel->setStyleSheet(
        "QLabel {"
        "    color: #495057;"
        "    font-size: 9pt;"
        "    background: transparent;"
        "}"
    );
    footerLayout->addWidget(featuresLabel, 1);
    
    enterButton = new QPushButton("Войти в систему", footerWidget);
    enterButton->setStyleSheet(
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
    enterButton->setCursor(Qt::PointingHandCursor);
    connect(enterButton, &QPushButton::clicked, this, &WelcomeWindow::onEnterSystem);
    
    footerLayout->addWidget(enterButton);
    
    mainLayout->addWidget(footerWidget);
}

void WelcomeWindow::updateRoomCards() {
    for (QFrame* card : roomCards) {
        cardsLayout->removeWidget(card);
        delete card;
    }
    roomCards.clear();
    
    QFrame* card1 = createRoomCard(
        "8-местный номер",
        "17-22 м²",
        "4 двухъярусные кровати",
        "Зеркало, Вешалки, Полотенце под залог 10 $, Ящики для хранения, Лампа для чтения, Кармашки, Столик",
        "от 50 $/ночь",
        RoomType::Room8Bed,
        getAvailableRoomsCount(RoomType::Room8Bed)
    );
    
    QFrame* card2 = createRoomCard(
        "6-местный номер",
        "12-17 м²",
        "3 двухъярусные кровати",
        "Зеркало, Вешалки, Полотенце под залог 10 $, Ящики для хранения, Лампа для чтения, Кармашки, Столик",
        "от 45 $/ночь",
        RoomType::Room6Bed,
        getAvailableRoomsCount(RoomType::Room6Bed)
    );
    
    QFrame* card3 = createRoomCard(
        "4-местный женский",
        "10,5-14 м²",
        "2 двухъярусные кровати",
        "Зеркало, Вешалки, Полотенце под залог 10 $, Ящики для хранения, Лампа для чтения, Столик, Кармашки",
        "от 32 $/ночь",
        RoomType::Room4BedFemale,
        getAvailableRoomsCount(RoomType::Room4BedFemale)
    );
    
    QFrame* card4 = createRoomCard(
        "Твин Джуниор",
        "5 м²",
        "1 двухъярусная кровать",
        "Зеркало, Вешалки, Полотенца, Питьевая вода, Ящик для хранения, Dog friendly до 10 кг",
        "от 20 $/ночь",
        RoomType::TwinJunior,
        getAvailableRoomsCount(RoomType::TwinJunior)
    );
    
    QFrame* card5 = createRoomCard(
        "Двухместный Эконом",
        "6-9 м²",
        "1 двуспальная кровать",
        "Зеркало, Вешалки, Полотенца, Напольная лампа, Питьевая вода, Столик, Dog friendly до 10 кг",
        "от 25 $/ночь",
        RoomType::DoubleEconomy,
        getAvailableRoomsCount(RoomType::DoubleEconomy)
    );
    
    QFrame* card6 = createRoomCard(
        "Стандартный Дабл",
        "10-12 м²",
        "1 двуспальная кровать",
        "Кресло, Вешалки, Полотенца, Столик, Питьевая вода, Dog friendly до 10 кг",
        "от 35 $/ночь",
        RoomType::StandardDouble,
        getAvailableRoomsCount(RoomType::StandardDouble)
    );
    
    QFrame* card7 = createRoomCard(
        "Loft Double",
        "6-9 м²",
        "1 двуспальная кровать",
        "Диван, Вешалки, Полотенца, Столик, Питьевая вода, Dog friendly до 10 кг",
        "от 28 $/ночь",
        RoomType::LoftDouble,
        getAvailableRoomsCount(RoomType::LoftDouble)
    );
    
    QFrame* card8 = createRoomCard(
        "Семейный номер",
        "15 м²",
        "1 двуспальная + 1 двухъярусная",
        "Зеркало, Вешалки, Полотенца, Столик, Питьевая вода, Dog friendly до 10 кг",
        "от 40 $/ночь",
        RoomType::FamilyRoom,
        getAvailableRoomsCount(RoomType::FamilyRoom)
    );
    
    cardsLayout->addWidget(card1, 0, 0);
    cardsLayout->addWidget(card2, 0, 1);
    cardsLayout->addWidget(card3, 1, 0);
    cardsLayout->addWidget(card4, 1, 1);
    cardsLayout->addWidget(card5, 2, 0);
    cardsLayout->addWidget(card6, 2, 1);
    cardsLayout->addWidget(card7, 3, 0);
    cardsLayout->addWidget(card8, 3, 1);
    
    roomCards.append({card1, card2, card3, card4, card5, card6, card7, card8});
}

QFrame* WelcomeWindow::createRoomCard(const QString& title, const QString& size,
                                     const QString& beds, const QString& features,
                                     const QString& price, RoomType roomType, int availableCount) {
    QFrame* card = new QFrame(cardsContainer);
    card->setStyleSheet(
        "QFrame {"
        "    background: white;"
        "    border-radius: 12px;"
        "    border: 1px solid #dee2e6;"
        "}"
    );
    card->setMinimumHeight(160);
    card->setMaximumWidth(650);
    
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(3);
    cardLayout->setContentsMargins(10, 10, 10, 10);
    
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet(
        "QLabel {"
        "    color: #212529;"
        "    font-size: 16pt;"
        "    font-weight: bold;"
        "    background: transparent;"
        "}"
    );
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    
    QLabel* availabilityLabel = new QLabel(
        availableCount > 0 ? QString("✓ Доступно") : QString("✗ Занято"),
        card
    );
    availabilityLabel->setStyleSheet(
        QString("QLabel {"
        "    color: %1;"
        "    font-size: 7pt;"
        "    font-weight: bold;"
        "    background: %2;"
        "    padding: 2px 6px;"
        "    border-radius: 6px;"
        "}").arg(
            availableCount > 0 ? "#27ae60" : "#e74c3c",
            availableCount > 0 ? "#d5f4e6" : "#fadbd8"
        )
    );
    titleLayout->addWidget(availabilityLabel);
    cardLayout->addLayout(titleLayout);
    
    QFrame* divider = new QFrame(card);
    divider->setFixedHeight(1);
    divider->setStyleSheet("QFrame { background-color: #27ae60; }");
    cardLayout->addWidget(divider);
    
    QLabel* sizeLabel = new QLabel("📐 " + size + " | 🛏️ " + beds, card);
    sizeLabel->setStyleSheet(
        "QLabel {"
        "    color: #6c757d;"
        "    font-size: 8pt;"
        "    background: transparent;"
        "    padding-top: 0px;"
        "}"
    );
    cardLayout->addWidget(sizeLabel);
    
    
    QLabel* featuresLabel = new QLabel(features, card);
    featuresLabel->setStyleSheet(
        "QLabel {"
        "    color: #495057;"
        "    font-size: 9pt;"
        "    background: transparent;"
        "    padding: 1px 0;"
        "}"
    );
    featuresLabel->setWordWrap(true);
    cardLayout->addWidget(featuresLabel);
    
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(8);
    
    QLabel* priceLabel = new QLabel(price, card);
    priceLabel->setStyleSheet(
        "QLabel {"
        "    color: #27ae60;"
        "    font-size: 14pt;"
        "    font-weight: bold;"
        "    background: transparent;"
        "}"
    );
    bottomLayout->addWidget(priceLabel);
    
    QPushButton* bookButton = new QPushButton("Забронировать", card);
    bookButton->setEnabled(availableCount > 0);
    bookButton->setStyleSheet(
        QString("QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    font-size: 9pt;"
        "    font-weight: bold;"
        "    padding: 5px 14px;"
        "    border: none;"
        "    border-radius: 4px;"
        "    min-width: 90px;"
        "}"
        "QPushButton:hover {"
        "    background-color: %2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: %3;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #95a5a6;"
        "    color: #ecf0f1;"
        "}").arg(
            availableCount > 0 ? "#27ae60" : "#95a5a6",
            availableCount > 0 ? "#229954" : "#7f8c8d",
            availableCount > 0 ? "#1e8449" : "#6c7a7b"
        )
    );
    bookButton->setCursor(availableCount > 0 ? Qt::PointingHandCursor : Qt::ArrowCursor);
    
    connect(bookButton, &QPushButton::clicked, [this, roomType]() {
        onBookRoom(roomType);
    });
    
    bottomLayout->addWidget(bookButton);
    cardLayout->addLayout(bottomLayout);
    
    return card;
}

void WelcomeWindow::onBookRoom(RoomType roomType) {
    GuestDialog dialog(this);
    
    dialog.setRoomType(roomType);
    
    if (dialog.exec() == QDialog::Accepted) {
        try {
            Guest guest = dialog.getGuest();
            int days = dialog.getDays();
            RoomType selectedType = dialog.getRoomType();
            RateType rateType = dialog.getRateType();
            bool wifi = true;
            bool miniBar = dialog.getMiniBar();
            bool balcony = dialog.getBalcony();
            bool jacuzzi = dialog.getJacuzzi();
            double price = dialog.getPrice();
            
            QDate plannedCheckInDate = dialog.getCheckInDate();
            std::time_t plannedCheckInTime = 0;
            if (plannedCheckInDate.isValid()) {
                QDateTime dateTime(plannedCheckInDate, QTime(14, 0, 0));
                plannedCheckInTime = dateTime.toSecsSinceEpoch();
            }
            
            int roomNumber = hotelSystem->bookRoom(guest, days, selectedType, 
                                                  wifi, miniBar, balcony, jacuzzi, price, rateType, plannedCheckInTime);
            
            const Room* room = hotelSystem->findRoom(roomNumber);
            if (room) {
                double totalPrice = room->calculateTotalPrice(days);
                QMessageBox::information(this, "Успешно!", 
                    QString("Номер %1 успешно забронирован на %2 день(дней)!\n"
                           "Цена за ночь: %3 $\n"
                           "Общая стоимость: %4 $")
                    .arg(roomNumber)
                    .arg(days)
                    .arg(static_cast<int>(room->getPricePerDay()))
                    .arg(static_cast<int>(totalPrice)));
            } else {
                QMessageBox::information(this, "Успешно!", 
                    QString("Номер %1 успешно забронирован на %2 день(дней)!")
                    .arg(roomNumber).arg(days));
            }
            
            updateRoomCards();
            
            try {
                hotelSystem->saveToFile("hotel_data_total");
            } catch (...) {
            }
        }
        catch (const BookingException& e) {
            QLabel* notification = new QLabel(
                QString("Ошибка бронирования: %1").arg(e.what()), this);
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
        catch (const std::exception& e) {
            QLabel* notification = new QLabel(
                QString("Ошибка: %1").arg(e.what()), this);
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
    }
}

int WelcomeWindow::getAvailableRoomsCount(RoomType roomType) const {
    if (!hotelSystem) return 999;
    
    return 999;
}

QString WelcomeWindow::getRoomTypeName(RoomType roomType) const {
    switch (roomType) {
    case RoomType::Room8Bed:
        return "8-местный";
    case RoomType::Room6Bed:
        return "6-местный";
    case RoomType::Room4BedFemale:
        return "4-местный женский";
    case RoomType::TwinJunior:
        return "Твин Джуниор";
    case RoomType::DoubleEconomy:
        return "Двухместный Эконом";
    case RoomType::StandardDouble:
        return "Стандартный Дабл";
    case RoomType::LoftDouble:
        return "Loft Double";
    case RoomType::FamilyRoom:
        return "Семейный";
    default:
        return "Неизвестно";
    }
}

void WelcomeWindow::onEnterSystem() {
    try {
        if (hotelSystem) {
            hotelSystem->saveToFile("hotel_data_total");
        }
    } catch (...) {
    }
    
    if (parent()) {
        accept();
    } else {
        MainWindow* mainWindow = new MainWindow(nullptr, hotelSystem);
        mainWindow->setAttribute(Qt::WA_DeleteOnClose, true);
        mainWindow->show();
        
        hotelSystem = nullptr;
        
        close();
    }
}

