#include "logic.h"

bool UserExists(const AppState& s, const std::string& username) {
    for (const auto& u : s.users)
        if (u.username == username) return true;
    return false;
}

bool LoginCheck(const AppState& s, const std::string& username, const std::string& password) {
    for (const auto& u : s.users)
        if (u.username == username && u.password == password) return true;
    return false;
}

void RegisterUser(AppState& s, const std::string& u, const std::string& p) {
    s.users.emplace_back(u, p);
}

int CalculateFine(int days) {
    if (days <= 0) return 0;
    return 5 + CalculateFine(days - 1);
}

int CountUserBills(const AppState& s, const std::string& username) {
    int count = 0;
    for (const auto& b : s.bills)
        if (b.username == username && !b.paid) count++;
    return count;
}

int TotalUserFines(const AppState& s, const std::string& username) {
    int total = 0;
    for (const auto& b : s.bills)
        if (b.username == username && !b.paid) total += b.amount;
    return total;
}
