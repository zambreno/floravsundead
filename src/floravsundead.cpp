/*****************************************************************************
 * Joseph Zambreno
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/


/*****************************************************************************
 * floravsundead.cpp - FloraVsUndead application. Takes as input move files
 * and an optional zombie file and runs a Plants vs Zombies-style simulation
 * for up to four teams.
 *
 *
 * NOTES:
 * 2/16/13 by JAZ::Design created.
 *****************************************************************************/


#include "floravsundead.hpp"

fvu::Game *myGame;

int main(int argc, char *argv[]) {

    myGame = new(std::nothrow)fvu::Game(argc,argv);

    myGame->init();
    myGame->printConfig();

    myGame->initSFML();
    myGame->loadResources();

    myGame->mainLoop();

    delete myGame;
    return 0;
}

namespace fvu {

    /*****************************************************************************
    * Function: Game::init()
    * Description: Initializes the game environment after compiling the team and
    * zombie files.
    *****************************************************************************/
    void Game::init() {


        // Set the random seed here
        srand(0);

        for (uint8_t i = 0; i < 4; i++) {
            for (uint8_t j = 0; j < NUM_ROWS; j++) {
                for (uint8_t k = 0; k < NUM_COLS; k++) {
                    plantGrid[i][j][k] = false;
                }
            }
        }

        compileZombies();
        compileTeams();
        myStatus.pan = 0.0;
        myStatus.mode = DEMO_START;
        myStatus.music_buffer = 0;
        myStatus.firstZombie = true;
        myStatus.end_music = false;
        myStatus.vol_counter = 110;
        myMusic[1].setVolume(myStatus.vol_counter);
        myTime = sf::Time::Zero;
        framecount = 0;
        for (uint8_t i = 0; i < 4; i++) {
            myStatus.scores[i] = 0;
            myTeams[i].budget = 0;
            myTeams[i].status = 0;
            myTeams[i].timer_ms = 0;
            myTeams[i].zombie_index = 0;
            myTeams[i].cur_cmd = 0;
            myTeams[i].zombies_done = false;
            myTeams[i].cmds_done = false;
            myTeams[i].groan_counter = rand() % 256;
        }

    }


    /*****************************************************************************
    * Function: Game::reset()
    * Description: Resets the game environment
    *****************************************************************************/
    void Game::reset() {

        /* Delete all the existing dynamically allocated resources*/
        for (uint8_t i = 0; i < 4; i++) {
            myZombies[i].clear();
            myPlants[i].clear();
            myParticles[i].clear();
            myTeams[i].cmds.clear();
        }

        init();
        if (myMusic[1].getStatus() == sf::Music::Stopped)
            myMusic[1].play();
        myStatus.mode = GAME_START;
        myClock.restart();

        /* Relocate the zombies so that they start using game_x, and game_y */
        for (uint8_t i = 0; i < 4; i++) {
            for (uint16_t j = 0; j < myZombies[i].size(); j++) {
                myZombies[i][j].endDemo();
            }
        for (uint16_t j = 0; j < myPlants[i].size(); j++) {
                myPlants[i][j].endDemo();
            }
        }
    }

    /*****************************************************************************
    * Function: Game::mainLoop()
    * Description: Runs the main update, drawing, and event processing loop.
    *****************************************************************************/
    void Game::mainLoop() {


        while (myWindow.isOpen()) {
            processEvents();

            switch (myStatus.mode) {
                case DEMO_START:
                case DEMO_MID:
                case DEMO_END:
                    demoMode();
                    break;
                case GAME_START:
                case GAME_MID:
                    updateGame();
                    drawWorld();
                    drawScoreboard();
                    drawMap();
                    //myTime = myClock.getElapsedTime();
                    //myTime = sf::milliseconds(1000/FRAME_RATE);
                    myStatus.time_ms -= 1000.0/FRAME_RATE;//myTime.asMilliseconds();
                    if (myStatus.time_ms <= 0.0) {
                        myStatus.time_ms = 0.0;
                        myStatus.mode = GAME_END;
                    }
                    //myClock.restart();
                   break;
                case GAME_END:
                    endGame();
                    break;
                default:
                    break;
            }

            myWindow.display();
        }

        return;

    }


