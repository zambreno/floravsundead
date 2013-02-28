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


int main(int argc, char *argv[]) {

    fvu::Game *myGame = new(std::nothrow)fvu::Game(argc,argv);

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

        //TODO: implement these eventually
        //compileZombies();
        compileTeams();
        myStatus.pan = 0.0;
        myStatus.mode = DEMO_START;
        myStatus.time_ms = myConfig.time_ms;
        for (uint8_t i = 0; i < 4; i++)
            myStatus.scores[i] = 0;

    }

    /*****************************************************************************
    * Function: Game::mainLoop()
    * Description: Runs the main update, drawing, and event processing loop.
    *****************************************************************************/
    void Game::mainLoop() {

        //NOTE: These tasks can be split into multiple threads, in that case we would
        //have to take care regarding which thread has the active window context.


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
    * Function: Game::endGame()
    * Description: Runs the end game sequence.
    *****************************************************************************/
    void Game::endGame() {

        drawWorld();
        drawScoreboard();
        drawMap();

    }



    /*****************************************************************************
    * Function: Game::endGame()
    * Description: Runs the end game sequence.
    *****************************************************************************/


    /*****************************************************************************
    * Function: Game::demoMode()
    * Description: Runs the initial screen pan before the game start.
    *****************************************************************************/
    void Game::demoMode() {

        static float dir = 3.0;

        if (myStatus.mode == DEMO_START) {
            myMusic[0].play();
            myStatus.mode = DEMO_MID;
        }

        drawWorld();
        drawScoreboard();
        drawMap();


        if (myStatus.mode == DEMO_MID) {

            myStatus.pan_prev = myStatus.pan;
            myStatus.pan += dir;
            if (myStatus.pan >= 352.5) {
                myStatus.pan = 352.5;
                dir = -dir;
            }
            if (myStatus.pan <= -352.5) {
                myStatus.pan = -352.5;
                dir = -dir;
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
        myConfig.time_ms = TIME_MS_DEFAULT;

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



