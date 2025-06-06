#include <QApplication>
#include "LoginWindow.h"
#include "AdminWindow.h"
#include "MainWindow.h"
#include "Library.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Library library;
    const auto userPath = std::filesystem::current_path().parent_path() / "data" / "user.dat";
    const auto bookPath = std::filesystem::current_path().parent_path() / "data" / "book.dat";
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
