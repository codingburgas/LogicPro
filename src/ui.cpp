#include "ui.h"
#include <cstring>
#include <cmath>
#include <vector>
#include <string>

// ── InputField ────────────────────────────────────────────────────────────────

void InputUpdate(InputField& f) {
    f.blinkTimer += GetFrameTime();
    if (!f.active) return;
    int key;
    while ((key = GetCharPressed()) > 0) {
        int len = (int)strlen(f.buf);
        if (key >= 32 && len < 254) {
            f.buf[len] = (char)key;
            f.buf[len+1] = '\0';
        }
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = (int)strlen(f.buf);
        if (len > 0) f.buf[len-1] = '\0';
    }
}

void InputDraw(InputField& f, Rectangle r, const char* placeholder, Font font) {
    // Animated border glow when active
    if (f.active) {
        Rectangle glow = { r.x-2, r.y-2, r.width+4, r.height+4 };
        DrawRectangleRounded(glow, 0.2f, 8, ColorAlpha2(COL_ACCENT, 40));
    }
    DrawRectangleRounded(r, 0.15f, 8, COL_CARD2);
    Color borderCol = f.active ? COL_ACCENT : COL_BORDER;
    DrawRoundedBorder(r, 0.15f, 8, borderCol);

    int len = (int)strlen(f.buf);
    std::string display;
    if (f.hidden) display = std::string(len, '*');
    else          display = std::string(f.buf, len);

    bool showPH = (len == 0);
    Color textCol = showPH ? COL_TEXT_HINT : COL_TEXT_PRI;
    const char* drawStr = showPH ? placeholder : display.c_str();
    DrawTextEx(font, drawStr,
        { r.x + 14.f, r.y + r.height/2.f - FONT_SM/2.f },
        (float)FONT_SM, 0.5f, textCol);

    if (f.active && fmod(f.blinkTimer, 1.0f) < 0.5f) {
        Vector2 m = MeasureTextEx(font, display.c_str(), (float)FONT_SM, 0.5f);
        float cx = r.x + 14.f + m.x + 2.f;
        float cy = r.y + r.height*0.22f;
        DrawRectangle((int)cx, (int)cy, 2, (int)(r.height*0.55f), COL_ACCENT);
    }
}

std::string InputText(const InputField& f) { return std::string(f.buf); }

// ── Button ────────────────────────────────────────────────────────────────────

bool Button(Rectangle r, const char* label, Font font, Color bg, Color fg) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, r);
    bool clicked = hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    Color fill = hovered ? ColorLighter(bg, 18) : bg;
    DrawRectangleRounded(r, 0.18f, 8, fill);
    if (hovered) DrawRoundedBorder(r, 0.18f, 8, ColorAlpha2(COL_ACCENT, 120));
    Vector2 ts = MeasureTextEx(font, label, (float)FONT_SM, 0.5f);
    DrawTextEx(font, label,
        { r.x + r.width/2.f - ts.x/2.f, r.y + r.height/2.f - ts.y/2.f },
        (float)FONT_SM, 0.5f, fg);
    return clicked;
}

bool ButtonAccent(Rectangle r, const char* label, Font font) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, r);
    bool clicked = hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    // Gradient-style button
    Color top = hovered ? ColorLighter(COL_ACCENT, 15) : COL_ACCENT;
    Color bot; bot.r=80; bot.g=100; bot.b=220; bot.a=255;
    if (hovered) bot = ColorLighter(bot, 15);

    // Draw as two halves for gradient effect
    Rectangle top_half = { r.x, r.y, r.width, r.height/2 };
    Rectangle bot_half = { r.x, r.y + r.height/2, r.width, r.height/2 };
    DrawRectangleRounded(r, 0.18f, 8, bot);
    DrawRectangleRounded(top_half, 0.18f, 8, top);

    if (hovered) {
        DrawRectangleRounded(r, 0.18f, 8, ColorAlpha2(COL_TEXT_PRI, 12));
    }

    Color fg; fg.r=255; fg.g=255; fg.b=255; fg.a=255;
    Vector2 ts = MeasureTextEx(font, label, (float)FONT_SM, 0.5f);
    DrawTextEx(font, label,
        { r.x + r.width/2.f - ts.x/2.f, r.y + r.height/2.f - ts.y/2.f },
        (float)FONT_SM, 0.5f, fg);
    return clicked;
}

