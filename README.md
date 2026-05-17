<div align="center">

<br/>

```
         _      ___ ____  ____      _    ____ __   __
        | |    |_ _| __ )|  _ \    / \  |  _ \\ \ / /
        | |     | ||  _ \| |_) |  / _ \ | |_) |\ V /
        | |___  | || |_) |  _ <  / ___ \|  _ <  | |
        |_____||___|____/|_| \_\/_/   \_\_| \_\ |_|
```

**Library Management System** - a minimalist desktop application for library management with a dark GUI powered by Raylib.

<br/>

![C++](https://img.shields.io/badge/C++17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Raylib](https://img.shields.io/badge/Raylib-5.0-black?style=for-the-badge)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

</div>

---

## What is this

A full-fledged library management system with a graphical user interface. Written in **C++17** using **Raylib 5.0**. 
No browsers, no Electron — a native binary that launches instantly. It features two user roles, a full lifecycle for book management, and a fine system for overdue returns.
---

## Quick Start

### Requirements

- [CMake 3.16+](https://cmake.org/download/)
- [Visual Studio 2022 Build Tools](https://visualstudio.microsoft.com/downloads/) *(Windows)*
  or `gcc`/`clang` *(Linux/macOS)*
- Internet connection for the first build *(CMake will download Raylib automatically)*

### Windows

```cmd
git clone https://github.com/YOUR_USERNAME/library-app.git
cd library-app

mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build . --config Release

Release\library.exe
```

### macOS

```bash
brew install raylib cmake
git clone https://github.com/YOUR_USERNAME/library-app.git
cd library-app && make
./library
```

### Linux (Ubuntu/Debian)

```bash
sudo apt update && sudo apt install libraylib-dev cmake build-essential
git clone https://github.com/YOUR_USERNAME/library-app.git
cd library-app && make
./library
```

> **Note:** On the first build on Windows, CMake will download Raylib (~5 MB) from GitHub. Subsequent builds work offline.

---

## Default Credentials

| Role | Login | Password |
|:---|:---|:---|
| Administrator | `admin` | `12345` |
| User | *registration* | *your choice* |

---

## Features

### Administrator Mode

```
Catalog Management
  ├── Add a book            (title, author, genre, date)
  ├── Delete a book         (with confirmation)
  └── View catalog          (with filters)

Archive
  └── History of all issued and deleted books

Fine System
  ├── Assign fine to a user
  ├── Calculation: 5 BGN × number of overdue days
  └── Automatic bill creation
```

### User Mode

```
View Books
  └── Filter by: author / genre / date / title

Borrow a Book
  └── Return period — 7 days

My Bills
  └── View accrued fines with the total amount
```

---

## Project Structure

```
library/
│
├── CMakeLists.txt          # Build (automatically downloads Raylib)
├── Makefile                # Alternative for Linux/macOS
│
├── include/
│   ├── types.h             # Data structures: Book, User, Bill, AppState
│   ├── ui.h                # UI primitives: buttons, input fields, toasts
│   ├── logic.h             # Business logic: login, search, fines
│   └── screens.h           # Declarations of all screens
│
└── src/
    ├── main.cpp            # Entry point, game loop, screen routing
    ├── ui.cpp              # Implementation of UI components
    ├── creens.cpp         # All application screens
    └── logic.cpp           # Implementation of business logic
```

---

## Design

**UI Components:**

- Input fields with a blinking cursor and password masking (*)
- Hover effects on all buttons
- Toast notifications with smooth fade-out
- Mouse wheel scrolling in lists
- Confirmation button before deleting a book

---

## Architecture

The application is built as a **Finite State Machine (FSM)**:

```
MAIN_MENU
  ├── LOGIN ──────────► USER_MENU ──► SHOW_BOOKS
  │                              ├──► TAKE_BOOK
  │                              └──► BILLS
  │
  └── LOGIN ──────────► ADMIN_MENU ──► ADD_BOOK
                                  ├──► DELETE_BOOK
                                  ├──► SHOW_BOOKS
                                  ├──► ARCHIVE
                                  └──► CALCULATE_FINE
```

Each screen is a separate function in `screens.cpp.` Switching screens is done by changing `AppState::currentScreen.` All state is stored in a single `AppState` struct, with no global variables.

---

## Fine Formula

fine formula is:

```cpp
int CalculateFine(int days) {
    if (days <= 0) return 0;
    return 5 + CalculateFine(days - 1);
}
// 1 day = 5 BGN, 5 days = 25 BGN, 10 days = 50 BGN
```

---

## Contributing

1. Fork the repository
2. Create a branch: `git checkout -b feature/my-feature`
3. Commit: `git commit -m 'Add my feature'`
4. Push: `git push origin feature/my-feature`
5. Open Pull Request

---

## License

MIT License — do whatever you want, mention the author.
---

<div align="center">

Made with ☕ and Raylib

</div>
