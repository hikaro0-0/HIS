#pragma once

#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QDialog>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QLabel>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QPushButton>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QVBoxLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QHBoxLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QWidget>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QFrame>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QGridLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QMessageBox>
#include "C:/Users/User/Desktop/HIS/include/HotelSystem.h"
#include "C:/Users/User/Desktop/HIS/include/GuestDialog.h"

class WelcomeWindow : public QDialog {
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget* parent = nullptr);
    ~WelcomeWindow();
    void setHotelSystem(HotelSystem* system);
    HotelSystem* getHotelSystem() const;

private slots:
    void onEnterSystem();
    void onBookRoom(RoomType roomType);

private:
    void setupUI();
    void updateRoomCards();
    QFrame* createRoomCard(const QString& title, const QString& size, 
                          const QString& beds, const QString& features, 
                          const QString& price, RoomType roomType, int availableCount);
    int getAvailableRoomsCount(RoomType roomType) const;
    QString getRoomTypeName(RoomType roomType) const;
    
    QPushButton* enterButton;
    QWidget* cardsContainer;
    QGridLayout* cardsLayout;
    HotelSystem* hotelSystem;
    bool ownsHotelSystem;
    QList<QFrame*> roomCards;
};


