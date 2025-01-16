#pragma once

/*
    ###################################################################################
    PFL.h
    PR00F Foundation Library
    This file is part of PFL.
    Made by PR00F88
    ###################################################################################
*/

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>

namespace std
{
    /**
    * There is no std::static_pointer_cast for weak_ptr so here is one.
    */
    template<class T, class U>
    std::weak_ptr<T> static_pointer_cast(std::weak_ptr<U> const& r)
    {
        return std::static_pointer_cast<T>(std::shared_ptr<U>(r));
    }

    /**
    * There is no std::dynamic_pointer_cast for weak_ptr so here is one.
    */
    template<class T, class U>
    std::weak_ptr<T> dynamic_pointer_cast(std::weak_ptr<U> const& r)
    {
        return std::dynamic_pointer_cast<T>(std::shared_ptr<U>(r));
    }
}


/**
    PR00F Foundation Library class.
    Useful functions.

    Not following the Pimpl idiom here as there are no hidden members in this class for the outside world.
*/
class PFL
{

public:
    // when I switch to C++11 I won't need this struct here and the custom gettimeofday() implementation
    // MSVC defines this in winsock2.h!?
    typedef struct timeval {
        long tv_sec;
        long tv_usec;
    } timeval;

    typedef uint32_t StringHash;

    static float PI;
    static float E;

    // ---------------------------------------------------------------------------

    /**
     * Trick to define an std::array without explicitly specifying its size.
     * Very useful before Cpp17, however sometimes even with Cpp17 it is needed.
     * Example:
     *   static constexpr auto textArray = PFL::std_array_of<const char*>(
     *     "text 1",
     *     "text 2",
     *     "text 3",
     *   );
     */
    template <typename V, typename... T>
    static constexpr auto std_array_of(T&&... t) -> std::array<V, sizeof...(T)>
    {
        return { {std::forward<T>(t)...} };
    }

    static int  gettimeofday(struct timeval * tp, struct timezone * tzp);                                  /**< Gets current time. */
    static long getTimeDiffInUs(const timeval& end, const timeval& begin);                                 /**< Gets difference of 2 timestamps. */
    static bool updateForMinDuration(timeval& timeRef, long durationUs);                                   /**< Updates timeRef if it is greater duration than durationUs. */
    static bool updateForMinDuration(timeval& timeRef, const timeval& timeBegin, const timeval& timeEnd);  /**< Updates timeRef if it is greater duration than (timeEnd - timeBegin). */
    static bool updateForMaxDuration(timeval& timeRef, long durationUs);                                   /**< Updates timeRef if it is less duration than durationUs. */
    static bool updateForMaxDuration(timeval& timeRef, const timeval& timeBegin, const timeval& timeEnd);  /**< Updates timeRef if it is less duration than (timeEnd - timeBegin). */

    static bool         fileExists(const char* path);     /**< Determines whether the given file exists. */
    static std::string  getExtension(const char* path);   /**< Extracts the extension from the path. */
    static std::string  getDirectory(const char* path);   /**< Extracts the directory from the path. */
    static std::string  getFilename(const char* path);    /**< Extracts the file name from the path. */
    static std::string  changeExtension(
        const char* path, const char* ext);               /**< Changes the file extension in the given path. */

    static unsigned int numCharAppears(
        const char searchFor, const char* buffer,
        size_t buffer_size);                              /**< Gets how many times the given character appears in the given buffer. */

    static unsigned int strClrTrails(
        char* srcStr,
        char targetChar1 = ' ', char targetChar2 = '\t'); /**< Removes trailing spaces and tabs from the given string. */
    static unsigned int strClrLeads(
        char* const srcStr,
        char targetChar1 = ' ', char targetChar2 = '\t'); /**< Removes leading spaces and tabs from the given string. */
    static unsigned int strClr(
        char* const srcStr,
        char targetChar1 = ' ', char targetChar2 = '\t'); /**< Removes leading and trailing spaces and tabs from the given string. */

    static StringHash calcHash(const std::string& str);   /**< Calculates a hash for the given string. */

    static float pi();                          /**< Returns PI. */

    static float roundf(float value);           /**< Rounds the given value to the nearest whole number. */
    static int   roundi(float value);           /**< Rounds the given value to the nearest whole number. */

    static float constrain(
        float value, float min, float max);     /**< Constrains the given value into the given [min,max] bounds. */

    static float degToRad(float degree);        /**< Converts the given angle from degrees to radians. */
    static float radToDeg(float radian);        /**< Converts the given angle from radians to degrees. */

    static int random(int from, int to);        /**< Generates a random number between from and to. */

    static float lerp(
        float v0, float v1, float t);           /**< Linear interpolation between v0 and v1. */

    static float smooth(
        float fCurrent, float fTarget, float fSpeed, float fEpsilon = 0.001f);  /**< Smoothly approach fTarget from fCurrent. */

    // ---------------------------------------------------------------------------

    PFL(); 
    PFL(const PFL&);
    PFL& operator= (const PFL&);
    virtual ~PFL();

}; // class PFL

