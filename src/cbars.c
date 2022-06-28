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
#include <assert.h>
#include <math.h>

#define cbar_streq(a, b) strcmp(a, b) == 0

typedef union cbar_effect_data_t {
    cbar_effect_t effect;
    uint8_t color_256;
    uint8_t color_rgb[3];
} cbar_effect_data_t;
typedef enum cbar_effect_type_t {
    CBAR_EFFECT_TYPE_INVALID,
    CBAR_EFFECT_TYPE_EFFECT,
    CBAR_EFFECT_TYPE_COLOR_256,
    CBAR_EFFECT_TYPE_COLOR_RGB,
    CBAR_EFFECT_TYPE_COLOR_BG256,
    CBAR_EFFECT_TYPE_COLOR_BGRGB,
} cbar_effect_type_t;

bool cbar_is_digit(const char* str){
    while(*str != '\0'){
        if(!(
            *str >= '0' && *str <= '9'
        )) return false;
        str++;
    }
    return true;
}
void cbar_strcpy_no_space(char* dest, const char* src){
    while(*src != '\0'){
        if(*src != ' '){
            *dest = *src;
            dest++;
        }
        src++;
    }
}

cbar_effect_data_t cbar_string_to_effect(const char* str, cbar_effect_type_t* type){
    cbar_effect_data_t data = {.effect = CBAR_EFFECT_RESET};
    *type = CBAR_EFFECT_TYPE_EFFECT;
    
    if(cbar_streq(str, "RESET")){
        data.effect = CBAR_EFFECT_RESET;
    } else if(cbar_streq(str, "BOLD")){
        data.effect = CBAR_EFFECT_BOLD;
    } else if(cbar_streq(str, "DIM")){
        data.effect = CBAR_EFFECT_DIM;
    } else if(cbar_streq(str, "ITALIC")){
        data.effect = CBAR_EFFECT_ITALIC;
    } else if(cbar_streq(str, "UNDERLINE")){
        data.effect = CBAR_EFFECT_UNDERLINE;
    } else if(cbar_streq(str, "BLINKING")){
        data.effect = CBAR_EFFECT_BLINKING;
    } else if(cbar_streq(str, "INVERSE")){
        data.effect = CBAR_EFFECT_INVERSE;
    } else if(cbar_streq(str, "HIDDEN")){
        data.effect = CBAR_EFFECT_HIDDEN;
    } else if(cbar_streq(str, "STRIKETHROUGH")){
        data.effect = CBAR_EFFECT_STRIKETHROUGH;
    } else if(cbar_streq(str, "RESET_BOLD_DIM")){
        data.effect = CBAR_EFFECT_RESET_BOLD_DIM;
    } else if(cbar_streq(str, "RESET_ITALIC")){
        data.effect = CBAR_EFFECT_RESET_ITALIC;
    } else if(cbar_streq(str, "RESET_UNDERLINE")){
        data.effect = CBAR_EFFECT_RESET_UNDERLINE;
    } else if(cbar_streq(str, "RESET_BLINKING")){
        data.effect = CBAR_EFFECT_RESET_BLINKING;
    } else if(cbar_streq(str, "RESET_INVERSE")){
        data.effect = CBAR_EFFECT_RESET_INVERSE;
    } else if(cbar_streq(str, "RESET_HIDDEN")){
        data.effect = CBAR_EFFECT_RESET_HIDDEN;
    } else if(cbar_streq(str, "RESET_STRIKETHROUGH")){
        data.effect = CBAR_EFFECT_RESET_STRIKETHROUGH;
    } else if(cbar_streq(str, "FG_BLACK")){
        data.effect = CBAR_EFFECT_FG_BLACK;
    } else if(cbar_streq(str, "FG_RED")){
        data.effect = CBAR_EFFECT_FG_RED;
    } else if(cbar_streq(str, "FG_GREEN")){
        data.effect = CBAR_EFFECT_FG_GREEN;
    } else if(cbar_streq(str, "FG_YELLOW")){
        data.effect = CBAR_EFFECT_FG_YELLOW;
    } else if(cbar_streq(str, "FG_BLUE")){
        data.effect = CBAR_EFFECT_FG_BLUE;
    } else if(cbar_streq(str, "FG_MAGENTA")){
        data.effect = CBAR_EFFECT_FG_MAGENTA;
    } else if(cbar_streq(str, "FG_CYAN")){
        data.effect = CBAR_EFFECT_FG_CYAN;
    } else if(cbar_streq(str, "FG_WHITE")){
        data.effect = CBAR_EFFECT_FG_WHITE;
    } else if(cbar_streq(str, "FG_RESET")){
        data.effect = CBAR_EFFECT_FG_RESET;
    } else if(cbar_streq(str, "BG_BLACK")){
        data.effect = CBAR_EFFECT_BG_BLACK;
    } else if(cbar_streq(str, "BG_RED")){
        data.effect = CBAR_EFFECT_BG_RED;
    } else if(cbar_streq(str, "BG_GREEN")){
        data.effect = CBAR_EFFECT_BG_GREEN;
    } else if(cbar_streq(str, "BG_YELLOW")){
        data.effect = CBAR_EFFECT_BG_YELLOW;
    } else if(cbar_streq(str, "BG_BLUE")){
        data.effect = CBAR_EFFECT_BG_BLUE;
    } else if(cbar_streq(str, "BG_MAGENTA")){
        data.effect = CBAR_EFFECT_BG_MAGENTA;
    } else if(cbar_streq(str, "BG_CYAN")){
        data.effect = CBAR_EFFECT_BG_CYAN;
    } else if(cbar_streq(str, "BG_WHITE")){
        data.effect = CBAR_EFFECT_BG_WHITE;
    } else if(cbar_streq(str, "BG_RESET")){
        data.effect = CBAR_EFFECT_BG_RESET;
    } else if(cbar_streq(str, "BRFG_BLACK")){
        data.effect = CBAR_EFFECT_BRFG_BLACK;
    } else if(cbar_streq(str, "BRFG_RED")){
        data.effect = CBAR_EFFECT_BRFG_RED;
    } else if(cbar_streq(str, "BRFG_GREEN")){
        data.effect = CBAR_EFFECT_BRFG_GREEN;
    } else if(cbar_streq(str, "BRFG_YELLOW")){
        data.effect = CBAR_EFFECT_BRFG_YELLOW;
    } else if(cbar_streq(str, "BRFG_BLUE")){
        data.effect = CBAR_EFFECT_BRFG_BLUE;
    } else if(cbar_streq(str, "BRFG_MAGENTA")){
        data.effect = CBAR_EFFECT_BRFG_MAGENTA;
    } else if(cbar_streq(str, "BRFG_CYAN")){
        data.effect = CBAR_EFFECT_BRFG_CYAN;
    } else if(cbar_streq(str, "BRFG_WHITE")){
        data.effect = CBAR_EFFECT_BRFG_WHITE;
    } else if(cbar_streq(str, "BRBG_BLACK")){
        data.effect = CBAR_EFFECT_BRBG_BLACK;
    } else if(cbar_streq(str, "BRBG_RED")){
        data.effect = CBAR_EFFECT_BRBG_RED;
    } else if(cbar_streq(str, "BRBG_GREEN")){
        data.effect = CBAR_EFFECT_BRBG_GREEN;
    } else if(cbar_streq(str, "BRBG_YELLOW")){
        data.effect = CBAR_EFFECT_BRBG_YELLOW;
    } else if(cbar_streq(str, "BRBG_BLUE")){
        data.effect = CBAR_EFFECT_BRBG_BLUE;
    } else if(cbar_streq(str, "BRBG_MAGENTA")){
        data.effect = CBAR_EFFECT_BRBG_MAGENTA;
    } else if(cbar_streq(str, "BRBG_CYAN")){
        data.effect = CBAR_EFFECT_BRBG_CYAN;
    } else if(cbar_streq(str, "BRBG_WHITE")){
        data.effect = CBAR_EFFECT_BRBG_WHITE;
    } else {
        if(strlen(str) < 3){
            *type = CBAR_EFFECT_TYPE_INVALID;
            return data;
        }
        if(strncmp(str, "RGB", 3) == 0 || strncmp(str, "BGRGB", 5) == 0){
            char* processed = calloc(strlen(str)+1, 1);
            cbar_strcpy_no_space(processed, str);
            size_t offset =0;
            if(strncmp(str, "BGRGB", 5) == 0){
                *type = CBAR_EFFECT_TYPE_COLOR_BGRGB;
                processed += 2;
                offset = 2;
            } else {
                *type = CBAR_EFFECT_TYPE_COLOR_RGB;
            }
            size_t processed_len = strlen(processed);
            if(processed_len < 10  || processed_len > 16                ||
               processed[3] != '(' || processed[processed_len-1] != ')'
            ){
                // min/max number of chars
                free(processed-offset);
                *type = CBAR_EFFECT_TYPE_INVALID;
                return data;
            }
            
            char sr[14] = {0};
            char sg[14] = {0};
            char sb[14] = {0};
            char* slices[] = {sr, sg, sb};
            char* current = &processed[4];
            char** slice_current = slices;
            while(*current != ')'){
                if(*current == '\0'){
                    free(processed-offset);
                    *type = CBAR_EFFECT_TYPE_INVALID;
                    return data;
                }
                if(*current == ','){
                    slice_current++;
                } else {
                    **slice_current = *current;
                    (*slice_current)++;
                }
                current++;
            }

            if(!cbar_is_digit(sr) || !cbar_is_digit(sg) || !cbar_is_digit(sb)){
                free(processed-offset);
                *type = CBAR_EFFECT_TYPE_INVALID;
                return data;
            }

            int r = atoi(sr);
            int g = atoi(sg);
            int b = atoi(sb);
            if(r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255){
                free(processed-offset);
                *type = CBAR_EFFECT_TYPE_INVALID;
                return data;
            }

            data = (cbar_effect_data_t){
                .color_rgb = {(uint8_t)r, (uint8_t)g, (uint8_t)b}
            };

            free(processed-offset);
        } else if(strncmp(str, "256", 3) == 0 || strncmp(str, "BG256", 5) == 0){
            char* processed = calloc(strlen(str)+1, 1);
            cbar_strcpy_no_space(processed, str);
            size_t offset = 0;
            if(strncmp(str, "BG256", 5) == 0){
                *type = CBAR_EFFECT_TYPE_COLOR_BG256;
                processed += 2;
                offset = 2;
            }
            else {
                *type = CBAR_EFFECT_TYPE_COLOR_256;
            }
            size_t processed_len = strlen(processed);
            if(processed_len < 6   || processed_len > 8                 ||
               processed[3] != '(' || processed[processed_len-1] != ')'
            ){
                // min/max number of chars
                free(processed-offset);
                *type = CBAR_EFFECT_TYPE_INVALID;
                return data;
            }

            char slice[4] = {0};
            char* current = &processed[4];
            char* slice_current = slice;
            while(*current != ')'){
                if(*current == '\0'){
                    free(processed-offset);
                    *type = CBAR_EFFECT_TYPE_INVALID;
                    return data;
                }
                *slice_current = *current;
                slice_current++;
                current++;
            }

            if(!cbar_is_digit(slice)){
                free(processed-offset);
                *type = CBAR_EFFECT_TYPE_INVALID;
                return data;
            }

            int n = atoi(slice);
            if(n < 0 || n > 255){
                free(processed-offset);
                *type = CBAR_EFFECT_TYPE_INVALID;
                return data;
            }

            data.color_256 = (uint8_t)n;

            free(processed-offset);
        } else {
            *type = CBAR_EFFECT_TYPE_INVALID;
        }
    }

    return data;
}

