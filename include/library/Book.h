//
// Created by BlackCyan on 25-6-4.
//

#pragma once
#include <fstream>
#include <qstring.h>

class Book {
    static constexpr size_t MAX_SIZE = 50;

    char title[MAX_SIZE]{};
    char author[MAX_SIZE]{};
    char publisher[MAX_SIZE]{};
    int publishYear{};
    QString ISBN{};
    int availableCopies{};
    int totalCopies{};

public:
    explicit Book(std::string_view title = "",
                  std::string_view author = "",
                  std::string_view publisher = "",
                  int publishYear = 0,
                  const QString &ISBN = "",
                  int totalCopies = 1);

    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

    [[nodiscard]] const char* getTitle() const;
    [[nodiscard]] const char* getAuthor() const;
    [[nodiscard]] const char* getPublisher() const;
    [[nodiscard]] int getPublishYear() const;
    [[nodiscard]] const QString& getISBN() const;
    [[nodiscard]] int getAvailableCopies() const;
    [[nodiscard]] int getTotalCopies() const;

    void setTitle(std::string_view title);
    void setAuthor(std::string_view author);
    void setPublisher(std::string_view publisher);
    void setPublishYear(int year);
    void setISBN(const QString &isbn);
    void setTotalCopies(int total);

    void decreaseAvailableCopies();
    void increaseAvailableCopies();

    bool isValidISBN() const;

};
