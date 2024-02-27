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
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CBAR_FORMAT_CHAR '$'
// F - Fill "$F"
#define CBAR_FILL_FORMAT 'F'
// N - None "$N"
#define CBAR_NONE_FORMAT 'N'
// P - Percent "$P"
#define CBAR_PERCENT_FORMAT 'P'
// E - Effect "$E X $E"
#define CBAR_START_END_EFFECT 'E'

#define cbar(l, s) (cbar_t){.progress = 0.0, .len = l, .bar = s}
#define cbar_display(b, ...) {\
    cbar_t cbar_bar_copy = b; \
    cbar_bar_copy.bar = malloc(snprintf(NULL, 0, b.bar, __VA_ARGS__)+1); \
    sprintf(cbar_bar_copy.bar, b.bar, __VA_ARGS__); \
    cbar_display_bar(&cbar_bar_copy); \
    free(cbar_bar_copy.bar); \
}

typedef struct cbar_t {
    // 0.0 - 1.0
    double progress;
    // Size of the bar in chars
    size_t len;
    // How the bar is displayed 
    char*  bar;
} cbar_t;
typedef enum cbar_effect_t {
    CBAR_EFFECT_RESET               = 0,
    CBAR_EFFECT_BOLD                = 1,
    CBAR_EFFECT_DIM                 = 2,
    CBAR_EFFECT_ITALIC              = 3,
    CBAR_EFFECT_UNDERLINE           = 4,
    CBAR_EFFECT_BLINKING            = 5,
    CBAR_EFFECT_INVERSE             = 7,
    CBAR_EFFECT_HIDDEN              = 8,
    CBAR_EFFECT_STRIKETHROUGH       = 9,

    CBAR_EFFECT_RESET_BOLD_DIM      = 22,
    CBAR_EFFECT_RESET_ITALIC        = 23,
    CBAR_EFFECT_RESET_UNDERLINE     = 24,
    CBAR_EFFECT_RESET_BLINKING      = 25,
    CBAR_EFFECT_RESET_INVERSE       = 27,      
    CBAR_EFFECT_RESET_HIDDEN        = 28,
    CBAR_EFFECT_RESET_STRIKETHROUGH = 29,
    
    CBAR_EFFECT_FG_BLACK            = 30,
    CBAR_EFFECT_FG_RED              = 31,
    CBAR_EFFECT_FG_GREEN            = 32,
    CBAR_EFFECT_FG_YELLOW           = 33,
    CBAR_EFFECT_FG_BLUE             = 34,
    CBAR_EFFECT_FG_MAGENTA          = 35,
    CBAR_EFFECT_FG_CYAN             = 36,
    CBAR_EFFECT_FG_WHITE            = 37,
    CBAR_EFFECT_FG_RESET            = 39,

    CBAR_EFFECT_BG_BLACK            = 40,
    CBAR_EFFECT_BG_RED              = 41,
    CBAR_EFFECT_BG_GREEN            = 42,
    CBAR_EFFECT_BG_YELLOW           = 43,
    CBAR_EFFECT_BG_BLUE             = 44,
    CBAR_EFFECT_BG_MAGENTA          = 45,
    CBAR_EFFECT_BG_CYAN             = 46,
    CBAR_EFFECT_BG_WHITE            = 47,
    CBAR_EFFECT_BG_RESET            = 49,

    CBAR_EFFECT_BRFG_BLACK          = 90,
    CBAR_EFFECT_BRFG_RED            = 91,
    CBAR_EFFECT_BRFG_GREEN          = 92,
    CBAR_EFFECT_BRFG_YELLOW         = 93,
    CBAR_EFFECT_BRFG_BLUE           = 94,
    CBAR_EFFECT_BRFG_MAGENTA        = 95,
    CBAR_EFFECT_BRFG_CYAN           = 96,
    CBAR_EFFECT_BRFG_WHITE          = 97,
    
    CBAR_EFFECT_BRBG_BLACK          = 100,
    CBAR_EFFECT_BRBG_RED            = 101,
    CBAR_EFFECT_BRBG_GREEN          = 102,
    CBAR_EFFECT_BRBG_YELLOW         = 103,
    CBAR_EFFECT_BRBG_BLUE           = 104,
    CBAR_EFFECT_BRBG_MAGENTA        = 105,
    CBAR_EFFECT_BRBG_CYAN           = 106,
    CBAR_EFFECT_BRBG_WHITE          = 107,
} cbar_effect_t;

