#pragma once

#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QMainWindow>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QTableWidget>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QPushButton>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QComboBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QLineEdit>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QLabel>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QSpinBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QTextEdit>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QVBoxLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QHBoxLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QGroupBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QMessageBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QFileDialog>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QHeaderView>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QRadioButton>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QButtonGroup>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QCheckBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QDoubleSpinBox>
#include "C:/Users/User/Desktop/HIS/include/HotelSystem.h"
#include "C:/Users/User/Desktop/HIS/include/GuestDialog.h"
#include <optional>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
   explicit MainWindow(QWidget* parent = nullptr, HotelSystem* sharedSystem = nullptr);
    ~MainWindow();

private slots:
    void onBookRoom();
    void onCheckIn();
    void onCheckOut();
    void onCancelBooking();
    void onDisplayAllRooms();
    void onDisplayBookedRooms();
    void onDisplayOccupiedRooms();
    void onShowStatistics();
    void onClearData();
    void onFilterChanged();
    void onSearchRooms();

private:
    void setupUI();
    void updateRoomTable();
    void updateStatistics();
    void showRoomDetailsDialog(const Room* room);
    void autoSave();
    QString getStatusString(RoomStatus status) const;
    QString getRoomTypeString(const Room* room) const;
    void showError(const QString& message);
    void showSuccess(const QString& message);
    int getSelectedRoomNumber() const;
    QWidget* createActionsWidget(const Room* room);
    void addRoomRowToTable(const Room* room, const QColor& backgroundColor);
    void selectRoomInTableAndExecute(const Room* room, std::function<void()> action);

    HotelSystem* hotelSystem;
    bool ownsHotelSystem;

    QTableWidget* roomTable;
    QLabel* statisticsLabel;

    QPushButton* clearButton;
    QPushButton* bookRoomButton;
    QButtonGroup* filterButtonGroup;
    QRadioButton* filterAllButton;
    QRadioButton* filterBookedButton;
    QRadioButton* filterOccupiedButton;
    std::optional<RoomStatus> currentFilter;
};


