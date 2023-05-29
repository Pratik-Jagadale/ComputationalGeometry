#ifndef _DLLEXPORT_H
#define _DLLEXPORT_H

#ifdef MAKEDLL
#define DllExport __declspec(dllexport)
#else // MAKEDLL
#define DllExport __declspec(dllimport)
#endif

#endif