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

/* Grid pixel heights */
#define NUM_ROWS 5
#define NUM_COLS 12
extern const float top_gridHeights[NUM_ROWS];
extern const float bottom_gridHeights[NUM_ROWS];
extern const float left_gridWidths[NUM_COLS];
extern const float right_gridWidths[NUM_COLS];

/* Command types and spellings */
#define NUM_CMD_SPELLINGS 3
typedef enum {SELECT_CMD=0, PLACE_CMD, FIRE_CMD, GOTO_CMD, NUM_CMD_TYPES} CMD_TYPE;
extern std::string cmdNames[NUM_CMD_TYPES][NUM_CMD_SPELLINGS];

/* Predicate types and spellings */
#define NUM_PRED_SPELLINGS 2
typedef enum {ALWAYS_PRED=0, NEVER_PRED, READY_PRED, ALIVE_PRED, DEAD_PRED, DAMAGE_PRED, EAT_PRED, HIT_PRED, SCORE_PRED, WINNING_PRED, LOSING_PRED, NUM_PRED_TYPES} PRED_TYPE;
extern std::string predNames[NUM_PRED_TYPES][NUM_PRED_SPELLINGS];


/* Texture file enum */
typedef enum {TEX_BACKGROUND=0, TEX_COURT, TEX_SCOREBOARD_BOTTOM, TEX_SCOREBOARD_TOP,
              GREEN_FONT, RED_FONT, WHITE_FONT, TEX_ZOMBIES, TEX_PLANTS, TEX_FENCE, NUM_TEXTURES} TEXTURE_ENUM;
extern std::string texFiles[NUM_TEXTURES];


/* Depth enum */
typedef enum {FRONT_DEPTH=0, PARTICLE_BOTTOM_DEPTH,
OBJECT_BOTTOM_DEPTH=4000, BOTTOMROW_1_DEPTH=8000, BOTTOMROW_2_DEPTH=12000, BOTTOMROW_3_DEPTH=16000, BOTTOMROW_4_DEPTH=20000, BOTTOMROW_5_DEPTH=24000, FENCE_DEPTH=28000,
PARTICLE_TOP_DEPTH, TOPROW_5_DEPTH=32000, TOPROW_4_DEPTH=36000, TOPROW_3_DEPTH=40000, TOPROW_2_DEPTH=44000, TOPROW_1_DEPTH=48000, OBJECT_TOP_DEPTH=52000,
FONT_DEPTH=60000, SCOREBOARD_DEPTH=60001, BACKGROUND_DEPTH, COURT_DEPTH, BACK_DEPTH} DEPTH_ENUM;


/* Sound effect file enum */
typedef enum {SFX_GROAN, SFX_GROAN2, SFX_GROAN3, SFX_GROAN4, SFX_GROAN5, SFX_GROAN6, SFX_WINMUSIC, SFX_GULP, SFX_YUCK, SFX_YUCK2,
                SFX_SUKHBIR, SFX_SUKHBIR2, SFX_SUKHBIR3, SFX_SUKHBIR4, SFX_SUKHBIR5, SFX_SUKHBIR6, SFX_SIREN, SFX_SHOVEL,
                SFX_SHIELDHIT, SFX_SHIELDHIT2, SFX_PORTAL, SFX_POLEVAULT, SFX_PLANT, SFX_PLANT2, SFX_LIMBS_POP,
                SFX_HUGEWAVE, SFX_FINALWAVE, SFX_DANCER, SFX_CHOMP, SFX_CHOMP2, SFX_CHOMPSOFT, SFX_BIGCHOMP, SFX_AWOOGA,
                SFX_SPLAT, SFX_SPLAT2, SFX_SPLAT3, SFX_TAP, SFX_TAP2, SFX_FROZEN, SFX_NEWSPAPER_RARRGH, SFX_NEWSPAPER_RARRGH2, SFX_NEWSPAPER_RIP,
                SFX_THROW, SFX_THROW2,
                NUM_SFX} SFX_ENUM;
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

