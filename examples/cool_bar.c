/*
    CBars a simple library for displaying terminal based progress bars
    Copyright (C) 2022  NongusStudios

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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
