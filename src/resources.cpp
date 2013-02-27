
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
    "sprites/fonts/red.png",
    "sprites/fonts/white.png"
    "sprites/sprites.png"
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
    { 279 , 0 , 309 , 44},
    { 310 , 0 , 340 , 44}
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
    { 260 , 0 , 265 , 36},
    { 266 , 0 , 271 , 36},
    };

/* Sprite map for WHITE_FONT */
uint16_t white_font_spriteMap[NUM_WHITE_FONT][4] = {
   {   0,  0, 30, 54}, {  31,  0, 62, 54}, {  63,  0, 93, 54}, {  94,  0, 121, 54},
   { 122,  0, 154, 54}, { 155,  0, 184, 54}, { 185,  0, 218, 54}, { 219,  0, 248, 54},
   { 249,  0, 262, 54}, { 263,  0, 292, 54}, { 293,  0, 325, 54}, { 326,  0, 357, 54},
   { 358,  0, 389, 54}, { 390,  0, 416, 54}, { 417,  0, 447, 54}, { 448,  0, 476, 54},
   { 477,  0, 508, 54}, { 509,  0, 542, 54}, { 543,  0, 570, 54}, { 571,  0, 604, 54},
   { 605,  0, 633, 54}, { 634,  0, 663, 54}, { 664,  0, 697, 54}, { 698,  0, 724, 54},
   { 725,  0, 748, 54}, { 749,  0, 776, 54}, { 777,  0, 803, 54}, { 804,  0, 828, 54},
   { 829,  0, 850, 54}, { 851,  0, 873, 54}, { 874,  0, 896, 54}, { 897,  0, 925, 54},
   { 926,  0, 951, 54}, { 952,  0, 977, 54}, { 978,  0, 992, 54}, { 993,  0, 1015, 54},
   {1016,  0, 1042, 54}, {1043,  0, 1055, 54}, {1056,  0, 1087, 54}, {1087,  0, 1114, 54},
   {1115,  0, 1140, 54}, {1141,  0, 1164, 54}, {1165,  0, 1188, 54}, {1189,  0, 1214, 54},
   {1215,  0, 1237, 54}, {1239,  0, 1264, 54}, {1265,  0, 1289, 54}, {1290,  0, 1313, 54},
   {1314,  0, 1340, 54}, {1341,  0, 1363, 54}, {1364,  0, 1389, 54}, {1390,  0, 1414, 54},
   {1415,  0, 1439, 54}, {1440,  0, 1453, 54}, {1454,  0, 1480, 54}, {1481,  0, 1506, 54},
   {1507,  0, 1531, 54}, {1532,  0, 1558, 54}, {1559,  0, 1584, 54}, {1585,  0, 1611, 54},
   {1612,  0, 1636, 54}, {1637,  0, 1661, 54}
    };

uint16_t zombie_object_spriteMap[NUM_ZOMBIES][4] = {




};