/* Sprite enum for TEX_FENCE */
typedef enum {FENCE_ALL=0, NUM_TEX_FENCE} TEX_FENCE_ENUM;
extern uint16_t tex_fence_spriteMap[][4];

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

/*Sprite enum for ZOMBIES */
typedef enum
{ZOMBIE_BUCKET_1 = 0, ZOMBIE_BUCKET_2, ZOMBIE_BUCKET_3, ZOMBIE_CONE_1, ZOMBIE_CONE_2,                                                                                          //1
ZOMBIE_CONE_3, ZOMBIE_DANCER_HEAD, ZOMBIE_DANCER_BELT, ZOMBIE_DANCER_BODY_1,ZOMBIE_DANCER_BODY_2,                                                                              //2
ZOMBIE_DANCER_COLAR, ZOMBIE_DANCER_EARING, ZOMBIE_DANCER_HAIR, ZOMBIE_DANCER_INNERARM_HAND, ZOMBIE_DANCER_INNERARM_LOWER,                                                      //3
ZOMBIE_DANCER_INNERARM_UPPER, ZOMBIE_DANCER_INNERLEG_FOOT, ZOMBIE_DANCER_INNERLEG_LOWER, ZOMBIE_DANCER_INNERLEG_UPPER, ZOMBIE_DANCER_OUTERARM_HAND,                            //4
ZOMBIE_DANCER_OUTERARM_LOWER, ZOMBIE_DANCER_OUTERARM_UPPER, ZOMBIE_DANCER_OUTERLEG_FOOT, ZOMBIE_DANCER_OUTERLEG_LOWER, ZOMBIE_DANCER_OUTERLEG_TOE,                             //5
ZOMBIE_DANCER_OUTERLEG_UPPER, ZOMBIE_DIGGER_BODY, ZOMBIE_DIGGER_DIG0, ZOMBIE_DIGGER_DIG_1, ZOMBIE_DIGGER_DIG_2,                                                                //6
ZOMBIE_DIGGER_DIG_3, ZOMBIE_DIGGER_DIG_4, ZOMBIE_DIGGER_DIG_5, ZOMBIE_DIGGER_DIRT, ZOMBIE_DIGGER_HARDHAT,                                                                      //7
ZOMBIE_DIGGER_HARDHAT_2, ZOMBIE_DIGGER_HARDHAT_3, ZOMBIE_DIGGER_HEAD, ZOMBIE_DIGGER_INNERARM_HAND, ZOMBIE_DIGGER_INNERARM_LOWER,                                               //8
ZOMBIE_DIGGER_INNERARM_UPPER, ZOMBIE_DIGGER_INNERLEG_FOOT, ZOMBIE_DIGGER_INNERLEG_LOWER, ZOMBIE_DIGGER_INNERLEG_UPPER, ZOMBIE_DIGGER_JAW,                                      //9
ZOMBIE_DIGGER_OUTERARM_HAND, ZOMBIE_DIGGER_OUTERARM_LOWER, ZOMBIE_DIGGER_OUTERARM_UPPER, ZOMBIE_DIGGER_OUTERARM_UPPER_2,ZOMBIE_DIGGER_OUTERLEG_FOOT,                           //10
ZOMBIE_DIGGER_OUTERLEG_LOWER, ZOMBIE_DIGGER_OUTERLEG_UPPER, ZOMBIE_DIGGER_PICKAXE, ZOMBIE_DIGGER_RISE_GROUND, ZOMBIE_DIGGER_RISE_2,                                            //11
ZOMBIE_DIGGER_RISE_3, ZOMBIE_DIGGER_RISE_4, ZOMBIE_DIGGER_RISE_5, ZOMBIE_DIGGER_RISE_6, ZOMBIE_EYE,                                                                            //12
ZOMBIE_FLAG_1, ZOMBIE_FLAG_3, ZOMBIE_FLAGHAND, ZOMBIE_FLAGPOLE, ZOMBIE_FOOTBALL_HEAD,                                                                                          //13
ZOMBIE_FOOTBALL_HELMET, ZOMBIE_FOOTBALL_HELMET_2, ZOMBIE_FOOTBALL_HELMET_3, ZOMBIE_FOOTBALL_LEFTARM_EATINGHAND, ZOMBIE_FOOTBALL_LEFTARM_EATINGLOWER,                           //14
ZOMBIE_FOOTBALL_LEFTARM_HAND, ZOMBIE_FOOTBALL_LEFTARM_LOWER, ZOMBIE_FOOTBALL_LEFTARM_UPPER, ZOMBIE_FOOTBALL_LEFTARM_UPPER_2, ZOMBIE_FOOTBALL_LEFTLEG_FOOT,                     //15
ZOMBIE_FOOTBALL_LEFTLEG_LOWER, ZOMBIE_FOOTBALL_LEFTLEG_UPPER, ZOMBIE_FOOTBALL_LOWERBODY, ZOMBIE_FOOTBALL_OUTERARM_EATINGHAND, ZOMBIE_FOOTBALL_RIGHTARM_HAND,                   //16
ZOMBIE_FOOTBALL_RIGHTARM_LOWER, ZOMBIE_FOOTBALL_RIGHTARM_UPPER, ZOMBIE_FOOTBALL_RIGHTLEG_FOOT, ZOMBIE_FOOTBALL_RIGHTLEG_LOWER, ZOMBIE_FOOTBALL_RIGHTLEG_UPPER,                 //17
ZOMBIE_FOOTBALL_UPPERBODY, ZOMBIE_FOOTBALL_UPPERBODY_2, ZOMBIE_FOOTBALL_UPPERBODY_3, ZOMBIE_GLASSES, ZOMBIE_HAIR,                                                              //18
ZOMBIE_HAND_ARM, ZOMBIE_HAND_FINGER_1_1, ZOMBIE_HAND_FINGER_1_2, ZOMBIE_HAND_FINGER_2_1, ZOMBIE_HAND_FINGER_2_2,                                                               //19
ZOMBIE_HAND_FINGER_3_1, ZOMBIE_HAND_FINGER_3_2, ZOMBIE_HAND_FINGER_4_1, ZOMBIE_HAND_FINGER_4_2, ZOMBIE_HAND_HAND,                                                              //20
ZOMBIE_HAND_OVERLAY, ZOMBIE_HAND_ROCK_7, ZOMBIE_HAND_ROCK_1, ZOMBIE_HAND_ROCK_2, ZOMBIE_HAND_ROCK_3,                                                                           //21
ZOMBIE_HAND_ROCK_4, ZOMBIE_HAND_ROCK_5, ZOMBIE_HAND_ROCK_6, ZOMBIE_HEAD, ZOMBIE_HEAD_GROSSOUT,                                                                                 //22
ZOMBIE_HEAD_SUNGLASSES_1, ZOMBIE_HEAD_SUNGLASSES_2, ZOMBIE_HEAD_SUNGLASSES_3, ZOMBIE_HEAD_SUNGLASSES_4, ZOMBIE_HEAD_2,                                                         //23
ZOMBIE_INNERARM_HAND, ZOMBIE_INNERARM_LOWER, ZOMBIE_INNERARM_SCREENDOOR, ZOMBIE_INNERARM_SCREENDOOR_HAND, ZOMBIE_INNERARM_UPPER,                                               //24
ZOMBIE_INNERLEG_FOOT, ZOMBIE_INNERLEG_LOWER, ZOMBIE_INNERLEG_UPPER, ZOMBIE_JAW, ZOMBIE_MUSTACHE_1,                                                                             //25
ZOMBIE_MUSTACHE_2, ZOMBIE_MUSTACHE_3, ZOMBIE_NECK, ZOMBIE_OUTERARM_HAND, ZOMBIE_OUTERARM_HAND_2,                                                                               //26
ZOMBIE_OUTERARM_LOWER, ZOMBIE_OUTERARM_SCREENDOOR, ZOMBIE_OUTERARM_UPPER, ZOMBIE_OUTERARM_UPPER_2, ZOMBIE_OUTERLEG_FOOT,                                                       //27
ZOMBIE_OUTERLEG_LOWER, ZOMBIE_OUTERLEG_UPPER, ZOMBIE_YETI_BODY, ZOMBIE_YETI_HEAD, ZOMBIE_YETI_INNERARM_HAND,                                                                   //28
ZOMBIE_YETI_INNERARM_LOWER, ZOMBIE_YETI_INNERARM_UPPER, ZOMBIE_YETI_INNERLEG_FOOT, ZOMBIE_YETI_INNERLEG_LOWER, ZOMBIE_YETI_INNERLEG_UPPER,                                     //29
ZOMBIE_YETI_JAW, ZOMBIE_YETI_OUTERARM_HAND, ZOMBIE_YETI_OUTERARM_LOWER, ZOMBIE_YETI_OUTERARM_UPPER, ZOMBIE_YETI_OUTERARM_UPPER_2,                                              //30
ZOMBIE_YETI_OUTERLEG_FOOT, ZOMBIE_YETI_OUTERLEG_LOWER, ZOMBIE_YETI_OUTERLEG_UPPER, ZOMBIEARM, ZOMBIEBODY,                                                                      //31
ZOMBIE_TONGUE, ZOMBIE_UNDERBODY, ZOMBIE_TIE, ZOMBIE_SHADOW, BLANK_SPRITE,                                                                                                      //32
WHITE_BOX_SPRITE, ZOMBIE_FLAG_INNERARM_LOWER, ZOMBIE_POLEVAULTER_BODY1, ZOMBIE_POLEVAULTER_BODY2, ZOMBIE_POLEVAULTER_HAIR,                                                     //33
ZOMBIE_POLEVAULTER_INNERARM_UPPER, ZOMBIE_POLEVAULTER_INNERARM_LOWER, ZOMBIE_POLEVAULTER_INNERHAND, ZOMBIE_POLEVAULTER_INNERLEG_FOOT, ZOMBIE_POLEVAULTER_INNERLEG_LOWER,       //34
ZOMBIE_POLEVAULTER_INNERLEG_TOE, ZOMBIE_POLEVAULTER_INNERLEG_UPPER, ZOMBIE_POLEVAULTER_OUTERARM_LOWER, ZOMBIE_POLEVAULTER_OUTERARM_UPPER, ZOMBIE_POLEVAULTER_OUTERARM_UPPER2,  //35
ZOMBIE_POLEVAULTER_OUTERLEG_FOOT, ZOMBIE_POLEVAULTER_OUTERLEG_LOWER, ZOMBIE_POLEVAULTER_OUTERLEG_TOE, ZOMBIE_POLEVAULTER_OUTERLEG_UPPER, ZOMBIE_POLEVAULTER_POLE,              //36
ZOMBIE_POLEVAULTER_POLE2, ZOMBIE_SCREENDOOR_1, ZOMBIE_SCREENDOOR_2, ZOMBIE_SCREENDOOR_3, ZOMBIE_PAPER_BODY,                                                                    //37
ZOMBIE_PAPER_GLASSES, ZOMBIE_PAPER_HAIRPIECE, ZOMBIE_PAPER_HANDS, ZOMBIE_PAPER_HANDS2,                                                                                         //38
ZOMBIE_PAPER_HANDS3, ZOMBIE_PAPER_HEAD_LOOK, ZOMBIE_PAPER_LEFTARM_LOWER, ZOMBIE_PAPER_LEFTARM_UPPER, ZOMBIE_PAPER_LEFTARM_UPPER2,                                              //39
ZOMBIE_PAPER_LEFTFOOT, ZOMBIE_PAPER_LEFTLEG_LOWER, ZOMBIE_PAPER_LEFTLEG_UPPER, ZOMBIE_PAPER_LOWERBODY1, ZOMBIE_PAPER_LOWERBODY2,                                               //40
ZOMBIE_PAPER_MADHEAD, ZOMBIE_PAPER_PAPER1, ZOMBIE_PAPER_PAPER2, ZOMBIE_PAPER_PAPER3, ZOMBIE_PAPER_RIGHTARM_LOWER,                                                              //41
ZOMBIE_PAPER_RIGHTARM_UPPER, ZOMBIE_PAPER_RIGHTFOOT, ZOMBIE_PAPER_RIGHTLEG_LOWER, ZOMBIE_PAPER_RIGHTLEG_UPPER, NUM_ZOMBIE_SPRITES}                                             //42
ZOMBIE_SPRITE_ENUM;
extern uint16_t zombie_object_spriteMap[][4];

