/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * object.cpp - FloraVsUndead application. This class contains the
 * enumerated types and class declaration for the main object class.
 *
 *
 * NOTES:
 * 2/18/13 by JAZ::Design created.
 *****************************************************************************/


#include "floravsundead.hpp"


namespace fvu {

    /*****************************************************************************
    * Function: Object::Object
    * Description: Object class constructor. Creates a set of linked lists
    * between parent and child.
    *****************************************************************************/
    Object::Object(animation_struct demo_anim, animation_struct game_anim, uint32_t anim_count, uint8_t texfile, uint16_t sprite, uint32_t depth, uint8_t num_children, Object *parent) {

        /* Copy the default values over */
        this->demo_anim = demo_anim;
        this->game_anim = game_anim;
        this->texfile = texfile;
        this->sprite = sprite;
        this->depth = depth;
        this->num_children = num_children;
        this->parent = parent;
        status = OBJECT_STATUS_DEMO;

        /* Update the object x/y/angle based on the randomized anim_count frame counter */
        this->x = demo_anim.start_x;
        if (demo_anim.delta_x != 0.0) {
            uint32_t x_step = (uint32_t)((demo_anim.end_x - demo_anim.start_x) / demo_anim.delta_x);
            this->x += (anim_count % x_step) * demo_anim.delta_x;
        }

        this->xscale = demo_anim.start_xscale;
        if (demo_anim.delta_xscale != 0.0) {
            uint32_t xscale_step = (uint32_t)((demo_anim.end_xscale - demo_anim.start_xscale) / demo_anim.delta_xscale);
            this->xscale += (anim_count % xscale_step) * demo_anim.delta_xscale;
        }

        this->y = demo_anim.start_y;
        if (demo_anim.delta_y != 0.0) {
            uint32_t y_step = (uint32_t)((demo_anim.end_y - demo_anim.start_y) / demo_anim.delta_y);
            this->y += (anim_count % y_step) * demo_anim.delta_y;
        }

        this->yscale = demo_anim.start_yscale;
        if (demo_anim.delta_yscale != 0.0) {
            uint32_t yscale_step = (uint32_t)((demo_anim.end_yscale - demo_anim.start_yscale) / demo_anim.delta_yscale);
            this->yscale += (anim_count % yscale_step) * demo_anim.delta_yscale;
        }

        this->angle = demo_anim.start_angle;
        if (demo_anim.delta_angle != 0.0) {
            uint32_t angle_step = (uint32_t)((demo_anim.end_angle - demo_anim.start_angle) / demo_anim.delta_angle);
            this->angle += (anim_count % angle_step) * demo_anim.delta_angle;
        }


        children = NULL;
        if (num_children > 0) {
            children = new Object *[num_children];
        }
    }


    /*****************************************************************************
    * Function: Object::draw
    * Description: Object drawing function. Recursively draws each object.
    *****************************************************************************/
    void Object::draw() {

        float texCoords[6];

        /* We don't draw the top-most object */
        glPushMatrix();
        if (parent) {
            /* Draw the current object */
            myGame->getTexCoords(texfile, sprite, texCoords);

            // Splitting up the x/y translations allows a smoother transition
//            glTranslatef(x, 0.0, 0);


            glTranslatef(x, y, 0);

            glTranslatef(texCoords[4]/2, texCoords[5]/2, 0.0);

            glRotatef(angle, 0.0, 0.0, 1.0);
            //glTranslatef(0.0, y, 0.0);

//            glTranslatef(-x, -y, 0);
            glTranslatef(-texCoords[4]/2, -texCoords[5]/2, 0.0);

            glBegin(GL_QUADS);
                glTexCoord2d(texCoords[0], texCoords[1]);
                glVertex3f(0.0, 0.0, depth);
                glTexCoord2d(texCoords[2], texCoords[1]);
                glVertex3f(texCoords[4]*xscale, 0.0, depth);
                glTexCoord2d(texCoords[2], texCoords[3]);
                glVertex3f(texCoords[4]*xscale, texCoords[5]*yscale, depth);
                glTexCoord2d(texCoords[0], texCoords[3]);
                glVertex3f(0.0, texCoords[5]*yscale, depth);
            glEnd();

        }

        /* Draw the children */
        for (uint8_t i = 0; i < num_children; i++) {
            children[i]->draw();
        }

        glPopMatrix();
    }