    /*****************************************************************************
    * Function: Game::updateGame()
    * Description: Updates all the main elements of the game.
    *****************************************************************************/
    void Game::updateGame() {

        bool gameDone;

        // Check for the gameDone condition
        for (uint16_t i = 0; i < 4; i++) {
            if ((myTeams[i].zombies_done == true)) { //&& (myTeams[i].cmds_done == true)) {
                gameDone = true;
            }
            else {
                gameDone = false;
                break;
            }
        }
        if (gameDone == true) {
            myStatus.mode = GAME_END;
            return;
        }


        // Zombie loop: grab the next zombie for each team. Note that they are sorted
        // graphically, so we have to perform a linear search based on zombie_index
        // We want the timestamps to be consistent for each team.
        //int32_t elapsed_ms = myTime.asMilliseconds();
        float elapsed_ms = 1000.0/FRAME_RATE;
        for (uint16_t i = 0; i < 4; i++) {

            // If we're done with zombies, continue
            if (myTeams[i].zombies_done == true) {
                continue;
            }


            // If we have no more zombies left to send, we can stop updating zombies once they're all inactive/winning
            if (myTeams[i].zombie_index >= myZombies[i].size()) {
                bool zombies_done = true;
                for (uint16_t k = 0; k < myZombies[i].size(); k++) {
                    if ((myZombies[i][k].getStatus() != ZOMBIE_STATUS_INACTIVE) &&
                        (myZombies[i][k].getStatus() != ZOMBIE_STATUS_GAMEOVER)) {
                        zombies_done = false;
                        break;
                    }
                }
                myTeams[i].zombies_done = zombies_done;
                continue;
            }

            uint16_t j;
            for (j = 0; j < myZombies[i].size(); j++) {
                if (myTeams[i].zombie_index == myZombies[i][j].getIndex()) {
                    break;
                }
            }

            // We should send any zombies that are ready
            if ((j >= myZombies[i].size()) || (myZombies[i][j].getStatus() != ZOMBIE_STATUS_GAME)) {
                continue;
            }
            // A delay of -1 means we wait for all previous zombies to be
            // inactive
            bool sendZombie = true;
            if (myZombies[i][j].getDelay() == -1.0) {
                for (uint16_t k = 0; k < myZombies[i].size(); k++) {
                    if ((myZombies[i][k].getStatus() != ZOMBIE_STATUS_GAME) &&
                        (myZombies[i][k].getStatus() != ZOMBIE_STATUS_INACTIVE) &&
                        (myZombies[i][k].getStatus() != ZOMBIE_STATUS_WINNING) &&
                        (myZombies[i][k].getStatus() != ZOMBIE_STATUS_GAMEOVER)) {
                        sendZombie = false;
                        break;
                    }
                }
            }
            // otherwise, count down until we our individual zombie delay is <= 0
            else {
                sendZombie = false;
                float mydelay = myZombies[i][j].getDelay();
                mydelay -= elapsed_ms;
                if (mydelay <= 0.0) {
                    mydelay = 0.0;
                    sendZombie = true;
                }
                myZombies[i][j].setDelay(mydelay);

            }

            if (sendZombie == true) {
                myZombies[i][j].setStatus(ZOMBIE_STATUS_ACTIVE);
                myTeams[i].zombie_index++;
                myTeams[i].groan_counter++;
                if (myStatus.firstZombie == true) {
                    playSound(SFX_AWOOGA, 100, true);
                    myStatus.firstZombie = false;
                }
                else {
                    // Play a sound for those "final wave" zombies
                    if (myZombies[i][j].getDelay() == -1) {
                        playSound(SFX_FINALWAVE, 100, true);
                    }
                    // Otherwise play a random zombie groan
                    else {
                        if (((myTeams[i].groan_counter % 7) == 0) && (myTeams[i].zombie_index > 7)) {
                            playSound(SFX_GROAN+(myTeams[i].groan_counter % 11), 50, false);
                        }
                    }
                }

            }

        }



        // Command loop: grab the next cmd for each team.
        for (uint16_t i = 0; i < 4; i++) {

            if (myTeams[i].cmds_done == true) {
                continue;
            }

            // Grab the current command
            fvu::cmd_type *mycmd = &myTeams[i].cmds[myTeams[i].cur_cmd];

            // Evaluate predicate, to determine if command should even be executed
            bool pred_true = true;

            if (mycmd->has_pred == true) {

                uint8_t myrow, mycol;
                switch (mycmd->pred) {
                    case ALWAYS_PRED:
                    default:
                        pred_true = true;
                        break;
                    case NEVER_PRED:
                        pred_true = false;
                        break;
                    // For our READY case, we can select all plants or a specific plant being ready
                    case READY_PRED:
                        if (mycmd->has_plant_pred == true) {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getID() == mycmd->plant_pred) {
                                    if ((myPlants[i][p].getStatus() == PLANT_STATUS_INACTIVE) || (myPlants[i][p].getStatus() == PLANT_STATUS_DEFAULT) ||(myPlants[i][p].action_count != 0)) {
                                        pred_true = false;
                                    }
                                    break;
                                }
                            }
                        }
                        else {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                // We can't count INACTIVE plants as not being ready in this context
                                if (myPlants[i][p].action_count != 0) {
                                    pred_true = false;
                                    break;
                                }
                            }
                        }
                        break;

                    // Are we alive or dead?
                    case ALIVE_PRED:
                        if (mycmd->has_plant_pred == true) {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getID() == mycmd->plant_pred) {
                                    if (myPlants[i][p].getHealth() == 0) {
                                        pred_true = false;
                                    }
                                    break;
                                }
                            }
                        }
                        else {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getHealth() == 0) {
                                    pred_true = false;
                                    break;
                                }
                            }
                        }

                        break;

                    case DEAD_PRED:
                        if (mycmd->has_plant_pred == true) {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getID() == mycmd->plant_pred) {
                                    if (myPlants[i][p].getHealth() != 0) {
                                        pred_true = false;
                                    }
                                    break;
                                }
                            }
                        }
                        else {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getHealth() != 0) {
                                    pred_true = false;
                                    break;
                                }
                            }
                        }

                        break;



                    // Have we or any plants been damaged? This is an easy check
                    case DAMAGE_PRED:
                        if (mycmd->has_plant_pred == true) {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getID() == mycmd->plant_pred) {
                                    if (myPlants[i][p].getHealth() == plantHealths[myPlants[i][p].getType()]) {
                                        pred_true = false;
                                    }
                                    break;
                                }
                            }
                        }
                        else {
                            pred_true = false;
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getHealth() < plantHealths[myPlants[i][p].getType()]) {
                                    pred_true = true;
                                    break;
                                }
                            }
                        }

                        break;
                    case EAT_PRED:
                        if (mycmd->has_plant_pred == true) {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getID() == mycmd->plant_pred) {
                                    if (myPlants[i][p].getStatus() != PLANT_STATUS_INACTIVE) {
                                        pred_true = false;
                                    }
                                    break;
                                }
                            }
                        }
                        // Has any of our plants been eaten?
                        else {
                            pred_true = false;
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getStatus() == PLANT_STATUS_INACTIVE) {
                                    pred_true = true;
                                    break;
                                }
                            }
                        }

                        break;
                    case HIT_PRED:
                        if (mycmd->has_plant_pred == true) {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getID() == mycmd->plant_pred) {
                                    if (myPlants[i][p].has_hit == false) {
                                        pred_true = false;
                                    }
                                    break;
                                }
                            }
                        }
                        // Has any of our plants hit?
                        else {
                            pred_true = false;
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].has_hit == true) {
                                    pred_true = true;
                                    break;
                                }
                            }
                        }

                        break;
                    case SCORE_PRED:
                        if (mycmd->has_plant_pred == true) {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getID() == mycmd->plant_pred) {
                                    if (myPlants[i][p].has_killed == false) {
                                        pred_true = false;
                                    }
                                    break;
                                }
                            }
                        }
                        // Has any of our plants scored/killed?
                        else {
                            pred_true = false;
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].has_killed == true) {
                                    pred_true = true;
                                    break;
                                }
                            }
                        }
                        break;
                    case WINNING_PRED:
                        for (uint8_t i_team = 0; i_team < 4; i_team++) {
                            if (myStatus.scores[i_team] > myStatus.scores[i]) {
                                pred_true = false;
                                break;
                            }
                        }
                        break;
                    case LOSING_PRED:
                        for (uint8_t i_team = 0; i_team < 4; i_team++) {
                            if (myStatus.scores[i_team] > myStatus.scores[i]) {
                                pred_true = false;
                                break;
                            }
                        }
                        break;
                    // Is a particular plant's row (or all rows) zombie-free?
                    case EMPTY_PRED:
                        myrow=7;
                        if (mycmd->has_plant_pred == true) {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getID() == mycmd->plant_pred) {
                                    myrow = myPlants[i][p].getRow();
                                    break;
                                }
                            }
                            for (uint16_t z = 0; z < myZombies[i].size(); z++) {
                                if ((myZombies[i][z].getRow() == myrow) && ((myZombies[i][z].getStatus() == ZOMBIE_STATUS_ACTIVE) || (myZombies[i][z].getStatus() == ZOMBIE_STATUS_EATING))) {
                                    pred_true = false;
                                    break;
                                }
                            }

                        }
                        else {
                            for (uint16_t z = 0; z < myZombies[i].size(); z++) {
                                if ((myZombies[i][z].getStatus() == ZOMBIE_STATUS_ACTIVE) || (myZombies[i][z].getStatus() == ZOMBIE_STATUS_EATING)) {
                                    pred_true = false;
                                    break;
                                }
                            }

                        }

                        break;
                    // Has a zombie passed us up?
                    case PASSED_PRED:
                        myrow = 5;
                        mycol = 0;
                        pred_true = false;
                        if (mycmd->has_plant_pred == true) {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                if (myPlants[i][p].getID() == mycmd->plant_pred) {
                                    myrow = myPlants[i][p].getRow();
                                    mycol = myPlants[i][p].getCol();
                                    break;
                                }
                            }
                            for (uint16_t z = 0; z < myZombies[i].size(); z++) {
                                if ((myZombies[i][z].getRow() == myrow) && (myZombies[i][z].getCol() < mycol) &&((myZombies[i][z].getStatus() == ZOMBIE_STATUS_ACTIVE) || (myZombies[i][z].getStatus() == ZOMBIE_STATUS_EATING))) {
                                    pred_true = true;
                                    break;
                                }
                            }
                        }
                        else {
                            for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                                myrow = myPlants[i][p].getRow();
                                mycol = myPlants[i][p].getCol();
                                for (uint16_t z = 0; z < myZombies[i].size(); z++) {
                                    if ((myZombies[i][z].getRow() == myrow) && (myZombies[i][z].getCol() < mycol) &&((myZombies[i][z].getStatus() == ZOMBIE_STATUS_ACTIVE) || (myZombies[i][z].getStatus() == ZOMBIE_STATUS_EATING))) {
                                        pred_true = true;
                                        break;
                                    }
                                }
                                if (pred_true == true)
                                    break;
                            }
                        }

                        break;

                }

                // If the predicate was inverse, flip the result
                if (mycmd->inv_pred == true) {
                    pred_true = !pred_true;
                }
            }


            if (pred_true == true) {

                switch (mycmd->cmd) {

                    // For fire commands, we should check if the plant is active first
                    case FIRE_CMD:

                        for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                            if (myPlants[i][p].getID() == mycmd->plant) {
                                if ((myPlants[i][p].getStatus() != PLANT_STATUS_INACTIVE) && (myPlants[i][p].getStatus() != PLANT_STATUS_DEFAULT) && (myPlants[i][p].action_count == 0))  {
                                    myPlants[i][p].fire();
                                    myPlants[i][p].action_count = plantSpeeds[myPlants[i][p].getType()];
                                }
                                break;
                            }
                        }
                        break;

                    // Move commands are similar to fire commands, except we have to do some extra bookkeeping as well
                    case PLACE_CMD:

                        for (uint16_t p = 0; p < myPlants[i].size(); p++) {
                            if (myPlants[i][p].getID() == mycmd->plant) {
                                // Are we ready to move?
                                if ((myPlants[i][p].getStatus() != PLANT_STATUS_INACTIVE) && (myPlants[i][p].action_count == 0)) {
                                    // Is there a free space where we want to move?
                                    if (myGame->plantGrid[i][mycmd->opt[0]-1][mycmd->opt[1]-1] == false) {
                                        myGame->plantGrid[i][mycmd->opt[0]-1][mycmd->opt[1]-1] = true;
                                        bool first_planting = true;
                                        if (myPlants[i][p].getStatus() != PLANT_STATUS_DEFAULT) {
                                            first_planting = false;
                                            myGame->plantGrid[i][myPlants[i][p].getRow()][myPlants[i][p].getCol()] = false;
                                        }
                                        myPlants[i][p].move();
                                        myPlants[i][p].place(i, mycmd->opt[0], mycmd->opt[1]);
                                        if (first_planting == false) {
                                            myPlants[i][p].action_count = PLANT_MOVE_INACTIVE;
                                        }
                                    }
                                }
                                break;
                            }
                        }
                        break;




                    case GOTO_CMD:
                        myTeams[i].cur_cmd = mycmd->opt[0];
                        break;
                    default:
                        break;
                }

            }

            // Advance the next command, after some per-command delay
            if ((mycmd->cmd != GOTO_CMD) || (pred_true == false)) {
                myTeams[i].cur_cmd++;
                if (myTeams[i].cur_cmd >= myTeams[i].cmds.size()) {
                    myTeams[i].cmds_done = true;
                }

            }

        }



        // Main update/sort loop
        for (uint16_t i = 0; i < 4; i++) {
            for (uint16_t j = 0; j < myZombies[i].size(); j++) {
                myZombies[i][j].update();
            }
            for (uint16_t j = 0; j < myPlants[i].size(); j++) {
                myPlants[i][j].update();
            }
            for (uint16_t j = 0; j < myParticles[i].size(); j++) {
                if (myParticles[i][j].isOffscreen()) {
                    myParticles[i].erase(myParticles[i].begin()+j);
                }
                else {
                    myParticles[i][j].update();
                }
            }


            if (framecount % SORT_GAME_FRAMES == 0) {

            /* Sort each zombie based on the custom zombie function */
            std::stable_sort(myZombies[i].begin(), myZombies[i].end());
            /* Sort each plant as well */
            std::stable_sort(myPlants[i].begin(), myPlants[i].end());
            /* Sort each particle as well */
            std::stable_sort(myParticles[i].begin(), myParticles[i].end());
            }

        }

        framecount++;
    }


    /*****************************************************************************
    * Function: Game::endGame()
    * Description: Runs the end game sequence.
    *****************************************************************************/
    void Game::endGame() {


        if (myStatus.end_music == false) {

            myStatus.vol_counter -= 10;
            myMusic[1].setVolume(myStatus.vol_counter);
            if (myStatus.vol_counter == 0) {
                myMusic[1].stop();
                playSound(SFX_WINMUSIC, 100, true);
                myStatus.end_music = true;
                printf("\n\n---------------------------------\n");
                printf("-- Final %s scores\n", EXEC_NAME);
                for (uint8_t i = 0; i < 4; i++) {
                    printf("-- \tTeam %-12s - %5d\n", myTeams[i].name, myStatus.scores[i]);
                }
                printf("---------------------------------\n\n");
            }
        }

        drawWorld();
        drawScoreboard();
        drawMap();

    }


    /*****************************************************************************
    * Function: Game::updateDemo()
    * Description: Updates all the main elements of the game, in demo mode
    *****************************************************************************/
    void Game::updateDemo() {

        for (uint16_t i = 0; i < 4; i++) {
            for (uint16_t j = 0; j < myZombies[i].size(); j++) {
                myZombies[i][j].updateDemo();
            }
            for (uint16_t j = 0; j < myPlants[i].size(); j++) {
                myPlants[i][j].updateDemo();
            }

            if (framecount % SORT_DEMO_FRAMES == 0) {
                /* Sort each zombie based on the custom zombie function */
                std::stable_sort(myZombies[i].begin(), myZombies[i].end());
                /* Sort each plant as well */
                std::stable_sort(myPlants[i].begin(), myPlants[i].end());
            }
        }

        framecount++;

    }

    /*****************************************************************************
    * Function: Game::demoMode()
    * Description: Runs the initial screen pan before the game start.
    *****************************************************************************/
    void Game::demoMode() {

        static float dir = 3.0;
        static float pause_cnt = 0;

        if (myStatus.mode == DEMO_START) {
            myMusic[0].play();
            myStatus.mode = DEMO_MID;
        }

        updateDemo();
        drawWorld();
        drawScoreboard();
        drawMap();


        if (myStatus.mode == DEMO_MID) {

            myStatus.pan_prev = myStatus.pan;
            myStatus.pan += dir;
            if (myStatus.pan >= 352.5) {
                myStatus.pan = 352.5;
                pause_cnt++;
                if (pause_cnt > 50) {
                    dir = -dir;
                    pause_cnt = 0;
                }
            }
            if (myStatus.pan <= -352.5) {
                myStatus.pan = -352.5;
                pause_cnt++;
                if (pause_cnt > 50) {
                    dir = -dir;
                    pause_cnt = 0;
                }
            }
        }
        else {
            myMusic[0].setVolume(100*myStatus.pan/myStatus.pan_prev);
            if (fabs(myStatus.pan) < fabs(1.0*dir)) {
                myStatus.pan = 0.0;
                myStatus.mode = GAME_START;
                myMusic[0].stop();
                myMusic[1].play();
                myClock.restart();
                framecount = 0;
                /* Relocate the zombies so that they start using game_x, and game_y */
                for (uint8_t i = 0; i < 4; i++) {
                    for (uint16_t j = 0; j < myZombies[i].size(); j++) {
                        myZombies[i][j].endDemo();
                    }
                    for (uint16_t j = 0; j < myPlants[i].size(); j++) {
                        myPlants[i][j].endDemo();
                    }
                }
            }
            else if (myStatus.pan < 0.0) {
                myStatus.pan += fabs(dir);
            }
            else {
                myStatus.pan -= fabs(dir);
            }
        }


    }


    /*****************************************************************************
    * Function: Game::compileTeams()
    * Description: Compiles the 4 team .fpl files to initialize commands and
    * other per-team information
    *****************************************************************************/
    void Game::compileTeams() {
        fvu::Team *team;
        FILE *team_file;

        for (uint8_t i_team = 0; i_team < 4; i_team++) {

            /* Open and compile the team file */
            if (myConfig.debug_level > 1) {
                printf("Compiling team file %s\n", myConfig.team_fname[i_team]);
            }

            team_file = fopen(myConfig.team_fname[i_team], "r");
            if (!team_file) {
                raise_error(ERR_NOFILE2, myConfig.team_fname[i_team]);
            }

            myStatus.scores[i_team] = 0;
            team = &myTeams[i_team];

            // Copy the file name, removing the .fpl extension
            team->name = (char *)calloc(strlen(myConfig.team_fname[i_team])-3, sizeof(char));
            if (!team->name) {
                raise_error(ERR_NOMEM, (char *)"team->name");
            }
            strncpy(team->name, myConfig.team_fname[i_team], strlen(myConfig.team_fname[i_team])-4);
            strlower(team->name);


            bool select_done = false;
            bool start_done = false;
            for (uint32_t line_count=1; !feof(team_file); line_count++) {
                char *fgets_ret;
                char linebuf[256], select_str1[16], select_str2[16], start_str[256];
                char place_str[16], label_str[256], target_str[256];
                char cmd_str[256], cmd_str2[256], pred_str[256];
                uint8_t select_ntok, start_ntok, place_ntok, label_ntok, pred_ntok, cmd_ntok;
                uint16_t select_tok, place_tok1, place_tok2, place_tok3;

                fgets_ret = fgets(linebuf, 256, team_file);
                select_str1[0] = 0;select_str2[0] = 0;

                strlower(linebuf);
                // If we have a '#', the line is a comment and we can skip it
                // We can also skip blank lines

                if ((fgets_ret == NULL) || (linebuf == 0) || (linebuf[0] == '#') ||
                    (linebuf[0] == 10) || (linebuf[0] == 13)) {
                    continue;
                }


                bool select_match = false;
                bool plant_match = false;
                fvu::Plant *local_plant;
                // Select instructions are first.
                select_ntok = sscanf(linebuf, " %s p%hu.%s", select_str1, &select_tok, select_str2);
                if (select_ntok == 3) {

                    for (uint8_t i = 0; i < NUM_CMD_SPELLINGS; i++) {
                        if (!strcmp(select_str1, cmdNames[SELECT_CMD][i].c_str())) {
                            select_match = true;
                            break;
                        }
                    }

                    // Selection commands have to come before everything else
                    if ((select_match == true) && (select_done == true)) {
                        printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                        printf("  cannot select plants at run-time\n");
                        raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                    }


                    // We should have matched a valid select command at this point
                   /* if ((select_match == false) && (select_done == false)) {
                        printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                        printf("  select command is invalid\n");
                        printf("  command was %s\n", linebuf);
                        raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                    }*/
                    if (select_match == false) {
                        goto L1;
                    }

                    // It's a valid command, let's see what we selected
                    for (uint8_t p = 0; p < NUM_PLANT_TYPE; p++) {
                        uint8_t i;
                        for (i = 0; i < NUM_PLANT_SPELLINGS; i++) {
                            if (!strcmp(select_str2, plantNames[p][i].c_str())) {
                                plant_match = true;
                                break;
                            }
                        }
                        if (plant_match == true) {

                            // Did we duplicate a plant ID?
                            for (uint16_t p2 = 0; p2 < myPlants[i_team].size(); p2++) {
                                if (myPlants[i_team][p2].getID() == select_tok) {
                                    printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                                    printf("  duplicate plant ID - p%hu was already selected\n", select_tok);
                                    raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                                }
                            }

                            myTeams[i_team].budget += plantCosts[p];
                            if (myTeams[i_team].budget > myStatus.budget) {
                                printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                                printf("  team is over budget\n");
                                printf("  budget limit is %hu\n", myStatus.budget);
                                raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                            }
                            local_plant = new Plant(p, select_tok);
                            // We overloaded the portal type to specify different target teams
                            if (p == PORTAL_PLANT) {
                                if ((i == 0) || (i == 1)) { // H_PORTAL
                                    local_plant->dest_team = (i_team + 2) % 4;
                                }
                                else if (i == 2) { // V_PORTAL
                                    if ((i_team % 2) == 1) {
                                        local_plant->dest_team = i_team - 1;
                                    }
                                    else {
                                        local_plant->dest_team = i_team + 1;
                                    }
                                }
                                else { // D_PORTAL
                                    switch (i_team) {
                                        case 0:
                                        default:
                                            local_plant->dest_team = 3;
                                            break;
                                        case 1:
                                            local_plant->dest_team = 2;
                                            break;
                                        case 2:
                                            local_plant->dest_team = 1;
                                            break;
                                        case 3:
                                            local_plant->dest_team = 0;
                                            break;
                                    }
                                }
                            }
                            myPlants[i_team].push_back(*local_plant);
                            delete local_plant;
                            break;
                        }
                    }

                    if (plant_match == false) {
                        printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                        printf("  invalid plant type\n");
                        raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                    }
                }


                // If we didn't match a select, we must be done with that command-type
                if (select_match == true) {
                    continue;
                }

                L1: select_done = true;

                // Until we reach the start line, we do not support predicates
                fvu::cmd_type *local_cmd;
                bool place_match = false;
                start_str[0] = 0;
                start_ntok = 0;
                if (start_done == false) {
                    start_ntok = sscanf(linebuf, " start: %[^\t\n]", start_str);
                    if (start_ntok == 1) {
                        start_done = true;
                    }
                    else {

                    // If we haven't started yet, every command should be a place/move
                    place_str[0] = 0;
                    place_ntok = sscanf(linebuf, " %s p%hu %hu, %hu", place_str, &place_tok1, &place_tok2, &place_tok3);
                    for (uint8_t i = 0; i < NUM_CMD_SPELLINGS; i++) {
                        if (!strcmp(place_str, cmdNames[PLACE_CMD][i].c_str())) {
                            place_match = true;
                            break;
                        }
                    }
                    if ((place_match == false) || (place_ntok != 4)) {
                        printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                        printf("  before Start:, all commands must be place/move\n");
                        printf("  command was %s\n", linebuf);
                        raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                    }
                    else {
                        place_match = false;
                        for (uint16_t p = 0; p < myPlants[i_team].size(); p++) {
                            if (myPlants[i_team][p].getID() == place_tok1) {
                                place_match = true;
                                if ((place_tok2 == 1) && (place_tok3 == 1)) {
                                    printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                                    printf("  place command [%hu, %hu] is reserved\n", place_tok2, place_tok3);
                                    raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                                }
                                if ((place_tok2 < 1) || (place_tok2 > NUM_ROWS) || (place_tok3 < 1) || (place_tok3 > NUM_COLS)) {
                                    printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                                    printf("  place command [%hu, %hu] out of range\n", place_tok2, place_tok3);
                                    raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                                }

                                if (plantGrid[i_team][place_tok2-1][place_tok3-1] == true) {
                                    printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                                    printf("  plant already placed at [%hu, %hu]\n", place_tok2, place_tok3);
                                    raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                                }
                                myPlants[i_team][p].place(i_team, place_tok2, place_tok3);
                                plantGrid[i_team][place_tok2-1][place_tok3-1] = true;
                                break;
                            }
                        }
                        if (place_match == false) {
                            printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                            printf("  attempt to place non-existent plant\n");
                            raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                        }
                    }

                    continue;
                    }
                }


                // Everything from here on out is a proper command, with potentially labels and predicates
                cmd_str[0] = 0;label_str[0] = 0;
                bool has_label = false;
                label_ntok = sscanf(linebuf, " %[^:]: %[^\t\n]", label_str, cmd_str);
                if (label_ntok == 2) {
                    has_label = true;
                }
                else {
                    sscanf(linebuf, " %[^\t\n]", cmd_str);
                }

                // Check for an if statement
                cmd_str2[0] = 0;
                pred_str[0] = 0;
                bool has_pred = false;
                bool inv_pred = false;
                bool has_plant_pred = false;
                uint16_t pred_tok;
                uint16_t plant_pred = 0;
                pred_ntok = sscanf(cmd_str, " if not p%hu.%[^,], %[^\t\n]", &pred_tok, pred_str, cmd_str2);
                if (pred_ntok == 3) {
                    has_pred = true;
                    has_plant_pred = true;
                    plant_pred = pred_tok;
                    inv_pred = true;
                }
                else {
                    pred_ntok = sscanf(cmd_str, " if p%hu.%[^,], %[^\t\n]", &pred_tok, pred_str, cmd_str2);
                    if (pred_ntok == 3) {
                        has_pred = true;
                        has_plant_pred = true;
                        plant_pred = pred_tok;
                        inv_pred = false;
                    }
                    else {
                        pred_ntok = sscanf(cmd_str, " if not %[^,], %[^\t\n]", pred_str, cmd_str2);
                        if (pred_ntok == 2) {
                            has_pred = true;
                            inv_pred = true;
                        }
                        else {
                            pred_ntok = sscanf(cmd_str, " if %[^,], %[^\t\n]", pred_str, cmd_str2);
                            if (pred_ntok == 2) {
                                has_pred = true;
                                inv_pred = false;
                            }
                            else {
                                sscanf(cmd_str, " %[^\t\n]", cmd_str2);
                            }
                        }
                    }
                }
                // If there is a predicate, check if the condition is valid
                bool pred_match = false;
                uint8_t pred = ALWAYS_PRED;
                if (has_pred == true) {
                    for (pred = 0; pred < NUM_PRED_TYPES; pred++) {
                        for (uint8_t i = 0; i < NUM_PRED_SPELLINGS; i++) {
                            if (!strcmp(pred_str, predNames[pred][i].c_str())) {
                                pred_match = true;
                                break;
                            }
                        }
                        if (pred_match == true) {
                            break;
                        }
                    }
                    if (pred_match == false) {
                        printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                        printf("  invalid condition of %s\n", pred_str);
                        raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                    }
                }

                // Now we can attempt to match the actual string
                uint8_t cmd;
                bool cmd_match = false;
                for (cmd = 0; cmd < NUM_CMD_TYPES; cmd++) {
                    for (uint8_t i = 0; i < NUM_CMD_SPELLINGS; i++) {
                        if (!strncmp(cmd_str2, cmdNames[cmd][i].c_str(), cmdNames[cmd][i].size())) {
                            cmd_match = true;
                            break;
                        }
                    }
                    if (cmd_match == true) {
                        break;
                    }
                }
                if (cmd_match == false) {
                    printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                    printf("  invalid command of %s\n", cmd_str2);
                    raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                }

                // Based on the command we matched, we can grab the target of the command.
                // We know we spelled the command correctly at this point so just grab everything else
                bool valid_cmd = false;
                uint16_t opt[2] = {0, 0};
                uint16_t plant = 0;
                switch (cmd) {
                  case PLACE_CMD:
                  default:
                    place_str[0] = 0;
                    cmd_ntok = sscanf(cmd_str2, "%s p%hu %hu, %hu", place_str, &plant, &opt[0], &opt[1]);
                    if (cmd_ntok == 4) {
                        valid_cmd = true;
                    }
                    break;
                  case FIRE_CMD:
                    place_str[0] = 0;
                    cmd_ntok = sscanf(cmd_str2, "%s p%hu", place_str, &plant);
                    if (cmd_ntok == 2) {
                        valid_cmd = true;
                    }
                    break;
                  case GOTO_CMD:
                    place_str[0] = 0;
                    target_str[0] = 0;
                    cmd_ntok = sscanf(cmd_str2, "%s %s", place_str, target_str);
                    if (cmd_ntok == 2) {
                        // Note that goto is not necessarily valid, depending on the target string which we can only check later
                        valid_cmd = true;
                    }
                    break;
                }

                if (valid_cmd == false) {
                    printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                    printf("  invalid command options of %s\n", cmd_str2);
                    raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                }

                // FINALLY, we can allocate and add a new command to the cmds structure
                local_cmd = new cmd_type;
                local_cmd->has_label = has_label;
                if (has_label == true)
                    strncpy(local_cmd->label_str, label_str, 16);
                if (cmd == GOTO_CMD)
                    strncpy(local_cmd->target_str, target_str, 16);
                local_cmd->inv_pred = inv_pred;
                local_cmd->has_pred = has_pred;
                local_cmd->has_plant_pred = has_plant_pred;
                local_cmd->plant_pred = plant_pred;
                local_cmd->cmd = cmd;
                local_cmd->pred = pred;
                local_cmd->plant = plant;
                local_cmd->line = line_count;
                local_cmd->opt[0] = opt[0];
                local_cmd->opt[1] = opt[1];

                myTeams[i_team].cmds.insert(myTeams[i_team].cmds.end(), 1, *local_cmd);
                delete local_cmd;

            }


            // Only after the file is compiled can we do a second pass and verify the target strings and plant targets
            for (uint16_t i = 0; i < myTeams[i_team].cmds.size(); i++) {

                if (myTeams[i_team].cmds[i].has_plant_pred == true) {
                    bool plant_pred_match = false;
                    for (uint16_t p = 0; p < myPlants[i_team].size(); p++) {
                        if (myPlants[i_team][p].getID() == myTeams[i_team].cmds[i].plant_pred) {
                            plant_pred_match = true;
                            break;
                        }
                    }
                    if (plant_pred_match == false) {
                        printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], myTeams[i_team].cmds[i].line);
                        printf("  command predicate plant target of p%hu not found\n", myTeams[i_team].cmds[i].plant_pred);
                        raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                    }
                }

                if (myTeams[i_team].cmds[i].cmd == GOTO_CMD) {
                    bool goto_valid = false;
                    for (uint16_t j = 0; j < myTeams[i_team].cmds.size(); j++) {
                        if (myTeams[i_team].cmds[j].has_label == true) {
                            if (!strcmp(myTeams[i_team].cmds[j].label_str, myTeams[i_team].cmds[i].target_str)) {
                                myTeams[i_team].cmds[i].opt[0] = j;
                                goto_valid = true;
                                break;
                            }
                        }
                    }
                    if (goto_valid == false) {
                        printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], myTeams[i_team].cmds[i].line);
                        printf("  goto target of %s not found\n", myTeams[i_team].cmds[i].target_str);
                        raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                    }
                }
                else {
                    bool plant_match = false;
                    for (uint16_t p = 0; p < myPlants[i_team].size(); p++) {
                        if (myPlants[i_team][p].getID() == myTeams[i_team].cmds[i].plant) {
                            plant_match = true;
                            break;
                        }
                    }
                    if (plant_match == false) {
                        printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], myTeams[i_team].cmds[i].line);
                        printf("  plant target of p%hu not found\n", myTeams[i_team].cmds[i].plant);
                        raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                    }

                    // Let's catch any invalid place commands now
                    if (myTeams[i_team].cmds[i].cmd == PLACE_CMD) {
                            bool place_valid = true;
                            if ((myTeams[i_team].cmds[i].opt[0] == 1) && (myTeams[i_team].cmds[i].opt[1] == 1))
                                place_valid = false;
                            if ((myTeams[i_team].cmds[i].opt[0] <= 0) || (myTeams[i_team].cmds[i].opt[0] >= 6))
                                place_valid = false;
                            if ((myTeams[i_team].cmds[i].opt[1] <= 0) || (myTeams[i_team].cmds[i].opt[1] >= 11))
                                place_valid = false;

                            if (place_valid == false) {
                                printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], myTeams[i_team].cmds[i].line);
                                printf("  place target of p%hu (%hu, %hu) is invalid\n", myTeams[i_team].cmds[i].plant, myTeams[i_team].cmds[i].opt[0], myTeams[i_team].cmds[i].opt[1]);
                                printf("  valid range for placement is ([1-5], [1-10]), and (1, 1) is also not allowed\n");
                                raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                            }
                    }

                }
            }



            // Third pass, have we duplicated any labels?
            for (uint16_t i = 0; i < myTeams[i_team].cmds.size(); i++) {
                if (myTeams[i_team].cmds[i].has_label == true) {
                    for (uint16_t j = 0; j < myTeams[i_team].cmds.size(); j++) {
                        if (i == j) continue;
                        if (myTeams[i_team].cmds[j].has_label == true) {
                            if (!strcmp(myTeams[i_team].cmds[i].label_str, myTeams[i_team].cmds[j].label_str)) {
                                printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], myTeams[i_team].cmds[j].line);
                                printf("  duplicate command label of %s\n", myTeams[i_team].cmds[i].label_str);
                                raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                            }
                        }
                    }
                }
            }


            if (myConfig.debug_level > 50) {
                printf("\nCommands are as follows: \n");
                printf("   ID    | LINE |   LABEL   | IF? | NOT? |   PRED   |   CMD   | PLNT | OPTS  \n");
                for (uint16_t i = 0; i < myTeams[i_team].cmds.size(); i++) {
                    printf("cmd-%03u: | ", i);
                    printf(" %3u | ", myTeams[i_team].cmds[i].line);
                    if (myTeams[i_team].cmds[i].has_label == true)
                        printf("%9s | ", myTeams[i_team].cmds[i].label_str);
                    else
                        printf("%9s | ", "none");
                    if (myTeams[i_team].cmds[i].has_pred == true) {
                        if (myTeams[i_team].cmds[i].has_plant_pred == true)
                            printf("%3hu | ", myTeams[i_team].cmds[i].plant_pred);
                        else
                            printf("%3s | ", "yes");
                    }
                    else {
                        printf("%3s | ", "no");
                    }
                    if (myTeams[i_team].cmds[i].inv_pred == true)
                        printf("%4s | ", "yes");
                    else
                        printf("%4s | ", "no");

                    printf("%8s | ", predNames[myTeams[i_team].cmds[i].pred][0].c_str());
                    printf("%7s | ", cmdNames[myTeams[i_team].cmds[i].cmd][0].c_str());
                    printf("%4hu | ", myTeams[i_team].cmds[i].plant);
                    printf("[%hu, %hu]", myTeams[i_team].cmds[i].opt[0], myTeams[i_team].cmds[i].opt[1]);
                    printf("\n");
                }
                printf("\n");
            }



        }


    }

    /*****************************************************************************
    * Function: Game::compileZombies()
    * Description: Compiles the .zom file to initialize zombie commands
    *****************************************************************************/
    void Game::compileZombies() {

        FILE *zom_file;
        char linebuf[256], select_str[16];
        char *fgets_ret;
        uint8_t budget_ntok, select_ntok, place_ntok, day_ntok, night_ntok, time_ntok;
        bool budget_flag;
        uint32_t line_count, zombie_counter;
        uint16_t zombie_index;
        uint16_t budget_tok, select_tok, music_tok;
        uint32_t time_tok;
        int16_t place_tok, delay_tok;

        /* Open the zombie file */
        if (myConfig.debug_level > 1) {
            printf("Compiling zombie file %s\n", myConfig.zom_fname);
        }

        zom_file = fopen(myConfig.zom_fname, "r");
        if (!zom_file) {
            raise_error(ERR_NOFILE3, myConfig.zom_fname);
        }

        budget_flag = false;
        zombie_counter = 0;zombie_index = 0;
        myStatus.time_ms = TIME_MS_DEFAULT;
        myStatus.day = true;
        myStatus.main_song = 1;

        for (line_count=1; !feof(zom_file); line_count++) {
            fgets_ret = fgets(linebuf, 256, zom_file);
            select_str[0] = 0;

            // If we have a '#', the line is a comment and we can skip it
            // We can also skip blank lines
            if ((!fgets_ret) || (linebuf == 0) || (linebuf[0] == '#') ||
                (linebuf[0] == 10) || (linebuf[0] == 13)) {
                continue;
            }

            // Check for team budget information
            strlower(linebuf);
            budget_ntok = sscanf(linebuf, " budget %hu", &budget_tok);

            // Check for day/night and music information
            strlower(linebuf);
            day_ntok = sscanf(linebuf, " day %hu", &music_tok);
            if (day_ntok != 1)
                night_ntok = sscanf(linebuf, " night %hu", &music_tok);

            // Check for time information
            strlower(linebuf);
            time_ntok = sscanf(linebuf, " time %u", &time_tok);

            // Check for select information. Two matches are needed
            select_ntok = sscanf(linebuf, " select %s %hu", select_str, &select_tok);

            // Check for place information. Two matches are needed
            place_ntok = sscanf(linebuf, " place %hd, %hd", &place_tok, &delay_tok);
            if (place_ntok != 2)
                place_ntok = sscanf(linebuf, " place %hd,%hd", &place_tok, &delay_tok);


            // A valid file has to match a command each line
            if (budget_ntok == 1) {
                budget_flag = true;
                myStatus.budget = budget_tok;
            }
            else if (day_ntok == 1) {
                myStatus.main_song = music_tok;
                myStatus.day = true;
            }
            else if (night_ntok == 1) {
                myStatus.main_song = music_tok;
                myStatus.day = false;
            }
            else if (time_ntok == 1) {
                myStatus.time_ms = time_tok;
            }

            else if (select_ntok == 2) {

                Zombie *local_zombie = NULL;
                bool zombie_match = false;
                uint8_t z;
                for (z = REGULAR_ZOMBIE; z < NUM_ZOMBIE_TYPE; z++) {
                    for (uint8_t j = 0; j < NUM_ZOMBIE_SPELLINGS; j++) {
                        if (!strcmp(select_str, zombieNames[z][j].c_str())) {
                            zombie_match = true;
                        }
                    }
                    if (zombie_match == true) {
                        break;
                    }
                }

                // Each zombie is specified for all 4 teams at once
                if (zombie_match == true) {
                    for (uint8_t l = 0; l < select_tok; l++) {
                        for (uint8_t k = 0; k < 4; k++) {
                            local_zombie = new Zombie(z, zombie_index);
                            myZombies[k].insert(myZombies[k].end(), 1, *local_zombie);
                            delete local_zombie;
                        }
                        zombie_index++;
                    }
                }
                else {
                    printf("Error compiling %s, line %d\n", myConfig.zom_fname, line_count);
                    printf("  Invalid zombie type");
                    raise_error(ERR_BADFILE3, myConfig.zom_fname);
                }
            }

            else if (place_ntok == 2) {
                if ((zombie_counter) >= myZombies[0].size()) {
                    printf("Error compiling %s, line %d\n", myConfig.zom_fname, line_count);
                    printf("  Cannot place zombies that haven't been selected\n");
                    printf("  select zombie count - %d, place zombie request - %d", myZombies[0].size(), zombie_counter+1);
                    raise_error(ERR_BADFILE3, myConfig.zom_fname);
                }
                if (place_tok > 4) {
                    printf("Error compiling %s, line %d\n", myConfig.zom_fname, line_count);
                    printf("  Valid zombie placement is in range [0, 4]\n");
                    raise_error(ERR_BADFILE3, myConfig.zom_fname);
                }
                for (uint8_t i = 0; i < 4; i++) {
                    myZombies[i][zombie_counter].place(place_tok, 1.0*delay_tok, i);
                }
                zombie_counter++;
            }
            else if ((place_ntok > 2) || (place_ntok < 2)) {
                printf("Error compiling %s, line %d\n", myConfig.zom_fname, line_count);
                printf("  place command requires 2 arguments (lane, delay)");
                printf("  current command is \'%s\'", linebuf);
                raise_error(ERR_BADFILE3, myConfig.zom_fname);
            }
            else if ((select_ntok > 2) || (select_ntok < 2)) {
                printf("Error compiling %s, line %d\n", myConfig.zom_fname, line_count);
                printf("  select command requires 2 arguments (Zombie name, count)");
                printf("  current command is \'%s\'", linebuf);
                raise_error(ERR_BADFILE3, myConfig.zom_fname);
            }
            else {
                printf("Error compiling %s, line %d\n", myConfig.zom_fname, line_count);
                printf("  Unknown command \'%s\'", linebuf);
                raise_error(ERR_BADFILE3, myConfig.zom_fname);
            }
        }

        // Check for required values
        if (budget_flag != true) {
            printf("Error compiling %s\n", myConfig.zom_fname);
            printf("  No budget specification\n");
            raise_error(ERR_BADFILE3, myConfig.zom_fname);
        }


        // We are done, get rid of the Zombies we didn't place.
        for (uint8_t i = 0; i < 4; i++) {
            myZombies[i].resize(zombie_counter, myZombies[i][0]);
        }
        fclose(zom_file);


        if (myConfig.debug_level > 50) {
            printf("\nCommands are as follows: \n");
            printf("   ID    |   TYPE  | LANE  |  DELAY |  \n");
            for (uint16_t i = 0; i < myZombies[0].size(); i++) {
                printf("zom-%03u: | ", myZombies[0][i].getIndex());
                printf(" %3u    |", myZombies[0][i].getType());
                printf("   %u   |", myZombies[0][i].getRow());
                printf("  %4.0f  |", myZombies[0][i].getDelay());
                printf("\n");
            }
            printf("\n");
        }


        return;
    }


    /*****************************************************************************
    * Function: Game::Game()
    * Description: Constructor for fvu::Game() class. Reads in and parses the
    * input command line in order to set configuration data
    *****************************************************************************/
    Game::Game(int argc, char **argv) {
        int8_t i, nfiles=1;

        /* We require at least two arguments on the command line */
        if (argc < 2) {
            raise_error(ERR_USAGE, NULL);
        }

        /* If the last command-line argument is -h or --help that is ok */
        if (!strncmp(argv[argc-1], "-h", 2) || !strncmp(argv[argc-1], "--help", 6)) {
            print_help();
        }

        /* Otherwise, either the last or the last four arguments should be files */
        if (argv[argc-1][0] == '-') {
            raise_error(ERR_USAGE, NULL);
        }
        else {
            if (argc > 2) {
                if ((argv[argc-2][0] == '-') || (argv[argc-3][0] == '-')) {
                    nfiles = 1;
                }
                else {
                    if (argc < 5) {
                        raise_error(ERR_USAGE, NULL);
                    }
                    else if ((argv[argc-2][0] == '-') || (argv[argc-3][0] == '-') ||
                             (argv[argc-4][0] == '-')) {
                        raise_error(ERR_USAGE, NULL);
                    }
                    else {
                        nfiles = 4;
                    }
                }
            }
            else {
                nfiles = 1;
            }
        }

        /* Set the default values */
        myConfig.debug_level = DEBUG_DEFAULT;
        myConfig.screen_height = SCREEN_HEIGHT_DEFAULT;
        myConfig.screen_width = SCREEN_WIDTH_DEFAULT;
        myConfig.screen_depth = SCREEN_DEPTH_DEFAULT;
        myConfig.zom_fname = (char *)malloc(strlen(ZOM_FNAME_DEFAULT)+1);

        if (!myConfig.zom_fname) {
            raise_error(ERR_NOMEM, NULL);
        }
        strcpy(myConfig.zom_fname, ZOM_FNAME_DEFAULT);
        for (i = 0; i < 4; i++) {
            myConfig.team_fname[i] = (char *)malloc(strlen(TEAM_FNAME_DEFAULT)+1);
            if (!myConfig.team_fname[i]) {
                raise_error(ERR_NOMEM, NULL);
            }
            strcpy(myConfig.team_fname[i], TEAM_FNAME_DEFAULT);
        }

        for (i = 1; i < argc-nfiles; i++) {
            /* Arguments must begin with '-' or '--' */
            if (argv[i][0] != '-' || strlen(argv[i]) < 2) {
                raise_error(ERR_USAGE, NULL);
            }

            /* Decode the single dash arguments */
            if (argv[i][1] != '-') {
                /* Single dash arguments should have just one character after the dash */
                if (strlen(argv[i]) > 2) {
                    raise_error(ERR_USAGE, NULL);
                }
                switch (argv[i][1]) {
                    case 'H':
                        i++;
                        sscanf(argv[i], "%hu", &myConfig.screen_height);
                        break;
                    case 'W':
                        i++;
                        sscanf(argv[i], "%hu", &myConfig.screen_width);
                        break;
                    case 'D':
                        i++;
                        sscanf(argv[i], "%d", &myConfig.debug_level);
                        break;
                    case 'Z':
                    case 'M':
                        i++;
                        myConfig.zom_fname = (char *)realloc(myConfig.zom_fname, strlen(argv[i])+1);
                        if (!myConfig.zom_fname) {
                            raise_error(ERR_NOMEM, NULL);
                        }
                        strcpy(myConfig.zom_fname, argv[i]);
                        break;
                    case 'h':
                        print_help();
                        break;
                    default:
                        raise_error(ERR_USAGE, NULL);
                        break;
                }
            }

            /* Decode the double dash arguments */
            else {
                if (!strncmp(argv[i], "--height", 8)) {
                    i++;
                    sscanf(argv[i], "%hu", &myConfig.screen_height);
                }
                else if (!strncmp(argv[i], "--width", 7)) {
                    i++;
                    sscanf(argv[i], "%hu", &myConfig.screen_width);
                }
                else if (!strncmp(argv[i], "--debug", 7)) {
                    i++;
                    sscanf(argv[i], "%d", &myConfig.debug_level);
                }
                else if (!strncmp(argv[i], "--zom", 5)) {
                    i++;
                    myConfig.zom_fname = (char *)realloc(myConfig.zom_fname, strlen(argv[i])+1);
                    if (!myConfig.zom_fname) {
                        raise_error(ERR_NOMEM, NULL);
                    }
                    strcpy(myConfig.zom_fname, argv[i]);
                }
                else if (!strncmp(argv[i], "--help", 6)) {
                    print_help();
                }
                else {
                    raise_error(ERR_USAGE, NULL);
                }
            }
        }

        /* The last nfiles command arguments are the input file names */
        for (i = argc-nfiles; i < argc; i++) {
            myConfig.team_fname[i-(argc-nfiles)] =
                (char *)realloc(myConfig.team_fname[i-(argc-nfiles)], strlen(argv[i])+1);
            if (!myConfig.team_fname[i-(argc-nfiles)]) {
                raise_error(ERR_NOMEM, NULL);
            }
            strcpy(myConfig.team_fname[i-(argc-nfiles)], argv[i]);
        }

        return;
    }


    /*****************************************************************************
    * Function: Game::~Game()
    * Description: Destructor for fvu::Game() class
    *****************************************************************************/
    Game::~Game() {
    }

} //namespace fvu
