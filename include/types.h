#pragma once
#include <string>
#include <vector>

struct Book {
    std::string title;
    std::string author;
    std::string genre;
    std::string releaseDate;
    std::string takenBy;
    int         borrowedDays = 0;  // how many days since borrowed

    Book(std::string t, std::string a, std::string g, std::string r, std::string tb = "")
        : title(t), author(a), genre(g), releaseDate(r), takenBy(tb), borrowedDays(0) {}
};

struct User {
    std::string username;
    std::string password;
    int         booksBorrowed = 0;
    int         totalFinesPaid = 0;

    User(std::string u, std::string p) : username(u), password(p), booksBorrowed(0), totalFinesPaid(0) {}
};

struct Bill {
    std::string username;
    std::string bookTitle;
    int         amount;
    bool        paid = false;

    Bill(std::string u, std::string b, int a) : username(u), bookTitle(b), amount(a), paid(false) {}
};

enum class Screen {
    MAIN_MENU,
    LOGIN,
    REGISTER,
    USER_MENU,
    ADMIN_MENU,
    SHOW_BOOKS,
    TAKE_BOOK,
    RETURN_BOOK,
    ADD_BOOK,
    DELETE_BOOK,
    ARCHIVE,
    CALCULATE_FINE,
    BILLS,
    STATS,
};

struct AppState {
    bool        loggedIn    = false;
    bool        isAdmin     = false;
    std::string currentUser = "";
    Screen      currentScreen = Screen::MAIN_MENU;
    float       globalTime  = 0.f;   // for animations

    std::vector<User> users;
    std::vector<Book> books;
    std::vector<Book> archiveBooks;
    std::vector<Bill> bills;
};
