//
// Created by BlackCyan on 25-6-6.
//

#pragma once

#include <QWidget>
#include "Library.h"

class QLineEdit;
class QPushButton;

class LoginWindow final : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(Library* library, QWidget* parent = nullptr);

signals:
    void loginSuccessful(User* user);

private slots:
    void onLoginClicked();
    void onPasswordReturnPressed();

private:
    QLineEdit* idEdit;
    QLineEdit* passwordEdit;
    QPushButton* loginButton;
    Library* library;
};