bool ButtonOutline(Rectangle r, const char* label, Font font, Color col) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, r);
    bool clicked = hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    Color fill = hovered ? ColorAlpha2(col, 30) : ColorAlpha2(col, 10);
    DrawRectangleRounded(r, 0.18f, 8, fill);
    DrawRoundedBorder(r, 0.18f, 8, hovered ? col : ColorAlpha2(col, 150));
    Vector2 ts = MeasureTextEx(font, label, (float)FONT_SM, 0.5f);
    DrawTextEx(font, label,
        { r.x + r.width/2.f - ts.x/2.f, r.y + r.height/2.f - ts.y/2.f },
        (float)FONT_SM, 0.5f, hovered ? col : ColorAlpha2(col, 200));
    return clicked;
}

bool DangerButton(Rectangle r, const char* label, Font font) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, r);
    bool clicked = hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    Color fill; fill.r=(unsigned char)(hovered?200:160); fill.g=50; fill.b=55; fill.a=255;
    DrawRectangleRounded(r, 0.18f, 8, fill);
    if (hovered) DrawRoundedBorder(r, 0.18f, 8, COL_DANGER);
    Vector2 ts = MeasureTextEx(font, label, (float)FONT_SM, 0.5f);
    DrawTextEx(font, label,
        { r.x + r.width/2.f - ts.x/2.f, r.y + r.height/2.f - ts.y/2.f },
        (float)FONT_SM, 0.5f, COL_TEXT_PRI);
    return clicked;
}

bool IconButton(Rectangle r, const char* icon, const char* label, Font font, Color bg, Color fg) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, r);
    bool clicked = hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    Color fill = hovered ? ColorLighter(bg, 15) : bg;
    DrawRectangleRounded(r, 0.15f, 8, fill);
    if (hovered) DrawRoundedBorder(r, 0.15f, 8, ColorAlpha2(fg, 80));

    // icon on left
    Vector2 is = MeasureTextEx(font, icon, (float)FONT_MD, 0.5f);
    DrawTextEx(font, icon,
        { r.x + 18.f, r.y + r.height/2.f - is.y/2.f },
        (float)FONT_MD, 0.5f, hovered ? fg : ColorAlpha2(fg, 180));

    // label
    Vector2 ls = MeasureTextEx(font, label, (float)FONT_SM, 0.5f);
    DrawTextEx(font, label,
        { r.x + 18.f + is.x + 10.f, r.y + r.height/2.f - ls.y/2.f },
        (float)FONT_SM, 0.5f, fg);
    return clicked;
}

// ── Cards ─────────────────────────────────────────────────────────────────────

void DrawCard(Rectangle r, Color bg, Color border) {
    DrawRectangleRounded(r, 0.08f, 8, bg);
    DrawRoundedBorder(r, 0.08f, 8, border);
}

void DrawCardDefault(Rectangle r) { DrawCard(r, COL_CARD, COL_BORDER); }

void DrawCardGradient(Rectangle r, Color top, Color bot) {
    DrawRectangleRounded(r, 0.08f, 8, ColorMix(top, bot, 0.5f));
    DrawRoundedBorder(r, 0.08f, 8, ColorAlpha2(COL_BORDER, 100));
}

void DrawGlowRect(Rectangle r, Color col, float intensity) {
    for (int i = 3; i >= 1; i--) {
        Rectangle gr = { r.x-(float)i*2, r.y-(float)i*2,
                         r.width+(float)i*4, r.height+(float)i*4 };
        DrawRectangleRounded(gr, 0.1f, 8,
            ColorAlpha2(col, (unsigned char)(intensity * 18 * (4-i))));
    }
}

