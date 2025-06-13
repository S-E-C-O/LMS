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
    QString isbn;
    std::time_t borrowTime;

    BorrowEntry(const QString& isbn, std::time_t time)
        : isbn(isbn), borrowTime(time) {}
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

