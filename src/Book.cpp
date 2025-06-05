//
// Created by BlackCyan on 25-6-4.
//

#include "Book.h"
#include <stdexcept>
#include <algorithm>

Book::Book(std::string_view title,
           std::string_view author,
           std::string_view publisher,
           const long publishYear,
           const long ISBN,
           const int totalCopies)
    : publishYear(publishYear), ISBN(ISBN), availableCopies(totalCopies), totalCopies(totalCopies) {

    auto copyString = [](const std::string_view& src, char* dest, const size_t maxSize) {
        if (src.size() >= maxSize) {
            throw std::invalid_argument("String too long");
        }
        std::ranges::copy(src, dest);
        dest[src.size()] = '\0';
    };

    copyString(title, this->title, MAX_SIZE);
    copyString(author, this->author, MAX_SIZE);
    copyString(publisher, this->publisher, MAX_SIZE);
}

void Book::serialize(std::ofstream& out) const {
    out.write(title, MAX_SIZE);
    out.write(author, MAX_SIZE);
    out.write(publisher, MAX_SIZE);
    out.write(reinterpret_cast<const char*>(&publishYear), sizeof(publishYear));
    out.write(reinterpret_cast<const char*>(&ISBN), sizeof(ISBN));
    out.write(reinterpret_cast<const char*>(&availableCopies), sizeof(availableCopies));
    out.write(reinterpret_cast<const char*>(&totalCopies), sizeof(totalCopies));
}

void Book::deserialize(std::ifstream& in) {
    in.read(title, MAX_SIZE);
    in.read(author, MAX_SIZE);
    in.read(publisher, MAX_SIZE);
    in.read(reinterpret_cast<char*>(&publishYear), sizeof(publishYear));
    in.read(reinterpret_cast<char*>(&ISBN), sizeof(ISBN));
    in.read(reinterpret_cast<char*>(&availableCopies), sizeof(availableCopies));
    in.read(reinterpret_cast<char*>(&totalCopies), sizeof(totalCopies));
}

// Getter
const char* Book::getTitle() const { return title; }
const char* Book::getAuthor() const { return author; }
const char* Book::getPublisher() const { return publisher; }
long Book::getPublishYear() const { return publishYear; }
long Book::getISBN() const { return ISBN; }
int Book::getAvailableCopies() const { return availableCopies; }
int Book::getTotalCopies() const { return totalCopies; }

void Book::decreaseAvailableCopies() {
    if (availableCopies > 0) --availableCopies;
}

void Book::increaseAvailableCopies() {
    if (availableCopies < totalCopies) ++availableCopies;
}
