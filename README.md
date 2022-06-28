# cbars/cbars.h
CBars, a small c99 library for progress bars in the terminal.

# Example
```c
cbar_t bar = cbar(48, "Loading Level: [$F'-'$F$N' '$N] $P%");
cbar_hide_cursor();
while(loading()){
    bar.progress = load();
    cbar_display_bar(&bar);
}
cbar_show_cursor();
printf("\n");
```

# Result
```
Loading Level: [------------------------------------            ] 76%
```

# Usage
CBars uses a custom syntax to define how a bar is rendered.<br>
> - $F and $N is used to render the bar (E.G. `"$F'-'$F$N' '$N"`).<br>
> > - $F (Fill) is where the filled part of the bar is rendered. <br>
> > - $N (None) is where the rest of the bar is rendered. <br>
> > - The character between the `''` will be used to print that part of the bar. <br>
> - $P is used to render the percentage of completion (0-100). <br>
> - $E is used to set effects (E.G. `"$E BOLD;256(6); $E"`). <br>
> > - Everything is separated with a semi-colon.<br>
> > - `256(n)` and `BG256(n)` is used for 256 colors.<br>
> > - `RGB(r, g, b)` and `BGRGB(r, g, b)` is used for rgb colors.<br>
> > - To see all possible effects go to `include/cbars/cbars.h` and look at the `cbar_effect_t` enum.<br>

# Missing features that I might implement when I feel like it
> Cursor Controls (Although you can use ANSII escape sequences in the bar string)
