//
// Created by BlackCyan on 25-6-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_BorrowInfoDialog.h" resolved

#include "BorrowInfoDialog.h"
#include "ui_BorrowInfoDialog.h"
#include <QDateTime>
#include <QDialogButtonBox>

BorrowInfoDialog::BorrowInfoDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::BorrowInfoDialog) {
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ISBN" << "借阅时间");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &BorrowInfoDialog::reject);
}

BorrowInfoDialog::~BorrowInfoDialog() {
    delete ui;
}

void BorrowInfoDialog::setBorrowList(const std::vector<BorrowEntry>& list) {
    ui->tableWidget->setRowCount(static_cast<int>(list.size()));
    for (int i = 0; i < static_cast<int>(list.size()); ++i) {
        const auto& entry = list[i];

        QString isbnStr = entry.isbn;
        QString timeStr = QDateTime::fromSecsSinceEpoch(entry.borrowTime).toString("yyyy-MM-dd hh:mm:ss");

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(isbnStr));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(timeStr));
    }
}

