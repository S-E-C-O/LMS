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

MainWindow::MainWindow(Library* library, User* user, QWidget* parent)
    : QMainWindow(parent), library(library), currentUser(user) {
    resize(QSize(1000, 600));

    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("请输入关键词...");

    searchButton = new QPushButton("搜索", this);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(5);
    tableWidget->setHorizontalHeaderLabels({"书名", "作者", "出版社", "出版年份", "ISBN"});
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    auto* layout = new QVBoxLayout(centralWidget);
    layout->addWidget(searchEdit);
    layout->addWidget(searchButton);
    layout->addWidget(tableWidget);

    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);

    populateTable(library->getAllBooks());
}

void MainWindow::onSearchClicked() const {
    const QString keyword = searchEdit->text();
    const std::vector<Book> results = library->searchBooksByTitle(keyword.toStdString());
    populateTable(results);
}

void MainWindow::populateTable(const std::vector<Book>& books) const {
    tableWidget->setRowCount(static_cast<int>(books.size()));
    for (int row = 0; row < books.size(); ++row) {
        const Book& book = books[row];
        tableWidget->setItem(row, 0, new QTableWidgetItem(book.getTitle()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(book.getAuthor()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(book.getPublisher()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(book.getPublishYear())));
        tableWidget->setItem(row, 4, new QTableWidgetItem(book.getISBN()));
    }
}
