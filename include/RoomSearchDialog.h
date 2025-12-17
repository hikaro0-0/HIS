#pragma once

#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QDialog>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QCheckBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QSpinBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QDoubleSpinBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QTableWidget>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QPushButton>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QVBoxLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QHBoxLayout>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QGroupBox>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QLabel>
#include "C:/Users/User/Desktop/HIS/include/HotelSystem.h"

class RoomSearchDialog : public QDialog {
    Q_OBJECT

public:
    explicit RoomSearchDialog(HotelSystem* hotelSystem, QWidget* parent = nullptr);
    ~RoomSearchDialog();

private slots:
    void onSearchCriteriaChanged();
    void onClearSearch();

private:
    void setupUI();
    void updateResultsTable();
    bool matchesSearchCriteria(const Room* room) const;
    QString getStatusString(RoomStatus status);
    QString getRoomTypeString(const Room* room);

    HotelSystem* hotelSystem;
    QTableWidget* resultsTable;
    QCheckBox* searchWiFiCheckBox;
    QCheckBox* searchMiniBarCheckBox;
    QCheckBox* searchBalconyCheckBox;
    QCheckBox* searchJacuzziCheckBox;
    QSpinBox* searchBedCountSpinBox;
    QDoubleSpinBox* searchPriceMinSpinBox;
    QDoubleSpinBox* searchPriceMaxSpinBox;
    QPushButton* clearSearchButton;
};

