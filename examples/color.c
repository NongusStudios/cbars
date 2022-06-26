#include <cbars/cbars.h>
#include <stdio.h>
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
    cbar_t progress_bar = cbar(32, '[', ']', '=', '-', "Progress: ", end);

    // order MATTERS : uses ANSII escape color codes
    cbar_color_desc_t colors[] = {
        // progress : begin color ('[') : end color (']') : fill color ('=') : none color ('-') : before color ("Progress: ") : after color ("XXX%")
        cbar_color_desc(0.0,  95, 95, 92, 31, 0, 0), // From 20% -  80% | Bright Yellow
        cbar_color_desc(0.5,  95, 95, 31, 92, 0, 0), // From 80% - >80% | Bright Green
    };

    const double PERIOD = 0.1;
    cbar_hide_cursor();
    while(progress_bar.progress < 1.0){
        usleep((useconds_t)(PERIOD*1000000.0));

        progress_bar.progress += ((double)rand() / (double)RAND_MAX) / 32.0;
        progress_bar.progress = my_clamp(progress_bar.progress, 0.0, 1.0);

        sprintf(end, " %u%c", (uint32_t)floor(progress_bar.progress*100.0), '%');

        cbar_display_bar_colored(&progress_bar, colors, 2);
    }
    cbar_show_cursor();
    return 0;
}

