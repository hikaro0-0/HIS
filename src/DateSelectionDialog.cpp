#include "C:/Users/User/Desktop/HIS/include/DateSelectionDialog.h"
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QApplication>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtGui/QTextCharFormat>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QDate>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QWidget>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtGui/QColor>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtGui/QFont>

DateSelectionDialog::DateSelectionDialog(QWidget* parent)
    : QDialog(parent) {
    setupUI();
}

void DateSelectionDialog::setupUI() {
    setWindowTitle("Выбор дат заезда и выезда");
    setFixedSize(600, 500);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    calendar = new QCalendarWidget(this);
    calendar->setMinimumDate(QDate::currentDate());
    calendar->setGridVisible(true);
    mainLayout->addWidget(calendar);
    
    QHBoxLayout* dateLayout = new QHBoxLayout();
    
    checkInDateEdit = new QDateEdit(this);
    checkInDateEdit->setDate(QDate::currentDate());
    checkInDateEdit->setCalendarPopup(true);
    checkInDateEdit->setDisplayFormat("yyyy-MM-dd");
    dateLayout->addWidget(new QLabel("Заезд:", this));
    dateLayout->addWidget(checkInDateEdit);
    
    checkOutDateEdit = new QDateEdit(this);
    checkOutDateEdit->setDate(QDate::currentDate().addDays(1));
    checkOutDateEdit->setCalendarPopup(true);
    checkOutDateEdit->setDisplayFormat("yyyy-MM-dd");
    dateLayout->addWidget(new QLabel("Выезд:", this));
    dateLayout->addWidget(checkOutDateEdit);
    
    dateLayout->addStretch();
    mainLayout->addLayout(dateLayout);
    
    daysLabel = new QLabel("1 день", this);
    daysLabel->setStyleSheet("QLabel { font-size: 12pt; font-weight: bold; color: #0066cc; }");
    daysLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(daysLabel);
    
    infoLabel = new QLabel("Выберите даты на календаре или используйте поля ввода выше", this);
    infoLabel->setStyleSheet("QLabel { color: #666; font-size: 9pt; }");
    infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(infoLabel);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    okButton = new QPushButton("ОК", this);
    okButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; font-weight: bold; padding: 8px 20px; border-radius: 5px; }"
                            "QPushButton:hover { background-color: #229954; }"
                            "QPushButton:pressed { background-color: #1e8449; }");
    connect(okButton, &QPushButton::clicked, this, &DateSelectionDialog::onAccept);
    
    cancelButton = new QPushButton("Отмена", this);
    cancelButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; font-weight: bold; padding: 8px 20px; border-radius: 5px; }"
                                "QPushButton:hover { background-color: #7f8c8d; }"
                                "QPushButton:pressed { background-color: #6c7a7b; }");
    connect(cancelButton, &QPushButton::clicked, this, &DateSelectionDialog::onReject);
    
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    connect(checkInDateEdit, &QDateEdit::dateChanged, this, &DateSelectionDialog::onCheckInDateChanged);
    connect(checkOutDateEdit, &QDateEdit::dateChanged, this, &DateSelectionDialog::onCheckOutDateChanged);
    connect(calendar, &QCalendarWidget::selectionChanged, this, &DateSelectionDialog::onCalendarSelectionChanged);
    
    selectedCheckInDate = QDate::currentDate();
    selectedCheckOutDate = QDate::currentDate().addDays(1);
    updateDaysLabel();
    updateCalendarColors();
}

void DateSelectionDialog::onCheckInDateChanged(const QDate& date) {
    disconnect(checkInDateEdit, &QDateEdit::dateChanged, this, &DateSelectionDialog::onCheckInDateChanged);
    
    selectedCheckInDate = date;
    calendar->setSelectedDate(date);
    
    connect(checkInDateEdit, &QDateEdit::dateChanged, this, &DateSelectionDialog::onCheckInDateChanged);
    
    updateDaysLabel();
    validateDates();
    updateCalendarColors();
}

