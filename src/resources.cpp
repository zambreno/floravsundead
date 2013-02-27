
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
   { 611,  653,  62,  67}, { 679,  655,  62,  67}, { 733,  585,  62,  67}, { 744,  658,  59,  57}, { 739,  716,  59,  57},
   { 676,  722,  59,  57}, { 620,  727,  53,  48}, { 577,  760,  29,  16}, { 353,  720,  64,  59}, { 274,  656,  30,  26},
   { 159,  645,  26,  53}, { 296,  682,  11,  18}, { 273,  715,  70,  50}, { 271,  684,  21,  25}, { 275,  767,  15,  23},
   { 291,  769,  15,  22}, { 313,  772,  35,  19}, { 548,  754,  26,  40}, { 520,  751,  22,  47}, { 485,  771,  27,  26},
   { 461,  769,  17,  27}, { 430,  768,  23,  30}, {   0,  808,  37,  19}, {  43,  804,  26,  42}, {  73,  810,  23,  19},
   {   0,  831,  19,  38}, { 188,  801,  82,  79}, { 275,  802,  98,  32}, { 362,  802,  98,  32}, { 451,  806,  98,  32},
   { 272,  837,  98,  32}, { 371,  840,  98,  32}, { 466,  842,  98,  32}, { 109,  817,  25,  11}, {  19,  852,  77,  47},
   {  98,  845,  77,  47}, { 553,  796,  77,  47}, { 571,  831,  57,  57}, { 639,  785,  22,  21}, { 638,  812,  28,  34},
   { 235,  857,  31,  24}, { 677,  784,  40,  26}, { 679,  814,  31,  33}, { 678,  856,  33,  25}, { 723,  789,  23,  15},
   { 718,  814,  24,  21}, { 718,  847,  36,  35}, { 754,  778,  42,  43}, { 757,  827,  42,  43}, {   1,  898,  37,  19},
   {  46,  896,  32,  20}, {  86,  891,  37,  29}, { 136,  887,  48,  47}, { 260,  884, 106,  23}, { 370,  882, 129, 188},
   { 494,  894, 129, 188}, { 606,  897, 129, 188}, { 249,  919, 124, 188}, { 128,  938, 129, 188}, { 111,  937,  20,  19},
   {  43,  923,  62,  67}, {   3,  976,  62,  67}, {   3,  924,  19,  25}, { 757,  871,  42,  92}, {  85,  998,  53,  52},
   {   4, 1049,  66,  73}, { 732,  977,  66,  73}, { 732, 1051,  66,  73}, { 214,  897,  32,  29}, {  95, 1061,  30,  10},
   {  92, 1083,  32,  29}, { 721,  858,  30,  10}, { 113,  967,  12,  16}, {  10,  654,  17,  22}, {   0, 1123,  38,  26},
   {  43, 1122,  16,  26}, {  67, 1117,  34,  34}, { 674, 1093,  50,  24}, { 643, 1088,  24,  30}, { 602, 1091,  35,  29},
   { 566, 1095,  31,  21}, { 102, 1119,  11,  18}, { 512, 1096,  47,  21}, { 729,  910,  21,  26}, { 479, 1082,  29,  41},
   { 790,    1, 133,  84}, { 803,   88, 133,  84}, { 802,  158, 133,  84}, { 416, 1073,  56,  22}, {   0, 1153,  64,  31},
   { 803,  223,  84, 125}, { 813,  352,  74,  51}, { 800,  407,  52,  61}, { 794,  492,  80,  64}, { 872,  412,  38,  77},
   {  81,  481,  62,  61}, { 875,  495,  40,  67}, { 812,  555,  38,  49}, { 868,  566,  51,  55}, { 811,  630, 102,  75},
   { 809,  716, 153,  51}, { 804,  775,  40,  45}, { 850,  772,  51,  65}, { 911,  769,  46,  47}, { 802,  843,  62,  48},
   { 869,  840,  76,  70}, { 806,  894,  57,  51}, { 879,  908,  54,  28}, { 935,    4,  53,  48}, { 939,   57,  53,  76},
   { 937,  140,  61,  48}, { 940,  199,  61,  48}, { 939,  254,  61,  48}, { 939,  307,  61,  48}, { 946,  360,  53,  48},
   { 996,    1,  20,  23}, { 996,   28,  19,  24}, { 992,   53,  45,  26}, {1002,   84,  11,  21}, {1021,   83,  15,  25},
   { 999,  113,  27,  17}, {1005,  137,  32,  36}, {1027,    2,  15,  26}, {1050,    4,  32,  15}, {1032,   30,  50,  22},
   {1038,   54,  50,  22}, {1040,   79,  50,  22}, {1045,  108,  20,  21}, {1050,  139,  25,  27}, {1007,  178,  24,  30},
   {1049,  178,  19,  28}, {1006,  211,  43,  50}, {1059,  215,  17,  35}, {1013,  270,  17,  35}, {1045,  265,  42,  21},
   {1013,  311,  24,  30}, {1044,  296,  21,  39}, { 812,  950, 108, 136}, { 924,  946,  86,  85}, { 928, 1033,  31,  30},
   { 112, 1139,  33,  64}, { 162, 1131,  47,  67}, { 222,  1130, 57,  23}, { 287, 1112,  52,  51}, { 353, 1116,  42,  43},
   { 223, 1163,  31,  31}, { 267, 1167,  31,  32}, { 413,  1103, 32,  60}, { 454, 1130,  48,  65}, { 518, 1129,  48,  65},
   { 581, 1126,  66,  30}, { 661, 1133,  56,  58}, { 730,  1135, 57,  60}, { 807, 1090,  26,  50},
    };



};

