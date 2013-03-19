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
typedef enum {OBJECT_STATUS_DEMO=0, OBJECT_STATUS_GAME, OBJECT_STATUS_INACTIVE} OBJECT_STATUS_ENUM;


namespace fvu {

    /* Helpful structs to contain the animation and placement information */
    struct animation_struct {
        float start_angle, delta_angle, end_angle;
        float start_x, delta_x, end_x;
        float start_y, delta_y, end_y;
    };

    /* Object class. Each plant/zombie has multiple objects, each with links to the parent
     * object and (potentially) multiple children. */
     class Object {
        private:
            animation_struct demo_anim, game_anim;
            float x, y, angle;
            uint16_t sprite;
            uint32_t depth;
            uint8_t status;
            uint8_t texfile;
        public:
            Object *parent;
            Object **children;
            uint8_t num_children;
            void endDemo();
            void update();
            Object(animation_struct demo_anim, animation_struct game_anim, uint32_t anim_count, uint8_t texfile, uint16_t sprite, uint32_t depth, uint8_t num_children, Object *parent);
            void draw();
     };

} // namespace fvu

