#include "C:/Users/User/Desktop/HIS/include/UIHelpers.h"
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QPoint>

static QString buildSmallButtonStyle(const QString& normalColor, const QString& hoverColor, const QString& pressedColor) {
    return QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    font-size: 8pt;"
        "    font-weight: bold;"
        "    padding: 4px 10px;"
        "    border: none;"
        "    border-radius: 3px;"
        "}"
        "QPushButton:hover {"
        "    background-color: %2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: %3;"
        "}"
    ).arg(normalColor, hoverColor, pressedColor);
}

void UIHelpers::showNotification(QWidget* parent, const QString& message, bool isError, int timeoutMs) {
    QLabel* notification = new QLabel(message, parent);
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
    
    QPoint pos = parent->mapToGlobal(QPoint(20, parent->height() - notification->sizeHint().height() - 40));
    notification->move(pos);
    notification->resize(330, notification->sizeHint().height());
    notification->show();
    
    QTimer::singleShot(timeoutMs, notification, &QLabel::close);
}

QString UIHelpers::getGreenButtonStyle(const QString& fontSize, const QString& padding) {
    return QString(
        "QPushButton {"
        "    background-color: #27ae60;"
        "    color: white;"
        "    font-size: %1;"
        "    font-weight: bold;"
        "    padding: %2;"
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
    ).arg(fontSize, padding);
}

QString UIHelpers::getRedButtonStyle(const QString& fontSize, const QString& padding) {
    return QString(
        "QPushButton {"
        "    background-color: #e74c3c;"
        "    color: white;"
        "    font-weight: bold;"
        "    font-size: %1;"
        "    padding: %2;"
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
    ).arg(fontSize, padding);
}

QString UIHelpers::getGreenSmallButtonStyle() {
    return buildSmallButtonStyle("#27ae60", "#229954", "#1e8449");
}

QString UIHelpers::getRedSmallButtonStyle() {
    return buildSmallButtonStyle("#e74c3c", "#c0392b", "#a93226");
}

QString UIHelpers::getBlueSmallButtonStyle() {
    return buildSmallButtonStyle("#3498db", "#2980b9", "#21618c");
}

QString UIHelpers::getOrangeSmallButtonStyle() {
    return buildSmallButtonStyle("#f39c12", "#e67e22", "#d68910");
}

QString UIHelpers::getGraySmallButtonStyle() {
    return buildSmallButtonStyle("#95a5a6", "#7f8c8d", "#6c7a7b");
}

QString UIHelpers::getGrayButtonStyle(const QString& fontSize, const QString& padding) {
    return QString(
        "QPushButton {"
        "    background-color: #95a5a6;"
        "    color: white;"
        "    font-size: %1;"
        "    font-weight: bold;"
        "    padding: %2;"
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
    ).arg(fontSize, padding);
}

QString UIHelpers::getGroupBoxStyle() {
    return QString(
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
}

QString UIHelpers::getBackgroundGradientStyle() {
    return QString(
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "    stop:0 #f8f9fa, stop:1 #e9ecef);"
    );
}

