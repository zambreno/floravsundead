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
float particleSpeeds[NUM_PARTICLE_TYPE] = {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0};

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
        live_count = 0;

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
            case SNOW_PROJECTILE:

                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the main projectile
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                if (type == PEA_PROJECTILE) {
                    myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, PROJECTILEPEA, 0, 0, myObject);
                }
                else if (type == SNOW_PROJECTILE) {
                    myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, PROJECTILE_SNOW_PEA, 0, 0, myObject);
                }

                break;

            case SHOVEL_PARTICLE:

                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the main shovel
                game_y -= 25.0;
                local_anim.set_defaults();
                local_anim.set_xscale(-0.65);
                local_anim.set_yscale(0.65);
                local_anim.set_angle(-40.0, 4.0, 40.0, ANCHOR_NW);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_PLANTS, SHOVEL_HI_RES, 0, 0, myObject);

                break;

            default:
                break;

        }

    }


   /*****************************************************************************
    * Function: Particle::Particle
    * Description: Class constructor. Uses an enum type to set particle-specific
    * parameters
    *****************************************************************************/
    Particle::Particle(uint8_t mytype, fvu::Zombie *myZombie) {

        uint32_t anim_count = 0;

        status = PARTICLE_STATUS_ACTIVE;
        type = mytype;
        this->myZombie = myZombie;
        row = myZombie->getRow();col = myZombie->getCol();
        team = myZombie->getTeam();
        offscreen = false;
        live_count = 0;

        // game_x and game_y are a function of the parent plant (tweaked as needed)
        dir = myZombie->getDir();
        game_x = myZombie->getGameX();
        game_y = myZombie->getGameY();

        /* Initialize particle information here so we can leave the rest of this function as object assembly */
        speed = particleSpeeds[type];

        std::vector<animation_struct> anim;
        animation_struct local_anim;

        switch(type) {


            case REGULAR_ARM_PARTICLE:
            default:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the zombie arm
                local_anim.set_defaults();
                local_anim.set_x(-10.0);
                local_anim.set_y(0.0, -2.0, -40.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_OUTERARM_LOWER,ZOMBIE_OUTERARM_LOWER_DEPTH, 1, myObject);

                local_anim.set_defaults();
                local_anim.set_xy(-4.0, -22.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, 0, TEX_ZOMBIES, ZOMBIE_OUTERARM_HAND, ZOMBIE_OUTERARM_HAND_DEPTH, 0, myObject->children[0]);

                break;

            case ZOMBIE_CONE_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                // children[0] is the damaged zombie cone
                local_anim.set_defaults();
                local_anim.set_x(-10.0);
                local_anim.set_y(20.0, -2.0, -40.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_CONE_3, ZOMBIE_HEAD_ACCESSORY_DEPTH, 0, myObject);

            case REGULAR_HEAD_PARTICLE:
                /* The object structure starts at the x/y location of the particle, and moves out in all directions */
                local_anim.set_defaults();
                anim.clear();anim.push_back(local_anim);
                myObject = new Object(anim, anim_count, 0, 0, 0, 1, NULL);

                local_anim.set_defaults();
                local_anim.set_angle(-12.0, 0.25, 5.0, ANCHOR_S);
                local_anim.set_xy(-24.5, 51.75);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_HEAD,ZOMBIE_HEAD_DEPTH, 2, myObject);


                local_anim.set_defaults();
                local_anim.set_angle(-2.0, 0.25, 3.0, ANCHOR_CENTER);
                local_anim.set_xy(-5, 20.0);
                anim.clear();anim.push_back(local_anim);
                myObject->children[0]->children[0] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_HAIR, ZOMBIE_HEAD_DEPTH, 0, myObject->children[0]);

                // children[0][0][2][1] is the jaw
                local_anim.set_defaults();
                local_anim.set_angle(4.0, -0.2, -12.0, ANCHOR_NE);
                local_anim.set_xy(10.2, -9.5);
                anim.clear();anim.push_back(local_anim);anim.push_back(local_anim);
                local_anim.set_angle(4.0, -1.0, -12.0, ANCHOR_NE);anim.push_back(local_anim);
                myObject->children[0]->children[1] = new Object(anim, anim_count, TEX_ZOMBIES, ZOMBIE_JAW, ZOMBIE_HEAD_DEPTH, 0, myObject->children[0]);


        }

    }


    /*****************************************************************************
    * Function: Particle::update
    * Description: Updates each particle
    *****************************************************************************/
    void Particle::update() {


        myObject->update();

        // Calculate any zombie impacts here
        switch(type) {
            case PEA_PROJECTILE:
            case SNOW_PROJECTILE:
                game_x += speed*dir;

                if ((game_x > 1050.0) || (game_x < -1050.0))
                    offscreen = true;

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
            case SHOVEL_PARTICLE:
                live_count++;
                if (live_count == 30)
                    offscreen = true;

                break;

            case REGULAR_ARM_PARTICLE:
            case POLE_ARM_PARTICLE:
            case FOOTBALL_ARM_PARTICLE:
                live_count++;
                if (live_count == 20)
                    offscreen = true;

                break;
            case REGULAR_HEAD_PARTICLE:
            case POLE_HEAD_PARTICLE:
            case FOOTBALL_HEAD_PARTICLE:
                live_count++;
                if (live_count == 30)
                    offscreen = true;

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
