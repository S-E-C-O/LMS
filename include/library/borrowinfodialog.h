//
// Created by BlackCyan on 25-6-7.
//

#pragma once

#include <QDialog>
#include <QTableWidget>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class BorrowInfoDialog; }
QT_END_NAMESPACE

struct BorrowEntry {
    long isbn;
    std::time_t borrowTime;
};

class BorrowInfoDialog : public QDialog {
    Q_OBJECT

public:
    explicit BorrowInfoDialog(QWidget* parent = nullptr);
    ~BorrowInfoDialog();

    void setBorrowList(const std::vector<BorrowEntry>& list);

private:
    Ui::BorrowInfoDialog* ui;
};

