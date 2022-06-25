# cbars/cbars.h
CBars is a small c99 library for progress bars in the terminal.

# Why??????
I was bored and wanted an excuse to code C.

# Example
```
char percent[10] = {0};
cbar_t bar = cbar(64, '[', ']', '-', ' ', "Loading Level: ", percent);

cbar_hide_cursor();
while(loading()){
    bar.progress = load();
    sprintf(percent, "%u%", (uint32_t)floor(bar.progress*100.0));

    cbar_display_bar(&bar);
}
cbar_show_cursor();
```

# Result (at 63%)
```
Loading Level: [---------------------------------------                       ] 63%
```

# Missing features that I might implement when I feel like it
> Background colors
> Text effects (Bold, Italic, etc)