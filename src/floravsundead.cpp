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
        for (uint8_t i = 0; i < 4; i++) {
            myStatus.scores[i] = 0;
            myTeams[i].status = 0;
            myTeams[i].timer_ms = 0;
            myTeams[i].zombie_index = 0;
            myTeams[i].cur_cmd = 0;
            myTeams[i].zombies_done = false;
            myTeams[i].cmds_done = false;
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
        }

        init();
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
                    myTime = myClock.getElapsedTime();
                    myStatus.time_ms -= myTime.asMilliseconds();
                    if (myStatus.time_ms <= 0) {
                        myStatus.time_ms = 0;
                        myStatus.mode = GAME_END;
                    }
                    myClock.restart();
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

        static bool firstZombie = true;
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


        // Zombie loo: grab the next zombie for each team. Note that they are sorted
        // graphically, so we have to perform a linear search based on zombie_index
        for (uint16_t i = 0; i < 4; i++) {

            // If we're done with zombies, continue
            if (myTeams[i].zombies_done == true) {
                continue;
            }


            // If we have no more zombies left to send, we can stop updating zombies once they're all inactive/winning
            if (myTeams[i].zombie_index >= myZombies[i].size()) {
                bool zombies_done = true;
                for (uint16_t k = 0; k < myZombies[i].size(); k++) {
                    if ((myZombies[i][k].getStatus() != ZOMBIE_STATUS_INACTIVE) && (myZombies[i][k].getStatus() != ZOMBIE_STATUS_WINNING)) {
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
            if (myZombies[i][j].getStatus() != ZOMBIE_STATUS_GAME) {
                continue;
            }
            // A delay of -1 means we wait for all previous zombies to be
            // inactive
            bool sendZombie = true;
            if (myZombies[i][j].getDelay() == -1) {
                for (uint16_t k = 0; k < myZombies[i].size(); k++) {
                    if ((myZombies[i][k].getStatus() != ZOMBIE_STATUS_GAME) && (myZombies[i][k].getStatus() != ZOMBIE_STATUS_INACTIVE) && (myZombies[i][k].getStatus() != ZOMBIE_STATUS_WINNING)) {
                        sendZombie = false;
                        break;
                    }
                }
            }
            // otherwise, count down until we our individual zombie delay is <= 0
            else {
                sendZombie = false;
                int16_t mydelay = myZombies[i][j].getDelay();
                mydelay -= myClock.getElapsedTime().asMilliseconds();
                if (mydelay <= 0) {
                    mydelay = 0;
                    sendZombie = true;
                }
                myZombies[i][j].setDelay(mydelay);

            }

            if (sendZombie == true) {

                myZombies[i][j].setStatus(ZOMBIE_STATUS_ACTIVE);
                myTeams[i].zombie_index++;
                if (firstZombie == true) {
                    mySounds[myStatus.music_buffer].setBuffer(mySoundBuffers[SFX_AWOOGA]);
                    mySounds[myStatus.music_buffer].play();
                    myStatus.music_buffer++;
                    myStatus.music_buffer %= NUM_SOUNDS;
                    firstZombie = false;
                }
                // Play a sound for those "final wave" zombies
                else if (myZombies[i][j].getDelay() == -1) {
                    mySounds[myStatus.music_buffer].setBuffer(mySoundBuffers[SFX_FINALWAVE]);
                    mySounds[myStatus.music_buffer].play();
                    myStatus.music_buffer++;
                    myStatus.music_buffer %= NUM_SOUNDS;
                }


            }

        }



        // Command loop: grab the next cmd for each team.
        for (uint16_t i = 0; i < 4; i++) {

            if (myTeams[i].cmds_done == true) {
                continue;
            }


            bool pred_true = false;

            // Evaluate predicate, to determine if command should even be executed
            pred_true = true;

            if (pred_true == true) {

            }

            // Advance the next command, after some per-command delay
            myTeams[i].cur_cmd++;
            if (myTeams[i].cur_cmd >= myTeams[i].cmds.size()) {
                myTeams[i].cmds_done = true;
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

            /* Sort each zombie based on the custom zombie function */
            std::stable_sort(myZombies[i].begin(), myZombies[i].end());
            /* Sort each plant as well */
            std::stable_sort(myPlants[i].begin(), myPlants[i].end());
        }
    }


    /*****************************************************************************
    * Function: Game::endGame()
    * Description: Runs the end game sequence.
    *****************************************************************************/
    void Game::endGame() {

        static bool end_music = false;
        static uint8_t vol_counter = 110;

        if (end_music == false) {

            vol_counter -= 10;
            myMusic[0].setVolume(vol_counter);
            if (vol_counter == 0) {
                myMusic[1].stop();
                mySounds[myStatus.music_buffer].setBuffer(mySoundBuffers[SFX_WINMUSIC]);
                mySounds[myStatus.music_buffer].play();
                myStatus.music_buffer++;
                myStatus.music_buffer %= NUM_SOUNDS;
                end_music = true;
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

            /* Sort each zombie based on the custom zombie function */
            std::stable_sort(myZombies[i].begin(), myZombies[i].end());
            /* Sort each plant as well */
            std::stable_sort(myPlants[i].begin(), myPlants[i].end());
        }

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
            team->name = (char *)calloc(9, sizeof(char));
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

                    // Selection commands have to come before everything else
                    if (select_done == true) {
                        printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                        printf("  cannot select plants at run-time\n");
                        raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                    }

                    for (uint8_t i = 0; i < NUM_CMD_SPELLINGS; i++) {
                        if (!strcmp(select_str1, cmdNames[SELECT_CMD][i].c_str())) {
                            select_match = true;
                            break;
                        }
                    }

                    // We should have matched a valid select command at this point
                    if (select_match == false) {
                        printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                        printf("  select command is invalid\n");
                        printf("  command was %s\n", linebuf);
                        raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                    }

                    // It's a valid command, let's see what we selected
                    for (uint8_t p = 0; p < NUM_PLANT_TYPE; p++) {
                        for (uint8_t i = 0; i < NUM_PLANT_SPELLINGS; i++) {
                            if (!strcmp(select_str2, plantNames[p][i].c_str())) {
                                plant_match = true;
                                break;
                            }
                        }
                        if (plant_match == true) {
                            myTeams[i_team].budget += plantCosts[p];
                            if (myTeams[i_team].budget > myStatus.budget) {
                                printf("Error compiling %s, line %d\n", myConfig.team_fname[i_team], line_count);
                                printf("  team is over budget\n");
                                printf("  budget limit is %hu\n", myStatus.budget);
                                raise_error(ERR_BADFILE2, myConfig.team_fname[i_team]);
                            }
                            local_plant = new Plant(p, select_tok);
                            myPlants[i_team].insert(myPlants[i_team].end(), 1, *local_plant);
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

                select_done = true;

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
                local_cmd->cmd = cmd;
                local_cmd->pred = pred;
                local_cmd->plant = plant;
                local_cmd->line = line_count;
                local_cmd->opt[0] = opt[0];
                local_cmd->opt[1] = opt[1];

                myTeams[i_team].cmds.insert(myTeams[i_team].cmds.end(), 1, *local_cmd);
                delete local_cmd;

            }


            // Only after the file is compiled can we do a second pass and verify the target strings
            for (uint16_t i = 0; i < myTeams[i_team].cmds.size(); i++) {
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
                    if (myTeams[i_team].cmds[i].has_pred == true)
                        printf("%3s | ", "yes");
                    else
                        printf("%3s | ", "no");
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

                for (uint8_t i = 0; i < 4; i++) {
                    myZombies[i][zombie_counter].place(place_tok, delay_tok, i);
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



