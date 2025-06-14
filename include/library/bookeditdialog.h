//
// Created by BlackCyan on 25-6-7.
//

#pragma once

#include <QDialog>

namespace Ui {
    class BookEditDialog;
}

class BookEditDialog : public QDialog {
    Q_OBJECT

public:
    explicit BookEditDialog(QWidget *parent = nullptr);
    ~BookEditDialog() override;

    void setBookInfo(const QString &title, const QString &author, const QString &publisher,
                     long year, const QString &isbn, int totalCopies) const;

    [[nodiscard]] QString getTitle() const;
    [[nodiscard]] QString getAuthor() const;
    [[nodiscard]] QString getPublisher() const;
    [[nodiscard]] long getYear() const;
    [[nodiscard]] QString getISBN() const;
    [[nodiscard]] int getTotal() const;

private:
    Ui::BookEditDialog *ui;
};
