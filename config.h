/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx       = 1;        /* border pixel of windows */
static const unsigned int snap           = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;     /* 0 means no systray */
static const int showbar                 = 1;        /* 0 means no bar */
static const int topbar                  = 1;        /* 0 means bottom bar */
static const char *fonts[]               = { "Terminesspowerline:antialias=true:size=8","Ionicons:antialias=true:size=8", "monospace:size=10" };
static const char dmenufont[]            = "Hack:size=8.5:antialias=true:autohint=false";
static const char col_gray1[]            = "#121212";
static const char col_gray2[]            = "#BDBDDB";
static const char col_gray3[]            = "#848484";
static const char col_gray4[]            = "#030303";
static const char col_cyan[]             = "#121212";
static const char *colors[SchemeLast][4] = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray1, col_gray1 },
	[SchemeSel]  = { col_gray2, col_gray1, col_gray2, col_gray2 },
};
static const unsigned int gappx = 5;


/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",             NULL,                                   NULL,                            0,      0, -1 },
	{ "Firefox",          NULL,                                   NULL,                            0,      0, -1 },
	{ "chromium",         "chromium",                             NULL,                            1 << 1, 0, -1 },
	{ "chromium",         "crx_oaomajcafbnckkfcmbagbbpgkpcpcmad", NULL,                            1 << 7, 0, -1 },
	{ "Telegram",         NULL,                                   NULL,                            1 << 9, 0, -1 },
	{ "telegram-desktop", NULL,                                   NULL,                            1 << 9, 0, -1 },
	{ "Pidgin",           NULL,                                   NULL,                            1 << 9, 0, -1 },
	{ "Gajim",            NULL,                                   NULL,                            1 << 9, 0, -1 },
	{ "Google-chrome",    "google-chrome",                        NULL,                            1 << 1, 0, -1 },
	{ "Google-chrome",    "crx_nloahgegolkkjdbbdgiknfglcmcimakb", NULL,                            1 << 7, 0, -1 },
    { "Virt-manager",     "virt-manager",                         "win7_boos_client auf QEMU/KVM", 1 << 3, 0, -1},
    { "Virt-manager",     "virt-manager",                         "boos_win7 auf QEMU/KVM",        1 << 4, 0, -1},
    { "Vivaldi-stable",   NULL,                                   NULL,                            1 << 1, 0, -1},
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "gaplessgrid.c"
#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
    { "(@)",      spiral }, 
	{ "[]",      gaplessgrid },
	{ "[]",      monocle },
	{ "[]",      tile },    /* first entry is default */
	{ "[]",      NULL },    /* no layout function means floating behavior */
    { "[\\]",     dwindle }, 
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2]          = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]    = { "/home/neo/.dotfiles/shortcuts/dmenu.sh", NULL };
static const char *projcmd[]     = { "/home/neo/.dotfiles/shortcuts/projmenu.sh", NULL };
static const char *termcmd[]     = { "urxvt", NULL };
static const char *filecmd[]     = { "nautilus", "--new-window", NULL };

static const char *volbarcmd[]   = { "/home/neo/Projekte/Python/dwmbarpy/src/lbpyctl.py", "vol", NULL };
static const char *wlanbarcmd[]  = { "/home/neo/Projekte/Python/dwmbarpy/src/lbpyctl.py", "wlan", NULL };
static const char *ethbarcmd[]   = { "/home/neo/Projekte/Python/dwmbarpy/src/lbpyctl.py", "eth", NULL };
static const char *batbarcmd[]   = { "/home/neo/Projekte/Python/dwmbarpy/src/lbpyctl.py", "bat", NULL };
static const char *datebarcmd[]  = { "/home/neo/Projekte/Python/dwmbarpy/src/lbpyctl.py", "date", NULL };

