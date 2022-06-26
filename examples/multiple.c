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
        cbar(BAR_SIZE, '|', '|', '*', ' ', "Bar4: ", ""),
    };
    size_t bar_count = 4;

    bool complete = false;
    const double PERIOD = 0.1;

    cbar_hide_cursor();
    while(!complete){
        usleep((useconds_t)(PERIOD*1000000.0));
        for(size_t i = 0; i < bar_count; i++){
            bars[i].progress += ((double)rand() / (double)RAND_MAX) / (double)(i+12);
        }
        cbar_display_bars(bars, bar_count);

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
