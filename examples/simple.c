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
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

int main(){
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