/* Shortcuts */
static const char *touchpad[]    = { "/home/neo/.dotfiles/shortcuts/touchpad_control.sh", NULL };
static const char *brigup[]      = { "/home/neo/.dotfiles/shortcuts/brig_up.sh", NULL };
static const char *brigdown[]    = { "/home/neo/.dotfiles/shortcuts/brig_down.sh", NULL };
static const char *volup[]       = { "/home/neo/.dotfiles/shortcuts/vol_high.sh", NULL };
static const char *voldown[]     = { "/home/neo/.dotfiles/shortcuts/vol_low.sh", NULL };
static const char *volmute[]     = { "/home/neo/.dotfiles/shortcuts/vol_toggle.sh", NULL };
static const char *suspendlock[] = { "/home/neo/.dotfiles/shortcuts/lock.sh", NULL };
static const char *lock[]        = { "/home/neo/.dotfiles/shortcuts/i3-lock.sh", NULL };
static const char *powermenu[]   = { "/home/neo/.dotfiles/shortcuts/powermenu.sh", NULL };
static const char *audiomenu[]   = { "/home/neo/.dotfiles/shortcuts/audiomenu.sh", NULL };
static const char *lightmenu[]   = { "/home/neo/.dotfiles/shortcuts/lightmenu.sh", NULL };
static const char *screenshot[]  = { "xfce4-screenshooter", "-s", "/home/neo/Bilder/Screenshots", "-r", NULL };

//#include "toggleview_focus.c"
#include "moveresize.c"
#include "tagall.c"
#include "maximize.c"
#include "zoomswap.c"
#include "push.c"
#include "focusmaster.c"
#include "shifttags.c"
#include "keepfloatingposition.c"

// Brig Up 0x1008ff02
// Brig Down 0x1008ff03

