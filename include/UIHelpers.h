#pragma once
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QLabel>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QWidget>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QString>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtCore/QTimer>

struct ButtonStyleParams {
    QString normalColor;
    QString hoverColor;
    QString pressedColor;
    QString fontSize;
    QString padding;
    QString borderRadius;
    QString minWidth;
    QString minHeight;
    
    ButtonStyleParams(const QString& normal, const QString& hover, const QString& pressed,
                     const QString& fontSize = "12pt", const QString& padding = "10px 30px",
                     const QString& borderRadius = "6px", const QString& minWidth = "", const QString& minHeight = "")
        : normalColor(normal), hoverColor(hover), pressedColor(pressed),
          fontSize(fontSize), padding(padding), borderRadius(borderRadius),
          minWidth(minWidth), minHeight(minHeight) {}
};

class UIHelpers {
public:
    static void showNotification(QWidget* parent, const QString& message, bool isError, int timeoutMs = 4000);
    static QString getGreenButtonStyle(const QString& fontSize = "12pt", const QString& padding = "10px 30px");
    static QString getGreenSmallButtonStyle();
    static QString getRedButtonStyle(const QString& fontSize = "9pt", const QString& padding = "6px 14px");
    static QString getRedSmallButtonStyle();
    static QString getBlueSmallButtonStyle();
    static QString getBlueButtonStyle(const QString& fontSize = "11pt", const QString& padding = "10px 20px", const QString& minWidth = "150px");
    static QString getOrangeSmallButtonStyle();
    static QString getGrayButtonStyle(const QString& fontSize = "9pt", const QString& padding = "6px 14px");
    static QString getGraySmallButtonStyle();
    static QString getGroupBoxStyle();
    static QString getBackgroundGradientStyle();
};

