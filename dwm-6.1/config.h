/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const char *fonts[] = {
	"monospace:size=10"
};
static const char dmenufont[]       = "monospace:size=10";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#00ff00";
//static const char selbordercolor[]  = "#005577";
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, 0: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */


/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
//	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	{ "Firefox",  NULL,       NULL,       0,       0,           -1 },
};

#define NUMCOLORS         6
static const char colors[NUMCOLORS][MAXCOLORS][8] = {
	// border   foreground background
        { "#f00033", "#dddddd", "#000033" },  // normal
	{ "#00ff00", "#ffffff", "#000088" },  // selected
	{ "#ff0000", "#000000", "#ffff00" },  // urgent/warning  (black on yellow)
	{ "#ff0000", "#ffffff", "#ffcc00" },  // warning
	{ "#ff0000", "#ffffff", "#ff0000" },  // error (white on red)
	{ "#ff0000", "#000000", "#00ff00" },  // highlight  (black on green)
	// add more here
};

/* layout(s) */
static const float mfact     = 0.62; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile },    /* first entry is default */
	{ "[F]",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
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
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
//static const char *roficmd[] = { "rofi -show run", NULL };
static const char *termcmd[]  = { "xfce4-terminal", NULL };
static const char *browsercmd[]  = { "firefox", NULL };
static const char *capscmd[]  = { "capslock.sh", NULL };
static const char *exitcmd[]  = { "shutdown.sh", NULL };

/*Thinkpad T430 special key commands*/
static const char *mutecmd[] = { "pulsemixer", "--toggle-mute", NULL };
//static const char *volupcmd[] = { "pulsemixer", "--change-volume", "+5", NULL };
//static const char *voldowncmd[] = { "pulsemixer", "--change-volume", "-5", NULL };
//static const char *miccmd[] = { "amixer", "set", "Capture", "toggle", NULL };
static const char *brupcmd[] = { "sudo", "xbacklight", "-inc", "10", NULL };
static const char *brdowncmd[] = { "sudo", "xbacklight", "-dec", "10", NULL };
static const char *mutemiccmd[]  = { "mic_toggle.sh", NULL };

static Key keys[] = {
	/* modifier           key        		function        argument */
	{ MODKEY,             XK_p,      		spawn,          {.v = dmenucmd } },
//	{ MODKEY,             XK_p,      		spawn,          {.v = roficmd } },
	{ False,              XK_Caps_Lock, 		spawn,       	{.v = capscmd } },
	{ MODKEY|ShiftMask,   XK_Return, 		spawn,          {.v = termcmd } },
	{ False, 	      XF86XK_AudioMute, 	spawn, 		{.v = mutecmd } },
	{ False,	      XF86XK_AudioMicMute, 	spawn, 		{.v = mutemiccmd } },
//	{ 0, 		      XF86XK_AudioLowerVolume, 	spawn, 		{.v = voldowncmd } },
//	{ 0, 		      XF86XK_AudioRaiseVolume, 	spawn, 		{.v = volupcmd } },
	{ 0, 		      XF86XK_MonBrightnessUp, 	spawn, 		{.v = brupcmd} },
	{ 0, 		      XF86XK_MonBrightnessDown, spawn, 		{.v = brdowncmd} },
	{ MODKEY,             XK_b,      		togglebar,      {0} },
	{ MODKEY,             XK_j,     		focusstack,     {.i = +1 } },
	{ MODKEY,             XK_k,     		focusstack,     {.i = -1 } },
	{ MODKEY,             XK_i,     		incnmaster,     {.i = +1 } },
	{ MODKEY,             XK_d,     		incnmaster,     {.i = -1 } },
	{ MODKEY,             XK_h,     		setmfact,       {.f = -0.05} },
	{ MODKEY,             XK_l,     		setmfact,       {.f = +0.05} },
	{ MODKEY,             XK_Return,		zoom,           {0} },
	{ MODKEY,             XK_Tab,   		view,           {0} },
	{ MODKEY|ShiftMask,   XK_q,     		killclient,     {0} },
	{ MODKEY|ShiftMask,   XK_f,     		spawn,          {.v = browsercmd } },
	{ MODKEY,             XK_t,     		setlayout,      {.v = &layouts[0]} },
	{ MODKEY,             XK_f,     		setlayout,      {.v = &layouts[1]} },
	{ MODKEY,             XK_m,     		setlayout,      {.v = &layouts[2]} },
	{ MODKEY,             XK_space, 		setlayout,      {0} },
	{ MODKEY|ShiftMask,   XK_space, 		togglefloating, {0} },
	{ MODKEY,             XK_0,     		view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,   XK_0,     		tag,            {.ui = ~0 } },
	{ MODKEY,             XK_comma, 		focusmon,       {.i = -1 } },
	{ MODKEY,             XK_period,		focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,   XK_comma, 		tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,   XK_period,		tagmon,         {.i = +1 } },
	TAGKEYS(              XK_1,                     0)
	TAGKEYS(              XK_2,                     1)
	TAGKEYS(              XK_3,                     2)
	TAGKEYS(              XK_4,                     3)
	TAGKEYS(              XK_5,                     4)
	TAGKEYS(              XK_6,                     5)
	TAGKEYS(              XK_7,                     6)
	TAGKEYS(              XK_8,                     7)
	TAGKEYS(              XK_9,                     8)
	{ MODKEY|ShiftMask,   XK_x,     		spawn,          {.v = exitcmd } },
//	{ MODKEY|ShiftMask,   XK_x,      quit,          {0} },
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

