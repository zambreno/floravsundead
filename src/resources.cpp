
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

/* Sprite map for GREEN_FONT */
uint16_t green_font_spriteMap[NUM_GREEN_FONT][4] = {
    {  0  , 0 ,  30 , 44},
    {  31 , 0 ,  60 , 44},
    {  61 , 0 ,  90 , 44},
    {  91 , 0 , 120 , 44},
    { 121 , 0 , 150 , 44},
    { 151 , 0 , 180 , 44},
    { 181 , 0 , 210 , 44},
    { 211 , 0 , 240 , 44},
    { 241 , 0 , 270 , 44},
    { 271 , 0 , 310 , 44},
    { 311 , 0 , 341 , 44}
    };

/* Sprite map for RED_FONT */
uint16_t red_font_spriteMap[NUM_RED_FONT][4] = {
    {  0  , 0 ,  26 , 36},
    {  27 , 0 ,  52 , 36},
    {  53 , 0 ,  78 , 36},
    {  79 , 0 , 104 , 36},
    { 105 , 0 , 130 , 36},
    { 131 , 0 , 156 , 36},
    { 157 , 0 , 182 , 36},
    { 183 , 0 , 208 , 36},
    { 209 , 0 , 234 , 36},
    { 235 , 0 , 260 , 36},
    { 361 , 0 , 266 , 36},
    };
