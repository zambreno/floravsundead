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
              GREEN, RED, NUM_TEXTURES} TEXTURE_ENUM;

/* Depth enum */
typedef enum {FRONT_DEPTH=0, SCOREBOARD, COURT, BACKGROUND, BACK_DEPTH} DEPTH_ENUM;


/* Sound effect file enum */
typedef enum {SFX_GROAN, SFX_GROAN2, SFX_GROAN3, SFX_GROAN4, SFX_GROAN5, SFX_GROAN6, NUM_SFX} SFX_ENUM;

/* Music  file enum */
typedef enum {MUSIC_INTRO1, MUSIC_MAIN1, MUSIC_MAIN2, MUSIC_MAIN3, MUSIC_MAIN4, MUSIC_MAIN5,
                NUM_MUSIC} MUSIC_ENUM;


