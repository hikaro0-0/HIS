#include "C:/Users/User/Desktop/HIS/include/UIHelpers.h"
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QPoint>

namespace {
    const QString SMALL_BUTTON_FONT_SIZE = "8pt";
    const QString SMALL_BUTTON_PADDING = "4px 10px";
    const QString SMALL_BUTTON_BORDER_RADIUS = "3px";
}

static QString buildButtonStyle(const ButtonStyleParams& params) {
    QString style = QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    font-size: %2;"
        "    font-weight: bold;"
        "    padding: %3;"
        "    border: none;"
        "    border-radius: %4;"
    ).arg(params.normalColor, params.fontSize, params.padding, params.borderRadius);
    
    if (!params.minWidth.isEmpty()) {
        style += QString("    min-width: %1;").arg(params.minWidth);
    }
    if (!params.minHeight.isEmpty()) {
        style += QString("    min-height: %1;").arg(params.minHeight);
    }
    
    style += QString(
        "}"
        "QPushButton:hover {"
        "    background-color: %1;"
        "}"
        "QPushButton:pressed {"
        "    background-color: %2;"
        "}"
    ).arg(params.hoverColor, params.pressedColor);
    
    return style;
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
    return buildButtonStyle(ButtonStyleParams("#27ae60", "#229954", "#1e8449", fontSize, padding, "6px", "150px", "35px"));
}

QString UIHelpers::getRedButtonStyle(const QString& fontSize, const QString& padding) {
    return buildButtonStyle(ButtonStyleParams("#e74c3c", "#c0392b", "#a93226", fontSize, padding, "4px", "90px"));
}

QString UIHelpers::getGreenSmallButtonStyle() {
    return buildButtonStyle(ButtonStyleParams("#27ae60", "#229954", "#1e8449", SMALL_BUTTON_FONT_SIZE, SMALL_BUTTON_PADDING, SMALL_BUTTON_BORDER_RADIUS));
}

QString UIHelpers::getRedSmallButtonStyle() {
    return buildButtonStyle(ButtonStyleParams("#e74c3c", "#c0392b", "#a93226", SMALL_BUTTON_FONT_SIZE, SMALL_BUTTON_PADDING, SMALL_BUTTON_BORDER_RADIUS));
}

QString UIHelpers::getBlueSmallButtonStyle() {
    return buildButtonStyle(ButtonStyleParams("#3498db", "#2980b9", "#21618c", SMALL_BUTTON_FONT_SIZE, SMALL_BUTTON_PADDING, SMALL_BUTTON_BORDER_RADIUS));
}

QString UIHelpers::getBlueButtonStyle(const QString& fontSize, const QString& padding, const QString& minWidth) {
    return buildButtonStyle(ButtonStyleParams("#3498db", "#2980b9", "#21618c", fontSize, padding, "6px", minWidth));
}

QString UIHelpers::getOrangeSmallButtonStyle() {
    return buildButtonStyle(ButtonStyleParams("#f39c12", "#e67e22", "#d68910", SMALL_BUTTON_FONT_SIZE, SMALL_BUTTON_PADDING, SMALL_BUTTON_BORDER_RADIUS));
}

QString UIHelpers::getGraySmallButtonStyle() {
    return buildButtonStyle(ButtonStyleParams("#95a5a6", "#7f8c8d", "#6c7a7b", SMALL_BUTTON_FONT_SIZE, SMALL_BUTTON_PADDING, SMALL_BUTTON_BORDER_RADIUS));
}

QString UIHelpers::getGrayButtonStyle(const QString& fontSize, const QString& padding) {
    return buildButtonStyle(ButtonStyleParams("#95a5a6", "#7f8c8d", "#6c7a7b", fontSize, padding, "4px", "90px"));
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

