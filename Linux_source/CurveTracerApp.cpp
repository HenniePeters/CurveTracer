//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

//***************************************************************
// Name:      CurveTracerApp.cpp
// Purpose:   Defines Application Class
// Author:    Hennie Peters
// Created:   2021-08-20
// Copyright: Hennie Peters
// License:   the latest GNU General Public License
//***************************************************************

#include "CurveTracerApp.h"

//(*AppHeaders
#include "CurveTracerMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP( CurveTracerApp );

bool CurveTracerApp::OnInit() {
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	CurveTracerDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;
}


