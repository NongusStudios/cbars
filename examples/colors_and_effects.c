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
    // $E EFFECT $E sets the current effect. To see all effects goto cbars.h/cbar_effect_t
    // NOTE1: $E 256(xxx) $E for 256 colors
    // NOTE2: $E RGB(r, g, b) for rgb color (0-255)
    // NOTE3: $E BOLD;UNDERLINE;256(157); $E to use multiple effects in one block
    // NOTE4: Spaces are ommitted
    // BG256 and BGRGB to set background 

                          // Set text to be bold     // Set color         // Set color and underline                // Set color                  // Reset underline and set color         // Reset foreground // Reset all effects             
    cbar_t bar = cbar(48, "$E BOLD; $ELoading Level: $E RGB(8, 104, 252); $E[$E RGB(8, 252, 104);UNDERLINE; $E$F'-'$F$E RGB(252, 8, 104); $E$N'-'$N$E RESET_UNDERLINE;RGB(8, 104, 252); $E] $E FG_RESET; $E$P% $E RESET; $E");
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
