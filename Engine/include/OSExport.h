#ifndef OSEXPORT_H
#define OSEXPORT_H

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
    #define OS_WIN 1
#endif

#ifdef OS_WIN
    #ifdef ENGINE_EXPORTING  
        #define ENGINE_API    __declspec(dllexport)  
    #else  
        #define ENGINE_API    __declspec(dllimport)  
    #endif

    #include <Windows.h>
#else
    #define ENGINE_API
#endif


#endif /*OSEXPORT_H*/