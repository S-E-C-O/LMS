//
// Created by BlackCyan on 25-6-6.
//

#include "AdminWindow.h"
#include "ui_AdminWindow.h"
#include "BorrowInfoDialog.h"
#include "bookeditdialog.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QContextMenuEvent>
#include <QRegularExpression>
#include <QMenu>

AdminWindow::AdminWindow(Library* library, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::AdminWindow), library(library),
      bookModel(new QStandardItemModel(this)), userModel(new QStandardItemModel(this)) {
    ui->setupUi(this);
    setupBookTable();
    setupUserTable();
    refreshBookTable();
    refreshUserTable();

    // 图书按钮连接
    connect(ui->btnAddBook, &QPushButton::clicked, this, &AdminWindow::onAddBook);
    connect(ui->btnEditBook, &QPushButton::clicked, this, &AdminWindow::onEditBook);
    connect(ui->btnDeleteBook, &QPushButton::clicked, this, &AdminWindow::onDeleteBook);
    connect(ui->btnSearchBook, &QPushButton::clicked, this, &AdminWindow::onSearchBook);

    // 用户按钮连接
    connect(ui->btnAddUser, &QPushButton::clicked, this, &AdminWindow::onAddUser);
    connect(ui->btnEditUser, &QPushButton::clicked, this, &AdminWindow::onEditUser);
    connect(ui->btnDeleteUser, &QPushButton::clicked, this, &AdminWindow::onDeleteUser);
    connect(ui->btnSearchUser, &QPushButton::clicked, this, &AdminWindow::onSearchUser);
    connect(ui->btnResetPassword, &QPushButton::clicked, this, &AdminWindow::onResetPasswordClicked);
}

AdminWindow::~AdminWindow() {
    delete ui;
}

void AdminWindow::trySaveData() {
    try {
        library->saveToFile(library->getDataFilePaths()[0], library->getDataFilePaths()[1]);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "保存失败", e.what());
    }
}

