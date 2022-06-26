#include <cbars/cbars.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

cbar_color_desc_t cbar_color_fill(double progress, uint32_t color){
    return (cbar_color_desc_t){
        .progress = progress,
        .begin_color = color,
        .end_color = color,
        .fill_color = color,
        .none_color = color,
        .before_color = color,
        .after_color = color,
    };
}
cbar_color_desc_t cbar_color_desc(
    double progress,
    uint32_t begin_color,
    uint32_t end_color,
    uint32_t fill_color,
    uint32_t none_color,
    uint32_t before_color,
    uint32_t after_color
){
    return (cbar_color_desc_t){
        .progress = progress,
        .begin_color = begin_color,
        .end_color = end_color,
        .fill_color = fill_color,
        .none_color = none_color,
        .before_color = before_color,
        .after_color = after_color,
    };
}


void set_color(uint32_t code){
    printf("\033[%um", code);
}
void reset_color(){
    printf("\033[0m");
}

cbar_t cbar(
    uint32_t length,
    char     begin,
    char     end,
    char     fill,
    char     none,
    char*    before,
    char*    after
){
    return (cbar_t){
        .progress = 0.0,
        .length   = length,
        .begin    = begin,
        .end      = end,
        .fill     = fill,
        .none     = none,
        .before   = before,
        .after    = after,
    };
}
void cbar_display_bar(const cbar_t* pbar){
    assert(pbar != NULL);

    const cbar_t bar = *pbar;
    assert(bar.length > 2);

    printf("\r%s%c", bar.before, bar.begin);
                                                       // subtract 2 to account for the beginning and end characters
    uint32_t progress = (uint32_t)floor(bar.progress * (bar.length-2));
    for(uint32_t i = 1; i <= bar.length-2; i++){
        if(i <= progress){
            printf("%c", bar.fill);
        } else {
            printf("%c", bar.none);
        }
    }
    printf("%c%s", bar.end, bar.after);

    fflush(stdout);
}
void cbar_display_bars(const cbar_t* bars, size_t count){
    for(size_t i = 0; i < count; i++){
        const cbar_t bar = bars[i];
        assert(bar.length > 2);

        printf("\r%s%c", bar.before, bar.begin);
                                                           // subtract 2 to account for the beginning and end characters
        uint32_t progress = (uint32_t)floor(bar.progress * (bar.length-2));
        for(uint32_t i = 1; i <= bar.length-2; i++){
            if(i <= progress){
                printf("%c", bar.fill);
            } else {
                printf("%c", bar.none);
            }
        }
        printf("%c%s\n", bar.end, bar.after);
    }
    printf("\033[%luF", count);
}
void cbar_display_bar_colored(
    const cbar_t* pbar,
    cbar_color_desc_t* descriptors, size_t desc_count
){
    assert(pbar != NULL);
    assert(desc_count > 0);
    const cbar_t bar = *pbar;
    assert(bar.length > 2);

    cbar_color_desc_t color_desc = {0};
    if(desc_count == 1) color_desc = descriptors[0];
    else {
        double progress = bar.progress;
        for(size_t i = 0; i < desc_count; i++){
            cbar_color_desc_t* current = descriptors + i;
            cbar_color_desc_t* next = (i+1 < desc_count) ? descriptors + (i+1) : NULL;

            if(next){
                if(progress >= current->progress && progress <= next->progress){
                    color_desc = *current;
                    break;
                }
            } else {
                if(progress >= current->progress){
                    color_desc = *current;
                    break;
                }
            }
        }
    }

    set_color(color_desc.before_color);
    printf("\r%s", bar.before);
    set_color(color_desc.begin_color);
    printf("%c", bar.begin);
                                                       // subtract 2 to account for the beginning and end characters
    uint32_t progress = (uint32_t)floor(bar.progress * (bar.length-2));
    for(uint32_t i = 1; i <= bar.length-2; i++){
        if(i <= progress){
            set_color(color_desc.fill_color);
            printf("%c", bar.fill);
        } else {
            set_color(color_desc.none_color);
            printf("%c", bar.none);
        }
    }

    set_color(color_desc.end_color);
    printf("%c", bar.end);
    set_color(color_desc.after_color);
    printf("%s", bar.after);
    reset_color();

    fflush(stdout);
}
void cbar_display_bars_colored(
    const cbar_t* bars,
    cbar_color_desc_t** descriptors,
    size_t* desc_counts,
    size_t count
){
    for(size_t i = 0; i < count; i++){
        const cbar_t bar = bars[i];
        assert(bar.length > 2);

        cbar_color_desc_t color_desc = {0};
        for(size_t j = 0; j < desc_counts[i]; j++){
            double progress = bar.progress;
            cbar_color_desc_t* current = descriptors[i] + j;
            cbar_color_desc_t* next = (j+1 < desc_counts[i]) ? descriptors[i] + (j+1) : NULL;
            if(next){
                if(progress >= current->progress && progress <= next->progress){
                    color_desc = *current;
                    break;
                }
            } else {
                if(progress >= current->progress){
                    color_desc = *current;
                    break;
                }
            }
        }

        set_color(color_desc.before_color);
        printf("%s", bar.before);
        set_color(color_desc.begin_color);
        printf("%c", bar.begin);
                                                        // subtract 2 to account for the beginning and end characters
        uint32_t progress = (uint32_t)floor(bar.progress * (bar.length-2));
        for(uint32_t i = 1; i <= bar.length-2; i++){
            if(i <= progress){
                set_color(color_desc.fill_color);
                printf("%c", bar.fill);
            } else {
                set_color(color_desc.none_color);
                printf("%c", bar.none);
            }
        }

        set_color(color_desc.end_color);
        printf("%c", bar.end);
        set_color(color_desc.after_color);
        printf("%s\n", bar.after);
        reset_color();
    }
    printf("\033[%luF", count);
}

void cbar_hide_cursor(){
    printf("\033[?25l");
}
void cbar_show_cursor(){
    printf("\033[?25h");
}
