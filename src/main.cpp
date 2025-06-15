#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include "LoginWindow.h"
#include "AdminWindow.h"
#include "MainWindow.h"
#include "Library.h"
#include <QFile>
#include <QDebug>

void removeReadOnlyAttribute(const QString& filePath) {
    QFile file(filePath);
    if (file.exists() && !file.isWritable()) {
        if (file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner |
                                QFileDevice::ReadGroup | QFileDevice::WriteGroup |
                                QFileDevice::ReadOther | QFileDevice::WriteOther)) {
            qDebug() << "Removed read-only attribute from:" << filePath;
                                } else {
                                    qWarning() << "Failed to remove read-only attribute from:" << filePath;
                                }
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);

    QString userFile = dataDir + "/user.dat";
    QString bookFile = dataDir + "/book.dat";

    removeReadOnlyAttribute(userFile);
    removeReadOnlyAttribute(bookFile);

    std::filesystem::path userPath = userFile.toStdString();
    std::filesystem::path bookPath = bookFile.toStdString();

    Library library(userPath, bookPath);

    LoginWindow login(&library);

    MainWindow* mainWin = nullptr;
    AdminWindow* adminWin = nullptr;

    QObject::connect(&login, &LoginWindow::loginSuccessful, [&](User* user){
        login.hide();

        if(user->getGroup() == Group::Admin) {
            adminWin = new AdminWindow(&library, user);
            adminWin->show();
        } else {
            mainWin = new MainWindow(&library, user);
            mainWin->show();
        }
    });

    login.show();

    return QApplication::exec();
}
