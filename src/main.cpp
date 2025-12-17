#include "C:/Users/User/Desktop/HIS/include/WelcomeWindow.h"
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QApplication>
#include <C:/Qt/6.10.1/msvc2022_64/include/QtWidgets/QStyleFactory>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setStyle(QStyleFactory::create("Fusion"));

    app.setApplicationName("Hotel Management System");
    app.setOrganizationName("Hotel Management");

    WelcomeWindow welcomeWindow;
    welcomeWindow.showMaximized();

    return app.exec();
}