typedef enum {ANIM_SPROUT, CABBAGEPULT_BASKET, CABBAGEPULT_BASKET_OVERLAY, CABBAGEPULT_BASKETVINE_1, CABBAGEPULT_BASKETVINE_2,                                                  //1
CABBAGEPULT_BASKETVINE_3, CABBAGEPULT_BASKETVINE_4, CABBAGEPULT_BLINK_1, CABBAGEPULT_BLINK_2, CABBAGEPULT_CABBAGE,                                                              //2
CABBAGEPULT_EYEBROW, CABBAGEPULT_EYEBROW_1, CABBAGEPULT_EYEBROW_2, CABBAGEPULT_HEAD, CABBAGEPULT_LEAF,                                                                          //3
CABBAGEPULT_STALK_1,CABBAGEPULT_STALK_2, CABBAGEPULT_STALK_3, CABBAGEPULT_STALK_4, CABBAGEPULT_STALK_5,                                                                         //4
CACTUS_ARM_1_1, CACTUS_ARM_1_2, CACTUS_ARM_2_1, CACTUS_ARM_2_2, /*CACTUS_BLINK_1,*/                                                                                             //5
CACTUS_BLINK_2, CACTUS_BODY_OVERLAY,CACTUS_BODY_OVERLAY_2, CACTUS_BODY_1, CACTUS_BODY_2,                                                                                        //6
CACTUS_BODY_3, CACTUS_FLOWER, CACTUS_FLOWER_TOP, CACTUS_LIPS, CHERRYBOMB_LEAF_1,                                                                                                //7
CHERRYBOMB_LEAF_1_TIP, CHERRYBOMB_LEAF_2, CHERRYBOMB_LEAF_2_TIP,CHERRYBOMB_LEAF_3, CHERRYBOMB_LEFT_1,                                                                           //8
CHERRYBOMB_LEFT_3, CHERRYBOMB_LEFTEYE_1_1, CHERRYBOMB_LEFTEYE_1_2, CHERRYBOMB_LEFTEYE_1_3, CHERRYBOMB_LEFTEYE_1_4,                                                              //9
CHERRYBOMB_LEFTEYE_1_5,CHERRYBOMB_LEFTEYE_2_1, CHERRYBOMB_LEFTEYE_2_2, CHERRYBOMB_LEFTEYE_2_3, CHERRYBOMB_LEFTEYE_2_4,                                                          //10
 CHERRYBOMB_LEFTEYE_2_5, CHERRYBOMB_LEFTMOUTH_1, CHERRYBOMB_LEFTMOUTH_2, CHERRYBOMB_LEFTMOUTH_3,CHERRYBOMB_LEFTMOUTH_4,                                                         //11
CHERRYBOMB_LEFTMOUTH_5, CHERRYBOMB_LEFTSTEM, CHERRYBOMB_RIGHT_1, CHERRYBOMB_RIGHT_3, CHERRYBOMB_RIGHTEYE_1_1,                                                                   //12
CHERRYBOMB_RIGHTEYE_1_2, HERRYBOMB_RIGHTEYE_1_3, CHERRYBOMB_RIGHTEYE_1_4,CHERRYBOMB_RIGHTEYE_1_5, CHERRYBOMB_RIGHTEYE_2_1,                                                      //13
CHERRYBOMB_RIGHTEYE_2_2, CHERRYBOMB_RIGHTEYE_2_3, CHERRYBOMB_RIGHTEYE_2_4, /*CHERRYBOMB_RIGHTEYE_2_5,*/ CHERRYBOMB_RIGHTMOUTH_1,                                                //14
/*CHERRYBOMB_RIGHTMOUTH_2,*/CHERRYBOMB_RIGHTMOUTH_3, CHERRYBOMB_RIGHTMOUTH_4, CHERRYBOMB_RIGHTMOUTH_5, CHERRYBOMB_RIGHTSTEM,                                                    //15
CHOMPER_BOTTOMLIP, CHOMPER_GROUNDLEAF_1, CHOMPER_GROUNDLEAF_1_TIP, CHOMPER_GROUNDLEAF_2, /*CHOMPER_GROUNDLEAF_2_TIP,*/                                                          //16
CHOMPER_GROUNDLEAF_3, CHOMPER_GROUNDLEAF_4, CHOMPER_HEADLEAF_1, CHOMPER_HEADLEAF_2, CHOMPER_HEADLEAF_3,                                                                         //17
CHOMPER_HEADLEAF_4, CHOMPER_INSIDEMOUTH, CHOMPER_SPIKE, CHOMPER_STEM_1, CHOMPER_STEM_2,                                                                                         //18
CHOMPER_STEM_3,CHOMPER_STOMACH, CHOMPER_TONGUE, CHOMPER_TOPJAW, CHOMPER_UNDERJAW,                                                                                               //19
DOOMSHROOM_EXPLOSION_TOP, FUMESHROOM_BLINK_1, FUMESHROOM_BLINK_2, FUMESHROOM_BODY, FUMESHROOM_HEAD,                                                                             //20
FUMESHROOM_SPOUT, FUMESHROOM_TIP,GATLINGPEA_BARREL, GATLINGPEA_BLINK_1, GATLINGPEA_BLINK_2,                                                                                     //21
GATLINGPEA_HEAD, GATLINGPEA_HELMET, GATLINGPEA_MOUTH, GATLINGPEA_MOUTH_OVERLAY, JALAPENO_BODY,                                                                                  //22
JALAPENO_BODY_2, JALAPENO_CHEEK, JALAPENO_EYE_1,JALAPENO_EYE_2, JALAPENO_EYEBROW_1,                                                                                             //23
JALAPENO_EYEBROW_2, JALAPENO_MOUTH, JALAPENO_STEM, LILYPAD_BODY, MELONPULT_BLINK_1,                                                                                             //24
/*MELONPULT_BLINK_2,  MELONPULT_BODY, MELONPULT_EYEBROW,*/ MELONPULT_MELON, MELONPULT_STALK,                                                                                    //25
MONEYBAG, MONEYBAG_HI_RES, PEA_PARTICLES, PEA_SPLATS, PEASHOOTER_BACKLEAF,                                                                                                      //26
PEASHOOTER_BACKLEAF_LEFTTIP, PEASHOOTER_BACKLEAF_RIGHTTIP, PEASHOOTER_BLINK_1, PEASHOOTER_BLINK_2, PEASHOOTER_EYEBROW,                                                          //27
PEASHOOTER_FRONTLEAF, PEASHOOTER_FRONTLEAF_LEFTTIP, PEASHOOTER_FRONTLEAF_RIGHTTIP, PEASHOOTER_HEAD, PEASHOOTER_HEADLEAF_2RDFARTHEST,                                            //28
PEASHOOTER_HEADLEAF_3RDFARTHEST, PEASHOOTER_HEADLEAF_FARTHEST,PEASHOOTER_HEADLEAF_NEAREST, PEASHOOTER_HEADLEAF_TIP_BOTTOM, PEASHOOTER_HEADLEAF_TIP_TOP,                         //29
PEASHOOTER_LIPS, PEASHOOTER_MOUTH, PEASHOOTER_SPROUT, PEASHOOTER_STALK_BOTTOM, PEASHOOTER_STALK_TOP,                                                                            //30
PORTAL_CIRCLE_CENTER,PORTAL_CIRCLE_GLOW, PORTAL_CIRCLE_OUTER, POT_BOTTOM, POT_BOTTOM_2,                                                                                         //31
POT_GLOW, POT_LEAF1, POT_LEAF2, POT_SHADOW, POT_STEM,                                                                                                                           //32
POT_TOP, POT_TOP_DARK, POT_WATER_BASE, POT_WATER_TOP, POT_WATER_TOP_GLOW,                                                                                                       //33
POTATOMINE_BLINK_1,POTATOMINE_BLINK_2, POTATOMINE_BODY, POTATOMINE_EYES, POTATOMINE_LIGHT_1,                                                                                    //34
POTATOMINE_LIGHT_2, POTATOMINE_MASHED, POTATOMINE_PARTICLES, POTATOMINE_ROCK_1, POTATOMINE_ROCK_2,                                                                              //35
POTATOMINE_ROCK_3, POTATOMINE_ROCK_4,POTATOMINE_ROCK_5, POTATOMINE_ROCK_6, POTATOMINE_STEM,                                                                                     //36
POTATOMINEFLASH, PROJECTILE_STAR, PROJECTILECACTUS, PROJECTILEPEA, PUFF_1,                                                                                                      //37
PUFF_2, PUFF_3, PUFF_4, PUFF_5, PUFF_6,                                                                                                                                         //38
PUFF_7, PUFFSHROOM_BLINK_1, PUFFSHROOM_BLINK_2,PUFFSHROOM_BODY, PUFFSHROOM_HEAD,                                                                                                //39
PUFFSHROOM_PUFF_1, PUFFSHROOM_PUFF_2, PUFFSHROOM_STEM, PUFFSHROOM_TIP, SHOVEL_HI_RES,                                                                                           //40
TALLNUT_BLINK_1, TALLNUT_BLINK_2, TALLNUT_BODY, TALLNUT_CRACKED_1, TALLNUT_CRACKED_2,                                                                                           //41
WALLNUT_BLINK1, WALLNUT_BLINK2, WALLNUT_BODY, WALLNUT_CRACKED_1, WALLNUT_CRACKED_2,                                                                                             //42
WALLNUT_TWITCH, BLANK_PLANT_SPRITE, WHITE_PLANT_SPRITE, SNOWPEA_BLINK_1, SNOWPEA_BLINK_2,                                                                                       //43
SNOWPEA_CRYSTALS_1, SNOWPEA_CRYSTALS_2, SNOWPEA_CRYSTALS_3, SNOWPEA_HEAD, SNOWPEA_MOUTH,                                                                                        //44
PROJECTILE_SNOW_PEA, CHOMPER_GROUNDLEAF_2_TIP, PEA_PARTICLES, PEA_SPLATS, SNOWPEA_PARTICLES,                                                                                    //45
SNOWPEA_SPLATSWALLNUT_PARTICLES_LARGE, WALLNUT_PARTICLES_SMALL,                                                                                                                 //46                                   //46
NUM_PLANT_SPRITES} PLANT_SPRITE_ENUM ;
extern uint16_t plant_object_spriteMap[][4];


