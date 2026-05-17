#include "raylib.h"
#include "ui.h"
#include "types.h"
#include "screens.h"

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(WIN_W, WIN_H, "Library — Management System");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    Font font = GetFontDefault();

    AppState state;
    Toast    toast;
    toast.col = C_SUCCESS();

    // Demo data
    state.books.emplace_back("The Pragmatic Programmer", "David Thomas",      "Technology", "1999");
    state.books.emplace_back("Clean Code",               "Robert C. Martin",  "Technology", "2008");
    state.books.emplace_back("Dune",                     "Frank Herbert",     "Sci-Fi",     "1965");
    state.books.emplace_back("1984",                     "George Orwell",     "Fiction",    "1949");
    state.books.emplace_back("The Great Gatsby",         "F. Scott Fitzgerald","Fiction",   "1925");
    state.books.emplace_back("Sapiens",                  "Yuval Noah Harari", "History",    "2011");
    state.books.emplace_back("Atomic Habits",            "James Clear",       "Self-Help",  "2018");
    state.books.emplace_back("The Hobbit",               "J.R.R. Tolkien",    "Fantasy",    "1937");

    while (!WindowShouldClose()) {
        state.globalTime += GetFrameTime();

        BeginDrawing();
        ClearBackground(C_BG());

        switch (state.currentScreen) {
            case Screen::MAIN_MENU:       ScreenMainMenu(state,font,toast);       break;
            case Screen::LOGIN:           ScreenLogin(state,font,toast);          break;
            case Screen::REGISTER:        ScreenRegister(state,font,toast);       break;
            case Screen::USER_MENU:       ScreenUserMenu(state,font,toast);       break;
            case Screen::ADMIN_MENU:      ScreenAdminMenu(state,font,toast);      break;
            case Screen::SHOW_BOOKS:      ScreenShowBooks(state,font,toast);      break;
            case Screen::TAKE_BOOK:       ScreenTakeBook(state,font,toast);       break;
            case Screen::RETURN_BOOK:     ScreenReturnBook(state,font,toast);     break;
            case Screen::ADD_BOOK:        ScreenAddBook(state,font,toast);        break;
            case Screen::DELETE_BOOK:     ScreenDeleteBook(state,font,toast);     break;
            case Screen::ARCHIVE:         ScreenArchive(state,font,toast);        break;
            case Screen::CALCULATE_FINE:  ScreenCalculateFine(state,font,toast);  break;
            case Screen::BILLS:           ScreenBills(state,font,toast);          break;
            case Screen::STATS:           ScreenStats(state,font,toast);          break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
