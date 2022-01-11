//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

#include <sstream>
#include <fstream>
#include <stdexcept>
#include "Config.h"

void AppConfigData::ReadOptions( char *mbstring ) {
    std::string line;
    std::ifstream cfgfile( mbstring );
    for( std::string line; std::getline( cfgfile, line ); ) {
        std::istringstream iss( line );
        std::string id, eq, val;
        bool error = false;
        if( !( iss >> id ) ) {
            error = true;
        } else if( id[0] == '#' ) {
            continue;
        } else if( !( iss >> eq >> val >> std::ws ) || eq != "=" || iss.get() != EOF ) {
            error = true;
        }
        if( error ) {
            // do something appropriate: throw, skip, warn, etc.
        } else {
            Options[id] = val;
        }
    }
}

void AppConfigData::Save( void ) {
    // Save will rewrite the whole file.
    // Comments are all lost
    // This method needs a redesign
    if( ! ReadOnly() ) {
        wxString wxsTemp;
        std::ofstream filestr;
        char mbstring[513];
        wcstombs( mbstring, wxsConfigPath.wc_str(), 512 );
        filestr.open( mbstring );
        if( filestr.is_open() ) {
            if( Options["port"] != "" ) {
                wxsTemp = _("port = ") + _(Options["port"].c_str()) + _("\n");
                filestr.write( wxsTemp.c_str(), wxsTemp.size() );
            }
            if( Options["pen_width"] != "" ) {
                wxsTemp = _("pen_width = ") + _(Options["pen_width"].c_str()) + _("\n");
                filestr.write( wxsTemp.c_str(), wxsTemp.size() );
            }

            if( Options["linesize"] != "" ) {
                wxsTemp = _("linesize = ") + _(Options["linesize"].c_str()) + _("\n");
                filestr.write( wxsTemp.c_str(), wxsTemp.size() );
            }
            if( Options["grid"] != "" ) {
                wxsTemp = _("grid = ") + _(Options["grid"].c_str()) + _("\n");
                filestr.write( wxsTemp.c_str(), wxsTemp.size() );
            }
            if( Options["ruler"] != "" ) {
                wxsTemp = _("ruler = ") + _(Options["ruler"].c_str()) + _("\n");
                filestr.write( wxsTemp.c_str(), wxsTemp.size() );
            }
            if( Options["interval"] != "" ) {
                wxsTemp = _("interval = ") + _(Options["interval"].c_str()) + _("\n");
                filestr.write( wxsTemp.c_str(), wxsTemp.size() );
            }
            if( Options["colour_even_r"] != "" ) {
                wxsTemp = _("colour_even_r = ") + _(Options["colour_even_r"].c_str()) + _("\n");
                filestr.write( wxsTemp.c_str(), wxsTemp.size() );
            }
            if( Options["colour_even_g"] != "" ) {
                 wxsTemp = _("colour_even_g = ") + _(Options["colour_even_g"].c_str()) + _("\n");
                filestr.write( wxsTemp.c_str(), wxsTemp.size() );
            }
            if( Options["colour_even_b"] != "" ) {
                wxsTemp = _("colour_even_b = ") + _(Options["colour_even_b"].c_str()) + _("\n");
                filestr.write( wxsTemp.c_str(), wxsTemp.size() );
            }
            if( Options["colour_odd_r"] != "" ) {
                wxsTemp = _("colour_odd_r = ") + _(Options["colour_odd_r"].c_str()) + _("\n");
                filestr.write( wxsTemp.c_str(), wxsTemp.size() );
            }
            if( Options["colour_odd_g"] != "" ) {
                wxsTemp = _("colour_odd_g = ") + _(Options["colour_odd_g"].c_str()) + _("\n");
                filestr.write( wxsTemp.c_str(), wxsTemp.size() );
            }
            if( Options["colour_odd_b"] != "" ) {
                wxsTemp = _("colour_odd_b = ") + _(Options["colour_odd_b"].c_str()) + _("\n");
                filestr.write( wxsTemp.c_str(), wxsTemp.size() );
            }
            filestr.close();
        }
    }
}

AppConfigData::AppConfigData( wxString wxAppName ) {
    bConfigPath = false;
    std::ifstream filestr;
    wxString wxsTempFile = _( "/etc/" ) + wxAppName + _( ".conf" );
    char mbstring[513];
    wcstombs( mbstring, wxsTempFile.wc_str(), 512 );
    filestr.open( mbstring );
    if( filestr.is_open() ) {
        wxsConfigPath = wxsTempFile;
        bReadOnly = true; // with a .conf file in /etc/ the user cannot change the port, he'll need root assistance
        filestr.close();
        ReadOptions( mbstring );
    } else {
        wxString wxsHome = _( getenv( "HOME" ) );
        wxString wxsTempFile = _( "~/.config/" ) + wxAppName + _( "/" ) + wxAppName + _( ".conf" );
        wxsTempFile.Replace( _( "~" ), wxsHome, false );
        wcstombs( mbstring, wxsTempFile.wc_str(), 512 );
        filestr.open( mbstring );
        if( filestr.is_open() ) {
            wxsConfigPath = wxsTempFile;
            filestr.close();
            ReadOptions( mbstring );
        }
    }
}

wxString AppConfigData::ConfigPath( void ) {
    return wxsConfigPath;
}

bool AppConfigData::ReadOnly( void ) {
    return bReadOnly;
}
