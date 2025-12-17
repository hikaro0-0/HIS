#include "C:/Users/User/Desktop/HIS/include/GuestDialog.h"
#include "C:/Users/User/Desktop/HIS/include/HotelSystem.h"
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QMessageBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QButtonGroup>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QRadioButton>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QFrame>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QScrollArea>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QTimer>

GuestDialog::GuestDialog(QWidget* parent)
    : QDialog(parent), rateButtonGroup(nullptr), rateScroll(nullptr), selectedDays(1), selectedCheckInDate(QDate::currentDate()) {
    setupUI();
    setWindowTitle("Бронирование номера - Информация о госте");
    setModal(true);
    resize(550, 720);
}

void GuestDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(6, 6, 6, 6);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSpacing(0);
    formLayout->setVerticalSpacing(0);
    formLayout->setContentsMargins(0, 0, 0, -25);
    formLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    formLayout->setLabelAlignment(Qt::AlignLeft);

    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Введите имя гостя");
    nameInput->setMaximumHeight(22);
    nameInput->setMinimumHeight(22);
    formLayout->addRow("Имя гостя:", nameInput);

    phoneInput = new QLineEdit(this);
    phoneInput->setPlaceholderText("+375 29 123-45-67");
    phoneInput->setMaximumHeight(22);
    phoneInput->setMinimumHeight(22);
    formLayout->addRow("Телефон гостя:", phoneInput);

    selectDatesButton = new QPushButton("Выбрать даты...", this);
    selectDatesButton->setStyleSheet("QPushButton { padding: 1px; }");
    selectDatesButton->setMaximumHeight(22);
    selectDatesButton->setMinimumHeight(22);
    connect(selectDatesButton, &QPushButton::clicked, this, &GuestDialog::onSelectDates);
    
    daysLabel = new QLabel("1 день", this);
    daysLabel->setStyleSheet("QLabel { font-weight: bold; color: #0066cc; }");
    
    QHBoxLayout* datesLayout = new QHBoxLayout();
    datesLayout->addWidget(selectDatesButton);
    datesLayout->addWidget(daysLabel);
    datesLayout->addStretch();
    
    formLayout->addRow("Продолжительность:", datesLayout);

    roomTypeCombo = new QComboBox(this);
    roomTypeCombo->addItem("8-местный номер", static_cast<int>(RoomType::Room8Bed));
    roomTypeCombo->addItem("6-местный номер", static_cast<int>(RoomType::Room6Bed));
    roomTypeCombo->addItem("4-местный женский", static_cast<int>(RoomType::Room4BedFemale));
    roomTypeCombo->addItem("Твин Джуниор", static_cast<int>(RoomType::TwinJunior));
    roomTypeCombo->addItem("Двухместный Эконом", static_cast<int>(RoomType::DoubleEconomy));
    roomTypeCombo->addItem("Стандартный Дабл", static_cast<int>(RoomType::StandardDouble));
    roomTypeCombo->addItem("Loft Double", static_cast<int>(RoomType::LoftDouble));
    roomTypeCombo->addItem("Семейный номер", static_cast<int>(RoomType::FamilyRoom));
    roomTypeCombo->setCurrentIndex(0);
    roomTypeCombo->setMaximumHeight(22);
    roomTypeCombo->setMinimumHeight(22);
    formLayout->addRow("Тип номера:", roomTypeCombo);

    miniBarCheckBox = new QCheckBox("Мини-бар", this);
    miniBarCheckBox->setChecked(true);
    miniBarCheckBox->setStyleSheet("QCheckBox { font-size: 9pt; }");
    formLayout->addRow("", miniBarCheckBox);

    balconyCheckBox = new QCheckBox("Балкон", this);
    balconyCheckBox->setChecked(true);
    balconyCheckBox->setStyleSheet("QCheckBox { font-size: 9pt; }");
    formLayout->addRow("", balconyCheckBox);

    jacuzziCheckBox = new QCheckBox("Джакузи", this);
    jacuzziCheckBox->setChecked(true);
    jacuzziCheckBox->setStyleSheet("QCheckBox { font-size: 9pt; }");
    formLayout->addRow("", jacuzziCheckBox);

    priceLabel = new QLabel(this);
    priceLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 9pt; color: #0066cc; }");
    priceLabel->setAlignment(Qt::AlignCenter);
    priceLabel->setMaximumHeight(30);
    formLayout->addRow("Цена за ночь:", priceLabel);

    QLabel* rateTitle = new QLabel("Выберите тариф:", this);
    rateTitle->setStyleSheet(
        "QLabel {"
        "    font-size: 10pt;"
        "    font-weight: bold;"
        "    padding: 0px;"
        "    color: #212529;"
        "}"
    );
    formLayout->addRow("", rateTitle);

    connect(roomTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GuestDialog::onRoomTypeChanged);
    connect(miniBarCheckBox, &QCheckBox::toggled, this, &GuestDialog::updatePrice);
    connect(balconyCheckBox, &QCheckBox::toggled, this, &GuestDialog::updatePrice);
    connect(jacuzziCheckBox, &QCheckBox::toggled, this, &GuestDialog::updatePrice);

    updateRoomOptions();
    
    updatePrice();

    mainLayout->addLayout(formLayout);
    
    rateContainer = new QWidget(this);
    QVBoxLayout* rateLayout = new QVBoxLayout(rateContainer);
    rateLayout->setSpacing(1);
    rateLayout->setContentsMargins(0, 0, 0, 0);
    
    rateButtonGroup = new QButtonGroup(this);
    
    createRateCard(RateType::NonRefundable, "Невозвратный тариф (-10%)",
                   "Условия отмены",
                   "Оплата: банковской картой",
                   rateLayout);
    
    createRateCard(RateType::FlexibleStart, "Гибкий старт",
                   "БЕСПЛАТНАЯ отмена брони",
                   "Оплата: банковской картой",
                   rateLayout);
    
    if (rateButtonGroup->buttons().size() > 0) {
        rateButtonGroup->buttons()[0]->setChecked(true);
    }
    
    connect(rateButtonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this, &GuestDialog::updatePrice);
    
    rateScroll = new QScrollArea(this);
    rateScroll->setWidget(rateContainer);
    rateScroll->setWidgetResizable(true);
    rateScroll->setFrameShape(QFrame::NoFrame);
    rateScroll->setMaximumHeight(400);
    rateScroll->setMinimumHeight(400);
    rateScroll->setStyleSheet(
        "QScrollArea {"
        "    border: none;"
        "    background: transparent;"
        "}"
        "QScrollBar:vertical {"
        "    background: #e9ecef;"
        "    width: 8px;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #27ae60;"
        "    border-radius: 4px;"
        "    min-height: 20px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #229954;"
        "}"
    );
    mainLayout->addWidget(rateScroll);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    okButton = new QPushButton("Забронировать", this);
    okButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #27ae60;"
        "    color: white;"
        "    font-size: 10pt;"
        "    font-weight: bold;"
        "    padding: 6px 20px;"
        "    border: none;"
        "    border-radius: 6px;"
        "    min-width: 130px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #229954;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1e8449;"
        "}"
    );
    okButton->setCursor(Qt::PointingHandCursor);

    cancelButton = new QPushButton("Выход", this);
    cancelButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #95a5a6;"
        "    color: white;"
        "    font-size: 10pt;"
        "    font-weight: bold;"
        "    padding: 6px 20px;"
        "    border: none;"
        "    border-radius: 6px;"
        "    min-width: 130px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #7f8c8d;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #6c7a7b;"
        "}"
    );
    cancelButton->setCursor(Qt::PointingHandCursor);

    connect(okButton, &QPushButton::clicked, this, &GuestDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &GuestDialog::onReject);

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

