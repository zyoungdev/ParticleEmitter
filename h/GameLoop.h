#ifndef _GAME_LOOP_
#define _GAME_LOOP_

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <cstdint>

using u_int32 = std::uint_fast32_t;
using String = std::string;

class GameLoop{
public:
    SDL_Event e;
    SDL_Window* win = nullptr;
    SDL_Renderer* rend = nullptr;

    // time_now_ms will wrap after 49 days of uptime
    u_int32 time_now_ms = 0;
    bool is_running = true;

    enum INTERPOLATIONS
    {
        ONE, TWO, THREE, FOUR
    };

private:
    u_int32 frames_per_second = 60;
    u_int32 single_frame_time_in_ms = 0;
    u_int32 max_frame_skip = 5;
    u_int32 frame_skips = 0;

    u_int32 next_frame_time = 0;
    u_int32 prev_frame_time = 0;

    float interpolation = 0.0;
    float delta_time = 0.0;

    INTERPOLATIONS ip_speed = INTERPOLATIONS::ONE;
    bool ip_flags[4] = {false, false, false, false};

    u_int32 tick_s = 0;
    u_int32 draw_count = 0;
    u_int32 update_count = 0;

    bool is_first_run = true;

public:

    /**
     * Constructor
     * @param target_frames_per_second
     * @param interpolations_per_frame
     */
    GameLoop(const u_int32& target_frames_per_second,
            const INTERPOLATIONS& interpolations_per_frame)
    {
        set_time_partitions(target_frames_per_second);
        ip_speed = interpolations_per_frame;
    }

    virtual ~GameLoop() {}

    /**
     * Bootstrap the engine
     */
    void start(const SDLWindow& sdlWin)
    {
        if (sdlWin.win != nullptr)
            win = sdlWin.win;

        if (sdlWin.rend != nullptr)
            rend = sdlWin.rend;

        init();
        main_loop();
    }

    void toggle_pause()
    {
        std::cout << "*** Paused ***\n";
        SDL_Event pause_event = e;
        Uint32 type = pause_event.type;
        Uint32 code = pause_event.key.keysym.scancode;

        SDL_WaitEvent(&e);

        while (!(e.type == type && e.key.keysym.scancode == code))
            SDL_WaitEvent(&e);

        reset_timers();
    }

    virtual void init() {}

    /**
     * A one second tick
     */
    virtual void second_tick()
    {
        console_output();
    }

    /**
     * Send time partition information to stdout
     */
    virtual void console_output()
    {
        static u_int32 time_count = 0;

        if ( is_first_run || time_count == 20)
        {
            std::cout << "Time Passed\tUpdate Count\tDraw Count\n";

            time_count = 0;
            is_first_run = false;
        }

        std::cout << time_now_ms/1000 << "\t\t" << update_count <<
            "\t\t" << draw_count << "\n";

        time_count++;
        update_count = draw_count = 0;
    }

    virtual void inputs(SDL_Event& e) {}

    virtual void collisions() {}

    virtual void update_positions(const float& delta) {}

    virtual void interpolate(const float& delta, const float& interpolation)
    {}

    virtual void draw() {}

private:
    void reset_timers()
    {
        time_now_ms = SDL_GetTicks();
        next_frame_time = time_now_ms + single_frame_time_in_ms;
        prev_frame_time = time_now_ms;
    }

    /**
     * Calculate correct time between frames
     * @param u
     */
    void set_time_partitions(const u_int32& u)
    {
        frames_per_second = u <= 0 ? 0 : u >= 240 ? 240 : u;
        single_frame_time_in_ms = static_cast<int>(1000 / frames_per_second);
    }

    /**
     * Handle quit and focus events
     */
    void check_for_quit()
    {
        switch (e.type)
        {
            case SDL_QUIT:
            {
                is_running = false;
                break;
            }
            case SDL_WINDOWEVENT:
            {
                if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                {
                    SDL_Event ev;
                    Uint32 type = e.type;
                    Uint32 win_event = SDL_WINDOWEVENT_FOCUS_GAINED;

                    while (!(ev.type == type && ev.window.event == win_event))
                        SDL_WaitEvent(&ev);

                    reset_timers();
                }
                break;
            }
        }
    }

    /**
     * Create one second tick then call virtual second_timer
     */
    void calc_second_timer()
    {
        if (time_now_ms >= tick_s + 1000)
        {
            second_tick();
            tick_s = time_now_ms;
        }
    }

    /**
     * Calculate the time difference between frames
     */
    void calc_delta_time()
    {
    	delta_time = (time_now_ms - prev_frame_time) / 1000.0f;
    }

    /**
     * Call virtual interpolate and raw functions
     */
    void interpolate_and_draw()
    {
        interpolate(delta_time, interpolation);
        draw();

        draw_count++;
    }

    /**
     * Partition each frame for interpolation
     */
    void partition_frame()
    {
        bool draw = false;

        if (!ip_flags[0] && interpolation >= 0.0f)
        {
            ip_flags[0] = true;
            draw = true;
        }
        else if (ip_speed == INTERPOLATIONS::TWO)
        {
            if (!ip_flags[1] && interpolation >= 0.50f)
            {
                ip_flags[1] = true;
                draw = true;
            }
        }
        else if (ip_speed == INTERPOLATIONS::THREE)
        {
            if (!ip_flags[1] && interpolation >= 0.33f)
            {
                ip_flags[1] = true;
                draw = true;
            }
            else if (!ip_flags[2] && interpolation >= 0.66f)
            {
                ip_flags[2] = true;
                draw = true;
            }
        }
        else if (ip_speed == INTERPOLATIONS::FOUR)
        {
            if (!ip_flags[1] && interpolation >= 0.25f)
            {
                ip_flags[1] = true;
                draw = true;
            }
            else if (!ip_flags[2] && interpolation >= 0.50f)
            {
                ip_flags[2] = true;
                draw = true;
            }
            else if (!ip_flags[3] && interpolation >= 0.75f)
            {
                ip_flags[3] = true;
                draw = true;
            }
        }

        if (draw)
            interpolate_and_draw();
    }

    /**
     * Main thread
     *   Run events as fast as the loop runs
     *   Call virtual collision and position functions
     *      Frame skip if CPU can't keep up
     */
    void main_loop()
    {
        while (is_running)
        {
            time_now_ms = SDL_GetTicks();

            while (SDL_PollEvent(&e))
            {
                check_for_quit();
                inputs(e);
            }

            frame_skips = 0;

            while( time_now_ms > next_frame_time && frame_skips < max_frame_skip)
            {
                calc_delta_time();
                update_positions(delta_time);
                collisions();

                next_frame_time += single_frame_time_in_ms;

                prev_frame_time = time_now_ms;

                ip_flags[0] = ip_flags[1] = ip_flags[2] = ip_flags[3] = false;

                frame_skips++;
                update_count++;
            }

            interpolation =
                static_cast<float>( time_now_ms +
                        single_frame_time_in_ms -
                        next_frame_time ) /
                static_cast<float>( single_frame_time_in_ms );

            partition_frame();
            calc_second_timer();
        }
    }
};

#endif
