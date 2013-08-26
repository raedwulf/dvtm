/* valid curses attributes are listed below they can be ORed
 *
 * A_NORMAL        Normal display (no highlight)
 * A_STANDOUT      Best highlighting mode of the terminal.
 * A_UNDERLINE     Underlining
 * A_REVERSE       Reverse video
 * A_BLINK         Blinking
 * A_DIM           Half bright
 * A_BOLD          Extra bright or bold
 * A_PROTECT       Protected mode
 * A_INVIS         Invisible or blank mode
 */
#define BLUE            (COLORS==256 ? 68 : COLOR_BLUE)
/* curses attributes for the currently focused window */
#define SELECTED_ATTR   COLOR(BLUE, -1) | A_NORMAL
/* curses attributes for normal (not selected) windows */
#define NORMAL_ATTR     COLOR(-1, -1) | A_NORMAL
/* curses attributes for the status bar */
#define BAR_ATTR        COLOR(BLUE, -1) | A_NORMAL
/* status bar (command line option -s) position */
#define BAR_POS		BAR_TOP /* BAR_BOTTOM, BAR_OFF */
/* determines whether the statusbar text should be right or left aligned */
#define BAR_ALIGN       ALIGN_RIGHT
/* separator between window title and window number */
#define SEPARATOR " | "
/* printf format string for the window title, first %s
 * is replaced by the title, second %s is replaced by
 * the SEPARATOR, %d stands for the window number */
#define TITLE "[%s%s#%d]"
/* master width factor [0.1 .. 0.9] */
#define MFACT 0.5
/* scroll back buffer size in lines */
#define SCROLL_HISTORY 500

#include "tile.c"
#include "grid.c"
#include "bstack.c"
#include "fullscreen.c"

/* by default the first layout entry is used */
static Layout layouts[] = {
	{ "[]=", tile },
	{ "+++", grid },
	{ "TTT", bstack },
	{ "[ ]", fullscreen },
};

#define MOD CTRL('a')

/* you can at most specifiy MAX_ARGS (3) number of arguments */
static Key keys[] = {
	{   0, ALT('w'),       { create,         { NULL }                    } },
	{   0, ALT('q'),       { create,         { NULL, NULL, "$CWD" }      } },
	{   0, ALT('`'),       { killclient,     { NULL }                    } },
	{   0, ALT('j'),       { focusnext,      { NULL }                    } },
	{   0, ALT('u'),       { focusnextnm,    { NULL }                    } },
	{   0, ALT('i'),       { focusprevnm,    { NULL }                    } },
	{   0, ALT('k'),       { focusprev,      { NULL }                    } },
	{   0, ALT('t'),       { setlayout,      { "[]=" }                   } },
	{   0, ALT('g'),       { setlayout,      { "+++" }                   } },
	{   0, ALT('b'),       { setlayout,      { "TTT" }                   } },
	{   0, ALT('m'),       { setlayout,      { "[ ]" }                   } },
	{   0, ALT(' '),       { setlayout,      { NULL }                    } },
	{   0, ALT('h'),       { setmfact,       { "-0.05" }                 } },
	{   0, ALT('l'),       { setmfact,       { "+0.05" }                 } },
	{   0, ALT('.'),       { toggleminimize, { NULL }                    } },
	{   0, ALT('s'),       { togglebar,      { NULL }                    } },
	{   0, ALT('M'),       { togglemouse,    { NULL }                    } },
	{   0, ALT('\n'),      { zoom ,          { NULL }                    } },
	{   0, ALT('1'),       { focusn,         { "1" }                     } },
	{   0, ALT('2'),       { focusn,         { "2" }                     } },
	{   0, ALT('3'),       { focusn,         { "3" }                     } },
	{   0, ALT('4'),       { focusn,         { "4" }                     } },
	{   0, ALT('5'),       { focusn,         { "5" }                     } },
	{   0, ALT('6'),       { focusn,         { "6" }                     } },
	{   0, ALT('7'),       { focusn,         { "7" }                     } },
	{   0, ALT('8'),       { focusn,         { "8" }                     } },
	{   0, ALT('9'),       { focusn,         { "9" }                     } },
	{   0, ALT('Q'),       { quit,           { NULL }                    } },
	{   0, ALT('G'),       { escapekey,      { NULL }                    } },
	{   0, ALT('a'),       { togglerunall,   { NULL }                    } },
	{   0, ALT('r'),       { redraw,         { NULL }                    } },
	{   0, ALT('X'),       { lock,           { NULL }                    } },
	{   0, ALT('B'),       { togglebell,     { NULL }                    } },
	{   0, ALT('c'),       { copymode,       { NULL }                    } },
	{   0, ALT('/'),       { copymode,       { "/" }                     } },
	{   0, ALT('?'),       { copymode,       { "?" }                     } },
	{   0, ALT('v'),       { paste,          { NULL }                    } },
	{ MOD, KEY_PPAGE,      { scrollback,     { "-1" }                    } },
	{ MOD, KEY_NPAGE,      { scrollback,     { "1"  }                    } },
	{ MOD, KEY_F(1),       { create,         { "man dvtm", "dvtm help" } } },
};

