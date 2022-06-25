#include <cbars/cbars.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

double my_clamp(double v, double min, double max){
    if(v < min) return min;
    if(v > max) return max;
    return v;
}

int main(){
    srand(time(NULL));
    
    char end[10] = {0};
    cbar_t progress_bar = cbar(64, '[', ']', '-', ' ', "Loading Level: ", end);
    const double PERIOD = 0.1;
    cbar_hide_cursor();
    while(progress_bar.progress < 1.0){
        usleep((useconds_t)(PERIOD*1000000.0));

        progress_bar.progress += ((double)rand() / (double)RAND_MAX) / 32.0;
        progress_bar.progress = my_clamp(progress_bar.progress, 0.0, 1.0);

        sprintf(end, " %u%c", (uint32_t)floor(progress_bar.progress*100.0), '%');

        cbar_display_bar(&progress_bar);
    }
    cbar_show_cursor();
    return 0;
}