// ── Labels ────────────────────────────────────────────────────────────────────

void DrawLabel(const char* text, int x, int y, int size, Color col, Font font) {
    DrawTextEx(font, text, { (float)x, (float)y }, (float)size, 0.5f, col);
}

void DrawCenteredLabel(const char* text, int y, int size, Color col, Font font) {
    Vector2 ts = MeasureTextEx(font, text, (float)size, 0.5f);
    DrawTextEx(font, text, { WIN_W/2.f - ts.x/2.f, (float)y }, (float)size, 0.5f, col);
}

void DrawRightLabel(const char* text, int right, int y, int size, Color col, Font font) {
    Vector2 ts = MeasureTextEx(font, text, (float)size, 0.5f);
    DrawTextEx(font, text, { (float)right - ts.x, (float)y }, (float)size, 0.5f, col);
}

// ── Toast ─────────────────────────────────────────────────────────────────────

void DrawToast(const char* msg, Color col, Font font, float& timer) {
    if (timer <= 0.f) return;
    timer -= GetFrameTime();
    if (timer < 0.f) timer = 0.f;
    float alpha = (timer > 0.5f) ? 1.f : timer/0.5f;

    Color bgCol = ColorAlpha2(COL_CARD2, (unsigned char)(240*alpha));
    Color txCol = ColorAlpha2(col,       (unsigned char)(255*alpha));
    Color brCol = ColorAlpha2(col,       (unsigned char)(200*alpha));

    Vector2 ts = MeasureTextEx(font, msg, (float)FONT_SM, 0.5f);
    float tw = ts.x + 44.f, th = 46.f;
    // Slide up from bottom
    float yOff = (timer > 0.3f) ? 0.f : (0.3f - timer)/0.3f * 20.f;
    Rectangle r = { WIN_W/2.f - tw/2.f, WIN_H - 76.f + yOff, tw, th };

    DrawGlowRect(r, col, alpha * 0.5f);
    DrawRectangleRounded(r, 0.3f, 8, bgCol);
    DrawRoundedBorder(r, 0.3f, 8, brCol);

    // colored left accent bar
    DrawRectangle((int)r.x+2, (int)r.y+6, 3, (int)th-12, txCol);

    DrawTextEx(font, msg, { r.x+20.f, r.y+th/2.f-ts.y/2.f },
               (float)FONT_SM, 0.5f, txCol);
}

// ─jndfjkdhj


void DrawScrollList(Rectangle bounds, const std::vector<std::string>& items,
                    int& scrollOffset, Font font) {
    const int itemH = 54;
    int visible = (int)(bounds.height / itemH);
    float wheel = GetMouseWheelMove();
    if (CheckCollisionPointRec(GetMousePosition(), bounds))
        scrollOffset -= (int)wheel * 2;
    int maxOff = (int)items.size() - visible;
    if (maxOff < 0) maxOff = 0;
    if (scrollOffset < 0)      scrollOffset = 0;
    if (scrollOffset > maxOff) scrollOffset = maxOff;

    BeginScissorMode((int)bounds.x,(int)bounds.y,(int)bounds.width,(int)bounds.height);

    for (int i = scrollOffset; i < (int)items.size() && i < scrollOffset+visible; ++i) {
        float ry = bounds.y + (float)((i - scrollOffset) * itemH);
        Rectangle row = { bounds.x, ry, bounds.width, (float)(itemH-3) };

        bool hovered = CheckCollisionPointRec(GetMousePosition(), row);
        Color rowBg = hovered ? COL_CARD2 : ((i%2==0) ? COL_CARD : COL_PANEL);
        DrawRectangleRounded(row, 0.06f, 4, rowBg);
        if (hovered) DrawRoundedBorder(row, 0.06f, 4, COL_BORDER2);

        // row number badge
        char num[8]; snprintf(num, sizeof(num), "%d", i+1);
        Color numBg = COL_ACCENT_DIM;
        DrawRectangle((int)row.x+8, (int)row.y+14, 26, 22, numBg);
        Vector2 ns = MeasureTextEx(font, num, (float)FONT_XS, 0.5f);
        DrawTextEx(font, num, { row.x+8.f+13.f-ns.x/2.f, row.y+14.f+11.f-ns.y/2.f },
                   (float)FONT_XS, 0.5f, COL_ACCENT);

        DrawTextEx(font, items[i].c_str(),
            { row.x+44.f, row.y+row.height/2.f-FONT_SM/2.f },
            (float)FONT_SM, 0.5f, hovered ? COL_TEXT_PRI : COL_TEXT_SEC);
    }
    EndScissorMode();

    if ((int)items.size() > visible) {
        float sbH = bounds.height * visible / (float)items.size();
        float sbY = bounds.y + bounds.height * scrollOffset / (float)items.size();
        DrawRectangle((int)(bounds.x+bounds.width-5),(int)sbY, 3,(int)sbH,
                      ColorAlpha2(COL_ACCENT, 120));
    }
}

