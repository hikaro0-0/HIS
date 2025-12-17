#pragma once

#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QDialog>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QLineEdit>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QLabel>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QPushButton>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QSpinBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QComboBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QCheckBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QPushButton>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QVBoxLayout>
#include "C:/Users/User/Desktop/HIS/include/DateSelectionDialog.h"
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QHBoxLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QFormLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QButtonGroup>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QRadioButton>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QFrame>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QScrollArea>
#include "C:/Users/User/Desktop/HIS/include/Guest.h"

enum class RoomType;
enum class RateType;

class GuestDialog : public QDialog {
    Q_OBJECT

public:
    explicit GuestDialog(QWidget* parent = nullptr);
    ~GuestDialog() = default;

    Guest getGuest() const;
    
    int getDays() const;
    
    QDate getCheckInDate() const;
    
    RoomType getRoomType() const;
    
    bool getWifi() const;
    
    bool getMiniBar() const;
    
    bool getBalcony() const;
    
    bool getJacuzzi() const;
    
    double getPrice() const;
    
    RateType getRateType() const;
    
    void setRoomType(RoomType roomType);
    
    bool isValid() const;

private slots:
    void onAccept();
    void onReject();
    void onRoomTypeChanged();
    void updatePrice();
    void onSelectDates();

private:
    void setupUI();
    void updateRoomOptions();
    double calculatePrice() const;
    bool validateInput();
    void showNotification(const QString& message, bool isError = true);

    QLineEdit* nameInput;
    QLineEdit* phoneInput;
    QPushButton* selectDatesButton;
    QLabel* daysLabel;
    QComboBox* roomTypeCombo;
    QCheckBox* miniBarCheckBox;
    QCheckBox* balconyCheckBox;
    QCheckBox* jacuzziCheckBox;
    QLabel* priceLabel;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QButtonGroup* rateButtonGroup;
    QWidget* rateContainer;
    QScrollArea* rateScroll;
    
    int selectedDays;
    QDate selectedCheckInDate;
    
    void createRateCard(RateType rateType, const QString& title, 
                       const QString& cancellation, const QString& payment,
                       QVBoxLayout* layout);
};
