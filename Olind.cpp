#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Book {
    string title;
    string author;
    int year;
    bool available = true;

public:
    Book(string t, string a, int y) : title(t), author(a), year(y) {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getYear() const { return year; }
    bool isAvailable() const { return available; }
    void setAvailable(bool a) { available = a; }

    friend ostream& operator<<(ostream& os, const Book& book) {
        os << book.title << "（" << book.author << "，" << book.year << "年）";
        if (book.available) {
            os << "（可借）";
        } else {
            os << "（已借出）";
        }
        return os;
    }

    friend bool operator==(const Book& a, const Book& b) {
        return a.title == b.title && a.author == b.author && a.year == b.year;
    }
};

class Library {
    vector<Book> books;

public:
    void addBook() {
        if (books.size() >= 100) {
            cout << "图书馆已满，无法添加新书" << endl;
        } else {
            string title, author;
            int year;
            cout << "请输入书名：";
            getline(cin, title);
            cout << "请输入作者名：";
            getline(cin, author);
            cout << "请输入出版年份：";
            cin >> year;
            cin.ignore();
            Book book(title, author, year);
            auto it = find(books.begin(), books.end(), book);
            if (it == books.end()) {
                books.push_back(book);
                cout << "添加成功" << endl;
            } else {
                cout << "该书已存在" << endl;
            }
        }
    }

    void searchBook() {
        cout << "请输入要查询的书名或作者名：";
        string keyword;
        getline(cin, keyword);
        vector<Book> results;
        for (const auto& book : books) {
            if (book.getTitle().find(keyword) != string::npos || book.getAuthor().find(keyword) != string::npos) {
                results.push_back(book);
            }
        }
        if (results.empty()) {
            cout << "未找到匹配的书籍" << endl;
        } else {
            cout << "共找到" << results.size() << "本书：" << endl;
            for (const auto& book : results) {
                cout << book << endl;
            }
        }
    }

    void borrowBook() {
        cout << "请输入要借阅的书名：";
        string title;
        getline(cin, title);
        auto it = find_if(books.begin(), books.end(), [&](const Book& book) { return book.getTitle() == title && book.isAvailable(); });
        if (it != books.end()) {
            it->setAvailable(false);
            cout << "借阅成功" << endl;
        } else {
            cout << "未找到该书或该书已借出" << endl;
        }
    }

    void returnBook() {
        cout << "请输入要归还的书名：";
        string title;
        getline(cin, title);
        auto it = find_if(books.begin(), books.end(), [&](const Book& book) { return book.getTitle() == title && !book.isAvailable(); });
        if (it != books.end()) {
            it->setAvailable(true);
            cout << "归还成功" << endl;
        } else {
            cout << "未找到该书或该书未借出" << endl;
        }
    }
};

int main() {
    Library library;
    string command;

    while (true) {
        cout << "请输入命令（add, search, borrow, return, quit）：";
        getline(cin, command);

        if (command == "add") {
            library.addBook();
        } else if (command == "search") {
            library.searchBook();
        } else if (command == "borrow") {
            library.borrowBook();
        } else if (command == "return") {
            library.returnBook();
        } else if (command == "quit") {
            break;
        } else {
            cout << "无效的命令，请重新输入" << endl;
        }
    }

    return 0;
}
