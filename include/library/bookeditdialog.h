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

    QString getTitle() const;
    QString getAuthor() const;
    QString getPublisher() const;
    long getYear() const;
    QString getISBN() const;
    int getTotal() const;

private:
    Ui::BookEditDialog *ui;
};
