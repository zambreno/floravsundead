
#include "floravsundead.hpp"

/* Grid pixel heights and widths */
const float top_gridHeights[NUM_ROWS] =    { 456.0,  357.0,  253.0,  163.0,   66.0};
const float bottom_gridHeights[NUM_ROWS] = {-480.0, -383.0, -293.0, -189.0,  -90.0};
const float left_gridWidths[NUM_COLS] = {-172.0, -261.0, -344.0, -430.0, -513.0, -586.0, -672.0, -748.0, -831.0, -915.0, -991.0, -1120.0};
const float right_gridWidths[NUM_COLS] = {172.0,  261.0,  344.0,  430.0,  513.0,  586.0,  672.0,  748.0,  831.0,  915.0, 991.0, 1120.0};


/* Command spellings */
std::string cmdNames[NUM_CMD_TYPES][NUM_CMD_SPELLINGS] = {
    {"select", "buy", "use"},
    {"place", "move", "shift"},
    {"fire", "bite", "shoot"},
    {"goto", "jump", "loop"}
    };


/* Predicate spellings */
std::string predNames[NUM_PRED_TYPES][NUM_PRED_SPELLINGS] = {
    {"always", "true"},
    {"never", "false"},
    {"ready", "free"},
    {"alive", "active"},
    {"dead", "inactive"},
    {"damage", "damaged"},
    {"eaten", "eat"},
    {"hit", "contact"},
    {"score", "scored"},
    {"winning", "leading"},
    {"losing", "trailing"},
    {"empty", "free"}
    };



/* Sound effect files */
std::string sfxFiles[NUM_SFX] = {
    "audio/sfx/winmusic.ogg",
    "audio/sfx/gulp.ogg",
    "audio/sfx/yuck.ogg",
    "audio/sfx/yuck2.ogg",
    "audio/sfx/groan.ogg",
    "audio/sfx/groan2.ogg",
    "audio/sfx/groan3.ogg",
    "audio/sfx/groan4.ogg",
    "audio/sfx/groan5.ogg",
    "audio/sfx/groan6.ogg",
    "audio/sfx/sukhbir.ogg",
    "audio/sfx/sukhbir2.ogg",
    "audio/sfx/sukhbir3.ogg",
    "audio/sfx/sukhbir4.ogg",
    "audio/sfx/sukhbir5.ogg",
    "audio/sfx/sukhbir6.ogg",
    "audio/sfx/siren.ogg",
    "audio/sfx/shovel.ogg",
    "audio/sfx/shieldhit.ogg",
    "audio/sfx/shieldhit2.ogg",
    "audio/sfx/portal.ogg",
    "audio/sfx/polevault.ogg",
    "audio/sfx/plant.ogg",
    "audio/sfx/plant2.ogg",
    "audio/sfx/limbs_pop.ogg",
    "audio/sfx/hugewave.ogg",
    "audio/sfx/finalwave.ogg",
    "audio/sfx/dancer.ogg",
    "audio/sfx/chomp.ogg",
    "audio/sfx/chomp2.ogg",
    "audio/sfx/chompsoft.ogg",
    "audio/sfx/bigchomp.ogg",
    "audio/sfx/awooga.ogg",
    "audio/sfx/splat.ogg",
    "audio/sfx/splat2.ogg",
    "audio/sfx/splat3.ogg",
    "audio/sfx/tap.ogg",
    "audio/sfx/tap2.ogg",
    "audio/sfx/frozen.ogg",
    "audio/sfx/newspaper_rarrgh.ogg",
    "audio/sfx/newspaper_rarrgh2.ogg",
    "audio/sfx/newspaper_rip.ogg",
    "audio/sfx/throw.ogg",
    "audio/sfx/throw2.ogg",
    "audio/sfx/plastichit.ogg",
    "audio/sfx/plastichit2.ogg"
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
    "sprites/fonts/white.png",
    "sprites/zombies.png",
    "sprites/plants.png",
    "sprites/fence.png"
    };

/* Sprite map for TEX_BACKGROUND */
uint16_t tex_background_spriteMap[NUM_TEX_BACKGROUND][4] = {
    {  0,  0,  1384, 1161}
    };