// ── Stat card ─────────────────────────────────────────────────────────────────

void DrawStatCard(Rectangle r, const char* icon, const char* value,
                  const char* label, Color accent, Font font) {
    // glow + card
    DrawGlowRect(r, accent, 0.4f);
    DrawRectangleRounded(r, 0.12f, 8, COL_CARD);
    DrawRoundedBorder(r, 0.12f, 8, COL_BORDER);

    // left accent stripe
    Rectangle stripe = { r.x, r.y+8, 3, r.height-16 };
    DrawRectangleRec(stripe, accent);

    // icon
    Vector2 is = MeasureTextEx(font, icon, (float)FONT_LG, 0.5f);
    DrawTextEx(font, icon, { r.x+16.f, r.y+r.height/2.f-is.y/2.f-8.f },
               (float)FONT_LG, 0.5f, ColorAlpha2(accent, 200));

    // value
    Vector2 vs = MeasureTextEx(font, value, (float)FONT_LG, 0.5f);
    DrawTextEx(font, value, { r.x+r.width/2.f-vs.x/2.f+10.f, r.y+12.f },
               (float)FONT_LG, 0.5f, COL_TEXT_PRI);

    // label
    Vector2 ls = MeasureTextEx(font, label, (float)FONT_XS, 0.5f);
    DrawTextEx(font, label, { r.x+r.width/2.f-ls.x/2.f+10.f, r.y+r.height-26.f },
               (float)FONT_XS, 0.5f, COL_TEXT_SEC);
}

// ── Sidebar item ─────────────────────────────────────────────────────────────

bool DrawSidebarItem(Rectangle r, const char* icon, const char* label,
                     bool active, Font font) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, r);
    bool clicked = hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    if (active) {
        DrawRectangleRounded(r, 0.15f, 8, ColorAlpha2(COL_ACCENT, 22));
        DrawRectangle((int)r.x, (int)r.y+4, 3, (int)r.height-8, COL_ACCENT);
    } else if (hovered) {
        DrawRectangleRounded(r, 0.15f, 8, ColorAlpha2(COL_TEXT_HINT, 30));
    }

    Color iconCol = active ? COL_ACCENT : (hovered ? COL_TEXT_PRI : COL_TEXT_SEC);
    Color textCol = active ? COL_TEXT_PRI : (hovered ? COL_TEXT_PRI : COL_TEXT_SEC);

    DrawTextEx(font, icon, { r.x+16.f, r.y+r.height/2.f-FONT_MD/2.f },
               (float)FONT_MD, 0.5f, iconCol);
    DrawTextEx(font, label, { r.x+46.f, r.y+r.height/2.f-FONT_SM/2.f },
               (float)FONT_SM, 0.5f, textCol);
    return clicked;
}