void DateSelectionDialog::onCheckOutDateChanged(const QDate& date) {
    disconnect(checkOutDateEdit, &QDateEdit::dateChanged, this, &DateSelectionDialog::onCheckOutDateChanged);
    
    selectedCheckOutDate = date;
    calendar->setSelectedDate(date);
    
    connect(checkOutDateEdit, &QDateEdit::dateChanged, this, &DateSelectionDialog::onCheckOutDateChanged);
    
    updateDaysLabel();
    validateDates();
    updateCalendarColors();
}

void DateSelectionDialog::onCalendarSelectionChanged() {
    QDate selectedDate = calendar->selectedDate();
    
    if (selectedCheckInDate == selectedCheckOutDate || selectedDate <= selectedCheckInDate) {
        selectedCheckInDate = selectedDate;
        checkInDateEdit->setDate(selectedDate);
        if (selectedCheckOutDate <= selectedCheckInDate) {
            selectedCheckOutDate = selectedCheckInDate.addDays(1);
            checkOutDateEdit->setDate(selectedCheckOutDate);
        }
    } else {
        selectedCheckOutDate = selectedDate;
        checkOutDateEdit->setDate(selectedDate);
    }
    
    updateDaysLabel();
    validateDates();
    updateCalendarColors();
}

void DateSelectionDialog::updateDaysLabel() {
    int days = selectedCheckInDate.daysTo(selectedCheckOutDate);
    if (days < 1) days = 1;
    
    QString daysText;
    if (days == 1) {
        daysText = "1 день";
    } else if (days >= 2 && days <= 4) {
        daysText = QString("%1 дня").arg(days);
    } else {
        daysText = QString("%1 дней").arg(days);
    }
    daysLabel->setText(daysText);
}

void DateSelectionDialog::validateDates() {
    if (selectedCheckOutDate <= selectedCheckInDate) {
        selectedCheckOutDate = selectedCheckInDate.addDays(1);
        checkOutDateEdit->setDate(selectedCheckOutDate);
    }
    
    QDate today = QDate::currentDate();
    if (selectedCheckInDate < today) {
        selectedCheckInDate = today;
        checkInDateEdit->setDate(today);
    }
    if (selectedCheckOutDate < today.addDays(1)) {
        selectedCheckOutDate = today.addDays(1);
        checkOutDateEdit->setDate(selectedCheckOutDate);
    }
}

void DateSelectionDialog::updateCalendarColors() {
    calendar->setDateTextFormat(QDate(), QTextCharFormat());
    
    QTextCharFormat checkInFormat;
    checkInFormat.setBackground(QColor(76, 175, 80));
    checkInFormat.setForeground(QColor(255, 255, 255));
    checkInFormat.setFontWeight(QFont::Bold);
    calendar->setDateTextFormat(selectedCheckInDate, checkInFormat);
    
    QTextCharFormat checkOutFormat;
    checkOutFormat.setBackground(QColor(139, 0, 0));
    checkOutFormat.setForeground(QColor(255, 255, 255));
    checkOutFormat.setFontWeight(QFont::Bold);
    calendar->setDateTextFormat(selectedCheckOutDate, checkOutFormat);
    
    QTextCharFormat stayFormat;
    stayFormat.setBackground(QColor(200, 255, 200));
    stayFormat.setForeground(QColor(0, 0, 0));
    
    QDate currentDate = selectedCheckInDate.addDays(1);
    while (currentDate < selectedCheckOutDate) {
        calendar->setDateTextFormat(currentDate, stayFormat);
        currentDate = currentDate.addDays(1);
    }
}

void DateSelectionDialog::onAccept() {
    validateDates();
    accept();
}

void DateSelectionDialog::onReject() {
    reject();
}

QDate DateSelectionDialog::getCheckInDate() const {
    return selectedCheckInDate;
}

QDate DateSelectionDialog::getCheckOutDate() const {
    return selectedCheckOutDate;
}

int DateSelectionDialog::getDays() const {
    int days = selectedCheckInDate.daysTo(selectedCheckOutDate);
    return (days < 1) ? 1 : days;
}

