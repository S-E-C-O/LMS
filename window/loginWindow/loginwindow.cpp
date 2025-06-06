//
// Created by BlackCyan on 25-6-6.
//

#include "loginwindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QIntValidator>
#include <QMessageBox>

LoginWindow::LoginWindow(Library* library, QWidget* parent)
    : QWidget(parent), library(library) {

    idEdit = new QLineEdit(this);
    idEdit->setPlaceholderText("User ID");
    // 只允许输入数字
    idEdit->setValidator(new QIntValidator(1, 999999999, this));

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    // 按回车登录
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onPasswordReturnPressed);

    loginButton = new QPushButton("Login", this);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(idEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked() {
    if (idEdit->text().isEmpty() || passwordEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Please enter both User ID and Password.");
        return;
    }

    bool ok;
    const long id = idEdit->text().toLong(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Login Failed", "Incorrect User ID or Password.");
        return;
    }

    const std::string password = passwordEdit->text().toStdString();

    if (User* user = library->loginUser(id, password)) {
        emit loginSuccessful(user);
    } else {
        QMessageBox::warning(this, "Login Failed", "Incorrect User ID or Password.");
    }
}

void LoginWindow::onPasswordReturnPressed() {
    onLoginClicked();
}
