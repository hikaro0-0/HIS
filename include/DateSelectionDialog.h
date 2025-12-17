#pragma once

#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QDialog>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QCalendarWidget>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QDateEdit>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QLabel>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QPushButton>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QVBoxLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QHBoxLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QMessageBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QDate>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QWidget>

class DateSelectionDialog : public QDialog {
    Q_OBJECT

public:
    explicit DateSelectionDialog(QWidget* parent = nullptr);
    ~DateSelectionDialog() = default;

    QDate getCheckInDate() const;
    QDate getCheckOutDate() const;
    int getDays() const;

private slots:
    void onCheckInDateChanged(const QDate& date);
    void onCheckOutDateChanged(const QDate& date);
    void onCalendarSelectionChanged();
    void onAccept();
    void onReject();

private:
    void setupUI();
    void updateDaysLabel();
    void validateDates();
    void updateCalendarColors();

    QCalendarWidget* calendar;
    QDateEdit* checkInDateEdit;
    QDateEdit* checkOutDateEdit;
    QLabel* daysLabel;
    QLabel* infoLabel;
    QPushButton* okButton;
    QPushButton* cancelButton;

    QDate selectedCheckInDate;
    QDate selectedCheckOutDate;
};