/*
 * Converts a cbar_t to a string,
 * returns a string allocated with calloc
*/
char* cbar_tostr(const cbar_t* bar);
/* 
 * Converts an E string ($E BOLD;FGBR_GREEN; $E) to ansii escape codes,
 * returns a string allocated with calloc
*/
char* cbar_effect_to_escape_codes(const char* effects);
void cbar_display_bar(const cbar_t* bar);
void cbar_display_bars(const cbar_t* bars, size_t count);

void cbar_hide_cursor();
void cbar_show_cursor();

#ifdef CBARS_IMPL

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

const cbar_effect_t CBARS_EFFECT_LOOKUP[] = {
    CBAR_EFFECT_RESET               ,
    CBAR_EFFECT_BOLD                ,
    CBAR_EFFECT_DIM                 ,
    CBAR_EFFECT_ITALIC              ,
    CBAR_EFFECT_UNDERLINE           ,
    CBAR_EFFECT_BLINKING            ,
    CBAR_EFFECT_INVERSE             ,
    CBAR_EFFECT_HIDDEN              ,
    CBAR_EFFECT_STRIKETHROUGH       ,
    CBAR_EFFECT_RESET_BOLD_DIM      ,
    CBAR_EFFECT_RESET_ITALIC        ,
    CBAR_EFFECT_RESET_UNDERLINE     ,
    CBAR_EFFECT_RESET_BLINKING      ,
    CBAR_EFFECT_RESET_INVERSE       , 
    CBAR_EFFECT_RESET_HIDDEN        ,
    CBAR_EFFECT_RESET_STRIKETHROUGH ,
    CBAR_EFFECT_FG_BLACK            ,
    CBAR_EFFECT_FG_RED              ,
    CBAR_EFFECT_FG_GREEN            ,
    CBAR_EFFECT_FG_YELLOW           ,
    CBAR_EFFECT_FG_BLUE             ,
    CBAR_EFFECT_FG_MAGENTA          ,
    CBAR_EFFECT_FG_CYAN             ,
    CBAR_EFFECT_FG_WHITE            ,
    CBAR_EFFECT_FG_RESET            ,
    CBAR_EFFECT_BG_BLACK            ,
    CBAR_EFFECT_BG_RED              ,
    CBAR_EFFECT_BG_GREEN            ,
    CBAR_EFFECT_BG_YELLOW           ,
    CBAR_EFFECT_BG_BLUE             ,
    CBAR_EFFECT_BG_MAGENTA          ,
    CBAR_EFFECT_BG_CYAN             ,
    CBAR_EFFECT_BG_WHITE            ,
    CBAR_EFFECT_BG_RESET            ,
    CBAR_EFFECT_BRFG_BLACK          ,
    CBAR_EFFECT_BRFG_RED            ,
    CBAR_EFFECT_BRFG_GREEN          ,
    CBAR_EFFECT_BRFG_YELLOW         ,
    CBAR_EFFECT_BRFG_BLUE           ,
    CBAR_EFFECT_BRFG_MAGENTA        ,
    CBAR_EFFECT_BRFG_CYAN           ,
    CBAR_EFFECT_BRFG_WHITE          ,
    CBAR_EFFECT_BRBG_BLACK          ,
    CBAR_EFFECT_BRBG_RED            ,
    CBAR_EFFECT_BRBG_GREEN          ,
    CBAR_EFFECT_BRBG_YELLOW         ,
    CBAR_EFFECT_BRBG_BLUE           ,
    CBAR_EFFECT_BRBG_MAGENTA        ,
    CBAR_EFFECT_BRBG_CYAN           ,
    CBAR_EFFECT_BRBG_WHITE          ,
};

