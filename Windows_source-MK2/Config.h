//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

#ifndef ConfigH
#define ConfigH

#include <sstream>
#include <fstream>

#include <vcl.h>
#include <map>
class AppConfigData {
public:
    std::map<std::string, std::string> Options;
    AppConfigData( AnsiString wxAppName );
    AnsiString ConfigPath( void);
    bool ReadOnly(void);
    void Save( void );
private:
    std::string wxsConfigPath; // = _("");
    bool bConfigPath; // = false;
    bool bReadOnly;
    void ReadOptions( char * );
};
#endif
