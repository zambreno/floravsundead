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
    Object::Object(std::vector<animation_struct> myanim, uint32_t anim_count, uint8_t texfile, uint16_t sprite, uint32_t depth, uint8_t num_children, Object *parent) {

        /* Copy the default values over */
        for (uint8_t i = 0; i < myanim.size(); i++) {
            this->anim.push_back(myanim[i]);
        }
        this->anim_mode = OBJECT_STATUS_DEMO;
        this->texfile = texfile;
        this->sprite = sprite;
        this->depth = depth;
        this->num_children = num_children;
        this->parent = parent;
        this->anchor = anim[0].anchor;

        /* Update the object x/y/angle based on the randomized anim_count frame counter */
        this->x = anim[0].start_x;
        if (anim[0].delta_x != 0.0) {
            uint32_t x_step = (uint32_t)((anim[0].end_x - anim[0].start_x) / anim[0].delta_x);
            this->x += (anim_count % x_step) * anim[0].delta_x;
        }

        this->xscale = anim[0].start_xscale;
        if (anim[0].delta_xscale != 0.0) {
            uint32_t xscale_step = (uint32_t)((anim[0].end_xscale - anim[0].start_xscale) / anim[0].delta_xscale);
            this->xscale += (anim_count % xscale_step) * anim[0].delta_xscale;
        }

        this->y = anim[0].start_y;
        if (anim[0].delta_y != 0.0) {
            uint32_t y_step = (uint32_t)((anim[0].end_y - anim[0].start_y) / anim[0].delta_y);
            this->y += (anim_count % y_step) * anim[0].delta_y;
        }

        this->yscale = anim[0].start_yscale;
        if (anim[0].delta_yscale != 0.0) {
            uint32_t yscale_step = (uint32_t)((anim[0].end_yscale - anim[0].start_yscale) / anim[0].delta_yscale);
            this->yscale += (anim_count % yscale_step) * anim[0].delta_yscale;
        }

        this->angle = anim[0].start_angle;
        if (anim[0].delta_angle != 0.0) {
            uint32_t angle_step = (uint32_t)((anim[0].end_angle - anim[0].start_angle) / anim[0].delta_angle);
            this->angle += (anim_count % angle_step) * anim[0].delta_angle;
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
            /* Get the sprite for the current object */
            myGame->getTexCoords(texfile, sprite, texCoords);

            /* Modelview transformations are applied in opposite order. No matter how we want to rotate,
             * we will be moving to the specified relative x/y position at the end */
            glTranslatef(x, y, 0);

            switch (anchor) {
                // Anchor==0 is the center of the object
                case ANCHOR_CENTER:
                default:
                    glTranslatef(texCoords[4]/2, texCoords[5]/2, 0.0);
                    glRotatef(angle, 0.0, 0.0, 1.0);
                    glTranslatef(-texCoords[4]/2, -texCoords[5]/2, 0.0);
                    break;
                // Anchor==1 is the Northwest corner of the object
                case ANCHOR_NW:
                    glTranslatef(0.0, texCoords[5], 0.0);
                    glRotatef(angle, 0.0, 0.0, 1.0);
                    glTranslatef(0.0, -texCoords[5], 0.0);
                    break;
                // Anchor==2 is the North center of the object
                case ANCHOR_N:
                    glTranslatef(texCoords[4]/2, texCoords[5], 0.0);
                    glRotatef(angle, 0.0, 0.0, 1.0);
                    glTranslatef(-texCoords[4]/2, -texCoords[5], 0.0);
                    break;
                // Anchor==3 is the Northeast corner of the object
                case ANCHOR_NE:
                    glTranslatef(texCoords[4], texCoords[5], 0.0);
                    glRotatef(angle, 0.0, 0.0, 1.0);
                    glTranslatef(-texCoords[4], -texCoords[5], 0.0);
                    break;
                // Anchor==4 is the East center of the object
                case ANCHOR_E:
                    glTranslatef(texCoords[4], texCoords[5]/2, 0.0);
                    glRotatef(angle, 0.0, 0.0, 1.0);
                    glTranslatef(-texCoords[4], -texCoords[5]/2, 0.0);
                    break;
                // Anchor==5 is the Southeast corner of the object
                case ANCHOR_SE:
                    glTranslatef(texCoords[4], 0.0, 0.0);
                    glRotatef(angle, 0.0, 0.0, 1.0);
                    glTranslatef(-texCoords[4], 0.0, 0.0);
                    break;
                // Anchor==6 is the South center of the object
                case ANCHOR_S:
                    glTranslatef(texCoords[4]/2, 0.0, 0.0);
                    glRotatef(angle, 0.0, 0.0, 1.0);
                    glTranslatef(-texCoords[4]/2, 0.0, 0.0);
                    break;
                // Anchor==7 is the Southwest corner of the object
                case ANCHOR_SW:
                    glRotatef(angle, 0.0, 0.0, 1.0);
                    break;
                // Anchor==8 is the West center of the object
                case ANCHOR_W:
                    glTranslatef(0.0, texCoords[5]/2, 0.0);
                    glRotatef(angle, 0.0, 0.0, 1.0);
                    glTranslatef(0.0, -texCoords[5]/2, 0.0);
                    break;

            }

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


        /* Update the angle - if we're close enough to the end, switch directions */
        if (anim[anim_mode].delta_angle != 0.0) {
            angle += anim[anim_mode].delta_angle;
            if ((fabs(anim[anim_mode].end_angle - angle) < fabs(anim[anim_mode].delta_angle)) ||
               (fabs(anim[anim_mode].start_angle - angle) < fabs(anim[anim_mode].delta_angle))) {
                anim[anim_mode].delta_angle = anim[anim_mode].delta_angle * -1.0;
            }
        }

        /* Update the x position - if we're close enough to the end, switch directions */
        if (anim[anim_mode].delta_x != 0.0) {
            x += anim[anim_mode].delta_x;
            if ((fabs(anim[anim_mode].end_x - x) < fabs(anim[anim_mode].delta_x)) ||
               (fabs(anim[anim_mode].start_x - x) < fabs(anim[anim_mode].delta_x))) {
                anim[anim_mode].delta_x = anim[anim_mode].delta_x * -1.0;
            }
        }


        /* Update the xscale  - if we're close enough to the end, switch directions */
        if (anim[anim_mode].delta_xscale != 0.0) {
            xscale += anim[anim_mode].delta_xscale;
            if ((fabs(anim[anim_mode].end_xscale - xscale) < fabs(anim[anim_mode].delta_xscale)) ||
               (fabs(anim[anim_mode].start_xscale - xscale) < fabs(anim[anim_mode].delta_xscale))) {
                anim[anim_mode].delta_xscale = anim[anim_mode].delta_xscale * -1.0;
            }
        }


        /* Update the y position - if we're close enough to the end, switch directions */
        if (anim[anim_mode].delta_y != 0.0) {
            y += anim[anim_mode].delta_y;
            if ((fabs(anim[anim_mode].end_y - y) < fabs(anim[anim_mode].delta_y)) ||
               (fabs(anim[anim_mode].start_y - y) < fabs(anim[anim_mode].delta_y))) {
                anim[anim_mode].delta_y = anim[anim_mode].delta_y * -1.0;
            }
        }


        /* Update the yscale  - if we're close enough to the end, switch directions */
        if (anim[anim_mode].delta_yscale != 0.0) {
            yscale += anim[anim_mode].delta_yscale;
            if ((fabs(anim[anim_mode].end_yscale - yscale) < fabs(anim[anim_mode].delta_yscale)) ||
               (fabs(anim[anim_mode].start_yscale - yscale) < fabs(anim[anim_mode].delta_yscale))) {
                anim[anim_mode].delta_yscale = anim[anim_mode].delta_yscale * -1.0;
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

        if (anim.size() > 1) {
            anim_mode = OBJECT_STATUS_GAME;

            x = anim[anim_mode].start_x;
            xscale = anim[anim_mode].start_xscale;
            y = anim[anim_mode].start_y;
            yscale = anim[anim_mode].start_yscale;
            angle = anim[anim_mode].start_angle;
            anchor = anim[anim_mode].anchor;
        }

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
        anchor = ANCHOR_CENTER;
    }

} // namespace fvu
