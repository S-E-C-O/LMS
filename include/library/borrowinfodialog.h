//
// Created by BlackCyan on 25-6-7.
//

#pragma once

#include <QDialog>
#include <QTableWidget>
#include <utility>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class BorrowInfoDialog; }
QT_END_NAMESPACE

struct BorrowEntry {
    QString isbn;
    std::time_t borrowTime;

    BorrowEntry(QString isbn, const std::time_t time)
        : isbn(std::move(isbn)), borrowTime(time) {}
};

class BorrowInfoDialog : public QDialog {
    Q_OBJECT

public:
    explicit BorrowInfoDialog(QWidget* parent = nullptr);
    ~BorrowInfoDialog() override;

    void setBorrowList(const std::vector<BorrowEntry>& list) const;

private:
    Ui::BorrowInfoDialog* ui;
};

