#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CBAR_FORMAT_CHAR '$'
// F - Fill "$F"
#define CBAR_FILL_FORMAT 'F'
// N - None "$N"
#define CBAR_NONE_FORMAT 'N'
// P - Percent "$P"
#define CBAR_PERCENT_FORMAT 'P'
// E - Effect "$E X $E"
#define CBAR_START_END_EFFECT 'E'

#define cbar(l, s) (cbar_t){.progress = 0.0, .len = l, .bar = s}
#define cbar_display(b, ...) {\
    cbar_t cbar_bar_copy = b; \
    cbar_bar_copy.bar = malloc(snprintf(NULL, 0, b.bar, __VA_ARGS__)+1); \
    sprintf(cbar_bar_copy.bar, b.bar, __VA_ARGS__); \
    cbar_display_bar(&cbar_bar_copy); \
    free(cbar_bar_copy.bar); \
}

typedef struct cbar_t {
    // 0.0 - 1.0
    double progress;
    // Size of the bar in chars
    size_t len;
    // How the bar is displayed 
    char*  bar;
} cbar_t;
typedef enum cbar_effect_t {
    CBAR_EFFECT_RESET               = 0,
    CBAR_EFFECT_BOLD                = 1,
    CBAR_EFFECT_DIM                 = 2,
    CBAR_EFFECT_ITALIC              = 3,
    CBAR_EFFECT_UNDERLINE           = 4,
    CBAR_EFFECT_BLINKING            = 5,
    CBAR_EFFECT_INVERSE             = 6,
    CBAR_EFFECT_HIDDEN              = 7,
    CBAR_EFFECT_STRIKETHROUGH       = 8,

    CBAR_EFFECT_RESET_BOLD_DIM      = 22,
    CBAR_EFFECT_RESET_ITALIC        = 23,
    CBAR_EFFECT_RESET_UNDERLINE     = 24,
    CBAR_EFFECT_RESET_BLINKING      = 25,
    CBAR_EFFECT_RESET_INVERSE       = 27,      
    CBAR_EFFECT_RESET_HIDDEN        = 28,
    CBAR_EFFECT_RESET_STRIKETHROUGH = 29,
    
    CBAR_EFFECT_FG_BLACK            = 30,
    CBAR_EFFECT_FG_RED              = 31,
    CBAR_EFFECT_FG_GREEN            = 32,
    CBAR_EFFECT_FG_YELLOW           = 33,
    CBAR_EFFECT_FG_BLUE             = 34,
    CBAR_EFFECT_FG_MAGENTA          = 35,
    CBAR_EFFECT_FG_CYAN             = 36,
    CBAR_EFFECT_FG_WHITE            = 37,
    CBAR_EFFECT_FG_RESET            = 39,

    CBAR_EFFECT_BG_BLACK            = 40,
    CBAR_EFFECT_BG_RED              = 41,
    CBAR_EFFECT_BG_GREEN            = 42,
    CBAR_EFFECT_BG_YELLOW           = 43,
    CBAR_EFFECT_BG_BLUE             = 44,
    CBAR_EFFECT_BG_MAGENTA          = 45,
    CBAR_EFFECT_BG_CYAN             = 46,
    CBAR_EFFECT_BG_WHITE            = 47,
    CBAR_EFFECT_BG_RESET            = 49,

    CBAR_EFFECT_BRFG_BLACK          = 90,
    CBAR_EFFECT_BRFG_RED            = 91,
    CBAR_EFFECT_BRFG_GREEN          = 92,
    CBAR_EFFECT_BRFG_YELLOW         = 93,
    CBAR_EFFECT_BRFG_BLUE           = 94,
    CBAR_EFFECT_BRFG_MAGENTA        = 95,
    CBAR_EFFECT_BRFG_CYAN           = 96,
    CBAR_EFFECT_BRFG_WHITE          = 97,
    
    CBAR_EFFECT_BRBG_BLACK          = 100,
    CBAR_EFFECT_BRBG_RED            = 101,
    CBAR_EFFECT_BRBG_GREEN          = 102,
    CBAR_EFFECT_BRBG_YELLOW         = 103,
    CBAR_EFFECT_BRBG_BLUE           = 104,
    CBAR_EFFECT_BRBG_MAGENTA        = 105,
    CBAR_EFFECT_BRBG_CYAN           = 106,
    CBAR_EFFECT_BRBG_WHITE          = 107,
} cbar_effect_t;

// returns a string allocated with calloc
char* cbar_tostr(const cbar_t* bar);
// returns a string allocated with calloc
char* cbar_effect_to_escape_codes(const char* effects);
void cbar_display_bar(const cbar_t* bar);
//void cbar_display_bars(const cbar_t* bars, size_t count);

void cbar_hide_cursor();
void cbar_show_cursor();

