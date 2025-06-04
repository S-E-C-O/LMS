//
// Created by BlackCyan on 25-6-4.
//

#include "Book.h"

#include <stdexcept>

Book::Book(std::string_view title, std::string_view author, std::string_view publisher, const long publishYear, const long ISBN) : publishYear(publishYear), ISBN(ISBN) {

    // 检查并复制 title
    if (title.size() >= MAX_SIZE) {
        throw std::invalid_argument("Title too long");
    }
    std::ranges::copy(title, this->title);
    this->title[title.size()] = '\0';

    // 检查并复制 author
    if (author.size() >= MAX_SIZE) {
        throw std::invalid_argument("Author too long");
    }
    std::ranges::copy(author, this->author);
    this->author[author.size()] = '\0';

    // 检查并复制 publisher
    if (publisher.size() >= MAX_SIZE) {
        throw std::invalid_argument("Publisher too long");
    }
    std::ranges::copy(publisher, this->publisher);
    this->publisher[publisher.size()] = '\0';
}

const char* Book::getTitle() const {
    return title;
}

const char* Book::getAuthor() const {
    return author;
}

const char* Book::getPublisher() const {
    return publisher;
}

long Book::getPublishYear() const {
    return publishYear;
}

long Book::getISBN() const {
    return ISBN;
}
