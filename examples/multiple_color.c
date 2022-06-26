#include <cbars/cbars.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

int main(){
    srand(time(NULL));

    const uint32_t BAR_SIZE = 32;
    cbar_t bars[] = {
        cbar(BAR_SIZE, '[', ']', '=', ' ', "Bar1: ", ""),
        cbar(BAR_SIZE, '<', '>', '#', ' ', "Bar2: ", ""),
        cbar(BAR_SIZE, '(', ')', '>', ' ', "Bar3: ", ""),
        cbar(BAR_SIZE, '|', '|', '*', '/', "Bar4: ", ""),
    };
    size_t bar_count = 4;

    cbar_color_desc_t bar1_colors[] = {
        cbar_color_desc(0.0,  35, 35, 31, 0, 0, 0),
        cbar_color_desc(0.5,  35, 35, 93, 0, 0, 0),
        cbar_color_desc(0.8,  35, 35, 92, 0, 0, 0),
    };
    cbar_color_desc_t bar2_colors[] = {
        cbar_color_desc(0.0,  95, 95, 92, 0, 0, 0),
        cbar_color_desc(0.3,  95, 95, 93, 0, 0, 0),
        cbar_color_desc(0.6,  95, 95, 31, 0, 0, 0),
    };
    cbar_color_desc_t bar3_colors[] = {
        cbar_color_desc(0.0,  32, 32, 31, 0, 0, 0),
        cbar_color_desc(0.1,  32, 32, 93, 0, 0, 0),
        cbar_color_desc(0.9,  32, 32, 92, 0, 0, 0),
    };
    cbar_color_desc_t bar4_colors[] = {
        cbar_color_desc(0.0,  95, 95, 31, 92, 0, 0),
        cbar_color_desc(0.5,  95, 95, 92, 31, 0, 0),
    };
    cbar_color_desc_t* color_descs[] = {
        bar1_colors,
        bar2_colors,
        bar3_colors,
        bar4_colors,
    };
    size_t color_sizes[] = {
        3, 3, 3, 2
    };

    bool complete = false;
    const double PERIOD = 0.1;

    cbar_hide_cursor();
    while(!complete){
        usleep((useconds_t)(PERIOD*1000000.0));
        for(size_t i = 0; i < bar_count; i++){
            bars[i].progress += ((double)rand() / (double)RAND_MAX) / (double)(i+12);
        }
        cbar_display_bars_colored(bars, color_descs, color_sizes, bar_count);

        complete = true;
        for(size_t i = 0; i < bar_count; i++){
            if(bars[i].progress < 1.0){ complete = false; break; }
        }
    }
    for(size_t i = 0; i < bar_count; i++){
        printf("\n");
    }
    cbar_show_cursor();

    return 0;
}
