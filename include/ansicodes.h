/*
 * ----------------------------------------------------------------------------
 * "THE NO-WARE LICENSE" (Revision -0):
 * Léo Barré wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff.
 * ...That's it.
 * ----------------------------------------------------------------------------
 */
#ifndef ANSICODES_H
#define ANSICODES_H

#ifndef ALTON // if you don't want ansi codes, add -DALTON (got it ? got it ?)
#define ANSICODE(i) "\x1b["i"m"
#else
#define ANSICODE(i) ""
#endif

#define ANSI_RESET             ANSICODE(  "0" )

#define ANSI_BOLD              ANSICODE(  "1" )
#define ANSI_ITALIC            ANSICODE(  "3" )
#define ANSI_UNDERLINE         ANSICODE(  "4" )
#define ANSI_BLINKING          ANSICODE(  "5" )
#define ANSI_INVERSE           ANSICODE(  "7" )
#define ANSI_STRIKETHROUGH     ANSICODE(  "9" )

#define ANSI_BOLD_off          ANSICODE( "22" )
#define ANSI_ITALIC_off        ANSICODE( "23" )
#define ANSI_UNDERLINE_off     ANSICODE( "24" )
#define ANSI_INVERSE_off       ANSICODE( "27" )
#define ANSI_STRIKETHROUGH_off ANSICODE( "29" )

#define ANSI_F_BLACK           ANSICODE( "30" )
#define ANSI_F_RED             ANSICODE( "31" )
#define ANSI_F_GREEN           ANSICODE( "32" )
#define ANSI_F_YELLOW          ANSICODE( "33" )
#define ANSI_F_BLUE            ANSICODE( "34" )
#define ANSI_F_MAGENTA         ANSICODE( "35" )
#define ANSI_F_CYAN            ANSICODE( "36" )
#define ANSI_F_WHITE           ANSICODE( "37" )

#define ANSI_F_DEFAULT         ANSICODE( "39" )

#define ANSI_B_BLACK           ANSICODE( "40" )
#define ANSI_B_RED             ANSICODE( "41" )
#define ANSI_B_GREEN           ANSICODE( "42" )
#define ANSI_B_YELLOW          ANSICODE( "43" )
#define ANSI_B_BLUE            ANSICODE( "44" )
#define ANSI_B_MAGENTA         ANSICODE( "45" )
#define ANSI_B_CYAN            ANSICODE( "46" )
#define ANSI_B_WHITE           ANSICODE( "47" )

#define ANSI_B_DEFAULT         ANSICODE( "49" )

#endif//ANSICODES
