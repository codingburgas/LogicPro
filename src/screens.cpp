#include "screens.h"
#include "ui.h"
#include "logic.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

// ── Toast ─────────────────────────────────────────────────────────────────────
void Toast::show(const char* m, Color c) {
    strncpy(msg, m, 255); msg[255]=0;
    col = c; timer = 2.8f;
}

// ── Shared layout helpers ─────────────────────────────────────────────────────

// Content area starts after sidebar
static const float CX = (float)SIDEBAR;
static const float CW = (float)(WIN_W - SIDEBAR);

static void DrawSidebar(AppState& s, Font font,
                        const char** icons, const char** labels,
                        Screen* screens, int count, int activeIdx) {
    // sidebar bg
    DrawRectangle(0, 0, SIDEBAR, WIN_H, COL_SIDEBAR);
    DrawRectangle(SIDEBAR-1, 0, 1, WIN_H, COL_BORDER);

    // logo area
    DrawRectangle(0, 0, SIDEBAR, 72, COL_PANEL);
    DrawRectangle(0, 71, SIDEBAR, 1, COL_BORDER);
    DrawRectangle(0, 0, 3, 72, COL_ACCENT);
    DrawLabel("#", 14, 16, FONT_XL-8, COL_ACCENT, font);
    DrawLabel("LIBRARY", 44, 20, FONT_MD, COL_TEXT_PRI, font);
    DrawLabel("v2.0", 44, 42, FONT_XS, COL_TEXT_HINT, font);

    // nav items
    for (int i = 0; i < count; i++) {
        Rectangle r = { 8, (float)(88 + i*52), (float)(SIDEBAR-16), 44 };
        if (DrawSidebarItem(r, icons[i], labels[i], i == activeIdx, font))
            s.currentScreen = screens[i];
    }

    // bottom: user badge
    DrawRectangle(0, WIN_H-70, SIDEBAR, 1, COL_BORDER);
    Rectangle userBadge = { 8, (float)(WIN_H-58), (float)(SIDEBAR-16), 46 };
    DrawRectangleRounded(userBadge, 0.15f, 8, COL_CARD);

    // avatar circle
    DrawCircle(30, WIN_H-35, 14, COL_ACCENT_DIM);
    char initials[3] = { (char)toupper(s.currentUser[0]), 0, 0 };
    Vector2 is = MeasureTextEx(font, initials, (float)FONT_SM, 0.5f);
    DrawTextEx(font, initials, { 30.f-is.x/2.f, WIN_H-35.f-is.y/2.f },
               (float)FONT_SM, 0.5f, COL_ACCENT);

    DrawLabel(s.currentUser.c_str(), 50, WIN_H-46, FONT_SM, COL_TEXT_PRI, font);
    DrawLabel(s.isAdmin ? "Administrator" : "Member",
              50, WIN_H-28, FONT_XS, COL_TEXT_HINT, font);
}

static void DrawContentHeader(const char* title, const char* subtitle, Font font) {
    DrawRectangle((int)CX, 0, (int)CW, 72, COL_PANEL);
    DrawRectangle((int)CX, 71, (int)CW, 1, COL_BORDER);
    DrawLabel(title, (int)CX+24, 16, FONT_LG, COL_TEXT_PRI, font);
    if (subtitle && subtitle[0])
        DrawLabel(subtitle, (int)CX+24, 46, FONT_XS, COL_TEXT_HINT, font);
}

// ── Animated background particles ────────────────────────────────────────────
struct Particle { float x, y, speed, size, alpha; };
static Particle particles[40];
static bool particlesInit = false;

static void InitParticles() {
    if (particlesInit) return;
    for (int i = 0; i < 40; i++) {
        particles[i].x     = (float)(GetRandomValue(0, WIN_W));
        particles[i].y     = (float)(GetRandomValue(0, WIN_H));
        particles[i].speed = 0.1f + GetRandomValue(0,10)*0.03f;
        particles[i].size  = 1.f + GetRandomValue(0,3)*0.5f;
        particles[i].alpha = 20.f + GetRandomValue(0,40);
    }
    particlesInit = true;
}

static void DrawParticles(float dt) {
    for (int i = 0; i < 40; i++) {
        particles[i].y -= particles[i].speed;
        if (particles[i].y < -4) {
            particles[i].y = (float)WIN_H + 4;
            particles[i].x = (float)GetRandomValue(0, WIN_W);
        }
        DrawCircle((int)particles[i].x, (int)particles[i].y,
                   particles[i].size,
                   ColorAlpha2(COL_ACCENT, (unsigned char)particles[i].alpha));
    }
    (void)dt;
}

// ═══════════════════════════════════════════════════════════════════════════════
// MAIN MENU
// ═══════════════════════════════════════════════════════════════════════════════