void AdminWindow::setupBookTable() const {
    bookModel->setHorizontalHeaderLabels({"Title", "Author", "Publisher", "Year", "ISBN", "Available", "Total"});
    ui->tableViewBooks->setModel(bookModel);
    ui->tableViewBooks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void AdminWindow::setupUserTable() const {
    userModel->setHorizontalHeaderLabels({"ID", "Name", "Group", "Borrowed Count"});
    ui->tableViewUsers->setModel(userModel);
    ui->tableViewUsers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void AdminWindow::refreshBookTable() const {
    bookModel->removeRows(0, bookModel->rowCount());
    for (const auto& book : library->getAllBooks()) {
        QList<QStandardItem*> row;
        row << new QStandardItem(book.getTitle())
            << new QStandardItem(book.getAuthor())
            << new QStandardItem(book.getPublisher())
            << new QStandardItem(QString::number(book.getPublishYear()))
            << new QStandardItem(book.getISBN())  // ISBN用字符串显示
            << new QStandardItem(QString::number(book.getAvailableCopies()))
            << new QStandardItem(QString::number(book.getTotalCopies()));
        bookModel->appendRow(row);
    }
}

void AdminWindow::refreshUserTable() const {
    userModel->removeRows(0, userModel->rowCount());
    for (const auto& user : library->getAllUsers()) {
        QString group = user.getGroup() == Group::Admin ? "Admin" : "User";
        userModel->appendRow({
            new QStandardItem(QString::number(user.getId())),
            new QStandardItem(user.getName()),
            new QStandardItem(group),
            new QStandardItem(QString::number(user.getBorrowedBooks().size()))
        });
    }
}

bool AdminWindow::isValidISBN(const QString& isbn) const {
    // 简单校验：10位或13位数字，10位最后允许是X
    QRegularExpression re("^(\\d{10}|\\d{9}[Xx]|\\d{13})$");
    return re.match(isbn).hasMatch();
}

// =================== 图书操作 ===================

void AdminWindow::onAddBook() {
    BookEditDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString isbnStr = dialog.getISBN();
        if (!isValidISBN(isbnStr)) {
            QMessageBox::warning(this, "错误", "ISBN格式不正确");
            return;
        }
        Book book(dialog.getTitle().toStdString(),
                  dialog.getAuthor().toStdString(),
                  dialog.getPublisher().toStdString(),
                  dialog.getYear(),
                  isbnStr,
                  dialog.getTotal());

        library->addBook(book);
        trySaveData();
        refreshBookTable();
    }
}

void AdminWindow::onEditBook() {
    const QModelIndex index = ui->tableViewBooks->currentIndex();
    if (!index.isValid()) return;

    QString isbnStr = bookModel->item(index.row(), 4)->text();
    Book* book = library->findBookByISBN(isbnStr);
    if (!book) return;

    BookEditDialog dialog(this);
    dialog.setBookInfo(book->getTitle(), book->getAuthor(), book->getPublisher(),
                       book->getPublishYear(), book->getISBN(), book->getTotalCopies());

    if (dialog.exec() == QDialog::Accepted) {
        QString newIsbnStr = dialog.getISBN();
        if (!isValidISBN(newIsbnStr)) {
            QMessageBox::warning(this, "错误", "ISBN格式不正确");
            return;
        }
        book->setTitle(dialog.getTitle().toStdString());
        book->setAuthor(dialog.getAuthor().toStdString());
        book->setPublisher(dialog.getPublisher().toStdString());
        book->setPublishYear(dialog.getYear());
        book->setISBN(newIsbnStr);
        book->setTotalCopies(dialog.getTotal());

        trySaveData();
        refreshBookTable();
    }
}

void AdminWindow::onDeleteBook() const {
    const QModelIndex index = ui->tableViewBooks->currentIndex();
    if (!index.isValid()) return;
    QString isbnStr = bookModel->item(index.row(), 4)->text();
    library->deleteBook(isbnStr);
    refreshBookTable();
}

void AdminWindow::onSearchBook() {
    const QString keyword = QInputDialog::getText(this, "Search Book", "Enter title keyword:");
    const bool showAll = keyword.trimmed().isEmpty();

    bookModel->removeRows(0, bookModel->rowCount());

    for (const auto& book : library->getAllBooks()) {
        if (showAll || QString::fromStdString(book.getTitle()).contains(keyword, Qt::CaseInsensitive)) {
            QList<QStandardItem*> row;
            row << new QStandardItem(book.getTitle())
                << new QStandardItem(book.getAuthor())
                << new QStandardItem(book.getPublisher())
                << new QStandardItem(QString::number(book.getPublishYear()))
                << new QStandardItem(book.getISBN())
                << new QStandardItem(QString::number(book.getAvailableCopies()))
                << new QStandardItem(QString::number(book.getTotalCopies()));
            bookModel->appendRow(row);
        }
    }
}

// =================== 用户操作 ===================

void AdminWindow::onResetPasswordClicked() {
    const QModelIndex index = ui->tableViewUsers->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "提示", "请先选中一个用户！");
        return;
    }

    const long long userId = userModel->item(index.row(), 0)->text().toLongLong();
    User* user = library->findUserById(userId);
    if (!user) {
        QMessageBox::critical(this, "错误", "未找到该用户！");
        return;
    }

    if (QMessageBox::question(this, "确认", "确定要将该用户密码重置为 123456？") != QMessageBox::Yes)
        return;

    user->resetPassword();
    QMessageBox::information(this, "成功", "密码已成功重置为 123456");

    trySaveData();
}

