//***************************************************************
// Name:      CurveTracerMain.h
// Purpose:   Code for Application Frame
// Author:    Hennie Peters (hennie[dot]peters[at]gmail[dot]com)
// Created:   2021-08-20
// Copyright: Hennie Peters
// License:
//***************************************************************

#ifndef CURVETRACERMAIN_H
#define CURVETRACERMAIN_H

#include <wx/wx.h>

//(*Headers(CurveTracerDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/timer.h>
//*)

class CurveTracerDialog: public wxDialog {
public:
    CurveTracerDialog( wxWindow* parent, wxWindowID id = -1 );
    virtual ~CurveTracerDialog();
    void ProgressIndicator( void );
    //union uInputData *OpenSerial(void);
private:
    //(*Handlers(CurveTracerDialog)
    void OnQuit( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );
    void OnTimer1Trigger(wxTimerEvent& event);
    void OnbtnConnectClick(wxCommandEvent& event);
    void OnbtnDisconnectClick(wxCommandEvent& event);
    void OnGridVisibleClick(wxCommandEvent& event);
    void OnchkGridVisibleClick(wxCommandEvent& event);
    void OngrpVoltageSelect(wxCommandEvent& event);
    void OngrpFrequencySelect(wxCommandEvent& event);
    //*)
    //(*Identifiers(CurveTracerDialog)
    static const long ID_STATICTEXT2;
    static const long ID_BUTTON1;
    static const long ID_BUTTON2;
    static const long ID_STATICTEXT3;
    static const long ID_STATICLINE1;
    static const long ID_STATICTEXT4;
    static const long ID_COMBOBOX1;
    static const long ID_BUTTON4;
    static const long ID_BUTTON5;
    static const long ID_STATICLINE3;
    static const long ID_CHECKBOX1;
    static const long ID_STATICLINE2;
    static const long ID_STATICTEXT6;
    static const long ID_RADIOBOX2;
    static const long ID_STATICTEXT7;
    static const long ID_RADIOBOX1;
    static const long ID_STATICTEXT1;
    static const long ID_TIMER1;
    //*)
    //(*Declarations(CurveTracerDialog)
    wxBoxSizer* BoxSizer1;
    wxButton* btnAbout;
    wxButton* btnConnect;
    wxButton* btnDisconnect;
    wxButton* btnQuit;
    wxCheckBox* chkGridVisible;
    wxComboBox* cboPort;
    wxRadioBox* grpFrequency;
    wxRadioBox* grpVoltage;
    wxStaticLine* StaticLine1;
    wxStaticLine* StaticLine2;
    wxStaticLine* StaticLine3;
    wxStaticText* StaticText1;
    wxStaticText* StaticText2;
    wxStaticText* StaticText4;
    wxStaticText* StaticText5;
    wxStaticText* lblDebug;
    wxStaticText* lblProgress;
    wxTimer Timer1;
    //*)

    bool bReady = false;
    bool bCboPortEnabled = true;
    struct PaintData *dataptr;
    void OnPaint( wxPaintEvent & evt );
    void OnSelect( wxCommandEvent& event );
    void Outdated( uint16_t coords );
    DECLARE_EVENT_TABLE()
};

#endif // CURVETRACERMAIN_H
