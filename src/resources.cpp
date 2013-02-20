
#include "floravsundead.hpp"

/* Sound effect files */
std::string sfxFiles[NUM_SFX] = {
    "audio/sfx/groan.ogg",
    "audio/sfx/groan2.ogg",
    "audio/sfx/groan3.ogg",
    "audio/sfx/groan4.ogg",
    "audio/sfx/groan5.ogg",
    "audio/sfx/groan6.ogg"
    };

/* Music files */
std::string musicFiles[NUM_MUSIC] = {
    "audio/music/intro1.ogg",
    "audio/music/main1.ogg",
    "audio/music/main2.ogg",
    "audio/music/main3.ogg",
    "audio/music/main4.ogg",
    "audio/music/main5.ogg"
    };

/* Texture files */
std::string texFiles[NUM_TEXTURES] = {
    "sprites/background.png",
    "sprites/court.png",
    "sprites/scoreboard_bottom.png",
    "sprites/scoreboard_top.png",
    "sprites/fonts/green.png",
    "sprites/fonts/red.png"
    };

/* Sprite map for TEX_BACKGROUND */
uint16_t tex_background_spriteMap[NUM_TEX_BACKGROUND][4] = {
    {  0,  0,  1384, 1161}
    };

/* Sprite map for TEX_COURT */
uint16_t tex_court_spriteMap[NUM_TEX_COURT][4] = {
    {  0,  0,  499, 939}
    };

/* Sprite map for TEX_SCOREBOARD_BOTTOM */
uint16_t tex_scoreboard_bottom_spriteMap[NUM_TEX_SCOREBOARD_BOTTOM][4] = {
    {  0,  0,  493, 182}
    };

/* Sprite map for TEX_SCOREBOARD_TOP */
uint16_t tex_scoreboard_top_spriteMap[NUM_TEX_SCOREBOARD_TOP][4] = {
    {  0,  0,  493, 182}
    };

/* Sprite map for GREEN_FONT */
uint16_t green_font_spriteMap[NUM_GREEN_FONT][4] = {
    {  0  , 0 ,  30 , 44},
    {  31 , 0 ,  61 , 44},
    {  62 , 0 ,  92 , 44},
    {  93 , 0 , 123 , 44},
    { 124 , 0 , 154 , 44},
    { 155 , 0 , 185 , 44},
    { 186 , 0 , 216 , 44},
    { 217 , 0 , 247 , 44},
    { 248 , 0 , 278 , 44},
    { 279 , 0 , 319 , 44},
    { 320 , 0 , 350 , 44}
    };

/* Sprite map for RED_FONT */
uint16_t red_font_spriteMap[NUM_RED_FONT][4] = {
    {  0  , 0 ,  25 , 36},
    {  26 , 0 ,  51 , 36},
    {  52 , 0 ,  77 , 36},
    {  78 , 0 , 103 , 36},
    { 104 , 0 , 129 , 36},
    { 130 , 0 , 155 , 36},
    { 156 , 0 , 181 , 36},
    { 182 , 0 , 207 , 36},
    { 208 , 0 , 233 , 36},
    { 234 , 0 , 259 , 36},
    { 360 , 0 , 265 , 36},
    };
