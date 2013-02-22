/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * resources.hpp - audio and texture enumerated types for the FloraVsUndead
 * application.
 *
 *
 * NOTES:
 * 2/18/13 by JAZ::Design created.
 *****************************************************************************/

#pragma once


/* Texture file enum */
typedef enum {TEX_BACKGROUND=0, TEX_COURT, TEX_SCOREBOARD_BOTTOM, TEX_SCOREBOARD_TOP,
              GREEN_FONT, RED_FONT, WHITE_FONT, NUM_TEXTURES} TEXTURE_ENUM;
extern std::string texFiles[NUM_TEXTURES];


/* Depth enum */
typedef enum {FRONT_DEPTH=0, FONT_DEPTH, OBJECT_DEPTH, SCOREBOARD_DEPTH, BACKGROUND_DEPTH, COURT_DEPTH, BACK_DEPTH} DEPTH_ENUM;


/* Sound effect file enum */
typedef enum {SFX_GROAN, SFX_GROAN2, SFX_GROAN3, SFX_GROAN4, SFX_GROAN5, SFX_GROAN6, NUM_SFX} SFX_ENUM;
extern std::string sfxFiles[NUM_SFX];


/* Music  file enum */
typedef enum {MUSIC_INTRO1, MUSIC_MAIN1, MUSIC_MAIN2, MUSIC_MAIN3, MUSIC_MAIN4, MUSIC_MAIN5,
                NUM_MUSIC} MUSIC_ENUM;
extern std::string musicFiles[NUM_MUSIC];


/* Sprite enums for the various font maps. We do this so that the resource arrays can be
 * accessed simply from getTexCoords() */
/* Sprite enum for TEX_BACKGROUND */
typedef enum {BACKGROUND_ALL=0, NUM_TEX_BACKGROUND} TEX_BACKGROUND_ENUM;
extern uint16_t tex_background_spriteMap[][4];

/* Sprite enum for TEX_COURT */
typedef enum {COURT_ALL=0, NUM_TEX_COURT} TEX_COURT_ENUM;
extern uint16_t tex_court_spriteMap[][4];

/* Sprite enum for TEX_SCOREBOARD_BOTTOM */
typedef enum {SCOREBOARD_BOTTOM_ALL=0, NUM_TEX_SCOREBOARD_BOTTOM} TEX_SCOREBOARD_BOTTOM_ENUM;
extern uint16_t tex_scoreboard_bottom_spriteMap[][4];

/* Sprite enum for TEX_SCOREBOARD_TOP */
typedef enum {SCOREBOARD_TOP_ALL=0, NUM_TEX_SCOREBOARD_TOP} TEX_SCOREBOARD_TOP_ENUM;
extern uint16_t tex_scoreboard_top_spriteMap[][4];


/* Sprite enum for GREEN_FONT */
typedef enum {GREEN_ZERO=0, GREEN_1, GREEN_2, GREEN_3, GREEN_4, GREEN_5, GREEN_6, GREEN_7, GREEN_8, GREEN_9, GREEN_NEG, NUM_GREEN_FONT} GREEN_FONT_ENUM;
extern uint16_t green_font_spriteMap[][4];


/* Sprite enum for RED_FONT */
typedef enum {RED_ZERO=0, RED_1, RED_2, RED_3, RED_4, RED_5, RED_6, RED_7, RED_8, RED_9, RED_COLON, RED_DOT, NUM_RED_FONT} RED_FONT_ENUM;
extern uint16_t red_font_spriteMap[][4];

/* Sprite enum for WHITE_FONT */
typedef enum {WHITE_A=0, WHITE_B, WHITE_C, WHITE_D, WHITE_E, WHITE_F, WHITE_G, WHITE_H, WHITE_I, WHITE_J, WHITE_K, WHITE_L, WHITE_M, WHITE_N, WHITE_O, WHITE_P, WHITE_Q, WHITE_R,
              WHITE_S, WHITE_T, WHITE_U, WHITE_V, WHITE_W, WHITE_X, WHITE_Y, WHITE_Z, WHITE_a, WHITE_b, WHITE_c, WHITE_d, WHITE_e, WHITE_f, WHITE_g, WHITE_h, WHITE_i, WHITE_j,
              WHITE_k, WHITE_l, WHITE_m, WHITE_n, WHITE_o, WHITE_p, WHITE_q, WHITE_r, WHITE_s, WHITE_t, WHITE_u, WHITE_v, WHITE_w, WHITE_x, WHITE_y, WHITE_z, WHITE_0, WHITE_1,
              WHITE_2, WHITE_3, WHITE_4, WHITE_5, WHITE_6, WHITE_7, WHITE_8, WHITE_9, NUM_WHITE_FONT} WHITE_FONT_ENUM;
extern uint16_t white_font_spriteMap[][4];


