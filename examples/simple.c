#include <cbars/cbars.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

int main(){
    // $E EFFECT $E sets the current effect. To see all effects goto cbars.h/cbar_effect_t
    // NOTE1: $E 256(xxx) $E for 256 colors
    // NOTE2: $E RGB(r, g, b) for rgb color (0-255)
    // NOTE3: $E BOLD,UNDERLINE,256(157) $E to use multiple effects in one block
    // NOTE4: Spaces are ommitted
    // BG256 and BGRGB to set background 

    // $F '-' $F Where the fill string is placed
    // $N '-' $N Where the none string is placed
    // The character for the none and fill is defined between
    // E.G. $N ' ' $N$F '-' $F would result in the bar being reversed

    // $P will be replaced with the percentage of the bar
    // First two args are the fill and none characters
    cbar_t bar = cbar(48, "Loading Level: [$F'-'$F$N' '$N] $P%");
    cbar_hide_cursor();
    while(bar.progress <= 1.0){
        bar.progress += 0.001;
        cbar_display_bar(&bar);
        usleep(0.001*1000000.0);
    }
    cbar_show_cursor();
    printf("\n");
    return 0;
}

