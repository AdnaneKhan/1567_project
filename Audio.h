#ifndef AUDIO_H
#define AUDIO_H

namespace Audio
{

    void play_start();

    /**
    *  Plays a prompt to tell user light was encountered
    */
    void play_light();

    /**
    *
    * Plays a prompt for the user to turn left.
    */
    void play_left();

    /**
    *
    * Plays a prompt for the user to turn right.
    */
    void play_right();

    /**
    *
    * Plays a prompt for the user to move forward.
    */
    void play_forward();

    /**
    *
    * Informs user of their progress in making the turn.
    */
    void turn_progress(int curr_degrees, int target_degrees);

    /**
    * Informs user that they have successfully completed their turn
    */
    void turn_complete();

    /**
    *  Informs user that in intersection has been reached.
    */
    void intersection();

    void play_reset();

    void play_goal();

    void play_destination();

    /**
    *  Informs user to turn in the specified direction
    *  \param dir for user to turn
    */
    void turn_dir(int dir);

    void play_stop();

    void continue_mov();

    void middle();

    void play_step();

    void front_r();

    void front_l();

    void right_close();

    void left_close();

    void downfront();
}


#endif