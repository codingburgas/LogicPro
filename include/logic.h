#pragma once
#include "types.h"
#include <string>

bool UserExists   (const AppState& s, const std::string& username);
bool LoginCheck   (const AppState& s, const std::string& u, const std::string& p);
void RegisterUser (AppState& s, const std::string& u, const std::string& p);
int  CalculateFine(int days);
int  CountUserBills(const AppState& s, const std::string& username);
int  TotalUserFines(const AppState& s, const std::string& username);
