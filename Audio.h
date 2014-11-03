#ifndef AUDIO_H
#define AUDIO_H

namespace Audio {
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

    /**
    *  Informs user to turn in the specified direction
    *  \param dir for user to turn
    */
    void turn_dir(int dir);
}


#endif