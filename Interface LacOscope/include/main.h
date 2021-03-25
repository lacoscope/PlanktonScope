#include <wx/wx.h>

#pragma once

std::string get_path_windows();
std::string get_path_linux();

#ifdef WINDOWS
    //#include <direct.h>
    #define getexepath get_path_windows
#else
    //#include <unistd.h>
    #define getexepath get_path_linux
 #endif

class MyApp : public wxApp
{
  public:
    virtual bool OnInit();
};
