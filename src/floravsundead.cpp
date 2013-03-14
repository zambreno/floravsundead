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

        compileZombies();
        compileTeams();
        myStatus.pan = 0.0;
        myStatus.mode = DEMO_START;
        myStatus.music_buffer = 0;
        for (uint8_t i = 0; i < 4; i++)
            myStatus.scores[i] = 0;

    }

    /*****************************************************************************
    * Function: Game::mainLoop()
    * Description: Runs the main update, drawing, and event processing loop.
    *****************************************************************************/
    void Game::mainLoop() {


        sf::Time myTime;
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

        for (uint16_t i = 0; i < myZombies.size(); i++) {
            myZombies[i].update();
        }

        /* Sort each zombie based on the custom zombie function */
        std::stable_sort(myZombies.begin(), myZombies.end());
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

        for (uint16_t i = 0; i < myZombies.size(); i++) {
            myZombies[i].updateDemo();
        }

        /* Sort each zombie based on the custom zombie function */
        std::stable_sort(myZombies.begin(), myZombies.end());
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
                for (uint16_t i = 0; i < myZombies.size(); i++) {
                    myZombies[i].endDemo();
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
        uint8_t i_team;
        fvu::Team *team;
        FILE *team_file;

        for (i_team = 0; i_team < 4; i_team++) {

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

        // Set the random seed here
        //srand(0);

        budget_flag = false;
        zombie_counter = 0;
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
                    for (uint8_t k = 0; k < 4*select_tok; k++) {
                        local_zombie = new Zombie(z);
                        myZombies.insert(myZombies.end(), 1, *local_zombie);
                        delete local_zombie;
                    }
                }
                else {
                    printf("Error compiling %s, line %d\n", myConfig.zom_fname, line_count);
                    printf("  Invalid zombie type");
                    raise_error(ERR_BADFILE3, myConfig.zom_fname);
                }
            }

            else if (place_ntok == 2) {
                if ((zombie_counter+4) > myZombies.size()) {
                    printf("Error compiling %s, line %d\n", myConfig.zom_fname, line_count);
                    printf("  Cannot place zombies that haven't been selected\n");
                    printf("  select zombie count - %d, place zombie request - %d", myZombies.size(), zombie_counter);
                    raise_error(ERR_BADFILE3, myConfig.zom_fname);
                }

                for (uint8_t i = 0; i < 4; i++) {
                    myZombies[zombie_counter++].place(place_tok, delay_tok, i);
                }
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
        myZombies.resize(zombie_counter, myZombies[0]);
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



