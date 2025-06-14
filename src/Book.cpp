//
// Created by BlackCyan on 25-6-4.
//

#include "Book.h"

#include <QRegularExpression>
#include <stdexcept>

Book::Book(std::string_view title,
           std::string_view author,
           std::string_view publisher,
           const int publishYear,
           const QString &ISBN,
           const int totalCopies)
    : publishYear(publishYear),
      availableCopies(totalCopies),
      totalCopies(totalCopies) {

    auto copyString = [](std::string_view src, char* dest, size_t maxSize) {
        if (src.size() >= maxSize)
            throw std::invalid_argument("String too long");
        std::ranges::copy(src, dest);
        dest[src.size()] = '\0';
    };

    copyString(title, this->title, MAX_SIZE);
    copyString(author, this->author, MAX_SIZE);
    copyString(publisher, this->publisher, MAX_SIZE);
    setISBN(ISBN);
}

void Book::serialize(QDataStream& out) const {
    out.writeRawData(title, MAX_SIZE);
    out.writeRawData(author, MAX_SIZE);
    out.writeRawData(publisher, MAX_SIZE);
    out << publishYear;
    out << ISBN;
    out << availableCopies;
    out << totalCopies;
}

void Book::deserialize(QDataStream& in) {
    if (in.readRawData(title, MAX_SIZE) != MAX_SIZE) throw std::runtime_error("Failed to read title");
    if (in.readRawData(author, MAX_SIZE) != MAX_SIZE) throw std::runtime_error("Failed to read author");
    if (in.readRawData(publisher, MAX_SIZE) != MAX_SIZE) throw std::runtime_error("Failed to read publisher");

    in >> publishYear;
    in >> ISBN;
    in >> availableCopies;
    in >> totalCopies;
}

// Getter
const char* Book::getTitle() const { return title; }
const char* Book::getAuthor() const { return author; }
const char* Book::getPublisher() const { return publisher; }
int Book::getPublishYear() const { return publishYear; }
const QString &Book::getISBN() const { return ISBN; }
int Book::getAvailableCopies() const { return availableCopies; }
int Book::getTotalCopies() const { return totalCopies; }

// Setter
void Book::setTitle(std::string_view title) {
    if (title.size() >= MAX_SIZE) throw std::invalid_argument("Title too long");
    std::ranges::copy(title, this->title);
    this->title[title.size()] = '\0';
}
void Book::setAuthor(std::string_view author) {
    if (author.size() >= MAX_SIZE) throw std::invalid_argument("Author too long");
    std::ranges::copy(author, this->author);
    this->author[author.size()] = '\0';
}
void Book::setPublisher(std::string_view publisher) {
    if (publisher.size() >= MAX_SIZE) throw std::invalid_argument("Publisher too long");
    std::ranges::copy(publisher, this->publisher);
    this->publisher[publisher.size()] = '\0';
}
void Book::setPublishYear(int year) {
    if (year < 0 || year > 3000)
        throw std::invalid_argument("Invalid publish year");
    this->publishYear = year;
}

void Book::setISBN(const QString& isbn) {
    ISBN = isbn;
}

void Book::setTotalCopies(int total) {
    if (total < 0)
        throw std::invalid_argument("Total copies cannot be negative");

    if (availableCopies > total)
        availableCopies = total;

    this->totalCopies = total;
}

void Book::decreaseAvailableCopies() {
    if (availableCopies > 0) --availableCopies;
}

void Book::increaseAvailableCopies() {
    if (availableCopies < totalCopies) ++availableCopies;
}

bool Book::isValidISBN() const {
    // 去除ISBN中的连字符和空格
    QString cleanISBN = ISBN;
    const static QRegularExpression re(R"([-\s])");
    cleanISBN.remove(re);

    // ISBN-13 校验
    const static QRegularExpression re13(R"(^\d{13}$)");
    if (cleanISBN.length() == 13 && cleanISBN.contains(re13)) {
        int sum = 0;
        for (int i = 0; i < 12; ++i) {
            int digit = cleanISBN[i].digitValue();
            if (digit < 0) return false;
            sum += (i % 2 == 0) ? digit : digit * 3;
        }
        int checkDigit = (10 - (sum % 10)) % 10;
        return checkDigit == cleanISBN[12].digitValue();
    }

    // ISBN-10 校验
    const static QRegularExpression re10(R"(^[0-9]{9}[0-9Xx]$)");
    if (cleanISBN.length() == 10 && cleanISBN.contains(re10)) {
        int sum = 0;
        for (int i = 0; i < 9; ++i) {
            int digit = cleanISBN[i].digitValue();
            if (digit < 0) return false;
            sum += (10 - i) * digit;
        }
        const char lastChar = cleanISBN[9].toUpper().toLatin1();
        sum += (lastChar == 'X') ? 10 : (lastChar - '0');
        return (sum % 11) == 0;
    }

    // 其它格式不合法
    return false;
}
