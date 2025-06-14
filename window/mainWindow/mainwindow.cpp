//
// Created by BlackCyan on 25-6-6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

MainWindow::MainWindow(Library* library, User* user, QWidget* parent)
    : QMainWindow(parent), library(library), currentUser(user) {
    resize(QSize(1000, 600));

    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("请输入关键词...");

    searchButton = new QPushButton("搜索", this);
    borrowButton = new QPushButton("借阅", this);
    returnButton = new QPushButton("归还", this);

    radioTitle = new QRadioButton("书名", this);
    radioAuthor = new QRadioButton("作者", this);
    radioISBN = new QRadioButton("ISBN", this);
    radioTitle->setChecked(true);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderLabels({"书名", "作者", "出版社", "出版年份", "ISBN", "可用数量"});
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

    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(borrowButton, &QPushButton::clicked, this, &MainWindow::onBorrowClicked);
    connect(returnButton, &QPushButton::clicked, this, &MainWindow::onReturnClicked);


    populateTable(library->getAllBooks());
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
    }
}
