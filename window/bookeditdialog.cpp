//
// Created by BlackCyan on 25-6-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_BookEditDialog.h" resolved

#include "bookeditdialog.h"
#include "ui_bookeditdialog.h"

BookEditDialog::BookEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookEditDialog)
{
    ui->setupUi(this);
}

BookEditDialog::~BookEditDialog() {
    delete ui;
}

void BookEditDialog::setBookInfo(const QString& title, const QString& author, const QString& publisher,
                                 long year, const QString& isbn, int totalCopies)
{
    ui->lineEditTitle->setText(title);
    ui->lineEditAuthor->setText(author);
    ui->lineEditPublisher->setText(publisher);
    ui->lineEditYear->setText(QString::number(year));
    ui->lineEditISBN->setText(isbn);
    ui->lineEditTotal->setText(QString::number(totalCopies));
}

QString BookEditDialog::getTitle() const {
    return ui->lineEditTitle->text();
}

QString BookEditDialog::getAuthor() const {
    return ui->lineEditAuthor->text();
}

QString BookEditDialog::getPublisher() const {
    return ui->lineEditPublisher->text();
}

long BookEditDialog::getYear() const {
    return ui->lineEditYear->text().toLong();
}

QString BookEditDialog::getISBN() const {
    return ui->lineEditISBN->text();
}

int BookEditDialog::getTotal() const {
    return ui->lineEditTotal->text().toInt();
}