static const ColorRule colorrules[] = {
	{ "", A_NORMAL, -1, -1 }, /* default */
#if 0
	/* title attrs     fgcolor      bgcolor */
	{ "ssh", A_NORMAL, COLOR_BLACK, 224      },
#endif
};

/* possible values for the mouse buttons are listed below:
 *
 * BUTTON1_PRESSED          mouse button 1 down
 * BUTTON1_RELEASED         mouse button 1 up
 * BUTTON1_CLICKED          mouse button 1 clicked
 * BUTTON1_DOUBLE_CLICKED   mouse button 1 double clicked
 * BUTTON1_TRIPLE_CLICKED   mouse button 1 triple clicked
 * BUTTON2_PRESSED          mouse button 2 down
 * BUTTON2_RELEASED         mouse button 2 up
 * BUTTON2_CLICKED          mouse button 2 clicked
 * BUTTON2_DOUBLE_CLICKED   mouse button 2 double clicked
 * BUTTON2_TRIPLE_CLICKED   mouse button 2 triple clicked
 * BUTTON3_PRESSED          mouse button 3 down
 * BUTTON3_RELEASED         mouse button 3 up
 * BUTTON3_CLICKED          mouse button 3 clicked
 * BUTTON3_DOUBLE_CLICKED   mouse button 3 double clicked
 * BUTTON3_TRIPLE_CLICKED   mouse button 3 triple clicked
 * BUTTON4_PRESSED          mouse button 4 down
 * BUTTON4_RELEASED         mouse button 4 up
 * BUTTON4_CLICKED          mouse button 4 clicked
 * BUTTON4_DOUBLE_CLICKED   mouse button 4 double clicked
 * BUTTON4_TRIPLE_CLICKED   mouse button 4 triple clicked
 * BUTTON_SHIFT             shift was down during button state change
 * BUTTON_CTRL              control was down during button state change
 * BUTTON_ALT               alt was down during button state change
 * ALL_MOUSE_EVENTS         report all button state changes
 * REPORT_MOUSE_POSITION    report mouse movement
 */

#ifdef NCURSES_MOUSE_VERSION
# define CONFIG_MOUSE /* compile in mouse support if we build against ncurses */
#endif

#define ENABLE_MOUSE true /* whether to enable mouse events by default */

#ifdef CONFIG_MOUSE
static Button buttons[] = {
	{ BUTTON1_CLICKED,        { mouse_focus,      { NULL  } } },
	{ BUTTON1_DOUBLE_CLICKED, { mouse_fullscreen, { "[ ]" } } },
	{ BUTTON2_CLICKED,        { mouse_zoom,       { NULL  } } },
	{ BUTTON3_CLICKED,        { mouse_minimize,   { NULL  } } },
};
#endif /* CONFIG_MOUSE */

static Cmd commands[] = {
	{ "create", { create,	{ NULL } } },
};

/* gets executed when dvtm is started */
static Action actions[] = {
	{ create, { NULL } },
};

static char const * const keytable[] = {
	/* add your custom key escape sequences */
};
