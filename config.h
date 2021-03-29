/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 16;        /* horiz inner gap between windows */
static const unsigned int gappiv    = 16;       /* vert inner gap between windows */
static const unsigned int gappoh    = 32;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 32;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */

static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */

static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 22;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "CaskaydiaCove Nerd Font:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";

static char termcol0[] = "#000000"; /* black   */
static char termcol1[] = "#ff0000"; /* red     */
static char termcol2[] = "#33ff00"; /* green   */
static char termcol3[] = "#ff0099"; /* yellow  */
static char termcol4[] = "#0066ff"; /* blue    */
static char termcol5[] = "#cc00ff"; /* magenta */
static char termcol6[] = "#00ffff"; /* cyan    */
static char termcol7[] = "#d0d0d0"; /* white   */
static char termcol8[]  = "#808080"; /* black   */
static char termcol9[]  = "#ff0000"; /* red     */
static char termcol10[] = "#33ff00"; /* green   */
static char termcol11[] = "#ff0099"; /* yellow  */
static char termcol12[] = "#0066ff"; /* blue    */
static char termcol13[] = "#cc00ff"; /* magenta */
static char termcol14[] = "#00ffff"; /* cyan    */
static char termcol15[] = "#ffffff"; /* white   */
static char *termcolor[] = {
  termcol0,
  termcol1,
  termcol2,
  termcol3,
  termcol4,
  termcol5,
  termcol6,
  termcol7,
  termcol8,
  termcol9,
  termcol10,
  termcol11,
  termcol12,
  termcol13,
  termcol14,
  termcol15,
};

static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title             tags mask     isfloating   isterminal noswallow monitor */
	{ "Gimp",     NULL,       NULL,             0,            1,           0,         0,        -1 },
	{ "Firefox",  NULL,       NULL,             1 << 8,       0,           0,         0,        -1 },
	{ "kitty",    NULL,       NULL,             0,            0,           1,         0,        -1 },
	{ "Alacritty",NULL,       NULL,             0,            0,           0,         0,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"


static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define Alt Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

// MOD+j/k - focus down / up in the stack
// MOD+tab - focus prev in stack
// MOD+a - focus top of the stack
// MOD+z - focus bottom of the stack
// add shift to move clients
#define STACKKEYS(MOD,ACTION) \
	{ MOD, 44,       ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, 45,       ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, 23,       ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, 38,       ACTION##stack, {.i = 0 } }, \
	{ MOD, 52,       ACTION##stack, {.i = -1 } },


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "kitty", NULL };

static Key keys[] = {
	/* modifier					key    function        argument */
	{ MODKEY,               	56,		togglebar,      {0} },				// b
	//{ MODKEY,               	44,    	focusstack,     {.i = +1 } },      	// j
	//{ MODKEY,               	45,    	focusstack,     {.i = -1 } },      	// k
	STACKKEYS(MODKEY,                          focus)
    STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,               	31,    	incnmaster,     {.i = +1 } },      	// i
	{ MODKEY,               	40,	   	incnmaster,     {.i = -1 } },      	// d
	{ MODKEY,               	43,	   	setmfact,       {.f = -0.05} },    	// h
	{ MODKEY,               	46,	   	setmfact,       {.f = +0.05} },    	// l
    { MODKEY|ShiftMask,         43,     setcfact,       {.f = -0.25} },     // h
	{ MODKEY|ShiftMask,         46,     setcfact,       {.f = +0.25} },     // l
    //{ MODKEY|ShiftMask,         ?,      setcfact,       {.f =  0.00} },
	{ MODKEY|ControlMask,           44, moveresize,     {.v = "0x 25y 0w 0h" } },   // j
	{ MODKEY|ControlMask,           45, moveresize,     {.v = "0x -25y 0w 0h" } },  // k
	{ MODKEY|ControlMask,           46, moveresize,     {.v = "25x 0y 0w 0h" } },   // l
	{ MODKEY|ControlMask,           43, moveresize,     {.v = "-25x 0y 0w 0h" } },  // h
	{ MODKEY|ControlMask|ShiftMask, 44, moveresize,     {.v = "0x 0y 0w 25h" } },   // j
	{ MODKEY|ControlMask|ShiftMask, 45, moveresize,     {.v = "0x 0y 0w -25h" } },  // k
	{ MODKEY|ControlMask|ShiftMask, 46, moveresize,     {.v = "0x 0y 25w 0h" } },   // l
	{ MODKEY|ControlMask|ShiftMask, 43, moveresize,     {.v = "0x 0y -25w 0h" } },  // h
	{ MODKEY,               	9,	   	zoom,           {0} },             	// Escape
	//{ MODKEY,               	23,    	view,           {0} },             	// Tab
	{ MODKEY,			    	24,    	killclient,     {0} },             	// q
	{ MODKEY,               	28,    	setlayout,      {.v = &layouts[0]} }, // t
	{ MODKEY,               	41,    	setlayout,      {.v = &layouts[1]} }, // f
	{ MODKEY,               	58,    	setlayout,      {.v = &layouts[2]} }, // m
	{ MODKEY|Alt,               28,    	setlayout,      {.v = &layouts[3]} }, // t
	{ MODKEY|ShiftMask,     	65,    	togglefloating, {0} },				// space
	{ MODKEY,               	19,    	view,           {.ui = ~0 } },     	// 0
	{ MODKEY|ShiftMask,     	19,    	tag,            {.ui = ~0 } },     	// 0
	{ MODKEY,               	59,    	focusmon,       {.i = -1 } },      	// comma
	{ MODKEY,               	60,    	focusmon,       {.i = +1 } },      	// period
	{ MODKEY|ShiftMask,     	59,    	tagmon,         {.i = -1 } },      	// comma
	{ MODKEY|ShiftMask,     	60,    	tagmon,         {.i = +1 } },      	// period
    { MODKEY|ShiftMask,         41,     togglefullscr,  {0} },				// f
	{ MODKEY|ShiftMask,         21,		incrogaps,      {.i = +4 } },		// equal
	{ MODKEY,                   20,  	incrogaps,      {.i = -4 } },		// minus
	{ MODKEY|Alt|ShiftMask,     21,  	incrigaps,      {.i = +4 } },		// equal
	{ MODKEY|Alt,               20,  	incrigaps,      {.i = -4 } },		// minus
	{ MODKEY|ControlMask,       19,     togglegaps,     {0} },				// 0
	{ MODKEY|Alt|ControlMask,   19,     defaultgaps,    {0} },				// 0
	TAGKEYS(                	10,    	                0)					// 1
	TAGKEYS(                	11,    	                1)                 	// 2
	TAGKEYS(                	12,    	                2)                 	// 3
	TAGKEYS(                	13,    	                3)                 	// 4
	TAGKEYS(                	14,    	                4)                 	// 5
	TAGKEYS(                	15,    	                5)                 	// 6
	TAGKEYS(                	16,    	                6)                 	// 7
	TAGKEYS(                	17,    	                7)                 	// 8
	TAGKEYS(                	18,    	                8)                 	// 9
	{ MODKEY|Alt,               27,     xrdb,           {.v = NULL } },     // r
	{ MODKEY|ShiftMask,     	22,    	quit,           {0} },             	// Backspace
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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

