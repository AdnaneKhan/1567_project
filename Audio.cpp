#include "Audio.h"
#include <iostream>

namespace Audio
{

    void play_start()
    {
#ifdef __arm__
        system("omxplayer -o local audio_prompts/begun.m4a &");
        #else
        system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/begun.m4a &");
#endif

        std::cout << "Program Started." << std::endl;
    }

    // Currently methods stubbed with calls to print
    // Playing actual sounds is to be implemented once all logic has been worked out

    void play_left()
    {

#ifdef __arm__
        system("omxplayer -o local audio_prompts/left.m4a &");
        #else
        system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/left.m4a &");
#endif

        std::cout << "Please turn left." << std::endl;
    }

    void play_right()
    {

#ifdef __arm__
        system("omxplayer -o local audio_prompts/right.m4a &");
        #else
        system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/right.m4a &");
#endif

        std::cout << "Please turn right." << std::endl;
    }

    void play_forward()
    {

#ifdef __arm__
        system("omxplayer -o local audio_prompts/forward.m4a &");
        #else
        system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/forward.m4a &");
#endif

        std::cout << "Please move forward." << std::endl;
    }

    void play_back()
    {

#ifdef __arm__
            system("omxplayer -o local audio_prompts/around.m4a &");
        #else
        system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/around.m4a &");
#endif

        std::cout << "Please turn around." << std::endl;
    }

    void turn_progress(int curr_degrees, int target_degrees)
    {


#ifdef __arm__
            system("omxplayer -o local audio_prompts/left.m4a &");
        #else
        system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/left.m4a &");
#endif

        std::cout << "You have turned " << curr_degrees << " degrees and you need to turn " << (target_degrees - curr_degrees) << " degrees." << std::endl;
    }

    void turn_complete()
    {

#ifdef __arm__
        system("omxplayer -o local audio_prompts/left.m4a &");
        #else
        system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/left.m4a &");
#endif

        std::cout << "You have succesfully completed your turn." << std::endl;
    }

    void intersection()
    {

#ifdef __arm__
        system("omxplayer -o local audio_prompts/intersection.m4a &");
        #else
        system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/intersection.m4a &");
#endif
        std::cout << "You have arrived at an intersection." << std::endl;
    }


    void play_goal()
    {

#ifdef __arm__
        system("omxplayer -o local audio_prompts/goal.m4a &");
        #else
        system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/goal.m4a &");
#endif
        std::cout << "The goal has been located.." << std::endl;
    }

    void play_destination()
    {
#ifdef __arm__
        system("omxplayer -o local audio_prompts/goal.m4a &");
        #else
        system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/goal.m4a &");
#endif
        std::cout << "The goal has been reached" << std::endl;
    }

    void turn_dir(int dir)
    {
        switch (dir)
        {
            case 0:
                play_forward();
                break;
            case 1:
                play_right();
                break;
            case 2:
                play_back();
                break;
            case 3:
                play_left();
                break;
            default:
                // No-op
                break;
        }
    }

    void play_light()
    {

#ifdef __arm__
        system("omxplayer -o local audio_prompts/light.m4a &");
        #else
        system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/light.m4a &");
#endif

        std::cout << "You have walked under a light." << std::endl;
    }

    void play_reset()
    {

#ifdef __arm__
        system("omxplayer -o local audio_prompts/reset.m4a &");
        #else
        system("afplay /Users/adnankhan/Box\\ Sync/Robots/1567_project/audio_prompts/reset.m4a &");
#endif

        std::cout << "Graph has been reset." << std::endl;
    }
}