size_t cbar_clamp(size_t v, size_t min, size_t max){
    if(v < min) return min;
    if(v > max) return max;
    return v;
}

/*
void cbar_set_effect(cbar_effect_t effect){
    printf("\033[%um", (uint32_t)effect);
}
void cbar_set_color_256(uint8_t code){
    printf("\033[38;5;%um", code);
}
void cbar_set_bg_color_256(uint8_t code){
    printf("\033[48;5;%um", code);
}
void cbar_set_color_rgb(uint8_t r, uint8_t g, uint8_t b){
    printf("\033[38;2;%u;%u;%um", r, g, b);
}
void cbar_set_bg_color_rgb(uint8_t r, uint8_t g, uint8_t b){
    printf("\033[38;2;%u;%u;%um", r, g, b);
}
void cbar_reset_effects(){
    printf("\033[0m");
}
*/

#define CBAR_EFFECT_STR "\033[%um"
#define CBAR_FG256_STR  "\033[38;5;%um"
#define CBAR_BG256_STR  "\033[48;5;%um"
#define CBAR_FGRGB_STR  "\033[38;2;%u;%u;%um"
#define CBAR_BGRGB_STR  "\033[48;2;%u;%u;%um"
#define CBAR_RESET_STR  "\033[0m"

bool cbar_append_fill(char** current, const char** bar_str, size_t amount_to_fill){
    (*bar_str)++;
    while(**bar_str != CBAR_FORMAT_CHAR){
        if(**bar_str == '\0'){
            return false;
        }
        if(**bar_str == 39){
            (*bar_str)++;
            char* str = calloc(amount_to_fill+1, 1);
            memset(str, **bar_str, amount_to_fill);
            strcat(*current, str);
            *current += amount_to_fill;
            free(str);
            (*bar_str)++;
            if(**bar_str != 39) return false;
        }
        (*bar_str)++;
    }
    *bar_str += 2;
    return true;
}

