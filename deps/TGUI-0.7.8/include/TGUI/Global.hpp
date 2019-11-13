/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012-2017 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TGUI_DEFINES_HPP
#define TGUI_DEFINES_HPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <TGUI/Config.hpp>
#include <TGUI/Borders.hpp>
#include <TGUI/Exception.hpp>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <locale>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Namespace that contains all TGUI functions and classes
namespace tgui
{
    class Texture;

    /// @internal When disabling the tab key usage, pressing tab will no longer focus another widget.
    extern TGUI_API bool TGUI_TabKeyUsageEnabled;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const float pi = 3.14159265358979f;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    std::string to_string(T value)
    {
        std::ostringstream oss;
        oss.imbue(std::locale::classic());
        oss << value;
        return oss.str();
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief While tab key usage is enabled (default), pressing tab will focus another widget.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API void enableTabKeyUsage();


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief When disabling the tab key usage, pressing tab will no longer focus another widget.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API void disableTabKeyUsage();


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Set a new resource path.
    ///
    /// This pathname is placed in front of every filename that is used to load a resource.
    ///
    /// @param path  New resource path
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API void setResourcePath(const std::string& path);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return the resource path.
    ///
    /// This pathname is placed in front of every filename that is used to load a resource.
    ///
    /// @return The current resource path
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API const std::string& getResourcePath();


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @internal
    // Check if two floats are equal, with regard to a small epsilon margin.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API bool compareFloats(float x, float y);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @internal
    // Check if a character is a whitespace character.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API bool isWhitespace(char character);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @internal
    // Convert a string to an integer. Acts like std::stoi which isn't supported in MinGW TDM.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API int stoi(const std::string& value);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @internal
    // Convert a string to a float. Acts like std::stof which isn't supported in MinGW TDM.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API float stof(const std::string& value);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @internal
    // Extract a bool from a string value.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API bool extractBoolFromString(const std::string& property, const std::string& value);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @internal
    // Move the iterator forward until no more whitespace is found
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API bool removeWhitespace(const std::string& line, std::string::const_iterator& c);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @internal
    // Converts a string to lowercase.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API std::string toLower(std::string str);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @internal
    // Trim the whitespace from a string.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API std::string trim(std::string str);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @internal
    // Splits a string.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API std::vector<std::string> split(const std::string& str, char delim);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @internal
    // Returns the color with its alpha channel multiplied with the alpha parameter
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TGUI_API sf::Color calcColorOpacity(const sf::Color& color, float alpha);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @internal
    // The line spacing returned by sfml is correct but there is extra space on top.
    // The text has to be moved up so that the line spacing really corresponds with the height of every line.
    // This function returns the offset that the text has to be moved.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    float getTextVerticalCorrection(const std::shared_ptr<sf::Font>& font, unsigned int characterSize, sf::Uint32 style = 0);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @internal
    /// @brief Finds the best character size for the text
    ///
    /// @param font    Font of the text
    /// @param height  Height that the text should fill
    /// @param fit     0 to choose best fit, 1 to select font of at least that height, -1 to select font of maximum that height
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int findBestTextSize(const std::shared_ptr<sf::Font>& font, float height, int fit = 0);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_DEFINES_HPP

