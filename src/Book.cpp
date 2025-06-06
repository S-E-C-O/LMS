//
// Created by BlackCyan on 25-6-4.
//

#include "Book.h"
#include <stdexcept>
#include <algorithm>

Book::Book(std::string_view title,
           std::string_view author,
           std::string_view publisher,
           long publishYear,
           long ISBN,
           int totalCopies)
    : publishYear(publishYear),
      ISBN(ISBN),
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
}

void Book::serialize(std::ofstream& out) const {
    if (!out) throw std::runtime_error("Invalid output stream");

    out.write(title, MAX_SIZE);
    out.write(author, MAX_SIZE);
    out.write(publisher, MAX_SIZE);
    out.write(reinterpret_cast<const char*>(&publishYear), sizeof(publishYear));
    out.write(reinterpret_cast<const char*>(&ISBN), sizeof(ISBN));
    out.write(reinterpret_cast<const char*>(&availableCopies), sizeof(availableCopies));
    out.write(reinterpret_cast<const char*>(&totalCopies), sizeof(totalCopies));
}


void Book::deserialize(std::ifstream& in) {
    if (!in.read(title, MAX_SIZE)) throw std::runtime_error("Failed to read title");
    if (!in.read(author, MAX_SIZE)) throw std::runtime_error("Failed to read author");
    if (!in.read(publisher, MAX_SIZE)) throw std::runtime_error("Failed to read publisher");

    if (!in.read(reinterpret_cast<char*>(&publishYear), sizeof(publishYear)))
        throw std::runtime_error("Failed to read publishYear");
    if (!in.read(reinterpret_cast<char*>(&ISBN), sizeof(ISBN)))
        throw std::runtime_error("Failed to read ISBN");
    if (!in.read(reinterpret_cast<char*>(&availableCopies), sizeof(availableCopies)))
        throw std::runtime_error("Failed to read availableCopies");
    if (!in.read(reinterpret_cast<char*>(&totalCopies), sizeof(totalCopies)))
        throw std::runtime_error("Failed to read totalCopies");
}



// Getter
const char* Book::getTitle() const { return title; }
const char* Book::getAuthor() const { return author; }
const char* Book::getPublisher() const { return publisher; }
int Book::getPublishYear() const { return publishYear; }
long Book::getISBN() const { return ISBN; }
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

void Book::decreaseAvailableCopies() {
    if (availableCopies > 0) --availableCopies;
}

void Book::increaseAvailableCopies() {
    if (availableCopies < totalCopies) ++availableCopies;
}