size_t cbar_get_effect_to_escape_codes_len(const char* effects){
    size_t count = 0;
    const char* current = effects;
    while(*current != '\0'){
        if(*current == ';'){
            count += strlen(CBAR_FGRGB_STR) + 3*3;
        }
        current++;
    }
    return count;
}
char* cbar_effect_to_escape_codes(const char* effects){
    size_t begin = 0;
    size_t end = 0;
    
    char* processed = calloc(strlen(effects)+1, 1);
    cbar_strcpy_no_space(processed, effects);
    
    char* codes = calloc(cbar_get_effect_to_escape_codes_len(processed)+1, 1);
    char* codes_original = codes;
    const char* current = processed;
    while(*current != '\0'){
        if(*current == ';'){
            size_t size = end-begin;
            char* slice = calloc(size, 1);
            strncpy(slice, processed+begin, size);
             
            cbar_effect_type_t type = CBAR_EFFECT_TYPE_INVALID;
            cbar_effect_data_t data = cbar_string_to_effect(slice, &type);
            switch(type){
                case CBAR_EFFECT_TYPE_EFFECT:
                    codes += (size_t)sprintf(codes, CBAR_EFFECT_STR, (uint8_t)data.effect);
                    break;
                case CBAR_EFFECT_TYPE_COLOR_256:
                    codes += (size_t)sprintf(codes, CBAR_FG256_STR, data.color_256);
                    break;
                case CBAR_EFFECT_TYPE_COLOR_BG256:
                    codes += (size_t)sprintf(codes, CBAR_BG256_STR, data.color_256);
                    break;
                case CBAR_EFFECT_TYPE_COLOR_RGB:
                    codes += (size_t)sprintf(
                        codes,
                        CBAR_FGRGB_STR,
                        data.color_rgb[0],
                        data.color_rgb[1],
                        data.color_rgb[2]
                    );
                    break;
                case CBAR_EFFECT_TYPE_COLOR_BGRGB:
                    codes += (size_t)sprintf(
                        codes,
                        CBAR_BGRGB_STR,
                        data.color_rgb[0],
                        data.color_rgb[1],
                        data.color_rgb[2]
                    );
                    break;
                default:
                    return NULL;
            }
            free(slice);
            begin = end+1;
        }
        end++;
        current++;
    }
    free(processed);

    return codes_original;
}

