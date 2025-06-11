#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include "LoginWindow.h"
#include "AdminWindow.h"
#include "MainWindow.h"
#include "Library.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Library library;
    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);

    std::filesystem::path userPath = (dataDir + "/user.dat").toStdString();
    std::filesystem::path bookPath = (dataDir + "/book.dat").toStdString();
    library.setDataFilePaths(userPath, bookPath);
    library.loadFromFile(userPath, bookPath);

    LoginWindow login(&library);

    MainWindow* mainWin = nullptr;
    AdminWindow* adminWin = nullptr;

    QObject::connect(&login, &LoginWindow::loginSuccessful, [&](User* user){
        login.hide();

        if(user->getGroup() == Group::Admin) {
            adminWin = new AdminWindow(&library);
            adminWin->show();
        } else {
            mainWin = new MainWindow(&library, user);
            mainWin->show();
        }
    });

    login.show();

    return QApplication::exec();
}
