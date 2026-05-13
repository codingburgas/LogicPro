#pragma once
#include "types.h"
#include "ui.h"

struct Toast {
    char  msg[256];
    Color col;
    float timer;

    Toast() {
        for(int i=0;i<256;i++) msg[i]=0;
        col=C_SUCCESS(); timer=0.f;
    }
    void show(const char* m, Color c);
};

void ScreenMainMenu     (AppState& s, Font font, Toast& toast);
void ScreenLogin        (AppState& s, Font font, Toast& toast);
void ScreenRegister     (AppState& s, Font font, Toast& toast);
void ScreenUserMenu     (AppState& s, Font font, Toast& toast);
void ScreenAdminMenu    (AppState& s, Font font, Toast& toast);
void ScreenShowBooks    (AppState& s, Font font, Toast& toast);
void ScreenTakeBook     (AppState& s, Font font, Toast& toast);
void ScreenReturnBook   (AppState& s, Font font, Toast& toast);
void ScreenAddBook      (AppState& s, Font font, Toast& toast);
void ScreenDeleteBook   (AppState& s, Font font, Toast& toast);
void ScreenArchive      (AppState& s, Font font, Toast& toast);
void ScreenCalculateFine(AppState& s, Font font, Toast& toast);
void ScreenBills        (AppState& s, Font font, Toast& toast);
void ScreenStats        (AppState& s, Font font, Toast& toast);
