# cbars/cbars.h
CBars is a small c99 library for progress bars in the terminal.

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

# Missing features that I might implement when I feel like it
> Cursor Controls (Although you can use ANSII escape sequences in the bar string)
