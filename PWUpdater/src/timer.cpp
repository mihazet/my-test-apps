/////////////////////////////////////////////////////////////////////////////
// Name:        win32/timer.cpp
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: timer.cpp,v 1.2 2004/11/30 12:39:17 jb Exp $
// Copyright:   (c) 2001 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined (__WXMSW__)

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
# ifndef DWORD_PTR 
#  define DWORD_PTR DWORD*
# endif
#endif

#include "wx/ctb/timer.h"

static void WINAPI timer_fnc(UINT /*uTimerID*/, 
                             UINT /*uMsg*/, 
                             DWORD_PTR dwUser,
                             DWORD_PTR /*dw1*/, 
                             DWORD_PTR /*dw2*/)
{
    timer_control *tc = (timer_control *)dwUser;

    if (tc->exitfnc) tc->exitfnc(NULL);
    if (tc->exitflag) *tc->exitflag = 1;
    tc->stop = 0;
};

timer::timer(unsigned int msecs,int* exitflag,void*(*exitfnc)(void*))
{
    control.msecs = msecs;
    if (!control.msecs) control.msecs = 1;
    control.exitflag = exitflag;
    control.exitfnc = exitfnc;
    control.stop = 0;
};

timer::~timer()
{
    stop(); // stop the thread
};

int timer::start()
{
    stop();
    control.stop = timeSetEvent(control.msecs,
        (control.msecs > 10) ? 5 : 1,
        (LPTIMECALLBACK) timer_fnc,
        (DWORD) &control,
        TIME_ONESHOT | TIME_CALLBACK_FUNCTION);
    return 0;
};

int timer::stop()
{
    if (control.stop)
        timeKillEvent(control.stop);
    control.stop = 0;
    return 0;
};

void kill_all_timer()
{
};

void sleepms(unsigned int ms)
{
    // set the granularity of Sleep() for the application, that
    // calls it so Sleep(1) will truly sleep for just a millisecond,
    // rather than the default 10!
    // See: http://www.geisswerks.com/ryan/FAQS/timing.html
    timeBeginPeriod(1);
    SleepEx(ms, false);
    timeEndPeriod(1);
};

#elif defined (__WXGTK__)

#include "wx/ctb/timer.h"
#include <unistd.h>

// a dummy function, see below
static void timer_exit(void *arg)
{
};

static void *timer_fnc(void *arg)
{
    // the timer thread should be canceled every time
    // (asyncronously)
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    // this is tricky, but absolutly necessarily to avoid segfaults
    // if the destructor finished a running thread
    pthread_cleanup_push(timer_exit, NULL);
    timer_control *tc = (timer_control *)arg;
    // linux allows a real sleep, means the timer thread will
    // be sleeping (no reduce of the system performance) 
    usleep(tc->usecs);
    // time is over, system reawake the thread.
    // if there is an exit function, calling it
    if (tc->exitfnc) tc->exitfnc(NULL);
    // set the exit flag
    if (tc->exitflag) *tc->exitflag = 1;
    // deallocate the system resources (thread)
    pthread_cleanup_pop(1);
    return NULL;
};

// the constructor initiate the internal control struct
timer::timer(unsigned int msecs, int *exitflag, void *(*exitfnc)(void *))
{
    control.usecs = msecs * 1000;
    control.exitflag = exitflag;
    control.exitfnc = exitfnc;
    stopped = 1;
};

// if a timer instance leave it's valid range, it automaticaly will
// be finished
timer::~timer()
{
    if (!stopped)
    {
        // only a running thread may be canceled
        stop();
    }
};

// starts the timer thread
int timer::start()
{
    stopped = 0;
    if (pthread_create(&tid,    // result parameter, covers the id
                                // of the new threads
        NULL,                   // thread attribute object, NULL means
                                // the defaults
        timer_fnc,              // start function of the thread
        &control                // start function parameter, must refer
                                // as void*
        ) == -1)
    {
        return -1;  // there was something going wrong
    }
    pthread_detach(tid);    // thread status must be "detach". In the other
                            // case, the destructor call of a running
                            // thread throws a segfault
    return 0;
};

// stop the timer thread
int timer::stop()
{
    if (control.exitflag && (*control.exitflag == 0))
    {
        pthread_cancel(tid);
    }
    stopped = 1;
    return 0;
};

void sleepms(unsigned int ms)
{
    usleep(ms * 1000);
};

#endif
