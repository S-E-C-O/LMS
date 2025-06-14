//
// Created by BlackCyan on 25-6-6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(Library* library, User* user, QWidget* parent)
    : QMainWindow(parent), library(library), currentUser(user) {
    const QIcon icon(":/resources/favicon.png");
    this->setWindowIcon(icon);
    resize(QSize(1000, 600));

    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("请输入关键词...");

    searchButton = new QPushButton("搜索", this);
    borrowButton = new QPushButton("借阅", this);
    returnButton = new QPushButton("归还", this);
    viewBorrowedButton = new QPushButton("查看已借阅图书", this);
    changePasswordButton = new QPushButton("修改密码", this);

    radioTitle = new QRadioButton("书名", this);
    radioAuthor = new QRadioButton("作者", this);
    radioISBN = new QRadioButton("ISBN", this);
    radioTitle->setChecked(true);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(7);
    tableWidget->setHorizontalHeaderLabels({"书名", "作者", "出版社", "出版年份", "ISBN", "可用数量", "是否已借阅"});
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    auto* layout = new QVBoxLayout(centralWidget);
    auto* buttonLayout = new QHBoxLayout;
    layout->addWidget(tableWidget);

    auto* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(radioTitle);
    searchLayout->addWidget(radioAuthor);
    searchLayout->addWidget(radioISBN);
    layout->addLayout(searchLayout);

    buttonLayout->addWidget(borrowButton);
    buttonLayout->addWidget(returnButton);
    layout->addLayout(buttonLayout);
    layout->addWidget(viewBorrowedButton);
    layout->addWidget(changePasswordButton);

    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(borrowButton, &QPushButton::clicked, this, &MainWindow::onBorrowClicked);
    connect(returnButton, &QPushButton::clicked, this, &MainWindow::onReturnClicked);
    connect(viewBorrowedButton, &QPushButton::clicked, this, &MainWindow::onViewBorrowedClicked);
    connect(changePasswordButton, &QPushButton::clicked, this, &MainWindow::onChangePasswordClicked);

    populateTable(library->getAllBooks());
}

void MainWindow::trySaveData() {
    try {
        library->saveToFile(library->getDataFilePaths()[0], library->getDataFilePaths()[1]);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "保存失败", e.what());
    }
}


void MainWindow::onSearchClicked() const {
    const QString keyword = searchEdit->text().trimmed();
    std::vector<Book> result;

    if (keyword.isEmpty()) {
        result = library->getAllBooks();
    } else if (radioTitle->isChecked()) {
        result = library->searchBooksByTitle(keyword.toStdString());
    } else if (radioAuthor->isChecked()) {
        result = library->searchBooksByAuthor(keyword.toStdString());
    } else if (radioISBN->isChecked()) {
        result = library->searchBooksByISBN(keyword.toStdString());
    }

    populateTable(result);
}



void MainWindow::onBorrowClicked() {
    int row = tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选中一本书！");
        return;
    }
    QString isbn = tableWidget->item(row, 4)->text();  // 第5列是ISBN
    if (library->borrowBook(currentUser->getId(), isbn)) {
        QMessageBox::information(this, "成功", "借阅成功！");
        onSearchClicked();  // 刷新表格
    } else {
        QMessageBox::warning(this, "失败", "借阅失败，可能是已借阅或无库存。");
    }
}

void MainWindow::onReturnClicked() {
    int row = tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选中一本书！");
        return;
    }
    QString isbn = tableWidget->item(row, 4)->text();  // 第5列是ISBN
    if (library->returnBook(currentUser->getId(), isbn)) {
        QMessageBox::information(this, "成功", "归还成功！");
        onSearchClicked();  // 刷新表格
    } else {
        QMessageBox::warning(this, "失败", "归还失败，你未借阅这本书。");
    }
}

void MainWindow::onViewBorrowedClicked() const {
    const auto& borrowedBooks = library->getBooksBorrowedByUser(currentUser->getId());
    populateTable(borrowedBooks);
}


void MainWindow::populateTable(const std::vector<Book>& books) const {
    tableWidget->setRowCount(static_cast<int>(books.size()));
    for (int row = 0; row < books.size(); ++row)
    {
        const Book& book = books[row];
        tableWidget->setItem(row, 0, new QTableWidgetItem(book.getTitle()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(book.getAuthor()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(book.getPublisher()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(book.getPublishYear())));
        tableWidget->setItem(row, 4, new QTableWidgetItem(book.getISBN()));
        tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(book.getAvailableCopies())));

        bool borrowed = library->isBookBorrowedByUser(currentUser->getId(), book.getISBN().toStdString());
        tableWidget->setItem(row, 6, new QTableWidgetItem(borrowed ? "是" : "否"));
    }
}

void MainWindow::onChangePasswordClicked() {
    bool ok1, ok2;
    QString oldPassword = QInputDialog::getText(this, "修改密码", "请输入原密码：", QLineEdit::Password, "", &ok1);
    if (!ok1) return;

    if (oldPassword != QString::fromStdString(currentUser->getPassword())) {
        QMessageBox::warning(this, "错误", "原密码错误！");
        return;
    }

    QString newPassword = QInputDialog::getText(this, "修改密码", "请输入新密码：", QLineEdit::Password, "", &ok2);
    if (!ok2) return;

    if (newPassword.isEmpty()) {
        QMessageBox::warning(this, "错误", "新密码不能为空！");
        return;
    }

    currentUser->setPassword(newPassword.toStdString());
    QMessageBox::information(this, "成功", "密码修改成功！");
    trySaveData();
}