    /*****************************************************************************
    * Function: Object::update
    * Description: Updates each object, depending on mode
    *****************************************************************************/
    void Object::update() {

        animation_struct *anim;

        if (status == OBJECT_STATUS_DEMO) {
            anim = &demo_anim;
        }
        else {
            anim = &game_anim;
        }

        /* Update the angle - if we're close enough to the end, switch directions */
        if (anim->delta_angle != 0.0) {
            angle += anim->delta_angle;
            if ((fabs(anim->end_angle - angle) < fabs(anim->delta_angle)) ||
               (fabs(anim->start_angle - angle) < fabs(anim->delta_angle))) {
                anim->delta_angle = anim->delta_angle * -1.0;
            }
        }

        /* Update the x position - if we're close enough to the end, switch directions */
        if (anim->delta_x != 0.0) {
            x += anim->delta_x;
            if ((fabs(anim->end_x - x) < fabs(anim->delta_x)) ||
               (fabs(anim->start_x - x) < fabs(anim->delta_x))) {
                anim->delta_x = anim->delta_x * -1.0;
            }
        }


        /* Update the xscale  - if we're close enough to the end, switch directions */
        if (anim->delta_xscale != 0.0) {
            xscale += anim->delta_xscale;
            if ((fabs(anim->end_xscale - xscale) < fabs(anim->delta_xscale)) ||
               (fabs(anim->start_xscale - xscale) < fabs(anim->delta_xscale))) {
                anim->delta_xscale = anim->delta_xscale * -1.0;
            }
        }


        /* Update the y position - if we're close enough to the end, switch directions */
        if (anim->delta_y != 0.0) {
            y += anim->delta_y;
            if ((fabs(anim->end_y - y) < fabs(anim->delta_y)) ||
               (fabs(anim->start_y - y) < fabs(anim->delta_y))) {
                anim->delta_y = anim->delta_y * -1.0;
            }
        }


        /* Update the yscale  - if we're close enough to the end, switch directions */
        if (anim->delta_yscale != 0.0) {
            yscale += anim->delta_yscale;
            if ((fabs(anim->end_yscale - yscale) < fabs(anim->delta_yscale)) ||
               (fabs(anim->start_yscale - yscale) < fabs(anim->delta_yscale))) {
                anim->delta_yscale = anim->delta_yscale * -1.0;
            }
        }



        /* Update the children */
        for (uint8_t i = 0; i < num_children; i++) {
            children[i]->update();
        }

    }


    /*****************************************************************************
    * Function: Object::endDemo
    * Description: Transfers to game mode for each object.
    *****************************************************************************/
    void Object::endDemo() {

        x = game_anim.start_x;
        xscale = game_anim.start_xscale;
        y = game_anim.start_y;
        yscale = game_anim.start_yscale;
        angle = game_anim.start_angle;
        status = OBJECT_STATUS_GAME;

        /* Update the children */
        for (uint8_t i = 0; i < num_children; i++) {
            children[i]->endDemo();
        }

    }



    /*****************************************************************************
    * Function: animation_struct::set_defaults
    * Description: Sets default values for an animation struct, which helps simplify
    * other parts of the fvu codebase.
    *****************************************************************************/
    void animation_struct::set_defaults() {
        start_angle = 0.0; delta_angle = 0.0; end_angle = 0.0;
        start_xscale = 1.0; delta_xscale = 0.0; end_xscale = 1.0;
        start_x = 0.0; delta_x = 0.0; end_x = 0.0;
        start_yscale = 1.0; delta_yscale = 0.0; end_yscale = 1.0;
        start_y = 0.0; delta_y = 0.0; end_y = 0.0;
    }

} // namespace fvu
