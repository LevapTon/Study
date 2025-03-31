/* ------------------------------------------------------------------------- */
/*  ������ ���� �������� ������ ��������� �������� �������,                  */
/*  �������������� �������� ���������� �������:                              */
/*  ������� �.�.                                                             */
/*  ������ ������������� ����������������. - �.: �����-�����, 2012. - 384 �. */
/*  ISBN 978-5-91359-102-9                                                   */
/*                                                                           */
/*  ��� � ���� �������, ����������� � ���� �������� ������ �������������     */
/*  ���� ��� ������������ � ���������� ���������������� ����������           */
/*  ������������ ��������, � ����� ��� ���������� ���������� �������������.  */
/*  ������������� ����� ���� � �������� ������ ��� �������� ��������         */
/*  ���������, ������ ������� ��������� � ����� �������������� ����          */
/*  �� ������ ����� � ���� ������������.                                     */
/*  �������� ������ ��������������� "��� ����", ��� ����� �� �� �� ����      */
/*  ����� ��� ������� �������� ����������� � ������������� ����������.       */
/*                                                                           */
/*  Copyright � 2008-2011 ������� �.�.                                       */
/* ------------------------------------------------------------------------- */


#ifndef _SYNPRINTF_HXX_
#define _SYNPRINTF_HXX_

#include <vector>
#include <cstdio>
#include <cstdarg>


#if defined(_OPENMP)

#include <omp.h>

namespace z {
inline
int synprintf(FILE *stream, const char *fmt, ...)
{
    using namespace std;
    va_list argptr;
    va_start(argptr, fmt);
    int rc = vsnprintf(NULL, 0, fmt, argptr);
    if (rc >= 0)
    {
        vector<char> buf(rc + 1);
        vsnprintf(&buf.front(), buf.size(), fmt, argptr);
        #pragma omp critical (synprintf)
        {
            fputs(&buf.front(), stream);
            fflush(stream);
        };
    };
    va_end(argptr);
    return rc;
}
} // namespace z

#elif defined(_POSIX_THREADS)

#include <pthread.h>

namespace z {
class criticalsection_type
{
    pthread_mutex_t mutex;
    criticalsection_type(const criticalsection_type &);
    criticalsection_type & operator =(const criticalsection_type &);
public:
    criticalsection_type(void)
    {
        ::pthread_mutex_init(&mutex, NULL);
    }
    ~criticalsection_type(void)
    {
        ::pthread_mutex_destroy(&mutex);
    }
    void enter(void)
    {
        ::pthread_mutex_lock(&mutex);
    }
    void leave(void)
    {
        ::pthread_mutex_unlock(&mutex);
    }
};

inline
int synprintf(FILE *stream, const char *fmt, ...)
{
    static criticalsection_type criticalsection;
    using namespace std;

    va_list argptr;
    va_start(argptr, fmt);
    int rc = vsnprintf(NULL, 0, fmt, argptr);
    if (rc >= 0)
    {
        vector<char> buf(rc + 1);
        vsnprintf(&buf.front(), buf.size(), fmt, argptr);
        criticalsection.enter();
        fputs(&buf.front(), stream);
        fflush(stream);
        criticalsection.leave();
    };
    va_end(argptr);
    return rc;
}
} // namespace z

#elif defined(_WIN32)

#include <windows.h>

namespace z {
class criticalsection_type
{
    CRITICAL_SECTION cs;
    criticalsection_type(const criticalsection_type &);
    criticalsection_type & operator =(const criticalsection_type &);
public:
    criticalsection_type(void)
    {
        ::InitializeCriticalSection(&cs);
    }
    ~criticalsection_type(void)
    {
        ::DeleteCriticalSection(&cs);
    }
    void enter(void)
    {
        ::EnterCriticalSection(&cs);
    }
    void leave(void)
    {
        ::LeaveCriticalSection(&cs);
    }
};

inline
int synprintf(FILE *stream, const char *fmt, ...)
{
    static criticalsection_type criticalsection;
    using namespace std;

    va_list argptr;
    va_start(argptr, fmt);
    int rc = vsnprintf(NULL, 0, fmt, argptr);
    if (rc >= 0)
    {
        vector<char> buf(rc + 1);
        vsnprintf(&buf.front(), buf.size(), fmt, argptr);
        criticalsection.enter();
        fputs(&buf.front(), stream);
        fflush(stream);
        criticalsection.leave();
    };
    va_end(argptr);
    return rc;
}
} // namespace z

#else

// no shared access exclusion
namespace z {
inline
int synprintf(FILE *stream, const char *fmt, ...)
{
    using namespace std;
    va_list argptr;
    va_start(argptr, fmt);
    int rc = vsnprintf(NULL, 0, fmt, argptr);
    if (rc >= 0)
    {
        vector<char> buf(rc + 1);
        vsnprintf(&buf.front(), buf.size(), fmt, argptr);
        fputs(&buf.front(), stream);
        fflush(stream);
    };
    va_end(argptr);
    return rc;
}
} // namespace z

#endif


#endif /* _SYNPRINTF_HXX_ */
