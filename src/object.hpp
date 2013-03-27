/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * object.hpp - FloraVsUndead application. This class contains the
 * enumerated types and class declaration for the main object class.
 *
 *
 * NOTES:
 * 2/18/13 by JAZ::Design created.
 *****************************************************************************/


#pragma once

#include "floravsundead.hpp"

/* Object status enum */
typedef enum {OBJECT_STATUS_DEMO=0, OBJECT_STATUS_GAME, OBJECT_STATUS_ACTION, OBJECT_STATUS_WINNING, OBJECT_STATUS_INACTIVE} OBJECT_STATUS_ENUM;

/* Object anchor enum */
typedef enum {ANCHOR_CENTER=0, ANCHOR_NW, ANCHOR_N, ANCHOR_NE, ANCHOR_E, ANCHOR_SE, ANCHOR_S, ANCHOR_SW, ANCHOR_W} ANCHOR_DIR_ENUM;

namespace fvu {

    /* Helpful structs to contain the animation and placement information */
    class animation_struct {
        public:
            float start_angle, delta_angle, end_angle;
            uint8_t anchor;
            float start_xscale, delta_xscale, end_xscale;
            float start_x, delta_x, end_x;
            float start_yscale, delta_yscale, end_yscale;
            float start_y, delta_y, end_y;
            void set_angle(float startval, float deltaval, float endval) {start_angle = startval; delta_angle = deltaval; end_angle = endval; anchor = ANCHOR_CENTER;}
            void set_angle(float startval, float deltaval, float endval, uint8_t myanchor) {start_angle = startval; delta_angle = deltaval; end_angle = endval; anchor = myanchor;}
            void set_angle(float startval) {start_angle = startval; delta_angle = 0.0; end_angle = startval; anchor = ANCHOR_CENTER;}
            void set_xscale(float startval, float deltaval, float endval) {start_xscale = startval; delta_xscale = deltaval; end_xscale = endval;}
            void set_xscale(float startval) {start_xscale = startval; delta_xscale = 0.0; end_xscale = startval;}
            void set_x(float startval, float deltaval, float endval) {start_x = startval; delta_x = deltaval; end_x = endval;}
            void set_x(float startval) {start_x = startval; delta_x = 0.0; end_x = startval;}
            void set_xy(float startval1, float startval2) {start_x = startval1; delta_x = 0.0; end_x = startval1;start_y = startval2; delta_y = 0.0; end_y = startval2;}
            void set_yscale(float startval, float deltaval, float endval) {start_yscale = startval; delta_yscale = deltaval; end_yscale = endval;}
            void set_yscale(float startval) {start_yscale = startval; delta_yscale = 0.0; end_yscale = startval;}
            void set_y(float startval, float deltaval, float endval) {start_y = startval; delta_y = deltaval; end_y = endval;}
            void set_y(float startval) {start_y = startval; delta_y = 0.0; end_y = startval;}
            void set_defaults();
    };

    /* Object class. Each plant/zombie has multiple objects, each with links to the parent
     * object and (potentially) multiple children. */
     class Object {
        public:
            std::vector<animation_struct> anim;
            uint8_t anim_mode;
            float x, y, angle;
            float delta_x, delta_y, delta_angle;
            uint8_t anchor;
            float xscale, yscale;
            float delta_xscale, delta_yscale;
            uint16_t sprite;
            uint32_t depth;
            uint8_t texfile;
            Object *parent;
            Object **children;
            uint8_t num_children;
            void setMode(uint8_t mode);
            void update();
            void updateSprite(uint16_t mySprite) {sprite = mySprite;}
            Object(std::vector<animation_struct> anim, uint32_t anim_count, uint8_t texfile, uint16_t sprite, uint32_t depth, uint8_t num_children, Object *parent);
            void draw();
     };

} // namespace fvu

