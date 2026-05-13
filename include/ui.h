#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

// ── Palette ───────────────────────────────────────────────────────────────────
inline Color C_BG()          { Color c; c.r=10;  c.g=10;  c.b=15;  c.a=255; return c; }
inline Color C_PANEL()       { Color c; c.r=18;  c.g=18;  c.b=26;  c.a=255; return c; }
inline Color C_CARD()        { Color c; c.r=24;  c.g=24;  c.b=36;  c.a=255; return c; }
inline Color C_CARD2()       { Color c; c.r=30;  c.g=30;  c.b=44;  c.a=255; return c; }
inline Color C_BORDER()      { Color c; c.r=40;  c.g=42;  c.b=58;  c.a=255; return c; }
inline Color C_BORDER2()     { Color c; c.r=60;  c.g=65;  c.b=90;  c.a=255; return c; }
inline Color C_ACCENT()      { Color c; c.r=99;  c.g=148; c.b=255; c.a=255; return c; }
inline Color C_ACCENT2()     { Color c; c.r=140; c.g=100; c.b=255; c.a=255; return c; }
inline Color C_ACCENT_DIM()  { Color c; c.r=40;  c.g=65;  c.b=130; c.a=255; return c; }
inline Color C_TEXT_PRI()    { Color c; c.r=235; c.g=235; c.b=245; c.a=255; return c; }
inline Color C_TEXT_SEC()    { Color c; c.r=120; c.g=125; c.b=150; c.a=255; return c; }
inline Color C_TEXT_HINT()   { Color c; c.r=60;  c.g=63;  c.b=85;  c.a=255; return c; }
inline Color C_DANGER()      { Color c; c.r=220; c.g=70;  c.b=70;  c.a=255; return c; }
inline Color C_SUCCESS()     { Color c; c.r=70;  c.g=200; c.b=110; c.a=255; return c; }
inline Color C_WARNING()     { Color c; c.r=220; c.g=170; c.b=50;  c.a=255; return c; }
inline Color C_GOLD()        { Color c; c.r=255; c.g=200; c.b=80;  c.a=255; return c; }
inline Color C_SIDEBAR()     { Color c; c.r=14;  c.g=14;  c.b=20;  c.a=255; return c; }

#define COL_BG         C_BG()
#define COL_PANEL      C_PANEL()
#define COL_CARD       C_CARD()
#define COL_CARD2      C_CARD2()
#define COL_BORDER     C_BORDER()
#define COL_BORDER2    C_BORDER2()
#define COL_ACCENT     C_ACCENT()
#define COL_ACCENT2    C_ACCENT2()
#define COL_ACCENT_DIM C_ACCENT_DIM()
#define COL_TEXT_PRI   C_TEXT_PRI()
#define COL_TEXT_SEC   C_TEXT_SEC()
#define COL_TEXT_HINT  C_TEXT_HINT()
#define COL_DANGER     C_DANGER()
#define COL_SUCCESS    C_SUCCESS()
#define COL_WARNING    C_WARNING()
#define COL_GOLD       C_GOLD()
#define COL_SIDEBAR    C_SIDEBAR()

// ── Window / font sizes ───────────────────────────────────────────────────────
static const int WIN_W   = 1100;
static const int WIN_H   = 700;
static const int SIDEBAR = 220;
static const int FONT_XS = 11;
static const int FONT_SM = 14;
static const int FONT_MD = 18;
static const int FONT_LG = 26;
static const int FONT_XL = 48;

// ── Color helpers ─────────────────────────────────────────────────────────────
inline Color ColorAlpha2(Color c, unsigned char a) {
    Color o; o.r=c.r; o.g=c.g; o.b=c.b; o.a=a; return o;
}
inline Color ColorLighter(Color c, int amt) {
    Color o;
    o.r=(unsigned char)(c.r+amt>255?255:c.r+amt);
    o.g=(unsigned char)(c.g+amt>255?255:c.g+amt);
    o.b=(unsigned char)(c.b+amt>255?255:c.b+amt);
    o.a=c.a; return o;
}
inline Color ColorMix(Color a, Color b, float t) {
    Color o;
    o.r=(unsigned char)(a.r+(b.r-a.r)*t);
    o.g=(unsigned char)(a.g+(b.g-a.g)*t);
    o.b=(unsigned char)(a.b+(b.b-a.b)*t);
    o.a=255; return o;
}
inline void DrawRoundedBorder(Rectangle r, float roundness, int segs, Color col) {
    DrawRectangleRoundedLines(r, roundness, segs, 1.5f, col);
}

// ── InputField ────────────────────────────────────────────────────────────────
struct InputField {
    char  buf[256];
    bool  active;
    float blinkTimer;
    bool  hidden;
    InputField() {
        for(int i=0;i<256;i++) buf[i]=0;
        active=false; blinkTimer=0.f; hidden=false;
    }
    void clear() { for(int i=0;i<256;i++) buf[i]=0; active=false; blinkTimer=0.f; }
};

void        InputUpdate(InputField& f);
void        InputDraw(InputField& f, Rectangle r, const char* placeholder, Font font);
std::string InputText(const InputField& f);

// ── Primitives ────────────────────────────────────────────────────────────────
bool Button(Rectangle r, const char* label, Font font, Color bg, Color fg);
bool ButtonAccent(Rectangle r, const char* label, Font font);
bool ButtonOutline(Rectangle r, const char* label, Font font, Color col);
bool DangerButton(Rectangle r, const char* label, Font font);
bool IconButton(Rectangle r, const char* icon, const char* label, Font font, Color bg, Color fg);

void DrawCard(Rectangle r, Color bg, Color border);
void DrawCardDefault(Rectangle r);
void DrawCardGradient(Rectangle r, Color top, Color bot);
void DrawGlowRect(Rectangle r, Color col, float intensity);

void DrawLabel(const char* text, int x, int y, int size, Color col, Font font);
void DrawCenteredLabel(const char* text, int y, int size, Color col, Font font);
void DrawRightLabel(const char* text, int right, int y, int size, Color col, Font font);

void DrawToast(const char* msg, Color col, Font font, float& timer);
void DrawScrollList(Rectangle bounds, const std::vector<std::string>& items,
                    int& scrollOffset, Font font);

// ── Stat card ─────────────────────────────────────────────────────────────────
void DrawStatCard(Rectangle r, const char* icon, const char* value,
                  const char* label, Color accent, Font font);

// ── Sidebar nav ───────────────────────────────────────────────────────────────
struct NavItem { const char* icon; const char* label; int screen; };
bool DrawSidebarItem(Rectangle r, const char* icon, const char* label,
                     bool active, Font font);
