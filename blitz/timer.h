// -*- C++ -*-
/***************************************************************************
 * blitz/Timer.h        Timer class, for use in benchmarking
 *
 * $Id$
 *
 * Copyright (C) 1997-2011 Todd Veldhuizen <tveldhui@acm.org>
 *
 * This file is a part of Blitz.
 *
 * Blitz is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Blitz is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with Blitz.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Suggestions:          blitz-devel@lists.sourceforge.net
 * Bugs:                 blitz-support@lists.sourceforge.net    
 *
 * For more information, please see the Blitz++ Home Page:
 *    https://sourceforge.net/projects/blitz/
 *
 ***************************************************************************/

// This class is not portable to non System V platforms.
// It will need to be rewritten for Windows, NT, Mac.
// NEEDS_WORK

#ifndef BZ_TIMER_H
#define BZ_TIMER_H

#ifndef BZ_BLITZ_H
 #include <blitz/blitz.h>
#endif

#ifdef BZ_HAVE_RUSAGE
 #include <sys/resource.h>
#else
 #include <time.h>
#endif

BZ_NAMESPACE(blitz)

class Timer {

public:
    Timer() 
    { 
        state_ = uninitialized;
    }

    void start()
    { 
        state_ = running;
        t1_ = systemTime();
    }

    void stop()
    {
        t2_ = systemTime();
        BZPRECONDITION(state_ == running);
        state_ = stopped;
    }
    
/* Compaq cxx compiler in ansi mode cannot print out long double type! */
#if defined(__DECCXX)
    double elapsedSeconds()
#else
    long double elapsedSeconds()
#endif
    {
        BZPRECONDITION(state_ == stopped);
        return t2_ - t1_;
    }

private:
    Timer(Timer&) { }
    void operator=(Timer&) { }

    long double systemTime()
    {
#ifdef BZ_HAVE_RUSAGE
        getrusage(RUSAGE_SELF, &resourceUsage_);
        double seconds = resourceUsage_.ru_utime.tv_sec 
            + resourceUsage_.ru_stime.tv_sec;
        double micros  = resourceUsage_.ru_utime.tv_usec 
            + resourceUsage_.ru_stime.tv_usec;
        return seconds + micros/1.0e6;
#else
        return clock() / (long double) CLOCKS_PER_SEC;
#endif
    }

    enum { uninitialized, running, stopped } state_;

#ifdef BZ_HAVE_RUSAGE
    struct rusage resourceUsage_;
#endif

    long double t1_, t2_;
};

BZ_NAMESPACE_END

#endif // BZ_TIMER_H