const char* CBARS_STRING_LOOKUP[] = {
    "RESET"               ,
    "BOLD"                ,
    "DIM"                 ,
    "ITALIC"              ,
    "UNDERLINE"           ,
    "BLINKING"            ,
    "INVERSE"             ,
    "HIDDEN"              ,
    "STRIKETHROUGH"       ,
    "RESET_BOLD_DIM"      ,
    "RESET_ITALIC"        ,
    "RESET_UNDERLINE"     ,
    "RESET_BLINKING"      ,
    "RESET_INVERSE"       ,
    "RESET_HIDDEN"        ,
    "RESET_STRIKETHROUGH" ,
    "FG_BLACK"            ,
    "FG_RED"              ,
    "FG_GREEN"            ,
    "FG_YELLOW"           ,
    "FG_BLUE"             ,
    "FG_MAGENTA"          ,
    "FG_CYAN"             ,
    "FG_WHITE"            ,
    "FG_RESET"            ,
    "BG_BLACK"            ,
    "BG_RED"              ,
    "BG_GREEN"            ,
    "BG_YELLOW"           ,
    "BG_BLUE"             ,
    "BG_MAGENTA"          ,
    "BG_CYAN"             ,
    "BG_WHITE"            ,
    "BG_RESET"            ,
    "BRFG_BLACK"          ,
    "BRFG_RED"            ,
    "BRFG_GREEN"          ,
    "BRFG_YELLOW"         ,
    "BRFG_BLUE"           ,
    "BRFG_MAGENTA"        ,
    "BRFG_CYAN"           ,
    "BRFG_WHITE"          ,
    "BRBG_BLACK"          ,
    "BRBG_RED"            ,
    "BRBG_GREEN"          ,
    "BRBG_YELLOW"         ,
    "BRBG_BLUE"           ,
    "BRBG_MAGENTA"        ,
    "BRBG_CYAN"           ,
    "BRBG_WHITE"          ,
};


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
    
    /*if(cbar_streq(str, "RESET")){
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
        data.effect = CBAR_EFFECT_BRBG_WHITE;*/

    size_t lut_size = sizeof(CBARS_EFFECT_LOOKUP)/sizeof(cbar_effect_t);
    for(size_t i = 0; i < lut_size; i++){
        if(cbar_streq(str, CBARS_STRING_LOOKUP[i])) {
            data.effect = CBARS_EFFECT_LOOKUP[i];
            return data;
        }
    }

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
    
    char* processed = calloc(strlen(effects)+13, 1);
    cbar_strcpy_no_space(processed, effects);
    
    char* codes = calloc(cbar_get_effect_to_escape_codes_len(processed)+1, 1);
    char* codes_original = codes;
    const char* current = processed;
    while(*current != '\0'){
        if(*current == ';'){
            size_t size = end-begin;
            char* slice = calloc(size+1, 1);
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


size_t cbar_estimate_formatted_size(const cbar_t* bar){
    size_t len = 0;
    const char* bar_str = bar->bar;
    while(*bar_str != '\0'){
        if(*bar_str == CBAR_FORMAT_CHAR){
            if(*(bar_str + 1) == CBAR_PERCENT_FORMAT) len += 3;
            else if(
                *(bar_str + 1) == CBAR_FILL_FORMAT || *(bar_str + 1) == CBAR_NONE_FORMAT
            ) len += (size_t)ceil((double)bar->len/2.0);
            else if(*(bar_str + 1) == CBAR_START_END_EFFECT) {
                bar_str++;
                while(*bar_str != CBAR_FORMAT_CHAR){
                    if(*bar_str == '\0') return len;
                    if(*bar_str == ';') len += strlen(CBAR_FGRGB_STR);
                    bar_str++;
                }
                if(*(++bar_str) == '\0') break;
            }
        } else len += 1;
        bar_str++;
    }
    return len;
}

char* cbar_tostr(const cbar_t* bar){
    assert(bar != NULL);
    size_t bar_str_len = cbar_estimate_formatted_size(bar);
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

#endif

