//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

//***************************************************************
// Name:      CurveTracerMain.h
// Purpose:   Code for Application Frame
// Author:    Hennie Peters
// Created:   2021-08-20
// Copyright: Hennie Peters
// License:   the latest GNU General Public License
//***************************************************************

#ifndef CURVETRACERMAIN_H
#define CURVETRACERMAIN_H

#include <wx/wx.h>

//(*Headers(CurveTracerDialog)
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/colordlg.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/radiobox.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/slider.h>
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
    void OnSlider1CmdScroll(wxScrollEvent& event);
    void OnchkRulerVisibleClick(wxCommandEvent& event);
    void OnSlider1CmdScrollChanged(wxScrollEvent& event);
    void OnBitmapButton2Click(wxCommandEvent& event);
    void OnbtnEvenClick(wxCommandEvent& event);
    void OnbtnOddClick(wxCommandEvent& event);
    void OnResize(wxSizeEvent& event);
    void OncboPortTextUpdated(wxCommandEvent& event);
    void OnSlider1CmdSliderUpdated(wxScrollEvent& event);
    void OnSlider1CmdScroll1(wxScrollEvent& event);
    void OnSlider1CmdScrollThumbTrack(wxScrollEvent& event);
    void OnSlider1CmdScroll2(wxScrollEvent& event);
    void OnCheckBox2Click(wxCommandEvent& event);
    void OnchkFatClick(wxCommandEvent& event);
    void OnchkThickClick(wxCommandEvent& event);
    void OnchkThinClick(wxCommandEvent& event);
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
    static const long ID_CHECKBOX2;
    static const long ID_STATICLINE2;
    static const long ID_STATICTEXT6;
    static const long ID_RADIOBOX2;
    static const long ID_STATICTEXT7;
    static const long ID_RADIOBOX1;
    static const long ID_STATICTEXT5;
    static const long ID_SLIDER1;
    static const long ID_STATICLINE4;
    static const long ID_STATICTEXT8;
    static const long ID_STATICTEXT9;
    static const long ID_BITMAPBUTTON2;
    static const long ID_STATICTEXT10;
    static const long ID_BITMAPBUTTON1;
    static const long ID_STATICLINE5;
    static const long ID_STATICTEXT11;
    static const long ID_RADIOBUTTON2;
    static const long ID_RADIOBUTTON1;
    static const long ID_RADIOBUTTON3;
    static const long ID_STATICLINE6;
    static const long ID_STATICTEXT1;
    static const long ID_TIMER1;
    //*)
    //(*Declarations(CurveTracerDialog)
    wxBitmapButton* btnEven;
    wxBitmapButton* btnOdd;
    wxBoxSizer* BoxSizer1;
    wxButton* btnAbout;
    wxButton* btnConnect;
    wxButton* btnDisconnect;
    wxButton* btnQuit;
    wxCheckBox* chkGridVisible;
    wxCheckBox* chkRulerVisible;
    wxColourDialog* ColourDialog1;
    wxComboBox* cboPort;
    wxRadioBox* grpFrequency;
    wxRadioBox* grpVoltage;
    wxRadioButton* optFat;
    wxRadioButton* optThick;
    wxRadioButton* optThin;
    wxSlider* Slider1;
    wxStaticLine* StaticLine1;
    wxStaticLine* StaticLine2;
    wxStaticLine* StaticLine3;
    wxStaticLine* StaticLine4;
    wxStaticLine* StaticLine5;
    wxStaticLine* StaticLine6;
    wxStaticText* StaticText1;
    wxStaticText* StaticText2;
    wxStaticText* StaticText3;
    wxStaticText* StaticText4;
    wxStaticText* StaticText5;
    wxStaticText* StaticText6;
    wxStaticText* StaticText7;
    wxStaticText* StaticText8;
    wxStaticText* lblDebug;
    wxStaticText* lblProgress;
    wxStaticText* wxUpdateInterval;
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
