//
// Created by BlackCyan on 25-6-6.
//

#pragma once

#include <QMainWindow>
#include <QRadioButton>
#include "Library.h"

class QComboBox;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(Library* library, User* user, QWidget* parent = nullptr);

private slots:
    void onSearchClicked() const;
    void onBorrowClicked();
    void onReturnClicked();
    void onViewBorrowedClicked() const;
    void onChangePasswordClicked();

private:
    Ui::MainWindow* ui;
    Library* library;
    User* currentUser;
    QComboBox* searchTypeCombo;

    void populateTable(const std::vector<Book>& books) const;
    void trySaveData();
};
