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
#include <stdbool.h>
#include <unistd.h>

int main(){
    cbar_t bars[] = {
        cbar(32, "Bar1: $P [$F'='$F$N' '$N]"),
        cbar(32, "Bar2: <$E RGB(8, 252, 104);BOLD; $E$F'-'$F$E RESET; $E$N' '$N> $P%$E RESET; $E"),
        cbar(32, "Bar3: $E RGB(252, 8, 104);BOLD; $E$N'_'$N$E RGB(8, 252, 104); $E$F'_'$F$E RESET; $E $P"),
    };
    bool complete = false;
    cbar_hide_cursor();
    while(!complete){
        complete = true;
        for(size_t i = 0; i < 3; i++){
            bars[i].progress += 0.0001;
            if(bars[i].progress <= 1.0) complete = false;
        }
        cbar_display_bars(bars, 3);
        usleep(0.001*1000000.0);
    }
    cbar_show_cursor();
    printf("\n\n\n");
    return 0;
}