bool GuestDialog::validateInput() {
    if (nameInput->text().trimmed().isEmpty()) {
        showNotification("Пожалуйста, введите имя гостя.", true);
        return false;
    }
    
    QString phone = phoneInput->text().trimmed();
    if (!phone.isEmpty()) {
        if (!phone.startsWith("+")) {
            showNotification("Номер телефона должен начинаться с '+' (например, +375291234567).", true);
            return false;
        }
        
        int digitCount = 0;
        for (int i = 1; i < phone.length(); ++i) {
            if (phone[i].isDigit()) {
                digitCount++;
            }
        }
        
        if (digitCount > 13) {
            showNotification("Номер телефона не должен превышать 13 цифр (не считая знак '+').", true);
            return false;
        }
        
        if (digitCount == 0) {
            showNotification("Номер телефона должен содержать хотя бы одну цифру после '+'.", true);
            return false;
        }
    }
    
    return true;
}

void GuestDialog::showNotification(const QString& message, bool isError) {
    QLabel* notification = new QLabel(message, this);
    QString borderColor = isError ? "#e74c3c" : "#27ae60";
    QString textColor = isError ? "#e74c3c" : "#27ae60";
    notification->setStyleSheet(
        QString("QLabel {"
        "    background-color: white;"
        "    color: %1;"
        "    padding: 15px 20px;"
        "    border-radius: 8px;"
        "    border: 2px solid %2;"
        "    font-size: 12pt;"
        "    font-weight: bold;"
        "    min-width: 300px;"
        "    max-width: 500px;"
        "}").arg(textColor, borderColor)
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
    
    QTimer::singleShot(isError ? 4000 : 3000, notification, &QLabel::close);
}

void GuestDialog::onAccept() {
    if (validateInput()) {
        accept();
    }
}

void GuestDialog::onReject() {
    reject();
}

Guest GuestDialog::getGuest() const {
    return Guest(
        nameInput->text().trimmed().toStdString(),
        phoneInput->text().trimmed().toStdString(),
        ""
    );
}

int GuestDialog::getDays() const {
    return selectedDays;
}

QDate GuestDialog::getCheckInDate() const {
    return selectedCheckInDate;
}

void GuestDialog::onSelectDates() {
    DateSelectionDialog dateDialog(this);
    if (dateDialog.exec() == QDialog::Accepted) {
        selectedDays = dateDialog.getDays();
        selectedCheckInDate = dateDialog.getCheckInDate();
        QString daysText;
        if (selectedDays == 1) {
            daysText = "1 день";
        } else if (selectedDays >= 2 && selectedDays <= 4) {
            daysText = QString("%1 дня").arg(selectedDays);
        } else {
            daysText = QString("%1 дней").arg(selectedDays);
        }
        daysLabel->setText(daysText);
        updatePrice();
    }
}

RoomType GuestDialog::getRoomType() const {
    return static_cast<RoomType>(roomTypeCombo->currentData().toInt());
}

void GuestDialog::setRoomType(RoomType roomType) {
    int index = static_cast<int>(roomType);
    for (int i = 0; i < roomTypeCombo->count(); ++i) {
        if (roomTypeCombo->itemData(i).toInt() == index) {
            roomTypeCombo->setCurrentIndex(i);
            updateRoomOptions();
            updatePrice();
            break;
        }
    }
}

bool GuestDialog::getWifi() const {
    return true;
}

bool GuestDialog::getMiniBar() const {
    return miniBarCheckBox->isChecked();
}

bool GuestDialog::getBalcony() const {
    return balconyCheckBox->isChecked();
}

bool GuestDialog::getJacuzzi() const {
    return jacuzziCheckBox->isChecked();
}

double GuestDialog::getPrice() const {
    return calculatePrice();
}

void GuestDialog::onRoomTypeChanged() {
    updateRoomOptions();
    updatePrice();
}

void GuestDialog::updateRoomOptions() {
    RoomType roomType = getRoomType();
    
    switch (roomType) {
    case RoomType::Room8Bed:
    case RoomType::Room6Bed:
    case RoomType::Room4BedFemale:
    case RoomType::TwinJunior:
        miniBarCheckBox->setVisible(false);
        balconyCheckBox->setVisible(false);
        jacuzziCheckBox->setVisible(false);
        break;
        
    case RoomType::DoubleEconomy:
    case RoomType::StandardDouble:
    case RoomType::LoftDouble:
        miniBarCheckBox->setVisible(true);
        balconyCheckBox->setVisible(true);
        jacuzziCheckBox->setVisible(false);
        break;
        
    case RoomType::FamilyRoom:
        miniBarCheckBox->setVisible(true);
        balconyCheckBox->setVisible(true);
        jacuzziCheckBox->setVisible(true);
        break;
    default:
        miniBarCheckBox->setVisible(false);
        balconyCheckBox->setVisible(false);
        jacuzziCheckBox->setVisible(false);
        break;
    }
}

double GuestDialog::calculatePrice() const {
    double basePrice = 0.0;
    RoomType roomType = getRoomType();
    
    switch (roomType) {
    case RoomType::TwinJunior:
        basePrice = 20.0;
        break;
    case RoomType::DoubleEconomy:
        basePrice = 25.0;
        break;
    case RoomType::LoftDouble:
        basePrice = 28.0;
        break;
    case RoomType::Room4BedFemale:
        basePrice = 32.0;
        break;
    case RoomType::StandardDouble:
        basePrice = 35.0;
        break;
    case RoomType::FamilyRoom:
        basePrice = 40.0;
        break;
    case RoomType::Room6Bed:
        basePrice = 45.0;
        break;
    case RoomType::Room8Bed:
        basePrice = 50.0;
        break;
    default:
        basePrice = 25.0;
        break;
    }
    
    if (miniBarCheckBox->isChecked() && miniBarCheckBox->isVisible()) {
        basePrice += 4.0;
    }
    
    if (balconyCheckBox->isChecked() && balconyCheckBox->isVisible()) {
        basePrice += 5.0;
    }
    
    if (jacuzziCheckBox->isChecked() && jacuzziCheckBox->isVisible()) {
        basePrice += 7.0;
    }
    
    RateType rateType = getRateType();
    if (rateType == RateType::NonRefundable) {
        basePrice *= 0.9;
    }
    
    return basePrice;
}

void GuestDialog::updatePrice() {
    double basePricePerNight = 0.0;
    RoomType roomType = getRoomType();
    
    switch (roomType) {
    case RoomType::TwinJunior:
        basePricePerNight = 20.0;
        break;
    case RoomType::DoubleEconomy:
        basePricePerNight = 25.0;
        break;
    case RoomType::LoftDouble:
        basePricePerNight = 28.0;
        break;
    case RoomType::Room4BedFemale:
        basePricePerNight = 32.0;
        break;
    case RoomType::StandardDouble:
        basePricePerNight = 35.0;
        break;
    case RoomType::FamilyRoom:
        basePricePerNight = 40.0;
        break;
    case RoomType::Room6Bed:
        basePricePerNight = 45.0;
        break;
    case RoomType::Room8Bed:
        basePricePerNight = 50.0;
        break;
    default:
        basePricePerNight = 25.0;
        break;
    }
    
    if (miniBarCheckBox->isChecked() && miniBarCheckBox->isVisible()) {
        basePricePerNight += 4.0;
    }
    if (balconyCheckBox->isChecked() && balconyCheckBox->isVisible()) {
        basePricePerNight += 5.0;
    }
    if (jacuzziCheckBox->isChecked() && jacuzziCheckBox->isVisible()) {
        basePricePerNight += 7.0;
    }
    
    RateType rateType = getRateType();
    double pricePerNight = basePricePerNight;
    if (rateType == RateType::NonRefundable) {
        pricePerNight *= 0.9;
    }
    
    int days = getDays();
    double totalPrice = pricePerNight * days;
    
    QString priceText = QString("%1 $ / ночь")
        .arg(static_cast<int>(pricePerNight));
    
    if (days > 1) {
        priceText += QString("\nИтого за %1 день(дней): %2 $")
            .arg(days).arg(static_cast<int>(totalPrice));
    }
    
    priceLabel->setText(priceText);
    
    if (rateButtonGroup) {
        for (QAbstractButton* button : rateButtonGroup->buttons()) {
            QWidget* card = button->parentWidget();
            if (card) {
                QLabel* cardPriceLabel = card->findChild<QLabel*>("ratePriceLabel");
                if (cardPriceLabel) {
                    double cardPrice = basePricePerNight * days;
                    RateType cardRateType = static_cast<RateType>(button->property("rateType").toInt());
                    if (cardRateType == RateType::NonRefundable) {
                        cardPrice *= 0.9;
                    }
                    cardPriceLabel->setText(QString("Стоимость за %1 ночи: %2 $")
                                           .arg(days).arg(QString::number(cardPrice, 'f', 2)));
                }
            }
        }
    }
}

bool GuestDialog::isValid() const {
    return result() == QDialog::Accepted;
}

RateType GuestDialog::getRateType() const {
    if (!rateButtonGroup) {
        return RateType::FlexibleStart;
    }
    
    QAbstractButton* checkedButton = rateButtonGroup->checkedButton();
    if (checkedButton) {
        return static_cast<RateType>(checkedButton->property("rateType").toInt());
    }
    
    return RateType::FlexibleStart;
}

void GuestDialog::createRateCard(RateType rateType, const QString& title, 
                                 const QString& cancellation, const QString& payment,
                                 QVBoxLayout* layout) {
    QFrame* card = new QFrame(rateContainer);
    card->setStyleSheet(
        "QFrame {"
        "    background: white;"
        "    border: 2px solid #dee2e6;"
        "    border-radius: 8px;"
        "    padding: 5px;"
        "}"
        "QFrame:hover {"
        "    border-color: #27ae60;"
        "}"
    );
    
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(1);
    cardLayout->setContentsMargins(4, 4, 4, 4);
    
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QRadioButton* radioButton = new QRadioButton(card);
    radioButton->setProperty("rateType", static_cast<int>(rateType));
    radioButton->setStyleSheet(
        "QRadioButton {"
        "    spacing: 8px;"
        "}"
        "QRadioButton::indicator {"
        "    width: 18px;"
        "    height: 18px;"
        "}"
        "QRadioButton::indicator:checked {"
        "    background-color: #27ae60;"
        "    border: 2px solid #27ae60;"
        "    border-radius: 9px;"
        "}"
    );
    
    QLabel* titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 11pt;"
        "    font-weight: bold;"
        "    color: #212529;"
        "}"
    );
    
    QPushButton* detailsButton = new QPushButton("Подробнее", card);
    detailsButton->setStyleSheet(
        "QPushButton {"
        "    background: transparent;"
        "    color: #6c757d;"
        "    font-size: 9pt;"
        "    border: none;"
        "    padding: 1px 4px;"
        "    text-decoration: underline;"
        "}"
        "QPushButton:hover {"
        "    color: #27ae60;"
        "}"
    );
    detailsButton->setCursor(Qt::PointingHandCursor);
    
    QPushButton* arrowButton = new QPushButton("▼", card);
    arrowButton->setProperty("isExpanded", false);
    arrowButton->setStyleSheet(
        "QPushButton {"
        "    background: white;"
        "    color: #6c757d;"
        "    font-size: 10pt;"
        "    border: 1px solid #dee2e6;"
        "    border-radius: 4px;"
        "    padding: 2px 8px;"
        "    min-width: 20px;"
        "    max-width: 20px;"
        "    min-height: 20px;"
        "    max-height: 20px;"
        "}"
        "QPushButton:hover {"
        "    background: #f8f9fa;"
        "    border-color: #27ae60;"
        "    color: #27ae60;"
        "}"
        "QPushButton:pressed {"
        "    background: #e9ecef;"
        "}"
    );
    arrowButton->setCursor(Qt::PointingHandCursor);
    
    titleLayout->addWidget(radioButton);
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(detailsButton);
    titleLayout->addWidget(arrowButton);
    
    rateButtonGroup->addButton(radioButton, static_cast<int>(rateType));
    cardLayout->addLayout(titleLayout);
    
    QWidget* detailsWidget = new QWidget(card);
    detailsWidget->setVisible(false);
    detailsWidget->setProperty("isExpanded", false);
    QVBoxLayout* detailsLayout = new QVBoxLayout(detailsWidget);
    detailsLayout->setSpacing(1);
    detailsLayout->setContentsMargins(0, 1, 0, 0);
    
    QLabel* descriptionLabel = new QLabel(detailsWidget);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 9pt;"
        "    color: #495057;"
        "    padding: 3px 0px;"
        "}"
    );
    
    QString description;
    switch (rateType) {
    case RateType::NonRefundable:
        description = "Скидка на проживание 10% при бронировании без возможности изменения и отмены.";
        break;
    case RateType::FlexibleStart:
        description = "Тариф \"Гибкий старт\" — это идеальное решение для тех, кто хочет начать свой путь в мире услуг без лишних затрат и обязательств. Он предлагает максимальную свободу и возможность адаптировать условия под свои потребности.";
        break;
    default:
        description = "";
        break;
    }
    descriptionLabel->setText(description);
    detailsLayout->addWidget(descriptionLabel);
    
    if (rateType == RateType::FlexibleStart) {
        QLabel* conditionsTitle = new QLabel("Условия тарифа:", detailsWidget);
        conditionsTitle->setStyleSheet(
            "QLabel {"
            "    font-size: 9pt;"
            "    font-weight: bold;"
            "    color: #212529;"
            "    padding-top: 3px;"
            "}"
        );
        detailsLayout->addWidget(conditionsTitle);
        
        QStringList conditions = {
            "При бронировании номера по тарифу требуется предоплата за первые сутки проживания. Эта сумма составляет стоимость одной ночи.",
            "Если вы отменяете бронирование за день до запланированного заезда, предоплата будет полностью возвращена на ваш счет (свяжитесь с ресепшн для оформления возврата).",
            "В случае отмены бронирования в день заезда или позже, предоплата не подлежит возврату."
        };
        
        for (const QString& condition : conditions) {
            QLabel* conditionLabel = new QLabel("• " + condition, detailsWidget);
            conditionLabel->setWordWrap(true);
            conditionLabel->setStyleSheet(
                "QLabel {"
                "    font-size: 8pt;"
                "    color: #495057;"
                "    padding-left: 8px;"
                "    padding-top: 2px;"
                "}"
            );
            detailsLayout->addWidget(conditionLabel);
        }
    }
    
    cardLayout->addWidget(detailsWidget);
    
    auto toggleDetails = [this, detailsWidget, arrowButton]() {
        bool isExpanded = detailsWidget->property("isExpanded").toBool();
        detailsWidget->setVisible(!isExpanded);
        detailsWidget->setProperty("isExpanded", !isExpanded);
        arrowButton->setText(isExpanded ? "▼" : "▲");
        arrowButton->setProperty("isExpanded", !isExpanded);
        
        if (rateContainer) {
            rateContainer->adjustSize();
        }
    };
    
    connect(detailsButton, &QPushButton::clicked, toggleDetails);
    connect(arrowButton, &QPushButton::clicked, toggleDetails);
    
    QHBoxLayout* cancelLayout = new QHBoxLayout();
    QLabel* cancelIcon = new QLabel("↻", card);
    cancelIcon->setStyleSheet("QLabel { font-size: 12pt; color: #6c757d; }");
    QLabel* cancelLabel = new QLabel(cancellation, card);
    cancelLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 9pt;"
        "    color: #495057;"
        "}"
    );
    cancelLayout->addWidget(cancelIcon);
    cancelLayout->addWidget(cancelLabel);
    cancelLayout->addStretch();
    cardLayout->addLayout(cancelLayout);
    
    QHBoxLayout* paymentLayout = new QHBoxLayout();
    QLabel* paymentIcon = new QLabel("💳", card);
    paymentIcon->setStyleSheet("QLabel { font-size: 12pt; }");
    QLabel* paymentLabel = new QLabel(payment, card);
    paymentLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 9pt;"
        "    color: #495057;"
        "}"
    );
    paymentLayout->addWidget(paymentIcon);
    paymentLayout->addWidget(paymentLabel);
    paymentLayout->addStretch();
    cardLayout->addLayout(paymentLayout);
    
    QLabel* priceLabel = new QLabel(card);
    priceLabel->setObjectName("ratePriceLabel");
    priceLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 10pt;"
        "    font-weight: bold;"
        "    color: #27ae60;"
        "    padding: 2px 0px;"
        "}"
    );
    int days = getDays();
    priceLabel->setText(QString("Стоимость за %1 ночи: ... $").arg(days));
    cardLayout->addWidget(priceLabel);
    
    if (rateType == RateType::NonRefundable) {
        QHBoxLayout* includedLayout = new QHBoxLayout();
        QLabel* giftIcon = new QLabel("🎁", card);
        giftIcon->setStyleSheet("QLabel { font-size: 12pt; }");
        QLabel* includedLabel = new QLabel("ВКЛЮЧЕНО: Мероприятия каждый вечер, Тренажерная зона", card);
        includedLabel->setStyleSheet(
            "QLabel {"
            "    font-size: 8pt;"
            "    color: #6c757d;"
            "}"
        );
        includedLayout->addWidget(giftIcon);
        includedLayout->addWidget(includedLabel);
        includedLayout->addStretch();
        cardLayout->addLayout(includedLayout);
    }
    
    if (rateType == RateType::NonRefundable) {
        QHBoxLayout* discountLayout = new QHBoxLayout();
        discountLayout->addStretch();
        QLabel* discountBadge = new QLabel("-10%", card);
        discountBadge->setStyleSheet(
            "QLabel {"
            "    background-color: #e74c3c;"
            "    color: white;"
            "    font-size: 9pt;"
            "    font-weight: bold;"
            "    padding: 3px 6px;"
            "    border-radius: 4px;"
            "}"
        );
        discountLayout->addWidget(discountBadge);
        cardLayout->addLayout(discountLayout);
    }
    
    layout->addWidget(card);
}