// Mute  0x1008ff12
// Lower 0x1008ff11
// Raise 0x1008ff13

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ 0,                                  0xff61,     spawn,          {.v = screenshot } },
	{ 0,                                  0x1008ff12, spawn,          {.v = volmute } },
	{ 0,                                  0x1008ff11, spawn,          {.v = voldown } },
	{ 0,                                  0x1008ff13, spawn,          {.v = volup } },
	{ 0,                                  0x1008ff03, spawn,          {.v = brigdown } },
	{ 0,                                  0x1008ff02, spawn,          {.v = brigup } },
	{ 0,                                  0x1008ff2a, spawn,          {.v = powermenu } },
	{ 0|Mod1Mask|ControlMask,             XK_k,       spawn,          {.v = suspendlock } },
	{ 0|Mod1Mask|ControlMask,             XK_l,       spawn,          {.v = lock } },
	{ MODKEY|ShiftMask,                   XK_a,       spawn,          {.v = audiomenu } },
	{ MODKEY|ShiftMask,                   XK_d,       spawn,          {.v = lightmenu } },
	{ MODKEY,                             XK_e,       spawn,          {.v = filecmd } },
	{ MODKEY|ShiftMask,                   XK_d,       spawn,          {.v = projcmd } },
	{ MODKEY,                             XK_d,       spawn,          {.v = dmenucmd } },
	{ MODKEY,                             XK_Return,  spawn,          {.v = termcmd } },
	{ MODKEY,                             XK_b,       togglebar,      {0} },
	{ MODKEY,                             XK_j,       focusstack,     {.i = +1 } },
	{ MODKEY,                             XK_k,       focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,                   XK_j,       pushdown,       {.f = 1} },
	{ MODKEY|ShiftMask,                   XK_k,       pushup,         {.f = 1} },
	{ MODKEY,                             XK_i,       incnmaster,     {.i = +1 } },
	{ MODKEY,                             XK_o,       incnmaster,     {.i = -1 } },
	{ MODKEY,                             XK_h,       setmfact,       {.f = -0.05} },
	{ MODKEY,                             XK_l,       setmfact,       {.f = +0.05} },
	{ MODKEY,                             XK_n,       zoom,           {0} },
	{ Mod1Mask,                           XK_Tab,     swapfocus,      {0} },
	{ MODKEY,                             XK_Tab,     view,           {0} },
	{ MODKEY|ShiftMask,                   XK_c,       killclient,     {0} },
	{ MODKEY,                             XK_t,       setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                             XK_f,       setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                             XK_m,       setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                             XK_s,       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                             XK_g,       setlayout,      {.v = &layouts[1]} },
	//{ MODKEY,                       XK_space,  toggleview_focus, {.ui = 1} },
	{ MODKEY|ControlMask,                 XK_space,   setlayout,      {0} },
	{ MODKEY|ShiftMask,                   XK_space,   togglefloating, {0} },
	{ MODKEY,                             XK_w,       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,                   XK_w,       tag,            {.ui = ~0 } },
	{ MODKEY,                             XK_comma,   focusmon,       {.i = -1 } },
	{ MODKEY,                             XK_period,  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,                   XK_comma,   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,                   XK_period,  tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1, 0)
	TAGKEYS(                        XK_2, 1)
	TAGKEYS(                        XK_3, 2)
	TAGKEYS(                        XK_4, 3)
	TAGKEYS(                        XK_5, 4)
	TAGKEYS(                        XK_6, 5)
	TAGKEYS(                        XK_7, 6)
	TAGKEYS(                        XK_8, 7)
	TAGKEYS(                        XK_9, 8)
	TAGKEYS(                        XK_0, 9)
	{ MODKEY,                             XK_q,       quit,           {1} },
	{ MODKEY|ShiftMask,                   XK_q,       quit,           {0} },
	//{ MODKEY|ControlMask,           XK_l,      togglehorizontalmax, {0} },
	//{ MODKEY|ControlMask,           XK_h,      togglehorizontalmax, {0} },
    //{ MODKEY|ControlMask,           XK_j,      toggleverticalmax,   {0} },
	//{ MODKEY|ControlMask,           XK_k,      toggleverticalmax,   {0} },
	//{ MODKEY|ControlMask,           XK_m,      togglemaximize,      {0} },
	{ MODKEY,                             XK_F1,      tagall,         {.v = "F1" } },
	{ MODKEY,                             XK_F2,      tagall,         {.v = "F2" } },
	{ MODKEY,                             XK_F3,      tagall,         {.v = "F3" } },
	{ MODKEY,                             XK_F4,      tagall,         {.v = "F4" } },
	{ MODKEY,                             XK_F5,      tagall,         {.v = "F5" } },
	{ MODKEY,                             XK_F6,      tagall,         {.v = "F6" } },
	{ MODKEY,                             XK_F7,      tagall,         {.v = "F7" } },
	{ MODKEY,                             XK_F8,      tagall,         {.v = "F8" } },
	{ MODKEY,                             XK_F9,      tagall,         {.v = "F9" } },
	{ MODKEY|ShiftMask,                   XK_F1,      spawn,          {.v = volbarcmd } },
	{ MODKEY|ShiftMask,                   XK_F2,      spawn,          {.v = wlanbarcmd } },
	{ MODKEY|ShiftMask,                   XK_F3,      spawn,          {.v = ethbarcmd } },
	{ MODKEY|ShiftMask,                   XK_F4,      spawn,          {.v = batbarcmd } },
	{ MODKEY|ShiftMask,                   XK_F5,      spawn,          {.v = datebarcmd } },
	{ MODKEY,                             XK_F6,      spawn,          {.v = touchpad } },
    /*
	{ MODKEY,                       XK_a,      moveresize,     {.v = "-10x 0y 0w 0h" } },
	{ MODKEY,                       XK_d,      moveresize,     {.v = "10x 0y 0w 0h" } },
	{ MODKEY,                       XK_s,      moveresize,     {.v = "0x 10y 0w 0h" } },
	{ MODKEY,                       XK_w,      moveresize,     {.v = "0x -10y 0w 0h" } },
	{ MODKEY|ControlMask,           XK_a,      moveresize,     {.v = "0X 0y 0w 0h" } },
	{ MODKEY|ControlMask,           XK_d,      moveresize,     {.v = "9000X 0y 0w 0h" } },
	{ MODKEY|ControlMask,           XK_s,      moveresize,     {.v = "0x 9000Y 0w 0h" } },
	{ MODKEY|ControlMask,           XK_w,      moveresize,     {.v = "0x 16Y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_a,      moveresize,     {.v = "0x 0y -10w 0h" } },
	{ MODKEY|ShiftMask,             XK_d,      moveresize,     {.v = "0x 0y 10w 0h" } },
	{ MODKEY|ShiftMask,             XK_s,      moveresize,     {.v = "0x 0y 0w 10h" } },
	{ MODKEY|ShiftMask,             XK_w,      moveresize,     {.v = "0x 0y 0w -10h" } },
    */
	{ MODKEY|ControlMask,                 XK_Return,  focusmaster,    {0} },
	{ MODKEY,                             XK_Left,    shifttags,      {.i = -1} },
	{ MODKEY,                             XK_Right,   shifttags,      {.i = +1} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

