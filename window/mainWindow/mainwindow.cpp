#include "mainwindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>

MainWindow::MainWindow(Library* library, User* user, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), library(library), currentUser(user) {
    ui->setupUi(this);
    setWindowIcon(QIcon(":/resources/favicon.png"));

    // 设置欢迎语
    if (currentUser) {
        ui->welcomeLabel->setText(QString("你好，%1").arg(currentUser->getName()));
    }

    // 表格初始化
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderLabels(
        {"书名", "作者", "出版社", "出版年份", "ISBN", "可用数量", "是否已借阅"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 添加搜索方式 ComboBox
    ui->searchTypeCombo->clear();
    ui->searchTypeCombo->addItems({"书名", "作者", "ISBN"});

    // 连接信号槽
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(ui->borrowButton, &QPushButton::clicked, this, &MainWindow::onBorrowClicked);
    connect(ui->returnButton, &QPushButton::clicked, this, &MainWindow::onReturnClicked);
    connect(ui->viewBorrowedButton, &QPushButton::clicked, this, &MainWindow::onViewBorrowedClicked);
    connect(ui->changePasswordButton, &QPushButton::clicked, this, &MainWindow::onChangePasswordClicked);

    // 初始加载所有书籍
    populateTable(library->getAllBooks());
}

void MainWindow::onSearchClicked() const {
    QString keyword = ui->searchEdit->text().trimmed();
    QString type = ui->searchTypeCombo->currentText();
    std::vector<Book> result;

    if (keyword.isEmpty()) {
        result = library->getAllBooks();
    } else if (type == "书名") {
        result = library->searchBooksByTitle(keyword.toStdString());
    } else if (type == "作者") {
        result = library->searchBooksByAuthor(keyword.toStdString());
    } else if (type == "ISBN") {
        result = library->searchBooksByISBN(keyword.toStdString());
    }

    populateTable(result);
}

void MainWindow::populateTable(const std::vector<Book>& books) const {
    ui->tableWidget->setRowCount(static_cast<int>(books.size()));
    for (int i = 0; i < books.size(); ++i) {
        const Book& book = books[i];
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(book.getTitle()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(book.getAuthor()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(book.getPublisher()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(book.getPublishYear())));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(book.getISBN()));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(book.getAvailableCopies())));
        bool borrowed = library->isBookBorrowedByUser(currentUser->getId(), book.getISBN().toStdString());
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(borrowed ? "是" : "否"));
    }
}

void MainWindow::onBorrowClicked() {
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选中一本书！");
        return;
    }
    QString isbn = ui->tableWidget->item(row, 4)->text();
    if (library->borrowBook(currentUser->getId(), isbn)) {
        QMessageBox::information(this, "成功", "借阅成功！");
        onSearchClicked();
    } else {
        QMessageBox::warning(this, "失败", "借阅失败，可能是已借阅或无库存。");
    }
}

void MainWindow::onReturnClicked() {
    const int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选中一本书！");
        return;
    }
    QString isbn = ui->tableWidget->item(row, 4)->text();
    if (library->returnBook(currentUser->getId(), isbn)) {
        QMessageBox::information(this, "成功", "归还成功！");
        onSearchClicked();
    } else {
        QMessageBox::warning(this, "失败", "归还失败，你未借阅这本书。");
    }
}

void MainWindow::onViewBorrowedClicked() const {
    auto borrowed = library->getBooksBorrowedByUser(currentUser->getId());
    populateTable(borrowed);
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
    if (!ok2 || newPassword.isEmpty()) {
        QMessageBox::warning(this, "错误", "新密码不能为空！");
        return;
    }

    currentUser->setPassword(newPassword.toStdString());
    QMessageBox::information(this, "成功", "密码修改成功！");
    trySaveData();
}

void MainWindow::trySaveData() {
    try {
        library->saveToFile(library->getDataFilePaths()[0], library->getDataFilePaths()[1]);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "保存失败", e.what());
    }
}
