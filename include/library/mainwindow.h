//
// Created by BlackCyan on 25-6-6.
//

#pragma once

#include <QMainWindow>
#include <QRadioButton>
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
    void onBorrowClicked();
    void onReturnClicked();
    void onViewBorrowedClicked();

private:
    QLineEdit* searchEdit;
    QRadioButton* radioTitle;
    QRadioButton* radioAuthor;
    QRadioButton* radioISBN;
    QPushButton* searchButton;
    QPushButton* borrowButton;
    QPushButton* returnButton;
    QPushButton* viewBorrowedButton;
    QTableWidget* tableWidget;
    Library* library;
    User* currentUser;

    void populateTable(const std::vector<Book>& books) const;
};