void AdminWindow::contextMenuEvent(QContextMenuEvent* event) {
    QPoint pos = ui->tableViewUsers->viewport()->mapFromGlobal(event->globalPos());
    QModelIndex index = ui->tableViewUsers->indexAt(pos);
    if (!index.isValid()) {
        QMainWindow::contextMenuEvent(event);
        return;
    }

    QMenu menu(this);
    QAction* viewBorrowInfoAction = menu.addAction("查看借阅信息");

    connect(viewBorrowInfoAction, &QAction::triggered, [this, index]() {
        long long userId = userModel->item(index.row(), 0)->text().toLongLong();
        showUserBorrowInfoDialog(userId);
    });

    menu.exec(event->globalPos());
}

void AdminWindow::showUserBorrowInfoDialog(long long userId) {
    const User* user = library->findUserById(userId);
    if (!user) {
        QMessageBox::warning(this, "错误", "未找到该用户");
        return;
    }

    std::vector<BorrowEntry> borrowList;
    for (const auto& isbn : user->getBorrowedBooks()) {
        if (auto borrowTimeOpt = user->getBorrowTime(isbn)) {
            borrowList.push_back({isbn, *borrowTimeOpt});
        }
    }

    BorrowInfoDialog dialog(this);
    dialog.setBorrowList(borrowList);
    dialog.exec();
}

void AdminWindow::onAddUser() {
    bool ok;
    const int id = QInputDialog::getInt(this, "Add User", "User ID:", 0, 0, 999999999, 1, &ok);
    if (!ok) return;
    const QString name = QInputDialog::getText(this, "Add User", "Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    const QStringList groupOptions = {"User", "Admin"};
    const QString groupStr = QInputDialog::getItem(this, "Add User", "Select Group:", groupOptions, 0, false, &ok);
    if (!ok || groupStr.isEmpty()) return;

    Group group = (groupStr == "Admin") ? Group::Admin : Group::User;
    if (!library->registerUser(User(name.toStdString(), "123456", id, group))) {
        QMessageBox::warning(this, "Failed", "User ID already exists.");
        return;
    }

    refreshUserTable();
}

void AdminWindow::onEditUser() {
    const QModelIndex index = ui->tableViewUsers->currentIndex();
    if (!index.isValid()) return;

    const int uid = userModel->item(index.row(), 0)->text().toInt();
    User* user = library->findUserById(uid);
    if (!user) return;

    bool ok;
    const QString name = QInputDialog::getText(this, "Edit User", "New Name:", QLineEdit::Normal, user->getName(), &ok);
    if (!ok || name.isEmpty()) return;

    const QStringList groupOptions = {"User", "Admin"};
    const QString groupStr = QInputDialog::getItem(this, "Edit User", "New Group:", groupOptions,
                                                   user->getGroup() == Group::Admin ? 1 : 0, false, &ok);
    if (!ok || groupStr.isEmpty()) return;

    Group group = (groupStr == "Admin") ? Group::Admin : Group::User;

    user->setName(name.toStdString());
    user->setGroup(group);

    trySaveData();
    refreshUserTable();
}

void AdminWindow::onDeleteUser() const {
    const QModelIndex index = ui->tableViewUsers->currentIndex();
    if (!index.isValid()) return;
    const int uid = userModel->item(index.row(), 0)->text().toInt();
    library->deleteUser(uid);
    refreshUserTable();
}

void AdminWindow::onSearchUser() {
    const QString keyword = QInputDialog::getText(this, "Search User", "Enter user name:");
    const bool showAll = keyword.trimmed().isEmpty();

    userModel->removeRows(0, userModel->rowCount());

    for (const auto& user : library->getAllUsers()) {
        if (showAll || QString::fromStdString(user.getName()).contains(keyword, Qt::CaseInsensitive)) {
            QString group = user.getGroup() == Group::Admin ? "Admin" : "User";
            userModel->appendRow({
                new QStandardItem(QString::number(user.getId())),
                new QStandardItem(user.getName()),
                new QStandardItem(group),
                new QStandardItem(QString::number(user.getBorrowedBooks().size()))
            });
        }
    }
}
