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


#pragma once

#include "floravsundead.hpp"
#include "zombie.hpp"
#include "plants.hpp"
#include "object.hpp"

/* Main enum for the plant types. */
typedef enum {PEA_PROJECTILE=0, SNOW_PROJECTILE, PEA_PARTICLE, SNOW_PARTICLE, CHOMP_PROJECTILE, CHOMP_PARTICLE, SHOVEL_PARTICLE, WALLNUT_LITTLE_PARTICLE, WALLNUT_BIG_PARTICLE, PLANTING_PARTICLE, PORTAL_PARTICLE,
REGULAR_ARM_PARTICLE, ZOMBIE_CONE_PARTICLE, ZOMBIE_BUCKET_PARTICLE, ZOMBIE_FOOTBALL_PARTICLE, FOOTBALL_ARM_PARTICLE, NEWS_ARM_PARTICLE, POLE_ARM_PARTICLE, REGULAR_HEAD_PARTICLE, POLE_HEAD_PARTICLE, FOOTBALL_HEAD_PARTICLE,
NEWS_HEAD_PARTICLE, ZOMBIE_NEWS_PARTICLE, ZOMBIE_QUESTION_PARTICLE,
ZOMBIE_POLE_PARTICLE, ZOMBIE_SCREEN_PARTICLE, YETI_ARM_PARTICLE, YETI_HEAD_PARTICLE, YETI_BOOM_PARTICLE, NUM_PARTICLE_TYPE} PARTICLE_TYPE;
extern float particleSpeeds[NUM_PARTICLE_TYPE];

/* Particle status enum */
typedef enum {PARTICLE_STATUS_INACTIVE = 0, PARTICLE_STATUS_ACTIVE} PARTICLE_STATUS_ENUM;

namespace fvu {

    /* Main particle class */
    class Particle {
        public:
            Particle(uint8_t type, fvu::Plant *myPlant);
            Particle(uint8_t type, fvu::Zombie *myZombie);
            //~Particle() {delete myObject;}
            void update();
            void draw(uint16_t index);
            uint8_t getStatus() {return status;}
            uint8_t getType() {return type;}
            uint16_t getRow() {return row;}
            uint16_t getCol() {return col;}
            bool isPlant() {return plant_particle;}
            bool isOffscreen() {return offscreen;}
            bool operator< (const Particle &rhs) const;

        private:
            uint8_t type;
            uint8_t status;
            uint16_t row, col;
            uint16_t live_count;
            float speed;
            bool offscreen;
            bool has_hit;
            float game_x, game_y;
            float dir;
            uint8_t team;
            bool frozen;
            bool plant_particle;
            fvu::Plant *myPlant;
            fvu::Zombie *myZombie;
            Object *myObject;
    };

} // namespace fvu

