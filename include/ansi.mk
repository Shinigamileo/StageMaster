#########################
#                       #
#     ANSI SETTINGS     #
#                       #
#########################

# Ansi layouts.
# I know it's a carbon copy of 'ansicodes.h',
# but I don't know how to do otherwise...

ifneq ($(shell command -v tput 2>/dev/null),)
  ifeq ($(shell test $(shell tput colors) -ge 8; echo $$?),0)

    ANSI_RESET         = \e[0m

    ANSI_BOLD          = \e[1m
    ANSI_ITALIC        = \e[3m
    ANSI_UNDERLINE     = \e[4m
    ANSI_BLINKING      = \e[5m
    ANSI_INVERSE       = \e[7m
    ANSI_HIDE          = \e[8m  # not official
    ANSI_STRIKETHROUGH = \e[9m

    ANSI_F_BLACK       = \e[30m
    ANSI_F_RED         = \e[31m
    ANSI_F_GREEN       = \e[32m
    ANSI_F_YELLOW      = \e[33m
    ANSI_F_BLUE        = \e[34m
    ANSI_F_MAGENTA     = \e[35m
    ANSI_F_CYAN        = \e[36m
    ANSI_F_WHITE       = \e[37m
    ANSI_F_DEFAULT     = \e[39m

    ANSI_B_BLACK       = \e[40m
    ANSI_B_RED         = \e[41m
    ANSI_B_GREEN       = \e[42m
    ANSI_B_YELLOW      = \e[43m
    ANSI_B_BLUE        = \e[44m
    ANSI_B_MAGENTA     = \e[45m
    ANSI_B_CYAN        = \e[46m
    ANSI_B_WHITE       = \e[47m
    ANSI_B_DEFAULT     = \e[49m

  endif
endif