void ScreenMainMenu(AppState& s, Font font, Toast& toast) {
    InitParticles();
    DrawRectangle(0, 0, WIN_W, WIN_H, COL_BG);

    // subtle grid lines
    for (int x = 0; x < WIN_W; x += 60)
        DrawRectangle(x, 0, 1, WIN_H, ColorAlpha2(COL_BORDER, 30));
    for (int y = 0; y < WIN_H; y += 60)
        DrawRectangle(0, y, WIN_W, 1, ColorAlpha2(COL_BORDER, 30));

    DrawParticles(GetFrameTime());

    // Left panel — branding
    float lw = WIN_W * 0.52f;
    DrawRectangle(0, 0, (int)lw, WIN_H, ColorAlpha2(COL_PANEL, 200));
    DrawRectangle((int)lw-1, 0, 1, WIN_H, COL_BORDER);

    // Big decorative "#"
    float t = s.globalTime;
    Color pulseCol = ColorAlpha2(COL_ACCENT,
        (unsigned char)(40 + 20*sinf(t*1.2f)));
    DrawTextEx(font, "#", { lw*0.08f, WIN_H*0.08f },
               (float)(FONT_XL*3), 0.5f, pulseCol);

    DrawTextEx(font, "LIBRARY",
               { lw*0.12f, WIN_H*0.42f },
               (float)FONT_XL, 0.5f, COL_TEXT_PRI);
    DrawTextEx(font, "Management System",
               { lw*0.12f, WIN_H*0.42f+58 },
               (float)FONT_MD, 0.5f, COL_TEXT_SEC);

    // feature bullets
    const char* features[] = {
        "* Manage your book catalog",
        "* Track borrowed books",
        "* Fine calculation system",
        "* Admin & member roles",
    };
    for (int i = 0; i < 4; i++) {
        float fy = WIN_H*0.62f + i*28;
        DrawTextEx(font, features[i], { lw*0.12f, fy },
                   (float)FONT_SM, 0.5f,
                   ColorAlpha2(COL_TEXT_HINT, 180));
    }

    // Right panel — login/register
    float rx = lw + (WIN_W-lw)/2.f;
    float ry = WIN_H/2.f;

    DrawCenteredLabel("Get Started", (int)(ry-100), FONT_LG, COL_TEXT_PRI, font);
    DrawCenteredLabel("Sign in to your account or create a new one",
                      (int)(ry-66), FONT_XS, COL_TEXT_HINT, font);

    float bw = 200, bh = 46;
    float bx = rx - bw/2;

    if (ButtonAccent({ bx, ry-28, bw, bh }, "Sign In", font))
        s.currentScreen = Screen::LOGIN;

    if (ButtonOutline({ bx, ry+34, bw, bh }, "Create Account", font, COL_ACCENT))
        s.currentScreen = Screen::REGISTER;

    DrawRectangle((int)bx, (int)(ry+96), (int)bw, 1, COL_BORDER);
    DrawCenteredLabel("Library Management System v2.0",
                      (int)(ry+106), FONT_XS, COL_TEXT_HINT, font);

    DrawToast(toast.msg, toast.col, font, toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// LOGIN
// ═══════════════════════════════════════════════════════════════════════════════

static InputField loginUser, loginPass;
static bool loginInit = false;

void ScreenLogin(AppState& s, Font font, Toast& toast) {
    if (!loginInit) {
        loginUser.clear(); loginPass.clear();
        loginPass.hidden = true; loginInit = true;
    }

    DrawRectangle(0, 0, WIN_W, WIN_H, COL_BG);
    for (int x = 0; x < WIN_W; x += 60)
        DrawRectangle(x, 0, 1, WIN_H, ColorAlpha2(COL_BORDER, 25));
    for (int y = 0; y < WIN_H; y += 60)
        DrawRectangle(0, y, WIN_W, 1, ColorAlpha2(COL_BORDER, 25));

    // card
    float pw = 420, ph = 380;
    float px = WIN_W/2.f - pw/2, py = WIN_H/2.f - ph/2;

    DrawGlowRect({px,py,pw,ph}, COL_ACCENT, 0.3f);
    DrawCard({px,py,pw,ph}, COL_PANEL, COL_BORDER);

    // header stripe
    DrawRectangleRounded({px,py,pw,72}, 0.08f, 8, COL_CARD2);
    DrawRectangle((int)px, (int)(py+60), (int)pw, 12, COL_PANEL);
    DrawRectangle((int)px, (int)py, 4, 72, COL_ACCENT);
    DrawLabel("Welcome back", (int)px+18, (int)py+12, FONT_MD, COL_TEXT_PRI, font);
    DrawLabel("Sign in to continue", (int)px+18, (int)py+36, FONT_XS, COL_TEXT_HINT, font);

    float fy = py + 92;
    DrawLabel("Username", (int)px+18, (int)fy, FONT_XS, COL_TEXT_SEC, font);
    Rectangle rfU = { px+18, fy+18, pw-36, 42 };

    DrawLabel("Password", (int)px+18, (int)(fy+78), FONT_XS, COL_TEXT_SEC, font);
    Rectangle rfP = { px+18, fy+96, pw-36, 42 };

    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        loginUser.active = CheckCollisionPointRec(mouse, rfU);
        loginPass.active = CheckCollisionPointRec(mouse, rfP);
        if (loginUser.active) loginPass.active = false;
    }

    InputUpdate(loginUser); InputDraw(loginUser, rfU, "Enter your username", font);
    InputUpdate(loginPass); InputDraw(loginPass, rfP, "Enter your password", font);

    if (ButtonAccent({ px+18, py+ph-62, pw-36, 44 }, "Sign In  ->", font)) {
        std::string u = InputText(loginUser), p = InputText(loginPass);
        if (u == "admin" && p == "12345") {
            s.loggedIn=true; s.isAdmin=true; s.currentUser="Administrator";
            s.currentScreen=Screen::ADMIN_MENU; loginInit=false;
        } else if (LoginCheck(s,u,p)) {
            s.loggedIn=true; s.isAdmin=false; s.currentUser=u;
            s.currentScreen=Screen::USER_MENU; loginInit=false;
        } else {
            toast.show("Wrong username or password", COL_DANGER);
        }
    }

    // back link
    if (ButtonOutline({px+18,py+ph+14,pw-36,34},"<- Back to main menu",font,COL_TEXT_HINT))
        s.currentScreen = Screen::MAIN_MENU;

    DrawToast(toast.msg, toast.col, font, toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// REGISTER
// ═══════════════════════════════════════════════════════════════════════════════

static InputField regUser, regPass, regPass2;
static bool regInit = false;

void ScreenRegister(AppState& s, Font font, Toast& toast) {
    if (!regInit) {
        regUser.clear(); regPass.clear(); regPass2.clear();
        regPass.hidden=true; regPass2.hidden=true; regInit=true;
    }

    DrawRectangle(0,0,WIN_W,WIN_H,COL_BG);
    for (int x=0;x<WIN_W;x+=60) DrawRectangle(x,0,1,WIN_H,ColorAlpha2(COL_BORDER,25));
    for (int y=0;y<WIN_H;y+=60) DrawRectangle(0,y,WIN_W,1,ColorAlpha2(COL_BORDER,25));

    float pw=420,ph=440;
    float px=WIN_W/2.f-pw/2, py=WIN_H/2.f-ph/2;

    DrawGlowRect({px,py,pw,ph},COL_ACCENT2,0.25f);
    DrawCard({px,py,pw,ph},COL_PANEL,COL_BORDER);

    DrawRectangleRounded({px,py,pw,72},0.08f,8,COL_CARD2);
    DrawRectangle((int)px,(int)(py+60),(int)pw,12,COL_PANEL);
    DrawRectangle((int)px,(int)py,4,72,COL_ACCENT2);
    DrawLabel("Create account",(int)px+18,(int)py+12,FONT_MD,COL_TEXT_PRI,font);
    DrawLabel("Join the library system",(int)px+18,(int)py+36,FONT_XS,COL_TEXT_HINT,font);

    float fy=py+92;
    struct { const char* lbl; const char* ph; InputField* f; } flds[] = {
        {"Username","Choose a username",&regUser},
        {"Password","Choose a password",&regPass},
        {"Confirm Password","Repeat your password",&regPass2},
    };

    Vector2 mouse=GetMousePosition();
    bool clicked=IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    for (int i=0;i<3;i++) {
        Rectangle rf={px+18,fy+18+i*96.f,pw-36,42};
        DrawLabel(flds[i].lbl,(int)px+18,(int)(fy+i*96.f),FONT_XS,COL_TEXT_SEC,font);
        if (clicked) flds[i].f->active=CheckCollisionPointRec(mouse,rf);
        InputUpdate(*flds[i].f);
        InputDraw(*flds[i].f,rf,flds[i].ph,font);
    }

    if (ButtonAccent({px+18,py+ph-62,pw-36,44},"Create Account  ->",font)) {
        std::string u=InputText(regUser),p=InputText(regPass),p2=InputText(regPass2);
        if (u.empty()||p.empty()) toast.show("All fields required",COL_WARNING);
        else if (p!=p2)           toast.show("Passwords do not match",COL_DANGER);
        else if (u.size()<3)      toast.show("Username must be 3+ characters",COL_WARNING);
        else if (UserExists(s,u)) toast.show("Username already taken",COL_DANGER);
        else {
            RegisterUser(s,u,p);
            toast.show("Account created! Please sign in.",COL_SUCCESS);
            regInit=false; s.currentScreen=Screen::LOGIN;
        }
    }

    if (ButtonOutline({px+18,py+ph+14,pw-36,34},"<- Back to main menu",font,COL_TEXT_HINT))
        s.currentScreen=Screen::MAIN_MENU;

    DrawToast(toast.msg,toast.col,font,toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// USER MENU (dashboard)
// ═══════════════════════════════════════════════════════════════════════════════

void ScreenUserMenu(AppState& s, Font font, Toast& toast) {
    DrawRectangle(0,0,WIN_W,WIN_H,COL_BG);

    const char* icons[]   = {"@","B","R","$","X"};
    const char* labels[]  = {"Dashboard","Browse Books","Return Book","My Bills","Logout"};
    Screen      scrs[]    = {Screen::USER_MENU,Screen::SHOW_BOOKS,Screen::RETURN_BOOK,
                              Screen::BILLS,Screen::USER_MENU};
    DrawSidebar(s,font,icons,labels,scrs,4,0);

    // logout from sidebar
    Rectangle logoutR={8,(float)(88+4*52),(float)(SIDEBAR-16),44};
    if (DrawSidebarItem(logoutR,"X","Logout",false,font)) {
        s.loggedIn=false; s.currentUser=""; s.currentScreen=Screen::MAIN_MENU;
    }

    // content area
    DrawRectangle((int)CX,0,(int)CW,WIN_H,COL_BG);
    DrawContentHeader("Dashboard","Overview of your library activity",font);

    // greeting
    char greet[128];
    snprintf(greet,sizeof(greet),"Good day, %s!",s.currentUser.c_str());
    DrawLabel(greet,(int)CX+24,90,FONT_MD,COL_TEXT_PRI,font);

    // Stats row
    int borrowed=0;
    for (const auto& b:s.archiveBooks) if(b.takenBy==s.currentUser) borrowed++;
    int bills=CountUserBills(s,s.currentUser);
    int fines=TotalUserFines(s,s.currentUser);
    int avail=(int)s.books.size();

    float sw=(CW-48)/4.f, sh=100;
    float sy=130.f;
    float sx=CX+12;

    DrawStatCard({sx,sy,sw-8,sh},"B",std::to_string(borrowed).c_str(),"Books Borrowed",COL_ACCENT,font);
    DrawStatCard({sx+sw,sy,sw-8,sh},"A",std::to_string(avail).c_str(),"Available",COL_SUCCESS,font);
    DrawStatCard({sx+sw*2,sy,sw-8,sh},"!",std::to_string(bills).c_str(),"Open Bills",
                 bills>0?COL_WARNING:COL_TEXT_HINT,font);
    DrawStatCard({sx+sw*3,sy,sw-8,sh},"$",std::to_string(fines).c_str(),"Total Fines (lv)",
                 fines>0?COL_DANGER:COL_TEXT_HINT,font);

    // Quick actions
    DrawLabel("Quick Actions",(int)CX+24,252,FONT_SM,COL_TEXT_SEC,font);
    DrawRectangle((int)CX+24,272,(int)CW-48,1,COL_BORDER);

    float aw=(CW-48)/2.f-8, ah=80;
    float ax=CX+24, ay=284;

    if (IconButton({ax,ay,aw,ah},"B","Browse Catalog",font,COL_CARD,COL_ACCENT))
        s.currentScreen=Screen::SHOW_BOOKS;
    if (IconButton({ax+aw+16,ay,aw,ah},"R","Return a Book",font,COL_CARD,COL_SUCCESS))
        s.currentScreen=Screen::RETURN_BOOK;
    if (IconButton({ax,ay+ah+12,aw,ah},"T","Take a Book",font,COL_CARD,COL_ACCENT2))
        s.currentScreen=Screen::TAKE_BOOK;
    if (IconButton({ax+aw+16,ay+ah+12,aw,ah},"$","View Bills",font,COL_CARD,
                   fines>0?COL_WARNING:COL_TEXT_SEC))
        s.currentScreen=Screen::BILLS;

    // Recently available books
    DrawLabel("Available Books",(int)CX+24,488,FONT_SM,COL_TEXT_SEC,font);
    DrawRectangle((int)CX+24,508,(int)CW-48,1,COL_BORDER);

    int shown=0;
    for (int i=0;i<(int)s.books.size()&&shown<3;i++,shown++) {
        Rectangle br={CX+24,(float)(520+shown*46),(float)(CW-48),40};
        DrawRectangleRounded(br,0.1f,8,COL_CARD);
        char bstr[200];
        snprintf(bstr,sizeof(bstr),"%s  —  %s  [%s]",
                 s.books[i].title.c_str(),s.books[i].author.c_str(),s.books[i].genre.c_str());
        DrawTextEx(font,bstr,{br.x+12,br.y+br.height/2.f-FONT_SM/2.f},
                   (float)FONT_SM,0.5f,COL_TEXT_SEC);
        // available badge
        DrawRectangleRounded({br.x+br.width-80,br.y+8,72,24},0.4f,8,
                             ColorAlpha2(COL_SUCCESS,40));
        DrawLabel("AVAILABLE",(int)(br.x+br.width-76),(int)(br.y+12),FONT_XS,COL_SUCCESS,font);
    }
    if (s.books.empty())
        DrawLabel("No books available at the moment.",(int)CX+24,528,FONT_SM,COL_TEXT_HINT,font);

    DrawToast(toast.msg,toast.col,font,toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// ADMIN MENU (dashboard)
// ═══════════════════════════════════════════════════════════════════════════════

void ScreenAdminMenu(AppState& s, Font font, Toast& toast) {
    DrawRectangle(0,0,WIN_W,WIN_H,COL_BG);

    const char* icons[]  = {"@","A","D","B","Z","$","X"};
    const char* labels[] = {"Dashboard","Add Book","Delete Book","Catalog",
                             "Archive","Fine System","Logout"};
    Screen scrs[]={Screen::ADMIN_MENU,Screen::ADD_BOOK,Screen::DELETE_BOOK,
                   Screen::SHOW_BOOKS,Screen::ARCHIVE,Screen::CALCULATE_FINE,
                   Screen::ADMIN_MENU};
    DrawSidebar(s,font,icons,labels,scrs,6,0);

    Rectangle logoutR={8,(float)(88+6*52),(float)(SIDEBAR-16),44};
    if (DrawSidebarItem(logoutR,"X","Logout",false,font)) {
        s.loggedIn=false; s.isAdmin=false; s.currentUser="";
        s.currentScreen=Screen::MAIN_MENU;
    }

    DrawRectangle((int)CX,0,(int)CW,WIN_H,COL_BG);
    DrawContentHeader("Admin Dashboard","Library management overview",font);

    // stats
    int totalBooks=(int)(s.books.size()+s.archiveBooks.size());
    int borrowed=0;
    for (const auto& b:s.archiveBooks) if(!b.takenBy.empty()) borrowed++;
    int members=(int)s.users.size();
    int openBills=0;
    for (const auto& b:s.bills) if(!b.paid) openBills++;

    float sw=(CW-48)/4.f, sh=100;
    float sx=CX+12, sy=130;

    DrawStatCard({sx,sy,sw-8,sh},"B",std::to_string((int)s.books.size()).c_str(),
                 "Available",COL_SUCCESS,font);
    DrawStatCard({sx+sw,sy,sw-8,sh},"Z",std::to_string(borrowed).c_str(),
                 "Borrowed",COL_WARNING,font);
    DrawStatCard({sx+sw*2,sy,sw-8,sh},"U",std::to_string(members).c_str(),
                 "Members",COL_ACCENT,font);
    DrawStatCard({sx+sw*3,sy,sw-8,sh},"$",std::to_string(openBills).c_str(),
                 "Open Bills",openBills>0?COL_DANGER:COL_TEXT_HINT,font);

    // Quick actions grid
    DrawLabel("Management",(int)CX+24,252,FONT_SM,COL_TEXT_SEC,font);
    DrawRectangle((int)CX+24,272,(int)CW-48,1,COL_BORDER);

    float aw=(CW-48)/3.f-8, ah=76;
    float ay=284;

    if (IconButton({CX+24,ay,aw,ah},"A","Add Book",font,COL_CARD,COL_ACCENT))
        s.currentScreen=Screen::ADD_BOOK;
    if (IconButton({CX+24+aw+12,ay,aw,ah},"D","Delete Book",font,COL_CARD,COL_DANGER))
        s.currentScreen=Screen::DELETE_BOOK;
    if (IconButton({CX+24+aw*2+24,ay,aw,ah},"B","Browse Catalog",font,COL_CARD,COL_SUCCESS))
        s.currentScreen=Screen::SHOW_BOOKS;
    if (IconButton({CX+24,ay+ah+12,aw,ah},"Z","Archive",font,COL_CARD,COL_TEXT_SEC))
        s.currentScreen=Screen::ARCHIVE;
    if (IconButton({CX+24+aw+12,ay+ah+12,aw,ah},"$","Fine System",font,COL_CARD,COL_WARNING))
        s.currentScreen=Screen::CALCULATE_FINE;
    if (IconButton({CX+24+aw*2+24,ay+ah+12,aw,ah},"S","Statistics",font,COL_CARD,COL_ACCENT2))
        s.currentScreen=Screen::STATS;

    // Recent archive entries
    DrawLabel("Recent Activity",(int)CX+24,490,FONT_SM,COL_TEXT_SEC,font);
    DrawRectangle((int)CX+24,510,(int)CW-48,1,COL_BORDER);

    int shown=0;
    for (int i=(int)s.archiveBooks.size()-1;i>=0&&shown<3;i--,shown++) {
        const auto& b=s.archiveBooks[i];
        Rectangle br={CX+24,(float)(522+shown*46),(float)(CW-48),40};
        DrawRectangleRounded(br,0.1f,8,COL_CARD);
        char bstr[200];
        if (!b.takenBy.empty())
            snprintf(bstr,sizeof(bstr),"%s  —  borrowed by %s",
                     b.title.c_str(),b.takenBy.c_str());
        else
            snprintf(bstr,sizeof(bstr),"%s  —  deleted",b.title.c_str());
        DrawTextEx(font,bstr,{br.x+12,br.y+br.height/2.f-FONT_SM/2.f},
                   (float)FONT_SM,0.5f,COL_TEXT_SEC);
        Color badgeCol=b.takenBy.empty()?COL_DANGER:COL_WARNING;
        const char* badgeTxt=b.takenBy.empty()?"DELETED":"BORROWED";
        float bw2=74;
        DrawRectangleRounded({br.x+br.width-bw2-4,br.y+8,bw2,24},0.4f,8,
                             ColorAlpha2(badgeCol,35));
        DrawLabel(badgeTxt,(int)(br.x+br.width-bw2),(int)(br.y+12),FONT_XS,badgeCol,font);
    }
    if (s.archiveBooks.empty())
        DrawLabel("No activity yet.",(int)CX+24,528,FONT_SM,COL_TEXT_HINT,font);

    DrawToast(toast.msg,toast.col,font,toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// SHOW BOOKS
// ═══════════════════════════════════════════════════════════════════════════════

static int bookScroll=0;
static InputField searchInput;
static bool showBooksInit=false;

void ScreenShowBooks(AppState& s, Font font, Toast& toast) {
    if (!showBooksInit) { searchInput.clear(); bookScroll=0; showBooksInit=true; }

    DrawRectangle(0,0,WIN_W,WIN_H,COL_BG);

    Screen back=s.isAdmin?Screen::ADMIN_MENU:Screen::USER_MENU;
    if (s.isAdmin) {
        const char* ic[] = {"<","A","D","B","Z","$"};
        const char* lb[] = {"Back","Add","Delete","Catalog","Archive","Fines"};
        Screen sc[]      = {back,Screen::ADD_BOOK,Screen::DELETE_BOOK,
                            Screen::SHOW_BOOKS,Screen::ARCHIVE,Screen::CALCULATE_FINE};
        DrawSidebar(s,font,ic,lb,sc,6,3);
    } else {
        const char* ic[] = {"<","B","T","R","$"};
        const char* lb[] = {"Back","Browse","Take","Return","Bills"};
        Screen sc[]      = {back,Screen::SHOW_BOOKS,Screen::TAKE_BOOK,
                            Screen::RETURN_BOOK,Screen::BILLS};
        DrawSidebar(s,font,ic,lb,sc,5,1);
    }

    DrawRectangle((int)CX,0,(int)CW,WIN_H,COL_BG);

    char subtitle[64];
    snprintf(subtitle,sizeof(subtitle),"%d books in catalog",(int)s.books.size());
    DrawContentHeader("Book Catalog",subtitle,font);

    // Search bar
    DrawRectangle((int)CX+24,84,(int)CW-48,1,COL_BORDER);
    Rectangle sr={CX+24,90,CW-48-110,38};
    Vector2 mouse=GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        searchInput.active=CheckCollisionPointRec(mouse,sr);
    InputUpdate(searchInput);
    InputDraw(searchInput,sr,"Search by title, author or genre...",font);

    // clear button
    if (!std::string(searchInput.buf).empty()) {
        if (ButtonOutline({CX+CW-130,90,100,38},"Clear",font,COL_TEXT_HINT))
            searchInput.clear();
    }

    // filter & build rows
    std::string q=InputText(searchInput);
    // lowercase q
    for (auto& c:q) c=(char)tolower(c);

    std::vector<std::string> rows;
    for (const auto& b:s.books) {
        if (!q.empty()) {
            std::string t=b.title,a=b.author,g=b.genre;
            for (auto& c:t) c=(char)tolower(c);
            for (auto& c:a) c=(char)tolower(c);
            for (auto& c:g) c=(char)tolower(c);
            if (t.find(q)==std::string::npos &&
                a.find(q)==std::string::npos &&
                g.find(q)==std::string::npos) continue;
        }
        char row[512];
        snprintf(row,sizeof(row),"%-30s  %-22s  %-14s  %s",
                 b.title.c_str(),b.author.c_str(),b.genre.c_str(),b.releaseDate.c_str());
        rows.push_back(row);
    }

    // column headers
    Rectangle hdr={CX+24,136,(float)(CW-48),28};
    DrawRectangleRounded(hdr,0.05f,4,COL_CARD2);
    DrawLabel("TITLE",(int)CX+68,142,FONT_XS,COL_TEXT_HINT,font);
    DrawLabel("AUTHOR",(int)CX+290,142,FONT_XS,COL_TEXT_HINT,font);
    DrawLabel("GENRE",(int)CX+490,142,FONT_XS,COL_TEXT_HINT,font);
    DrawLabel("YEAR",(int)CX+630,142,FONT_XS,COL_TEXT_HINT,font);

    if (rows.empty()) {
        DrawCenteredLabel(q.empty()?"No books in catalog":"No results found",
                          WIN_H/2,FONT_MD,COL_TEXT_HINT,font);
    } else {
        DrawScrollList({CX+24,170,(float)(CW-48),(float)(WIN_H-180)},
                       rows,bookScroll,font);
    }

    DrawToast(toast.msg,toast.col,font,toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// TAKE BOOK
// ═══════════════════════════════════════════════════════════════════════════════

static InputField takeInput;
static bool takeInit=false;

void ScreenTakeBook(AppState& s, Font font, Toast& toast) {
    if (!takeInit) { takeInput.clear(); takeInit=true; }

    DrawRectangle(0,0,WIN_W,WIN_H,COL_BG);

    // sidebar
    const char* icons[] ={"<","B","T","R","$"};
    const char* labels[]={"Back","Browse","Take Book","Return","Bills"};
    Screen scrs[]={Screen::USER_MENU,Screen::SHOW_BOOKS,Screen::TAKE_BOOK,
                   Screen::RETURN_BOOK,Screen::BILLS};
    DrawSidebar(s,font,icons,labels,scrs,5,2);

    DrawRectangle((int)CX,0,(int)CW,WIN_H,COL_BG);
    DrawContentHeader("Take a Book","Borrow a book from the catalog",font);

    float pw=480,ph=260;
    float px=CX+(CW-pw)/2, py=WIN_H/2.f-ph/2;

    DrawGlowRect({px,py,pw,ph},COL_ACCENT2,0.3f);
    DrawCard({px,py,pw,ph},COL_PANEL,COL_BORDER);

    DrawLabel("Book Title",(int)px+18,(int)py+28,FONT_XS,COL_TEXT_SEC,font);
    Rectangle rf={px+18,py+48,pw-36,44};
    Vector2 mouse=GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        takeInput.active=CheckCollisionPointRec(mouse,rf);
    InputUpdate(takeInput);
    InputDraw(takeInput,rf,"Enter the exact book title...",font);

    // show matching hint
    std::string q=InputText(takeInput);
    if (!q.empty()) {
        int matches=0;
        for (const auto& b:s.books) {
            std::string t=b.title;
            for (auto& c:t) c=(char)tolower(c);
            std::string ql=q;
            for (auto& c:ql) c=(char)tolower(c);
            if (t.find(ql)!=std::string::npos) {
                if (matches==0) {
                    char hint[200];
                    snprintf(hint,sizeof(hint),"  ->  %s by %s",
                             b.title.c_str(),b.author.c_str());
                    DrawLabel(hint,(int)px+18,(int)(py+104),FONT_XS,COL_TEXT_HINT,font);
                }
                matches++;
            }
        }
    }

    // info box
    DrawRectangleRounded({px+18,py+126,pw-36,54},0.1f,8,COL_CARD2);
    DrawRectangle((int)px+18,(int)py+126,3,54,COL_WARNING);
    DrawLabel("! Return period: 7 days",(int)px+30,(int)py+134,FONT_XS,COL_TEXT_SEC,font);
    DrawLabel("Overdue books incur a fine of 5 lv per day.",
              (int)px+30,(int)py+152,FONT_XS,COL_TEXT_HINT,font);

    if (ButtonAccent({px+18,py+ph-54,pw-36,44},"Borrow Book  ->",font)) {
        std::string title=InputText(takeInput);
        bool found=false;
        for (int i=0;i<(int)s.books.size();i++) {
            if (s.books[i].title==title) {
                s.books[i].takenBy=s.currentUser;
                s.archiveBooks.push_back(s.books[i]);
                s.books.erase(s.books.begin()+i);
                toast.show("Book borrowed! Return within 7 days.",COL_SUCCESS);
                takeInit=false; found=true; break;
            }
        }
        if (!found) toast.show("Book not found in catalog",COL_DANGER);
    }

    DrawToast(toast.msg,toast.col,font,toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// RETURN BOOK
// ═══════════════════════════════════════════════════════════════════════════════

static InputField returnInput;
static bool returnInit=false;

void ScreenReturnBook(AppState& s, Font font, Toast& toast) {
    if (!returnInit) { returnInput.clear(); returnInit=true; }

    DrawRectangle(0,0,WIN_W,WIN_H,COL_BG);

    const char* icons[] ={"<","B","T","R","$"};
    const char* labels[]={"Back","Browse","Take","Return Book","Bills"};
    Screen scrs[]={Screen::USER_MENU,Screen::SHOW_BOOKS,Screen::TAKE_BOOK,
                   Screen::RETURN_BOOK,Screen::BILLS};
    DrawSidebar(s,font,icons,labels,scrs,5,3);

    DrawRectangle((int)CX,0,(int)CW,WIN_H,COL_BG);
    DrawContentHeader("Return a Book","Return a borrowed book",font);

    // list user's borrowed books
    DrawLabel("Your borrowed books:",(int)CX+24,90,FONT_XS,COL_TEXT_SEC,font);
    int yborrow=0;
    bool hasBorrowed=false;
    for (const auto& b:s.archiveBooks) {
        if (b.takenBy==s.currentUser) {
            hasBorrowed=true;
            Rectangle br={CX+24,(float)(108+yborrow*42),(float)(CW-48),36};
            DrawRectangleRounded(br,0.1f,8,COL_CARD);
            DrawRoundedBorder(br,0.1f,8,COL_BORDER);
            char bstr[200];
            snprintf(bstr,sizeof(bstr),"%s  —  %s",b.title.c_str(),b.author.c_str());
            DrawTextEx(font,bstr,{br.x+12,br.y+br.height/2.f-FONT_XS/2.f},
                       (float)FONT_XS,0.5f,COL_TEXT_PRI);
            DrawRectangleRounded({br.x+br.width-78,br.y+6,70,24},0.4f,8,
                                 ColorAlpha2(COL_WARNING,35));
            DrawLabel("BORROWED",(int)(br.x+br.width-76),(int)(br.y+10),FONT_XS,COL_WARNING,font);
            yborrow++;
            if (yborrow>=4) break;
        }
    }
    if (!hasBorrowed)
        DrawLabel("You have no borrowed books.",(int)CX+24,112,FONT_SM,COL_TEXT_HINT,font);

    float pw=480,ph=200;
    float px=CX+(CW-pw)/2, py=WIN_H-280.f;

    DrawCard({px,py,pw,ph},COL_PANEL,COL_BORDER);
    DrawLabel("Enter title to return",(int)px+18,(int)py+20,FONT_XS,COL_TEXT_SEC,font);
    Rectangle rf={px+18,py+42,pw-36,44};
    Vector2 mouse=GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        returnInput.active=CheckCollisionPointRec(mouse,rf);
    InputUpdate(returnInput);
    InputDraw(returnInput,rf,"Book title...",font);

    DrawRectangleRounded({px+18,py+98,pw-36,40},0.1f,8,ColorAlpha2(COL_SUCCESS,18));
    DrawLabel("Returning a book removes it from your borrowed list.",
              (int)px+28,(int)py+112,FONT_XS,COL_TEXT_HINT,font);

    if (ButtonOutline({px+18,py+ph-54,pw-36,44},"Return Book",font,COL_SUCCESS)) {
        std::string title=InputText(returnInput);
        bool found=false;
        for (int i=0;i<(int)s.archiveBooks.size();i++) {
            if (s.archiveBooks[i].takenBy==s.currentUser &&
                s.archiveBooks[i].title==title) {
                Book ret=s.archiveBooks[i];
                ret.takenBy="";
                s.books.push_back(ret);
                s.archiveBooks.erase(s.archiveBooks.begin()+i);
                toast.show("Book returned successfully!",COL_SUCCESS);
                returnInit=false; found=true; break;
            }
        }
        if (!found) toast.show("Book not found in your borrowed list",COL_DANGER);
    }

    DrawToast(toast.msg,toast.col,font,toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// ADD BOOK
// ═══════════════════════════════════════════════════════════════════════════════

static InputField addTitle,addAuthor,addGenre,addDate;
static bool addInit=false;

void ScreenAddBook(AppState& s, Font font, Toast& toast) {
    if (!addInit) {
        addTitle.clear(); addAuthor.clear();
        addGenre.clear(); addDate.clear(); addInit=true;
    }

    DrawRectangle(0,0,WIN_W,WIN_H,COL_BG);

    const char* icons[] ={"<","A","D","B","Z","$"};
    const char* labels[]={"Back","Add Book","Delete","Catalog","Archive","Fines"};
    Screen scrs[]={Screen::ADMIN_MENU,Screen::ADD_BOOK,Screen::DELETE_BOOK,
                   Screen::SHOW_BOOKS,Screen::ARCHIVE,Screen::CALCULATE_FINE};
    DrawSidebar(s,font,icons,labels,scrs,6,1);

    DrawRectangle((int)CX,0,(int)CW,WIN_H,COL_BG);
    DrawContentHeader("Add New Book","Add a book to the catalog",font);

    float pw=500, ph=450;
    float px=CX+(CW-pw)/2, py=90;

    DrawGlowRect({px,py,pw,ph},COL_ACCENT,0.2f);
    DrawCard({px,py,pw,ph},COL_PANEL,COL_BORDER);

    struct FldDef { const char* lbl; const char* ph; InputField* f; };
    FldDef flds[]={
        {"Book Title *","e.g. The Great Gatsby",&addTitle},
        {"Author *","e.g. F. Scott Fitzgerald",&addAuthor},
        {"Genre","e.g. Classic Fiction",&addGenre},
        {"Release Year","e.g. 1925",&addDate},
    };

    Vector2 mouse=GetMousePosition();
    bool clicked=IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    for (int i=0;i<4;i++) {
        float fy=py+28+i*96.f;
        Rectangle rf={px+20,fy+18,pw-40,44};
        DrawLabel(flds[i].lbl,(int)px+20,(int)fy,FONT_XS,COL_TEXT_SEC,font);
        if (clicked) {
            flds[i].f->active=CheckCollisionPointRec(mouse,rf);
            for (int j=0;j<4;j++) if(j!=i) flds[j].f->active=false;
        }
        InputUpdate(*flds[i].f);
        InputDraw(*flds[i].f,rf,flds[i].ph,font);
    }

    // required note
    DrawLabel("* Required fields",(int)px+20,(int)(py+ph-70),FONT_XS,COL_TEXT_HINT,font);

    if (ButtonAccent({px+20,py+ph-46,pw-40,42},"Add to Catalog  ->",font)) {
        std::string t=InputText(addTitle),a=InputText(addAuthor);
        std::string g=InputText(addGenre),d=InputText(addDate);
        if (t.empty()||a.empty()) toast.show("Title and Author are required",COL_WARNING);
        else {
            s.books.emplace_back(t,a,g,d);
            char msg[128]; snprintf(msg,sizeof(msg),"'%s' added to catalog!",t.c_str());
            toast.show(msg,COL_SUCCESS);
            addInit=false;
        }
    }

    DrawToast(toast.msg,toast.col,font,toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// DELETE BOOK
// ═══════════════════════════════════════════════════════════════════════════════

static InputField delInput;
static bool delInit=false;
static int  delConfirmIdx=-1;

void ScreenDeleteBook(AppState& s, Font font, Toast& toast) {
    if (!delInit) { delInput.clear(); delInit=true; delConfirmIdx=-1; }

    DrawRectangle(0,0,WIN_W,WIN_H,COL_BG);

    const char* icons[] ={"<","A","D","B","Z","$"};
    const char* labels[]={"Back","Add Book","Delete","Catalog","Archive","Fines"};
    Screen scrs[]={Screen::ADMIN_MENU,Screen::ADD_BOOK,Screen::DELETE_BOOK,
                   Screen::SHOW_BOOKS,Screen::ARCHIVE,Screen::CALCULATE_FINE};
    DrawSidebar(s,font,icons,labels,scrs,6,2);

    DrawRectangle((int)CX,0,(int)CW,WIN_H,COL_BG);
    DrawContentHeader("Delete Book","Remove a book from the catalog",font);

    float pw=480,ph=220;
    float px=CX+(CW-pw)/2, py=140;

    DrawCard({px,py,pw,ph},COL_PANEL,COL_BORDER);
    DrawLabel("Book Title",(int)px+18,(int)py+24,FONT_XS,COL_TEXT_SEC,font);
    Rectangle rf={px+18,py+44,pw-36,44};
    Vector2 mouse=GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        delInput.active=CheckCollisionPointRec(mouse,rf);
    InputUpdate(delInput);
    InputDraw(delInput,rf,"Enter exact book title...",font);

    DrawRectangleRounded({px+18,py+104,pw-36,44},0.1f,8,ColorAlpha2(COL_DANGER,15));
    DrawRectangle((int)px+18,(int)py+104,3,44,COL_DANGER);
    DrawLabel("! Deleted books are moved to the archive.",
              (int)px+30,(int)py+112,FONT_XS,COL_TEXT_HINT,font);
    DrawLabel("This action cannot be undone.",
              (int)px+30,(int)py+128,FONT_XS,COL_TEXT_HINT,font);

    if (delConfirmIdx<0) {
        if (ButtonOutline({px+18,py+ph-50,pw-36,42},"Find Book",font,COL_ACCENT)) {
            std::string title=InputText(delInput);
            for (int i=0;i<(int)s.books.size();i++)
                if (s.books[i].title==title) { delConfirmIdx=i; break; }
            if (delConfirmIdx<0) toast.show("Book not found",COL_DANGER);
        }
    } else {
        // confirm modal
        float mw=440,mh=160;
        float mx=CX+(CW-mw)/2, my=py+ph+20;

        DrawGlowRect({mx,my,mw,mh},COL_DANGER,0.5f);
        DrawCard({mx,my,mw,mh},COL_CARD2,COL_DANGER);
        DrawRectangle((int)mx,(int)my,4,(int)mh,COL_DANGER);

        char conf[200];
        snprintf(conf,sizeof(conf),"Delete \"%s\" ?",s.books[delConfirmIdx].title.c_str());
        DrawLabel(conf,(int)mx+18,(int)my+20,FONT_SM,COL_TEXT_PRI,font);
        DrawLabel("This book will be archived.",(int)mx+18,(int)my+46,FONT_XS,COL_TEXT_SEC,font);

        float hw=(mw-48-12)/2.f;
        if (DangerButton({mx+18,my+mh-54,hw,44},"Delete",font)) {
            s.archiveBooks.push_back(s.books[delConfirmIdx]);
            s.books.erase(s.books.begin()+delConfirmIdx);
            toast.show("Book deleted and archived",COL_SUCCESS);
            delInit=false; delConfirmIdx=-1;
        }
        if (Button({mx+18+hw+12,my+mh-54,hw,44},"Cancel",font,COL_CARD,COL_TEXT_PRI))
            delConfirmIdx=-1;
    }

    DrawToast(toast.msg,toast.col,font,toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// ARCHIVE
// ═══════════════════════════════════════════════════════════════════════════════

static int archScroll=0;

void ScreenArchive(AppState& s, Font font, Toast& toast) {
    DrawRectangle(0,0,WIN_W,WIN_H,COL_BG);

    const char* icons[] ={"<","A","D","B","Z","$"};
    const char* labels[]={"Back","Add Book","Delete","Catalog","Archive","Fines"};
    Screen scrs[]={Screen::ADMIN_MENU,Screen::ADD_BOOK,Screen::DELETE_BOOK,
                   Screen::SHOW_BOOKS,Screen::ARCHIVE,Screen::CALCULATE_FINE};
    DrawSidebar(s,font,icons,labels,scrs,6,4);

    DrawRectangle((int)CX,0,(int)CW,WIN_H,COL_BG);

    char sub[64];
    snprintf(sub,sizeof(sub),"%d archived entries",(int)s.archiveBooks.size());
    DrawContentHeader("Archive",sub,font);

    // tab headers
    int borrowed=0,deleted=0;
    for (const auto& b:s.archiveBooks)
        if (b.takenBy.empty()) deleted++; else borrowed++;

    char bh[32],dh[32];
    snprintf(bh,sizeof(bh),"Borrowed (%d)",borrowed);
    snprintf(dh,sizeof(dh),"Deleted (%d)",deleted);

    // column header
    Rectangle hdr={CX+24,80,(float)(CW-48),30};
    DrawRectangleRounded(hdr,0.05f,4,COL_CARD2);
    DrawLabel("TITLE",(int)CX+68,88,FONT_XS,COL_TEXT_HINT,font);
    DrawLabel("AUTHOR",(int)CX+340,88,FONT_XS,COL_TEXT_HINT,font);
    DrawLabel("STATUS",(int)CX+560,88,FONT_XS,COL_TEXT_HINT,font);
    DrawLabel("BORROWER",(int)CX+680,88,FONT_XS,COL_TEXT_HINT,font);

    if (s.archiveBooks.empty()) {
        DrawCenteredLabel("Archive is empty",WIN_H/2,FONT_MD,COL_TEXT_HINT,font);
        DrawToast(toast.msg,toast.col,font,toast.timer);
        return;
    }

    std::vector<std::string> rows;
    for (const auto& b:s.archiveBooks) {
        char row[512];
        if (!b.takenBy.empty())
            snprintf(row,sizeof(row),"%-28s  %-20s  BORROWED   %s",
                     b.title.c_str(),b.author.c_str(),b.takenBy.c_str());
        else
            snprintf(row,sizeof(row),"%-28s  %-20s  DELETED    —",
                     b.title.c_str(),b.author.c_str());
        rows.push_back(row);
    }

    DrawScrollList({CX+24,118,(float)(CW-48),(float)(WIN_H-130)},rows,archScroll,font);
    DrawToast(toast.msg,toast.col,font,toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// CALCULATE FINE
// ═══════════════════════════════════════════════════════════════════════════════

static InputField fineUser,fineDays;
static bool fineInit=false;
static int  finePreview=-1;

void ScreenCalculateFine(AppState& s, Font font, Toast& toast) {
    if (!fineInit) { fineUser.clear(); fineDays.clear(); fineInit=true; finePreview=-1; }

    DrawRectangle(0,0,WIN_W,WIN_H,COL_BG);

    const char* icons[] ={"<","A","D","B","Z","$"};
    const char* labels[]={"Back","Add Book","Delete","Catalog","Archive","Fines"};
    Screen scrs[]={Screen::ADMIN_MENU,Screen::ADD_BOOK,Screen::DELETE_BOOK,
                   Screen::SHOW_BOOKS,Screen::ARCHIVE,Screen::CALCULATE_FINE};
    DrawSidebar(s,font,icons,labels,scrs,6,5);

    DrawRectangle((int)CX,0,(int)CW,WIN_H,COL_BG);
    DrawContentHeader("Fine System","Calculate and issue overdue fines",font);

    float pw=500,ph=330;
    float px=CX+(CW-pw)/2, py=100;

    DrawCard({px,py,pw,ph},COL_PANEL,COL_BORDER);

    DrawLabel("Username",(int)px+18,(int)py+24,FONT_XS,COL_TEXT_SEC,font);
    Rectangle rfU={px+18,py+44,pw-36,44};

    DrawLabel("Days Overdue",(int)px+18,(int)py+108,FONT_XS,COL_TEXT_SEC,font);
    Rectangle rfD={px+18,py+128,pw-36,44};

    Vector2 mouse=GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        fineUser.active=CheckCollisionPointRec(mouse,rfU);
        fineDays.active=CheckCollisionPointRec(mouse,rfD);
        if (fineUser.active) fineDays.active=false;
    }
    InputUpdate(fineUser); InputDraw(fineUser,rfU,"Username...",font);
    InputUpdate(fineDays); InputDraw(fineDays,rfD,"Number of days overdue...",font);

    // live preview
    std::string dstr=InputText(fineDays);
    int days=dstr.empty()?0:atoi(dstr.c_str());
    if (days>0) {
        int preview=CalculateFine(days);
        char pv[64]; snprintf(pv,sizeof(pv),"Fine preview:  %d lv",preview);
        DrawRectangleRounded({px+18,py+188,pw-36,44},0.1f,8,ColorAlpha2(COL_WARNING,20));
        DrawRectangle((int)px+18,(int)py+188,3,44,COL_WARNING);
        DrawLabel(pv,(int)px+30,(int)py+194,FONT_SM,COL_WARNING,font);
        DrawLabel("(5 lv x days)",(int)px+30,(int)py+210,FONT_XS,COL_TEXT_HINT,font);
    }

    if (ButtonAccent({px+18,py+ph-50,pw-36,44},"Issue Bill  ->",font)) {
        std::string uname=InputText(fineUser);
        bool found=false;
        for (const auto& b:s.archiveBooks) {
            if (b.takenBy==uname) {
                found=true;
                int amount=CalculateFine(days);
                s.bills.emplace_back(uname,b.title,amount);
                char msg[128];
                snprintf(msg,sizeof(msg),"Bill issued: %d lv to %s",amount,uname.c_str());
                toast.show(msg,COL_WARNING);
                fineInit=false; break;
            }
        }
        if (!found) toast.show("User has no borrowed books",COL_DANGER);
    }

    DrawToast(toast.msg,toast.col,font,toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// BILLS (user)
// ═══════════════════════════════════════════════════════════════════════════════

static int billScroll=0;

void ScreenBills(AppState& s, Font font, Toast& toast) {
    DrawRectangle(0,0,WIN_W,WIN_H,COL_BG);

    const char* icons[] ={"<","B","T","R","$"};
    const char* labels[]={"Back","Browse","Take","Return","My Bills"};
    Screen scrs[]={Screen::USER_MENU,Screen::SHOW_BOOKS,Screen::TAKE_BOOK,
                   Screen::RETURN_BOOK,Screen::BILLS};
    DrawSidebar(s,font,icons,labels,scrs,5,4);

    DrawRectangle((int)CX,0,(int)CW,WIN_H,COL_BG);
    DrawContentHeader("My Bills","Outstanding fines and charges",font);

    int total=0;
    std::vector<std::string> rows;
    for (const auto& b:s.bills) {
        if (b.username==s.currentUser&&!b.paid) {
            char row[256];
            snprintf(row,sizeof(row),"%-36s  %d lv",b.bookTitle.c_str(),b.amount);
            rows.push_back(row);
            total+=b.amount;
        }
    }

    if (rows.empty()) {
        DrawGlowRect({CX+(CW-300)/2,WIN_H/2-60,300,120},COL_SUCCESS,0.4f);
        DrawCard({CX+(CW-300)/2,WIN_H/2-60,300,120},COL_CARD,COL_BORDER);
        DrawCenteredLabel("OK",WIN_H/2-30,FONT_LG,COL_SUCCESS,font);
        DrawCenteredLabel("No outstanding bills!",WIN_H/2+10,FONT_SM,COL_TEXT_SEC,font);
    } else {
        // column header
        Rectangle hdr={CX+24,80,(float)(CW-48),30};
        DrawRectangleRounded(hdr,0.05f,4,COL_CARD2);
        DrawLabel("BOOK TITLE",(int)CX+68,88,FONT_XS,COL_TEXT_HINT,font);
        DrawLabel("AMOUNT",(int)(CX+CW-130),88,FONT_XS,COL_TEXT_HINT,font);

        DrawScrollList({CX+24,118,(float)(CW-48),(float)(WIN_H-200)},rows,billScroll,font);

        // total bar
        DrawRectangle((int)CX,WIN_H-80,(int)CW,80,COL_PANEL);
        DrawRectangle((int)CX,WIN_H-81,(int)CW,1,COL_BORDER);

        char totalStr[64];
        snprintf(totalStr,sizeof(totalStr),"Total outstanding: %d lv",total);
        DrawLabel(totalStr,(int)CX+24,WIN_H-56,FONT_MD,
                  total>0?COL_WARNING:COL_SUCCESS,font);
        DrawLabel("Contact the librarian to pay your fines.",
                  (int)CX+24,WIN_H-32,FONT_XS,COL_TEXT_HINT,font);
    }

    DrawToast(toast.msg,toast.col,font,toast.timer);
}

// ═══════════════════════════════════════════════════════════════════════════════
// STATS (admin)
// ═══════════════════════════════════════════════════════════════════════════════

void ScreenStats(AppState& s, Font font, Toast& toast) {
    DrawRectangle(0,0,WIN_W,WIN_H,COL_BG);

    const char* icons[] ={"<","A","D","B","Z","$","S"};
    const char* labels[]={"Back","Add Book","Delete","Catalog","Archive","Fines","Stats"};
    Screen scrs[]={Screen::ADMIN_MENU,Screen::ADD_BOOK,Screen::DELETE_BOOK,
                   Screen::SHOW_BOOKS,Screen::ARCHIVE,Screen::CALCULATE_FINE,Screen::STATS};
    DrawSidebar(s,font,icons,labels,scrs,7,6);

    DrawRectangle((int)CX,0,(int)CW,WIN_H,COL_BG);
    DrawContentHeader("Statistics","Library overview and metrics",font);

    // compute stats
    int totalBooks  =(int)(s.books.size()+s.archiveBooks.size());
    int available   =(int)s.books.size();
    int borrowed    =0;
    for (const auto& b:s.archiveBooks) if(!b.takenBy.empty()) borrowed++;
    int deleted     =(int)s.archiveBooks.size()-borrowed;
    int members     =(int)s.users.size();
    int totalBills  =(int)s.bills.size();
    int totalFines  =0;
    for (const auto& b:s.bills) totalFines+=b.amount;

    // Big stat grid
    float sw=(CW-60)/3.f, sh=110;
    float sx=CX+18, sy=100;

    DrawStatCard({sx,sy,sw-6,sh},"B",std::to_string(totalBooks).c_str(),"Total Books",COL_ACCENT,font);
    DrawStatCard({sx+sw,sy,sw-6,sh},"A",std::to_string(available).c_str(),"Available",COL_SUCCESS,font);
    DrawStatCard({sx+sw*2,sy,sw-6,sh},"Z",std::to_string(borrowed).c_str(),"Borrowed",COL_WARNING,font);

    DrawStatCard({sx,sy+sh+14,sw-6,sh},"U",std::to_string(members).c_str(),"Members",COL_ACCENT2,font);
    DrawStatCard({sx+sw,sy+sh+14,sw-6,sh},"D",std::to_string(deleted).c_str(),"Deleted",COL_DANGER,font);
    DrawStatCard({sx+sw*2,sy+sh+14,sw-6,sh},"$",std::to_string(totalFines).c_str(),"Total Fines (lv)",COL_GOLD,font);

    // Bar chart: genre breakdown
    DrawLabel("Genre Breakdown",(int)CX+24,(int)(sy+sh*2+42),FONT_SM,COL_TEXT_SEC,font);
    DrawRectangle((int)CX+24,(int)(sy+sh*2+64),(int)(CW-48),1,COL_BORDER);

    // count genres
    std::vector<std::pair<std::string,int>> genres;
    for (const auto& b:s.books) {
        std::string g=b.genre.empty()?"Unknown":b.genre;
        bool found=false;
        for (auto& gp:genres) if(gp.first==g){gp.second++;found=true;break;}
        if (!found) genres.push_back({g,1});
    }
    std::sort(genres.begin(),genres.end(),[](auto& a,auto& b){return a.second>b.second;});

    int maxG=genres.empty()?1:genres[0].second;
    float barY=sy+sh*2+76;
    float barW=CW-60;
    Color barCols[]={COL_ACCENT,COL_ACCENT2,COL_SUCCESS,COL_WARNING,COL_GOLD};
    for (int i=0;i<(int)genres.size()&&i<5;i++) {
        float bh2=28;
        float fill=barW*(genres[i].second/(float)maxG)*0.85f;
        Rectangle bar={CX+24,barY+(float)i*38,fill,bh2};
        DrawRectangleRounded(bar,0.2f,4,ColorAlpha2(barCols[i%5],160));
        DrawLabel(genres[i].first.c_str(),(int)CX+28,(int)(barY+i*38+6),FONT_XS,COL_TEXT_PRI,font);
        char cnt[8]; snprintf(cnt,sizeof(cnt),"%d",genres[i].second);
        DrawLabel(cnt,(int)(CX+24+fill+8),(int)(barY+i*38+6),FONT_XS,COL_TEXT_SEC,font);
    }
    if (genres.empty())
        DrawLabel("Add books to see genre stats.",(int)CX+24,(int)(barY+8),FONT_SM,COL_TEXT_HINT,font);

    DrawToast(toast.msg,toast.col,font,toast.timer);
}
