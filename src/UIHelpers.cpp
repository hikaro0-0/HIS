#include "C:/Users/User/Desktop/HIS/include/UIHelpers.h"
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QPoint>

namespace {
    const QString SMALL_BUTTON_FONT_SIZE = "8pt";
    const QString SMALL_BUTTON_PADDING = "4px 10px";
    const QString SMALL_BUTTON_BORDER_RADIUS = "3px";
    
    struct ButtonColorScheme {
        QString normal;
        QString hover;
        QString pressed;
    };
    
    const ButtonColorScheme GREEN_SCHEME = {"#27ae60", "#229954", "#1e8449"};
    const ButtonColorScheme RED_SCHEME = {"#e74c3c", "#c0392b", "#a93226"};
    const ButtonColorScheme BLUE_SCHEME = {"#3498db", "#2980b9", "#21618c"};
    const ButtonColorScheme ORANGE_SCHEME = {"#f39c12", "#e67e22", "#d68910"};
    const ButtonColorScheme GRAY_SCHEME = {"#95a5a6", "#7f8c8d", "#6c7a7b"};
    
    struct NotificationColorScheme {
        QString color;
    };
    
    const NotificationColorScheme ERROR_NOTIFICATION = {"#e74c3c"};
    const NotificationColorScheme SUCCESS_NOTIFICATION = {"#27ae60"};
    
    QString buildNotificationStyle(const NotificationColorScheme& scheme) {
        return QString("QLabel {"
            "    background-color: white;"
            "    color: %1;"
            "    padding: 15px 20px;"
            "    border-radius: 8px;"
            "    border: 2px solid %1;"
            "    font-size: 12pt;"
            "    font-weight: bold;"
            "    min-width: 300px;"
            "    max-width: 500px;"
            "}").arg(scheme.color);
    }
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

namespace {
    QString buildSmallButtonStyle(const ButtonColorScheme& scheme) {
        return buildButtonStyle(ButtonStyleParams(scheme.normal, scheme.hover, scheme.pressed, 
                                                   SMALL_BUTTON_FONT_SIZE, SMALL_BUTTON_PADDING, SMALL_BUTTON_BORDER_RADIUS));
    }
}

void UIHelpers::showNotification(QWidget* parent, const QString& message, bool isError, int timeoutMs) {
    QLabel* notification = new QLabel(message, parent);
    const NotificationColorScheme& scheme = isError ? ERROR_NOTIFICATION : SUCCESS_NOTIFICATION;
    
    notification->setStyleSheet(buildNotificationStyle(scheme));
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
    return buildButtonStyle(ButtonStyleParams(GREEN_SCHEME.normal, GREEN_SCHEME.hover, GREEN_SCHEME.pressed, fontSize, padding, "6px", "150px", "35px"));
}

QString UIHelpers::getRedButtonStyle(const QString& fontSize, const QString& padding) {
    return buildButtonStyle(ButtonStyleParams(RED_SCHEME.normal, RED_SCHEME.hover, RED_SCHEME.pressed, fontSize, padding, "4px", "90px"));
}

QString UIHelpers::getGreenSmallButtonStyle() {
    return buildSmallButtonStyle(GREEN_SCHEME);
}

QString UIHelpers::getRedSmallButtonStyle() {
    return buildSmallButtonStyle(RED_SCHEME);
}

QString UIHelpers::getBlueSmallButtonStyle() {
    return buildSmallButtonStyle(BLUE_SCHEME);
}

QString UIHelpers::getBlueButtonStyle(const QString& fontSize, const QString& padding, const QString& minWidth) {
    return buildButtonStyle(ButtonStyleParams(BLUE_SCHEME.normal, BLUE_SCHEME.hover, BLUE_SCHEME.pressed, fontSize, padding, "6px", minWidth));
}

QString UIHelpers::getOrangeSmallButtonStyle() {
    return buildSmallButtonStyle(ORANGE_SCHEME);
}

QString UIHelpers::getGraySmallButtonStyle() {
    return buildSmallButtonStyle(GRAY_SCHEME);
}

QString UIHelpers::getGrayButtonStyle(const QString& fontSize, const QString& padding) {
    return buildButtonStyle(ButtonStyleParams(GRAY_SCHEME.normal, GRAY_SCHEME.hover, GRAY_SCHEME.pressed, fontSize, padding, "4px", "90px"));
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

