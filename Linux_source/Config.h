//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

#include <wx/app.h>
#include <map>

class AppConfigData {
public:
    std::map<wxString, wxString> Options;
    AppConfigData( wxString wxAppName );
    wxString ConfigPath( void);
    bool ReadOnly(void);
    void Save( void );
private:
    wxString wxsConfigPath = _("");
    bool bConfigPath = false;
    bool bReadOnly = false;
    void ReadOptions( char * );
};