/* Sprite map for TEX_COURT */
uint16_t tex_court_spriteMap[NUM_TEX_COURT][4] = {
    {  0,  0,  499, 939}
    };

/* Sprite map for TEX_FENCE */
uint16_t tex_fence_spriteMap[NUM_TEX_FENCE][4] = {
    {  0,  0,  778, 150}
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

uint16_t zombie_object_spriteMap[NUM_ZOMBIE_SPRITES][4] = {

  {611,653,673,720}  , {679,655,741,722}     , {733,585,795,652}     , {744,658,803,715}  , {739,716,798,773}     , //1
  {676,722,735,779}  , {620,727,673,775}     , {577,760,606,776}     , {353,720,417,779}  , {274,656,304,682}     , //2
  {159,645,185,698}  , {296,682,307,700}     , {273,715,343,765}     , {271,684,292,709}  , {275,767,290,790}     , //3
  {291,769,306,791}  , {313,772,348,791}     , {548,754,574,794}     , {520,751,542,798}  , {485,771,512,797}     , //4
  {461,769,478,796}  , {430,768,453,798}     , {0,808,37,827}        , {43,804,69,846}    , {73,810,96,829}       , //5
  {0,831,19,869}     , {188,801,270,880}     , {275,802,373,834}     , {362,802,460,834}  , {451,806,549,838}     , //6
  {272,837,370,869}  , {371,840,469,872}     , {466,842,564,874}     , {109,817,134,828}  , {19,852,96,899}       , //7
  {98,845,175,892}   , {553,796,630,843}     , {571,831,628,888}     , {639,785,661,806}  , {638,812,666,846}     , //8
  {235,857,266,881}  , {677,784,717,810}     , {679,814,710,847}     , {678,856,711,881}  , {723,789,746,804}     , //9
  {718,814,742,835}  , {718,847,754,882}     , {754,778,796,821}     , {757,827,799,870}  , {1,898,38,917}        , //10
  {46,896,78,916}    , {86,891,123,920}      , {136,887,184,934}     , {260,884,366,907}  , {370,882,499,1070}    , //11
  {494,894,623,1082} , {606,897,735,1085}    , {249,919,373,1107}    , {128,938,257,1126} , {111,937,131,956}     , //12
  {43,923,105,990}   , {3,976,65,1043}       , {3,924,22,949}        , {757,871,799,963}  , {85,998,138,1050}     , //13
  {4,1049,70,1122}   , {732,977,798,1050}    , {732,1051,798,1124}   , {214,897,246,926}  , {95,1061,125,1071}    , //14
  {92,1083,124,1112} , {721,888,751,898}     , {113,967,125,983}     , {10,654,27,676}    , {0,1123,38,1149}      , //15
  {43,1122,59,1148}  , {67,1117,101,1151}    , {674,1093,724,1117}   , {643,1088,667,1118}, {602,1091,637,1120}   , //16
  {566,1095,597,1116}, {102,1119,113,1137}   , {512,1096,559,1117}   , {729,910,750,936}  , {479,1082,508,1123}   , //17
  {790,1,923,85}     , {803,88,875,150}      , {859,178,935,219}     , {416,1073,472,1095}, {0,1153,64,1184}      , //18
  {803,223,887,348}  , {813,352,887,403}     , {800,407,852,468}     , {794,492,874,556}  , {872,412,910,489}     , //19
  {81,481,143,542}   , {875,495,915,562}     , {812,555,850,604}     , {868,566,919,621}  , {811,630,913,705}     , //20
  {809,716,962,767}  , {804,775,844,820}     , {850,772,901,837}     , {911,769,957,816}  , {802,843,864,891}     , //21
  {869,840,945,910}  , {806,894,863,945}     , {879,908,933,936}     , {935,4,988,52}     , {939,57,992,133}      , //22
  {937,140,998,188}  , {940,199,1001,247}    , {939,254,1000,302}    , {939,307,1000,355} , {946,360,999,408}     , //23
  {996,1,1016,24}    , {996,28,1015,52}      , {992,53,1037,79}      , {1002,84,1013,105} , {1021,83,1036,108}    , //24
  {999,113,1026,130} , {1005,137,1037,173}   , {1027,2,1042,28}      , {1050,4,1082,19}   , {1032,30,1082,52}     , //25
  {1038,54,1088,76}  , {1040,79,1090,101}    , {1045,108,1065,129}   , {1050,139,1075,166}, {1007,178,1031,208}   , //26
  {1049,178,1068,206}, {1006,211,1049,261}   , {1059,215,1076,250}   , {1013,270,1030,305}, {1045,265,1087,286}   , //27
  {1013,311,1037,341}, {1044,296,1065,335}   , {812,950,920,1086}    , {924,946,1010,1031}, {928,1033,959,1063}   , //28
  {112,1139,145,1203}, {162,1131,209,1198}   , {222,1130,279,1153}   , {287,1112,339,1163}, {353,1116,395,1159}   , //29
  {223,1163,254,1194}, {267,1167,298,1199}   , {413,1103,445,1163}   , {454,1130,502,1195}, {518,1129,566,1194}   , //30
  {581,1126,647,1156}, {661,1133,717,1191}   , {724,1134,793,1194}   , {807,1090,833,1140}, {843,1092,900,1159}   , //31
  {1015,352,1025,369}, {1040,347,1057,366}   , {1051,342,1068,372}   , {411,455,482,501}  , {1198,1196,1199,1197} , //32
  {927,1108,1035,1111},{960, 424, 984,443}   , {878,95,931,155}      , {816,164,857,197}  , {1088,5,1159,46}      , //33
  {1168,4,1187,41}   , {1091,54,1112,66}     , {1089,78,1112,102}    , {1086,112,1116,130}, {1087,138,1113,169}   , //34
  {1082,176,1113,194}, {1091,201,1102,220}   , {1126,51,1152,82}     , {1093,228,1103,253}, {1086,293,1096,318}   , //35
  {1122,97,1159,116} , {1169,50,1195,80}     , {1169,94,1201,112}    , {1128,126,1141,143}, {802,1170,1006,1177}  , //36
  {801,1189,1005,1196},{949,455,1015,578}    , {942,587,1008,710}    , {1031,461,1097,584}, {1042,615,1108,688}   , //37
  {971,733,1021,752} , {967,765,1019,788}    , {961,801,1047,865}    , {1050,708,1136,790}, {1047,765,1133,840}   , //38
  {950,870,1003,918} , {1025,891,1072,926}   , {1032,945,1068,981}   ,{1034,1002,1070,1038},{975,1036,1020,1061}  , //39
  {1089,843,1112,878}, {1086,887,1094,898}   , {1079,907,1138,941}   , {1082,947,1131,961}, {1084,968,1137,1016}  , //40
  {1052,1050,1138,1132},{1115,157,1201,239}  , {1112,242,1198,324}   , {1082,327,1124,359}, {1133,328,1166,358}   , //41
  {1085,364,1122,387}  ,{1139,362,1165,386}  , {1183,328,1197,344}   , {1094,399,1118,441}                          //42

};

 uint16_t plant_object_spriteMap[NUM_PLANT_SPRITES][4] = {

     {0, 0, 18, 14},	     {20, 0, 83, 40},	    {84, 0, 147, 40},	   {152, 0, 169, 21},	   {179, 0, 194, 23},	//1
  {155, 27, 168, 39},	  {178, 29, 189, 38},	   {203, 0, 221, 12},	  {203, 15, 221, 27},	   {227, 0, 257, 27},	//2
  {202, 31, 212, 39},	  {229, 31, 243, 39},	  {248, 31, 260, 41},	   {264, 0, 318, 47},	   {327, 0, 346, 19},	//3
  {331, 22, 344, 37},	   {348, 0, 371, 11},  	  {356, 33, 365, 46},	   {375, 0, 389, 18},	   {394, 0, 424, 57},	//4
   {421, 0, 449, 25},	  {424, 29, 449, 67},	   {452, 1, 467, 23},	  {451, 27, 484, 37},	   {469, 0, 506, 9} ,	//5
  {450, 46, 498, 59},	  {490, 15, 538, 28},	   {541, 0, 600, 74},	   {602, 0, 665, 28},	  {602, 33, 666, 86},	//6
   {675, 0, 718, 33},	  {378, 25, 386, 33},	  {505, 35, 522, 62},	  {371, 45, 391, 65},	  {524, 48, 533, 65},	//7
  {668, 33, 684, 52},	  {339, 44, 344, 51},	  {698, 32, 720, 45},	   {733, 0, 784, 45},	 {669, 58, 727, 118},	//8
  {351, 58, 364, 68},	  {314, 50, 330, 60},	  {273, 53, 292, 63},	  {246, 47, 266, 59},	  {217, 46, 236, 61},	//9
  {188, 48, 208, 61},	  {163, 47, 184, 62},	  {136, 45, 157, 59},	  {109, 43, 130, 62},	    {75, 40, 95, 63},	//10
    {42, 47, 66, 59},	     {4, 46, 32, 60},	  {743, 54, 776, 68},	  {742, 80, 779, 94},	{744, 104, 784, 119},	//11
 {643, 86, 658, 119},	 {574, 86, 619, 127},	 {506, 79, 564, 135},	  {485, 68, 496, 79},	  {464, 68, 477, 75},	//12
  {442, 70, 458, 77},	  {479, 84, 496, 93},	  {456, 80, 475, 91},	  {435, 81, 450, 92},	  {414, 69, 431, 80},	//13
  {319, 69, 337, 81},	  {364, 70, 383, 86},	  {394, 90, 403, 98},	{449, 100, 474, 109},	  {327, 76, 358, 89},	//14
  {287, 70, 322, 80},	 {231, 69, 284, 117},	 {173, 71, 231, 128},	  {124, 70, 167, 97},	   {92, 72, 120, 89},	//15
 {91, 100, 131, 126},	    {23, 67, 65, 85},	   {41, 93, 87, 112},	   {15, 90, 36, 102},	     {1, 62, 16, 83},	//16
 {291, 90, 302, 104},	  {308, 84, 320, 99},	 {332, 94, 381, 136},	{316, 108, 328, 125},	{298, 106, 309, 123},	//17
{389, 107, 420, 134},	{261, 119, 271, 127},	{429, 116, 473, 156},	{401, 138, 427, 157},	{474, 139, 544, 183},	//18
{556, 138, 621, 213},	{627, 127, 774, 284},	{588, 219, 619, 237},	{552, 220, 583, 238},	{467, 194, 545, 244},	//19
{373, 171, 465, 242},	{375, 145, 386, 167},	{356, 164, 370, 190},	{301, 138, 344, 165},	{322, 173, 350, 196},	//20
{273, 139, 301, 162},	{195, 132, 265, 197},	{108, 131, 190, 218},	 {55, 135, 107, 195},	  {16, 115, 54, 175},	//21
{741, 283, 798, 369},	{665, 292, 736, 399},	{775, 126, 788, 138},	{776, 143, 799, 166},	{783, 174, 793, 185},	//22
{603, 242, 622, 253},	{587, 243, 598, 253},	{554, 240, 579, 260},	{339, 200, 366, 215},	{234, 201, 330, 285},	//23
{192, 221, 225, 238},      {0, 190, 55, 238},	 {60, 226, 101, 249},	{567, 284, 658, 362},	{381, 263, 563, 434},	//24
{279, 181, 312, 192},	{272, 297, 368, 321},	{220, 287, 264, 309},	{349, 329, 364, 341},	{332, 325, 344, 335},	//25
{181, 269, 209, 292},	{185, 296, 213, 319},	{152, 272, 174, 283},	{111, 290, 178, 330},	 {95, 253, 117, 284},	//26
  {73, 255, 88, 286},	{302, 345, 372, 410},	{273, 323, 290, 347},	{251, 317, 266, 335},	{280, 356, 298, 369},	//27
{257, 348, 274, 370},	{342, 230, 359, 244},	{347, 249, 361, 261},	{208, 321, 248, 374},	{163, 330, 198, 379},	//28
{345, 270, 363, 284},	{782, 194, 797, 213},	{788, 221, 800, 238},	{574, 370, 612, 489},	{620, 394, 685, 538},	//29
{718, 420, 783, 562},	{489, 433, 556, 454},	{414, 431, 481, 452},	{298, 420, 410, 523},	{786, 249, 798, 258},	//30
{785, 263, 797, 271},	{411, 455, 484, 504},	{554, 262, 565, 269},	{213, 310, 296, 374},	{119, 391, 202, 455},	//31
 {39, 346, 144, 378},	  {0, 382, 115, 458},	  {0, 460, 145, 576},	{156, 458, 200, 482},	{143, 487, 187, 511},	//32
{204, 454, 305, 520},	   {1, 239, 45, 263},	  {39, 257, 67, 285},	 {63, 286, 109, 332},	  {1, 706, 133, 799},	//33
  {0, 676, 138, 702},	   {2, 270, 12, 280},	  {31, 292, 65, 311},	   {0, 284, 20, 299},	   {0, 309, 35, 330},	//34
  {43, 321, 65, 333},	   {0, 333, 19, 346},	{252, 571, 297, 588},	{150, 517, 182, 549},	   {0, 593, 38, 631},	//35
  {50, 575, 80, 591},	 {80, 569, 108, 597},	{112, 566, 140, 600},	  {39, 593, 61, 623},	  {65, 598, 96, 629},	//36
{135, 551, 175, 588},	 {97, 596, 151, 649},	{150, 587, 204, 640},	{184, 517, 238, 570},	   {4, 633, 17, 646},   //37
  {23, 633, 36, 646},	  {42, 631, 91, 669},	{243, 524, 324, 577},	{202, 579, 234, 606},	{205, 608, 226, 629},	//38
{104, 655, 111, 669},	{122, 653, 135, 677},	{141, 674, 257, 799},	{334, 525, 386, 558},	{246, 590, 298, 623},	//39
{312, 568, 411, 714},	{415, 514, 514, 660},	{523, 501, 622, 645},	{244, 629, 300, 658},	{500, 465, 556, 494},   //40
{625, 545, 725, 645},	{415, 664, 515, 764},	{508, 648, 608, 748},	{198, 650, 223, 662},{1198, 1196, 1199, 1197},  //41
{927, 1108, 1035, 1111},{932, 458, 960, 481},   {933, 487, 961, 510},   {930, 515, 970, 581},   {938, 592, 973, 625},   //42
{989, 458, 1027, 499}, {994, 502, 1064, 567}, {1000, 574, 1033, 623},   {974, 639, 1002,667},  {983, 684, 1002, 697},   //43
 {1094, 6, 1104, 17},    {1105, 6, 1115, 17},    {1116, 6, 1127, 17},   {1087, 24, 1109, 48},   {1110, 24, 1134, 48},   //44
{1135, 24, 1160, 48},   {1161, 24, 1181, 48},   {1091, 53, 1101, 64},   {1102, 53, 1112, 64},   {1113, 53, 1123, 64},   //45
{1083, 66, 1105, 90},   {1106, 66, 1129, 90},   {1130, 66, 1155, 90},   {1157, 66, 1179, 90},  {1087, 94, 1104, 112},   //46
{1105, 94, 1122, 112}, {1123, 94, 1140, 112},  {1141, 94, 1158, 112},  {1159, 94, 1178, 112}, {1095, 121, 1101, 129},   //47
{1102, 121, 1109, 129},{1110, 121, 1119, 129},{1120, 121, 1126, 129}, {1127, 121, 1135, 129}, {1087, 113, 1104, 152},   //48
{1107, 134, 1125, 153},{1123, 136, 1139, 150},{1136, 133, 1162, 155}, {1162, 135, 1180, 153}, {1086, 153, 1108, 173},   //49
{1106, 154, 1126, 170},{1124, 154, 1138, 168},{975, 710, 1175, 910}
} ;


