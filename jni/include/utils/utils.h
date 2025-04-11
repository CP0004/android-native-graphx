#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Get the current time in milliseconds.
 *
 * @return long The current time in milliseconds.
 */
long get_current_time_milliseconds()
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto duration = duration_cast<milliseconds>(now.time_since_epoch());
    return duration.count();
}

/**
 * @brief Generate a random number between the specified minimum and maximum values.
 *
 * @param min The minimum value of the random number (inclusive).
 * @param max The maximum value of the random number (inclusive).
 * @return int The generated random number.
 */
int generate_random_number(const int min, const int max)
{
    std::random_device rd;  // Generate random device seed
    std::mt19937 gen(rd()); // Mersenne Twister random number generator

    std::uniform_int_distribution<int> distrib(min, max);

    return distrib(gen);
}

/**
 * @brief Check if a string is a non-empty sequence of digits.
 *
 * @param s The string to check.
 * @return bool True if the string is non-empty and consists only of digits, false otherwise.
 */
bool is_number(const string &s)
{
    try
    {
        if (s.empty())
        {
            throw invalid_argument("The string is empty.");
        }

        if (!all_of(s.begin(), s.end(), ::isdigit))
        {
            throw invalid_argument("The string contains non-digit characters.");
        }
    }
    catch (const invalid_argument &ex)
    {
        // Catching the exception and printing the error message using Android Log
        LOGD(TEST_EXP, "is_number Error: %s", ex.what());
        return false; // Returning false since the string is not a number
    }

    return true; // Returning true if no exception is thrown
}

/**
 * Converts a string of ASCII encoded codepoints to a wide string.
 *
 * @param text The string of ASCII encoded codepoints to convert.
 * @return The resulting wide string.
 * @throws invalid_argument if the input string is empty.
 */
wstring char_to_wstring(const string &text)
{
    try
    {
        if (text.empty())
        {
            throw invalid_argument("Empty input string.");
        }

        wstringstream ss;
        string item;

        istringstream stringStream(text);
        while (getline(stringStream, item, ':'))
        {
            if (!item.empty())
            {
                int codepoint = stoi(item);
                ss << static_cast<wchar_t>(codepoint);
            }
        }

        return ss.str();
    }
    catch (const exception &ex)
    {
        // Catching the exception and printing the error message using Android Log
        LOGD(LOG_TAG, "char_to_wstring Error: %s", ex.what());
        return L""; // Returning an empty wide string in case of an error
    }
}

/**
 * Converts a wide string to a UTF-8 encoded string.
 *
 * @param wstr The wide string to convert.
 * @return The resulting UTF-8 encoded string.
 */
string wstring_to_utf8(const wstring &wstr)
{
    try
    {
        if (wstr.empty())
        {
            throw invalid_argument("Empty wide string.");
        }

        wstring_convert<codecvt_utf8<wchar_t>> myconv;
        return myconv.to_bytes(wstr);
    }
    catch (const exception &ex)
    {
        // Catching the exception and printing the error message using Android Log
        LOGD(LOG_TAG, "wstring_to_utf8 Error: %s", ex.what());
        return ""; // Returning an empty string in case of an error
    }
}

/**
 * Check if a given location is out of range.
 *
 * @param location The location to check.
 * @param width The width of the screen.
 * @param height The height of the screen.
 * @return True if the location is out of range, false otherwise.
 */
bool is_out_range(const Vector3 &location, const float width, const float height)
{
    try
    {
        if (width <= 0.0f || height <= 0.0f)
        {
            throw invalid_argument("Width or height is not positive.");
        }

        const float frame = 10.0f;
        if (location.Z <= 0.0f ||
            location.X < frame || location.X > width - frame ||
            location.Y < frame || location.Y > height - frame)
        {
            return true;
        }

        return false;
    }
    catch (const exception &ex)
    {
        // Catching the exception and printing the error message using Android Log
        LOGD(LOG_TAG, "is_out_range Error: %s", ex.what());
        return true; // Returning true in case of an error
    }
}

/**
 * Splits a string into a vector of substrings based on a delimiter.
 *
 * @param str The string to split.
 * @param delimiter The delimiter character.
 * @return A vector of substrings.
 * @throws invalid_argument If the delimiter is not a valid character.
 */
vector<string> split(const string &str, const char delimiter)
{
    try
    {
        if (!isprint(delimiter))
        {
            throw invalid_argument("Delimiter is not a printable character.");
        }

        vector<string> tokens;
        string token;
        istringstream tokenStream(str);

        while (getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }

        return tokens;
    }
    catch (const exception &ex)
    {
        // Catching the exception and printing the error message using Android Log
        LOGD(LOG_TAG, "split Error: %s", ex.what());
        return {}; // Returning an empty vector in case of an error
    }
}

/**
 * Finds the longest substring from a vector of substrings.
 *
 * @param SubstringType The type of the substrings.
 * @param substrings The vector of substrings.
 * @return The longest substring.
 */
string find_longest_substring(const vector<string> &substrings)
{
    try
    {
        if (substrings.empty())
        {
            throw invalid_argument("Vector of substrings is empty.");
        }

        string longest;
        for (const auto &substring : substrings)
        {
            if (substring.length() > longest.length())
            {
                longest = substring;
            }
        }
        return longest;
    }
    catch (const exception &ex)
    {
        // Catching the exception and printing the error message using Android Log
        LOGD(LOG_TAG, "Error: %s", ex.what());
        return ""; // Returning an empty string in case of an error
    }
}

/**
 * Adds text centered at a given position to an ImDrawList.
 *
 * @param draw_list The ImDrawList to add the text to.
 * @param text The text to add.
 * @param font The font to use for the text.
 * @param font_size The size of the font.
 * @param pos The position to center the text at.
 * @param color The color of the text.
 * @param align_center Whether to align the text to the center (default: true).
 * @return void
 */
void draw_text_centered(ImDrawList *draw_list, const string &text, ImFont *font, float font_size, ImVec2 pos, ImU32 color, bool align_center = true)
{
    try
    {
        if (text.empty())
        {
            throw invalid_argument("Empty text.");
        }

        if (font == nullptr)
        {
            throw invalid_argument("Font is nullptr.");
        }

        ImVec2 text_size;
        if (align_center)
        {
            text_size = font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, text.c_str());
            pos.x -= text_size.x / 2.0f;
            pos.y -= text_size.y / 2.0f;
        }

        draw_list->AddText(font, font_size, {pos.x + 1, pos.y + 1}, IM_COL32(0, 0, 0, 200), text.c_str());
        draw_list->AddText(font, font_size, pos, color, text.c_str());
    }
    catch (const exception &ex)
    {
        // Catching the exception and printing the error message using Android Log
        LOGD(LOG_TAG, "Error: %s", ex.what());
    }
}

class FrameRateLimiter
{
public:
    FrameRateLimiter(float fps) : targetFrameTime(1000.0f / fps), lastFrame(chrono::steady_clock::now()) {}

    void Limit()
    {
        auto currentFrame = chrono::steady_clock::now();
        auto frameTime = chrono::duration_cast<chrono::milliseconds>(currentFrame - lastFrame).count();

        if (frameTime < targetFrameTime)
        {
            this_thread::sleep_for(chrono::milliseconds(static_cast<long>(targetFrameTime - frameTime)));
        }

        lastFrame = chrono::steady_clock::now();
    }

    void setFPS(float fps)
    {
        targetFrameTime = 1000.0f / fps;
    }

private:
    float targetFrameTime;
    chrono::steady_clock::time_point lastFrame;
};

#endif // UTILS_H