#include <cbars/cbars.h>
#include <unistd.h>

int main(){
    cbar_t bar = cbar(24, "$E 256(12),BOLD, $EAcquiring Knowledge: |$E BRFG_GREEN,BOLD_DIM_RESET $E$F'-'$F>$N' '$N $E 256(12), $E|$E BRFG_RED, $E$N'-'$N>$F' '$F    $E 256(12), $E$P%$E RESET $E");
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
