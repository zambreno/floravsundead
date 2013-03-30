/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * particles.hpp - FloraVsUndead application. This class contains the
 * enumerated types and class declaration for the pellets and particles class.
 *
 *
 * NOTES:
 * 3/28/13 by JAZ::Design created.
 *****************************************************************************/


#include "floravsundead.hpp"


/* Particle speeds.*/
float particleSpeeds[NUM_PARTICLE_TYPE] = {5.0, 5.0, 5.0, 5.0, 5.0};

namespace fvu {

    /*****************************************************************************
    * Function: Particle::draw
    * Description: Particle drawing function. Sets the initial modeling position
    * and draws each Object recursively.
    *****************************************************************************/
    void Particle::draw(uint16_t index) {

        float x, y, z;

        // Particles should be in front of plants.
        x = game_x;
        y = game_y;
        z = index;

        // Put objects on a different depth level if they're on the bottom
        if (team % 2 == 1) {
            z += PARTICLE_BOTTOM_DEPTH;
        }
        else {
            z += PARTICLE_TOP_DEPTH;
        }

        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(dir, 1.0, 1.0);
        myObject->draw();
        glPopMatrix();
    }


    /*****************************************************************************
    * Function: Particle::Particle
    * Description: Class constructor. Uses an enum type to set particle-specific
    * parameters
    *****************************************************************************/
    Particle::Particle(uint8_t mytype, fvu::Plant *myPlant) {

        uint32_t anim_count = 0;

        status = PARTICLE_STATUS_ACTIVE;
        type = mytype;
        this->myPlant = myPlant;
        row = myPlant->getRow();col = myPlant->getCol();
        team = myPlant->getTeam();
        offscreen = false;

        // game_x and game_y are a function of the parent plant (tweaked as needed)
        dir = myPlant->getDir();
        game_x = myPlant->getGameX()+dir*25;
        game_y = myPlant->getGameY();

        /* Initialize particle information here so we can leave the rest of this function as object assembly */
        speed = particleSpeeds[type];

        std::vector<animation_struct> anim;
        animation_struct local_anim;

        switch(type) {
            case PEA_PROJECTILE:
            default:

                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the main projectile
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, PROJECTILEPEA, 0, 0, myObject);

                break;

        }

    }



    /*****************************************************************************
    * Function: Particle::update
    * Description: Updates each particle
    *****************************************************************************/
    void Particle::update() {


        // This needs to be particle-specific as well
        game_x += speed*dir;

        if ((game_x > 1050.0) || (game_x < -1050.0))
            offscreen = true;

        myObject->update();

        // Calculate any zombie impacts here
        switch(type) {
            case PEA_PROJECTILE:
                // We iterate through the loop forwards to get the closest zombie that is in hit range
                for (uint16_t j = 0; j < myGame->myZombies[team].size(); j++) {
                    uint8_t status = myGame->myZombies[team][j].getStatus();
                    if (((status == ZOMBIE_STATUS_ACTIVE) || (status == ZOMBIE_STATUS_EATING)) &&
                        (myGame->myZombies[team][j].getRow() == row) &&
                        (fabs(myGame->myZombies[team][j].getGameX() - game_x) < 15.0)) {
                        myGame->myZombies[team][j].shoot(this);
                        offscreen = true;
                        break;
                    }
                }
                break;
            default:
                break;
        }


    }



    /*****************************************************************************
    * Function: Particle::operator <
    * Description: Overloads the < operator for comparison / sort purposes
    *****************************************************************************/
    bool Particle::operator< (const Particle &rhs) const {

        /* We want pseudo-3D, so lower = closer. Otherwise, closer to the sides
         * should be closer to the screen. */
        if (game_y == rhs.game_y) {
            return (fabsf(game_x) > fabsf(rhs.game_x));
        }
        else return (game_y < rhs.game_y);
    }




} // namespace fvu
