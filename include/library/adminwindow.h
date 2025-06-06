//
// Created by BlackCyan on 25-6-6.
//

#pragma once
#include <QMainWindow>
#include <QStandardItemModel>
#include "Library.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AdminWindow; }
QT_END_NAMESPACE

class AdminWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit AdminWindow(Library* library, QWidget* parent = nullptr);
    ~AdminWindow() override;

private slots:
    // 图书操作
    void onAddBook();
    void onEditBook();
    void onDeleteBook() const;
    void onSearchBook();

    // 用户操作
    void onAddUser();
    void onEditUser();
    void onDeleteUser() const;
    void onSearchUser();
    void onResetPasswordClicked();

private:
    Ui::AdminWindow* ui;
    Library* library;

    QStandardItemModel* bookModel;
    QStandardItemModel* userModel;

    void setupBookTable() const;
    void setupUserTable() const;
    void refreshBookTable() const;
    void refreshUserTable() const;
};
