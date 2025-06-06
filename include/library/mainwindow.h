//
// Created by BlackCyan on 25-6-6.
//

#pragma once

#include <QMainWindow>
#include "Library.h"

class QLineEdit;
class QPushButton;
class QTableWidget;
class User;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(Library* library, User* user, QWidget* parent = nullptr);

private slots:
    void onSearchClicked() const;

private:
    QLineEdit* searchEdit;
    QPushButton* searchButton;
    QTableWidget* tableWidget;
    Library* library;
    User* currentUser;

    void populateTable(const std::vector<Book>& books) const;
};
