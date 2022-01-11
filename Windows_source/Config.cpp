//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

#include <sstream>
#include <fstream>
#include <stdexcept>
#include "Config.h"
#include "main.h"

void AppConfigData::ReadOptions( char *sIniFile ) {
    unsigned long iFileLength = 64;
    char *pszBuffer = new char[iFileLength];
    try {
        GetPrivateProfileString( "options", "grid", "true", pszBuffer, iFileLength, sIniFile );
        Options["grid"] = pszBuffer;
        GetPrivateProfileString( "options", "ruler", "false", pszBuffer, iFileLength, sIniFile );
        Options["ruler"] = pszBuffer;
        GetPrivateProfileString( "options", "port", "", pszBuffer, iFileLength, sIniFile );
        Options["port"] = pszBuffer;
        GetPrivateProfileString( "options", "width", "898", pszBuffer, iFileLength, sIniFile );
        Options["width"] = pszBuffer;
        GetPrivateProfileString( "options", "height", "625", pszBuffer, iFileLength, sIniFile );
        Options["height"] = pszBuffer;
        GetPrivateProfileString( "options", "left", "0", pszBuffer, iFileLength, sIniFile );
        Options["left"] = pszBuffer;
        GetPrivateProfileString( "options", "top", "0", pszBuffer, iFileLength, sIniFile );
        Options["top"] = pszBuffer;
        GetPrivateProfileString( "options", "colour1", "52224", pszBuffer, iFileLength, sIniFile );
        Options["colour1"] = pszBuffer;
        GetPrivateProfileString( "options", "colour2", "39680", pszBuffer, iFileLength, sIniFile );
        Options["colour2"] = pszBuffer;
        GetPrivateProfileString( "options", "interval", "14", pszBuffer, iFileLength, sIniFile );
        Options["interval"] = pszBuffer;
        GetPrivateProfileString( "options", "PenWidth", "2", pszBuffer, iFileLength, sIniFile );
        Options["PenWidth"] = pszBuffer;
        delete pszBuffer;
    } catch(...) {
        Options["grid"] = "false";
        Options["ruler"] = "false";
        Options["port"] = "";
        Options["width"] = "898";
        Options["height"] = "625";
        Options["colour1"] = 52224; //even
        Options["colour2"] = 39680; //odd
        Options["interval"] = "14";
        Options["PenWidth"] = "2";
    }
}
//---------------------------------------------------------------------------
void AppConfigData::Save( void ) {
    if( ! ReadOnly() ) {
        AnsiString as;
        if( Options["port"] != "" ) {
            WritePrivateProfileString( "options", "port", Options["port"].c_str(), wxsConfigPath.c_str() );
        }
        if( Options["grid"] != "" ) {
            WritePrivateProfileString( "options", "grid", Options["grid"].c_str(), wxsConfigPath.c_str() );
        }
        if( Options["ruler"] != "" ) {
            WritePrivateProfileString( "options", "ruler", Options["ruler"].c_str(), wxsConfigPath.c_str() );
        }
        if( Options["width"] != "" ) {
            as = Options["width"].c_str();
            WritePrivateProfileString( "options", "width", as.c_str(), wxsConfigPath.c_str() );
        }
        if( Options["height"] != "" ) {
            as = Options["height"].c_str();
            WritePrivateProfileString( "options", "height", as.c_str(), wxsConfigPath.c_str() );
        } 
        if( Options["left"] != "" ) {
            as = Options["left"].c_str();
            WritePrivateProfileString( "options", "left", as.c_str(), wxsConfigPath.c_str() );
        }
        if( Options["top"] != "" ) {
            as = Options["top"].c_str();
            WritePrivateProfileString( "options", "top", as.c_str(), wxsConfigPath.c_str() );
        }
        if( Options["colour1"] != "" ) {
            as = Options["colour1"].c_str();
            WritePrivateProfileString( "options", "colour1", as.c_str(), wxsConfigPath.c_str() );
        }
        if( Options["colour2"] != "" ) {
            as = Options["colour2"].c_str();
            WritePrivateProfileString( "options", "colour2", as.c_str(), wxsConfigPath.c_str() );
        }
        if( Options["interval"] != "" ) {
            as = Options["interval"].c_str();
            WritePrivateProfileString( "options", "interval", as.c_str(), wxsConfigPath.c_str() );
        }
        if( Options["PenWidth"] != "" ) {
            as = Options["PenWidth"].c_str();
            WritePrivateProfileString( "options", "PenWidth", as.c_str(), wxsConfigPath.c_str() );
        }
    }
}
//---------------------------------------------------------------------------
AppConfigData::AppConfigData( AnsiString wxAppName ) {
    wxsConfigPath = "";
    bConfigPath = false;
    bReadOnly = false;
    bConfigPath = false;
    char mbstring[513];

    AnsiString wxsHome = getenv( "APPDATA" );
    AnsiString wxsTempFile;
    wxsTempFile = wxsHome;
    wxsTempFile += "\\";
    wxsTempFile += wxAppName;
    wxsTempFile += "\\";
    if( ! DirectoryExists( wxsTempFile ) ) {
        if( ! CreateDir( wxsTempFile ) ) {
            throw Exception("Cannot create c:\\temp directory.");
        }
    }
    wxsTempFile += wxAppName;
    wxsTempFile += ".conf";
    int iFileHandle;
    if( ! FileExists( wxsTempFile ) ) {
        iFileHandle = FileCreate( wxsTempFile );
        if( ! iFileHandle ) {
            throw Exception("Cannot create config file.");
        } else {
            FileClose(iFileHandle);
        }
    }
    iFileHandle = FileOpen( wxsTempFile, fmOpenRead );
    if( ! iFileHandle ) {
        throw Exception("Cannot open config file.");
    } else {
        wxsConfigPath = wxsTempFile.c_str();
        FileClose(iFileHandle);
        ReadOptions( wxsTempFile.c_str() );
    }
}
//---------------------------------------------------------------------------
AnsiString AppConfigData::ConfigPath( void ) {
    return wxsConfigPath.c_str();
}
//---------------------------------------------------------------------------
bool AppConfigData::ReadOnly( void ) {
    return bReadOnly;
}
//---------------------------------------------------------------------------
