#include <include/main.h>

int main(int argc, char *argv[])
{
    is_render = true;
    if (!init_graphic(false)) // init graphic
    {
        is_render = false;
        return -1;
    }
    TouchInput::touchInputStart(); // start touch input
    FrameRateLimiter limiter(control_desired_fps); // frame rate limiter change fps in file include/main.h "control_desired_fps"

    while (is_render)
    {
        limiter.setFPS(control_desired_fps); // set fps

        if (orientation != displayInfo.orientation)
        {
            orientation = static_cast<int>(displayInfo.orientation);
            width = static_cast<int>(displayInfo.width);
            height = static_cast<int>(displayInfo.height);
        }

        draw_begin(); // start canvas view draw only below code

        ImDrawList *draw_list = ImGui::GetBackgroundDrawList();

        // normal text
        draw_text_centered(draw_list,
                           string("FPS: " + to_string((int)ImGui::GetIO().Framerate)), ImGui::GetFont(), 40.0f,
                           {(float)width / 20.0f, (float)height / 11.0f},
                           IM_COL32(255, 255, 0, 255), false);

        // arabic text support
        draw_text_centered(draw_list,
                           FarsiType::ConvertToFAGlyphs(wstring_to_utf8(char_to_wstring("السلام عليكم يا عالم"))),
                           ImGui::GetDefaultFont(),
                           50.0f,
                           {(float)width / 2.0f, (float)height / 2.0f},
                           IM_COL32(255, 255, 0, 255),
                           true);

        draw_end(); // end canvas view draw only above code
        limiter.Limit(); // limit fps
    }

    shut_down(); // shut down graphic
    TouchInput::touchInputStop(); // stop touch input
    is_render = false; // stop render
    return 0;
}