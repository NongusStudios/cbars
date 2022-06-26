#include <stdint.h>
#include <stddef.h>

typedef struct cbar_color_desc_t {
    double   progress;
    uint32_t begin_color;
    uint32_t end_color;
    uint32_t fill_color;
    uint32_t none_color;
    uint32_t before_color;
    uint32_t after_color;
} cbar_color_desc_t;
// Creates a color descriptor all in one color
cbar_color_desc_t cbar_color_fill(double progress, uint32_t color);
// Creates a color descriptor
cbar_color_desc_t cbar_color_desc(
    double progress,
    uint32_t begin_color,
    uint32_t end_color,
    uint32_t fill_color,
    uint32_t none_color,
    uint32_t before_color,
    uint32_t after_color
);

typedef struct cbar_t {
    double   progress; // 0.0 - 1.0
    uint32_t length;
    char     begin;
    char     end;
    char     fill;
    char     none;
    char*    before;
    char*    after;
} cbar_t;

// creates a new bar with a progress of 0.0
cbar_t cbar(
    uint32_t length,
    char     begin,
    char     end,
    char     fill,
    char     none,
    char*    before,
    char*    after
);
void cbar_display_bar(const cbar_t* bar);
void cbar_display_bars(const cbar_t* bars, size_t count);

void cbar_display_bar_colored(
    const cbar_t* bar,
    cbar_color_desc_t* descriptors, size_t desc_count
);
void cbar_display_bars_colored(
    const cbar_t* bars,
    cbar_color_desc_t** descriptors,
    size_t* desc_counts,
    size_t count
);

void cbar_hide_cursor();
void cbar_show_cursor();