char* cbar_tostr(const cbar_t* bar){
    assert(bar != NULL);
    // Very confident everything will fit, but may as well be sure
    const size_t UNCERTAINTY = 8;
    size_t bar_str_len = strlen(bar->bar) + bar->len + UNCERTAINTY;
    char* str = calloc(bar_str_len+1, 1);
    
    size_t amount_filled = (size_t)floor(bar->progress*(double)bar->len);
    size_t percent = (size_t)floor(bar->progress*100.0);
    amount_filled = cbar_clamp(amount_filled, 0, bar->len);
    percent = cbar_clamp(percent, 0, 100);

    const char* bar_str = bar->bar;
    char* current = str;
    while(*bar_str != '\0'){
        if(*bar_str == CBAR_FORMAT_CHAR){
            char next = *(bar_str + 1);
            switch(next){
                case CBAR_FILL_FORMAT: {
                    if(!cbar_append_fill(&current, &bar_str, amount_filled)){
                        fprintf(stderr, "CBARS Error: Invalid syntax.\n");
                        free(str);
                        return NULL;
                    }
                    break;
                }
                case CBAR_NONE_FORMAT: {
                    if(!cbar_append_fill(&current, &bar_str, bar->len-amount_filled)){
                        fprintf(stderr, "CBARS Error: Invalid syntax.\n");
                        free(str);
                        return NULL;
                    }
                    break;
                }
                case CBAR_PERCENT_FORMAT: {
                    char spercent[4] = {0};
                    sprintf(spercent, "%lu", percent);
                    strcat(current, spercent);
                    current += strlen(spercent);
                    bar_str += 2;
                    break;
                }
                case CBAR_START_END_EFFECT: {
                    bar_str += 2;
                    size_t sz = 0;
                    const char* bar_str2 = bar_str;
                    while(*bar_str2 != CBAR_FORMAT_CHAR){
                        sz++;
                        bar_str2++;
                    }

                    char* effect_str = calloc(sz+1, 1);
                    strncpy(effect_str, bar_str, sz);
                    char* codes = cbar_effect_to_escape_codes(effect_str);
                    if(!codes){
                        fprintf(stderr, "CBARS Error: Failed parse $E string.\n");
                        free(str);
                        free(effect_str);
                        return NULL;
                    }
                    free(effect_str);
                    strcat(current, codes);
                    current += strlen(codes);
                    free(codes);
                    bar_str += sz+2;
                    break;
                }
                default:
                    *current = *bar_str;
                    bar_str++;
                    break;
            }
        } else {
            *current = *bar_str;
            current++;
            bar_str++;
        }
    }

    return str;
}

void cbar_display_bar(const cbar_t* bar){
    assert(bar != NULL);
    assert(bar->bar != NULL);
   
    char* bar_str = cbar_tostr(bar);
    if(!bar_str){
        return;
    }

    printf("\r%s", bar_str);
    fflush(stdout);
    free(bar_str);
}
void cbar_display_bars(const cbar_t* bars, size_t count){
    for(size_t i = 0; i < count; i++){
        char* bar_str = cbar_tostr(&bars[i]);
        if(!bar_str) continue;
        printf("%s\n", bar_str);
        free(bar_str);
    }
    printf("\033[%luF", count);
}

void cbar_hide_cursor(){
    printf("\033[?25l");
}
void cbar_show_cursor(){
    printf("\033[?25h");
}

