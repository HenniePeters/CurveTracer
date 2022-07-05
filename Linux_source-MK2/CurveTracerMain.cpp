//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

//***************************************************************
// Name:      CurveTracerMain.cpp
// Purpose:   Code for Application Frame
// Author:    Hennie Peters
// Created:   2021-08-20
// Copyright: Hennie Peters
// License:   the latest GNU General Public License
//***************************************************************
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <wx/msgdlg.h>
#include <wx/wx.h>
#include "Serial.h"
#include "CommunicationProtocol.h"
#include "CurveTracerMain.h"
#include "Config.h"
#include "Graph.h"
#include "Fifo.h"
#include "CurveTracer.xpm"

#ifdef _WIN32
//#include <Windows.h>
#else
#include <unistd.h>
#endif

struct f fields;
struct f held;
unsigned char buffer[sizeof( struct f )];
class PaintGraphics *graph = NULL;
int iPrintingInterval = 50;
int iFullScaleX = 1, iFullScaleY = 1, iLineSize = 2;

//(*InternalHeaders(CurveTracerDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f
};

wxString wxbuildinfo( wxbuildinfoformat format ) {
    wxString wxbuild( _T( "\r\nCurveTracer v. 1.0.0 beta\r\n\r\n" ) );
    wxbuild << _( CB_VERSION );
    wxbuild << _( "\r\n" );
    wxbuild << wxVERSION_STRING;
    if( format == long_f ) {
#if defined(__WXMSW__)
        wxbuild << _T( "-Windows" );
#elif defined(__UNIX__)
        wxbuild << _T( "-Linux" );
#endif
#if wxUSE_UNICODE
        wxbuild << _T( "-Unicode build" );
#else
        wxbuild << _T( "-ANSI build" );
#endif // wxUSE_UNICODE
    }
    return wxbuild;
}

//(*IdInit(CurveTracerDialog)
const long CurveTracerDialog::ID_STATICTEXT2 = wxNewId();
const long CurveTracerDialog::ID_BUTTON1 = wxNewId();
const long CurveTracerDialog::ID_BUTTON2 = wxNewId();
const long CurveTracerDialog::ID_STATICTEXT3 = wxNewId();
const long CurveTracerDialog::ID_STATICLINE1 = wxNewId();
const long CurveTracerDialog::ID_STATICTEXT4 = wxNewId();
const long CurveTracerDialog::ID_COMBOBOX1 = wxNewId();
const long CurveTracerDialog::ID_BUTTON4 = wxNewId();
const long CurveTracerDialog::ID_BUTTON5 = wxNewId();
const long CurveTracerDialog::ID_STATICLINE3 = wxNewId();
const long CurveTracerDialog::ID_CHECKBOX1 = wxNewId();
const long CurveTracerDialog::ID_CHECKBOX2 = wxNewId();
const long CurveTracerDialog::ID_STATICLINE2 = wxNewId();
const long CurveTracerDialog::ID_STATICTEXT6 = wxNewId();
const long CurveTracerDialog::ID_RADIOBOX2 = wxNewId();
const long CurveTracerDialog::ID_STATICTEXT7 = wxNewId();
const long CurveTracerDialog::ID_RADIOBOX1 = wxNewId();
const long CurveTracerDialog::ID_STATICTEXT5 = wxNewId();
const long CurveTracerDialog::ID_SLIDER1 = wxNewId();
const long CurveTracerDialog::ID_STATICLINE4 = wxNewId();
const long CurveTracerDialog::ID_STATICTEXT8 = wxNewId();
const long CurveTracerDialog::ID_BITMAPBTN_EVEN = wxNewId();
const long CurveTracerDialog::ID_STATICTEXT9 = wxNewId();
const long CurveTracerDialog::ID_BITMAPBTN_ODD = wxNewId();
const long CurveTracerDialog::ID_STATICTEXT10 = wxNewId();
const long CurveTracerDialog::ID_BITMAPBTN_STORE = wxNewId();
const long CurveTracerDialog::ID_STATICTEXT12 = wxNewId();
const long CurveTracerDialog::ID_STATICLINE5 = wxNewId();
const long CurveTracerDialog::ID_STATICTEXT11 = wxNewId();
const long CurveTracerDialog::ID_RADIOBUTTON2 = wxNewId();
const long CurveTracerDialog::ID_RADIOBUTTON1 = wxNewId();
const long CurveTracerDialog::ID_RADIOBUTTON3 = wxNewId();
const long CurveTracerDialog::ID_STATICLINE6 = wxNewId();
const long CurveTracerDialog::ID_TOGGLEBUTTON1 = wxNewId();
const long CurveTracerDialog::ID_STATICTEXT1 = wxNewId();
const long CurveTracerDialog::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE( CurveTracerDialog, wxDialog )
    //(*EventTable(CurveTracerDialog)
    //*)
    EVT_PAINT( CurveTracerDialog::OnPaint )
    EVT_COMBOBOX( ID_COMBOBOX1, CurveTracerDialog::OnSelect )
END_EVENT_TABLE()

////////////////////////////////////////////////////////////////////////////////////////
/// global variables
AppConfigData *AppConfig = new AppConfigData( _( "CurveTracer" ) );
int comport_nr = 0;
class FifoClass fifo;
wxString wxsComPort = "";

#if  defined(__WXMSW__)
const int serial_port_count = 32;
const char *serial_ports[] = {"", "COM1",  "COM2",  "COM3",  "COM4",    "COM5",  "COM6",  "COM7",  "COM8",  "COM9",  "COM10", "COM11", "COM12",
                              "COM13", "COM14", "COM15", "COM16",   "COM17", "COM18", "COM19", "COM20", "COM21", "COM22", "COM23", "COM24",
                              "COM25", "COM26", "COM27", "COM28",   "COM29", "COM30", "COM31", "COM32"
                             };
#elif defined(__UNIX__)
const int serial_port_count = 38;
const char *serial_ports[] = {"", "ttyS0",   "ttyS1",   "ttyS2",   "ttyS3",     "ttyS4",    "ttyS5",    "ttyS6",   "ttyS7",     "ttyS8",   "ttyS9",   "ttyS10", "ttyS11",
                              "ttyS12",  "ttyS13",  "ttyS14",  "ttyS15",    "ttyUSB0",  "ttyUSB1",  "ttyUSB2", "ttyUSB3",   "ttyUSB4", "ttyUSB5",
                              "ttyAMA0", "ttyAMA1", "ttyACM0", "ttyACM1",   "rfcomm0",  "rfcomm1",  "ircomm0", "ircomm1",   "cuau0",   "cuau1",   "cuau2",   "cuau3",
                              "cuaU0",   "cuaU1",   "cuaU2",   "cuaU3"
                             };
#endif

////////////////////////////////////////////////////////////////////////////////////////
CurveTracerDialog::CurveTracerDialog( wxWindow* parent, wxWindowID id ) {
    //(*Initialize(CurveTracerDialog)
    wxBoxSizer* BoxSizer10;
    wxBoxSizer* BoxSizer11;
    wxBoxSizer* BoxSizer12;
    wxBoxSizer* BoxSizer13;
    wxBoxSizer* BoxSizer14;
    wxBoxSizer* BoxSizer15;
    wxBoxSizer* BoxSizer16;
    wxBoxSizer* BoxSizer17;
    wxBoxSizer* BoxSizer18;
    wxBoxSizer* BoxSizer19;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer3;
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer5;
    wxBoxSizer* BoxSizer6;
    wxBoxSizer* BoxSizer7;
    wxBoxSizer* BoxSizer8;
    wxBoxSizer* BoxSizer9;
    wxBoxSizer* BoxSizerEven;
    wxBoxSizer* BoxSizerOdd;
    wxBoxSizer* BoxSizerStore;

    Create(parent, wxID_ANY, _("CurveTracer"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(1000,700));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer3 = new wxBoxSizer(wxVERTICAL);
    BoxSizer4 = new wxBoxSizer(wxVERTICAL);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, wxEmptyString, wxDefaultPosition, wxSize(750,750), 0, _T("ID_STATICTEXT2"));
    BoxSizer4->Add(StaticText2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    BoxSizer3->Add(BoxSizer4, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_TOP|wxFIXED_MINSIZE, 0);
    BoxSizer5 = new wxBoxSizer(wxVERTICAL);
    BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    btnAbout = new wxButton(this, ID_BUTTON1, _("About"), wxDefaultPosition, wxSize(100,34), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(btnAbout, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    btnQuit = new wxButton(this, ID_BUTTON2, _("Quit"), wxDefaultPosition, wxSize(100,34), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer2->Add(btnQuit, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    BoxSizer8->Add(BoxSizer2, 0, wxALIGN_LEFT, 4);
    lblProgress = new wxStaticText(this, ID_STATICTEXT3, _("/"), wxDefaultPosition, wxSize(189,31), wxALIGN_CENTRE, _T("ID_STATICTEXT3"));
    wxFont lblProgressFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("DejaVu Sans"),wxFONTENCODING_DEFAULT);
    lblProgress->SetFont(lblProgressFont);
    BoxSizer8->Add(lblProgress, 0, wxALL|wxEXPAND|wxSHAPED, 5);
    BoxSizer5->Add(BoxSizer8, 0, wxEXPAND, 5);
    BoxSizer6 = new wxBoxSizer(wxVERTICAL);
    StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    StaticLine1->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
    BoxSizer6->Add(StaticLine1, 0, wxTOP|wxBOTTOM|wxEXPAND, 1);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT4, _("Serial port:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    BoxSizer6->Add(StaticText1, 0, wxALL|wxEXPAND, 5);
    cboPort = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxDefaultPosition, wxSize(155,34), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
    BoxSizer6->Add(cboPort, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    btnConnect = new wxButton(this, ID_BUTTON4, _("Connect"), wxDefaultPosition, wxSize(108,32), 0, wxDefaultValidator, _T("ID_BUTTON4"));
    BoxSizer9->Add(btnConnect, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnDisconnect = new wxButton(this, ID_BUTTON5, _("Disconnect"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    BoxSizer9->Add(btnDisconnect, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer6->Add(BoxSizer9, 0, wxEXPAND, 5);
    BoxSizer5->Add(BoxSizer6, 0, wxTOP|wxEXPAND, 15);
    BoxSizer7 = new wxBoxSizer(wxVERTICAL);
    BoxSizer10 = new wxBoxSizer(wxVERTICAL);
    StaticLine3 = new wxStaticLine(this, ID_STATICLINE3, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE3"));
    StaticLine3->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
    BoxSizer10->Add(StaticLine3, 0, wxTOP|wxBOTTOM|wxEXPAND, 1);
    BoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    chkGridVisible = new wxCheckBox(this, ID_CHECKBOX1, _("Show grid"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    chkGridVisible->SetValue(false);
    BoxSizer13->Add(chkGridVisible, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    chkRulerVisible = new wxCheckBox(this, ID_CHECKBOX2, _("Show ruler"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    chkRulerVisible->SetValue(false);
    chkRulerVisible->Disable();
    BoxSizer13->Add(chkRulerVisible, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer10->Add(BoxSizer13, 0, wxTOP|wxEXPAND, 10);
    BoxSizer7->Add(BoxSizer10, 0, wxTOP|wxEXPAND, 20);
    BoxSizer11 = new wxBoxSizer(wxVERTICAL);
    StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
    StaticLine2->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
    BoxSizer11->Add(StaticLine2, 0, wxTOP|wxBOTTOM|wxEXPAND, 1);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT6, _("Measuring voltage:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    StaticText4->Hide();
    BoxSizer11->Add(StaticText4, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
    wxString __wxRadioBoxChoices_1[2] =
    {
    	_("Sine 2V"),
    	_("Sine 12V")
    };
    grpVoltage = new wxRadioBox(this, ID_RADIOBOX2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 2, __wxRadioBoxChoices_1, 1, wxRA_HORIZONTAL, wxDefaultValidator, _T("ID_RADIOBOX2"));
    grpVoltage->Hide();
    BoxSizer11->Add(grpVoltage, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer7->Add(BoxSizer11, 0, wxTOP|wxEXPAND, 15);
    BoxSizer12 = new wxBoxSizer(wxVERTICAL);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT7, _("Frequency:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    BoxSizer12->Add(StaticText5, 0, wxTOP|wxLEFT|wxEXPAND, 5);
    wxString __wxRadioBoxChoices_2[3] =
    {
    	_("50 Hz (20 ms)"),
    	_("200 Hz (5 ms)"),
    	_("1 kHz (1 ms)")
    };
    grpFrequency = new wxRadioBox(this, ID_RADIOBOX1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 3, __wxRadioBoxChoices_2, 2, wxRA_SPECIFY_COLS|wxRA_HORIZONTAL|wxRA_SPECIFY_ROWS|wxRA_VERTICAL, wxDefaultValidator, _T("ID_RADIOBOX1"));
    BoxSizer12->Add(grpFrequency, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer7->Add(BoxSizer12, 0, wxTOP|wxEXPAND, 15);
    BoxSizer14 = new wxBoxSizer(wxVERTICAL);
    wxUpdateInterval = new wxStaticText(this, ID_STATICTEXT5, _("Update interval: 250 ms"), wxDefaultPosition, wxSize(231,20), 0, _T("ID_STATICTEXT5"));
    BoxSizer14->Add(wxUpdateInterval, 0, wxTOP|wxLEFT|wxEXPAND, 5);
    Slider1 = new wxSlider(this, ID_SLIDER1, 1, 1, 20, wxDefaultPosition, wxSize(236,31), 0, wxDefaultValidator, _T("ID_SLIDER1"));
    BoxSizer14->Add(Slider1, 1, wxEXPAND, 5);
    StaticLine4 = new wxStaticLine(this, ID_STATICLINE4, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE4"));
    BoxSizer14->Add(StaticLine4, 0, wxTOP|wxBOTTOM|wxEXPAND, 1);
    BoxSizer7->Add(BoxSizer14, 0, wxTOP|wxEXPAND, 15);
    BoxSizer15 = new wxBoxSizer(wxVERTICAL);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT8, _("Samples colour:"), wxDefaultPosition, wxSize(299,17), 0, _T("ID_STATICTEXT8"));
    BoxSizer15->Add(StaticText3, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizerEven = new wxBoxSizer(wxVERTICAL);
    btnEven = new wxBitmapButton(this, ID_BITMAPBTN_EVEN, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBTN_EVEN"));
    BoxSizerEven->Add(btnEven, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT9, _("Even"), wxDefaultPosition, wxSize(34,17), 0, _T("ID_STATICTEXT9"));
    BoxSizerEven->Add(StaticText6, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 15);
    BoxSizer16->Add(BoxSizerEven, 1, wxALIGN_LEFT, 0);
    BoxSizerOdd = new wxBoxSizer(wxVERTICAL);
    btnOdd = new wxBitmapButton(this, ID_BITMAPBTN_ODD, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBTN_ODD"));
    BoxSizerOdd->Add(btnOdd, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    StaticText7 = new wxStaticText(this, ID_STATICTEXT10, _("Odd"), wxDefaultPosition, wxSize(31,17), 0, _T("ID_STATICTEXT10"));
    BoxSizerOdd->Add(StaticText7, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 15);
    BoxSizer16->Add(BoxSizerOdd, 1, wxALIGN_LEFT, 0);
    BoxSizerStore = new wxBoxSizer(wxVERTICAL);
    btnHeld = new wxBitmapButton(this, ID_BITMAPBTN_STORE, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBTN_STORE"));
    BoxSizerStore->Add(btnHeld, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 3);
    StaticText9 = new wxStaticText(this, ID_STATICTEXT12, _("Stored"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    BoxSizerStore->Add(StaticText9, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 15);
    BoxSizer16->Add(BoxSizerStore, 1, wxALIGN_LEFT, 0);
    BoxSizer15->Add(BoxSizer16, 0, wxEXPAND, 5);
    BoxSizer7->Add(BoxSizer15, 1, wxTOP|wxEXPAND, 15);
    BoxSizer17 = new wxBoxSizer(wxVERTICAL);
    StaticLine5 = new wxStaticLine(this, ID_STATICLINE5, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE5"));
    BoxSizer17->Add(StaticLine5, 0, wxBOTTOM|wxEXPAND, 15);
    StaticText8 = new wxStaticText(this, ID_STATICTEXT11, _("Line size:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    BoxSizer17->Add(StaticText8, 0, wxALL|wxALIGN_LEFT, 5);
    BoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    optFat = new wxRadioButton(this, ID_RADIOBUTTON2, _("Fat"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
    BoxSizer18->Add(optFat, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    optThick = new wxRadioButton(this, ID_RADIOBUTTON1, _("Thick"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
    optThick->SetValue(true);
    BoxSizer18->Add(optThick, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    optThin = new wxRadioButton(this, ID_RADIOBUTTON3, _("Thin"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
    BoxSizer18->Add(optThin, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer17->Add(BoxSizer18, 0, wxEXPAND, 5);
    StaticLine6 = new wxStaticLine(this, ID_STATICLINE6, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE6"));
    BoxSizer17->Add(StaticLine6, 0, wxTOP|wxBOTTOM|wxEXPAND, 1);
    BoxSizer7->Add(BoxSizer17, 0, wxEXPAND, 15);
    BoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    btnToggleHold = new wxToggleButton(this, ID_TOGGLEBUTTON1, _("Hold sample"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON1"));
    BoxSizer19->Add(btnToggleHold, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer7->Add(BoxSizer19, 0, wxTOP, 15);
    lblDebug = new wxStaticText(this, ID_STATICTEXT1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    BoxSizer7->Add(lblDebug, 1, wxALL|wxEXPAND, 5);
    BoxSizer5->Add(BoxSizer7, 0, wxEXPAND, 5);
    BoxSizer1->Add(BoxSizer5, 0, wxALL|wxEXPAND, 5);
    SetSizer(BoxSizer1);
    Timer1.SetOwner(this, ID_TIMER1);
    Timer1.Start(50, false);
    ColourDialog1 = new wxColourDialog(this);
    SetSizer(BoxSizer1);
    Layout();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CurveTracerDialog::OnAbout);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CurveTracerDialog::OnQuit);
    Connect(ID_COMBOBOX1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&CurveTracerDialog::OncboPortTextUpdated);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CurveTracerDialog::OnbtnConnectClick);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CurveTracerDialog::OnbtnDisconnectClick);
    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CurveTracerDialog::OnchkGridVisibleClick);
    Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CurveTracerDialog::OnchkRulerVisibleClick);
    Connect(ID_RADIOBOX2,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&CurveTracerDialog::OngrpVoltageSelect);
    Connect(ID_RADIOBOX1,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&CurveTracerDialog::OngrpFrequencySelect);
    Connect(ID_SLIDER1,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&CurveTracerDialog::OnSlider1CmdScrollThumbTrack);
    Connect(ID_SLIDER1,wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&CurveTracerDialog::OnSlider1CmdScrollChanged);
    Connect(ID_BITMAPBTN_EVEN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CurveTracerDialog::OnbtnEvenClick);
    Connect(ID_BITMAPBTN_ODD,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CurveTracerDialog::OnbtnOddClick);
    Connect(ID_BITMAPBTN_STORE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CurveTracerDialog::OnbtnHeldClick);
    Connect(ID_RADIOBUTTON2,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&CurveTracerDialog::OnchkFatClick);
    Connect(ID_RADIOBUTTON1,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&CurveTracerDialog::OnchkThickClick);
    Connect(ID_RADIOBUTTON3,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&CurveTracerDialog::OnchkThinClick);
    Connect(ID_TOGGLEBUTTON1,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&CurveTracerDialog::OnbtnToggleHoldToggle);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&CurveTracerDialog::OnTimer1Trigger);
    //*)
    SetIcon( CurveTracer_xpm );
    //btnHoldSample->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CurveTracerDialog::OnbtnHoldSampleRightClick), NULL, this);
    wxString wxsTemp = AppConfig->Options["colour_even_r"];
    wxString wxsTemp2 = AppConfig->Options["colour_even_g"];
    wxString wxsTemp3 = AppConfig->Options["colour_even_b"];
    unsigned char r, g, b;
    if( wxsTemp != _( "" ) && wxsTemp2 != _( "" ) && wxsTemp3 != _( "" )  ) {
        r = wxAtoi( wxsTemp );
        g = wxAtoi( wxsTemp2 );
        b = wxAtoi( wxsTemp3 );
        btnEven->SetBackgroundColour( wxColour( r, g, b ) );
    } else {
        btnEven->SetBackgroundColour( wxColour( 0, 255, 0 ) );
    }
    wxsTemp = AppConfig->Options["colour_odd_r"];
    wxsTemp2 = AppConfig->Options["colour_odd_g"];
    wxsTemp3 = AppConfig->Options["colour_odd_b"];
    if( wxsTemp != _( "" ) && wxsTemp2 != _( "" ) && wxsTemp3 != _( "" )  ) {
        r = wxAtoi( wxsTemp );
        g = wxAtoi( wxsTemp2 );
        b = wxAtoi( wxsTemp3 );
        btnOdd->SetBackgroundColour( wxColour( r, g, b ) );
    } else {
        btnOdd->SetBackgroundColour( wxColour( 0, 192, 0 ) );
    }
    wxsTemp = AppConfig->Options["colour_store_r"];
    wxsTemp2 = AppConfig->Options["colour_store_g"];
    wxsTemp3 = AppConfig->Options["colour_store_b"];
    if( wxsTemp != _( "" ) && wxsTemp2 != _( "" ) && wxsTemp3 != _( "" )  ) {
        r = wxAtoi( wxsTemp );
        g = wxAtoi( wxsTemp2 );
        b = wxAtoi( wxsTemp3 );
        btnHeld->SetBackgroundColour( wxColour( r, g, b ) );
    } else {
        btnHeld->SetBackgroundColour( wxColour( 100, 100, 100 ) );
    }
    wxsTemp = AppConfig->Options["linesize"];
    if( wxsTemp == _( "1" ) ) {
        iLineSize = 1;
        optThin->SetValue( true );
    } else if( wxsTemp == _( "4" ) ) {
        iLineSize = 4;
        optFat->SetValue( true );
    } else {
        iLineSize = 2;
        optThick->SetValue( true );
    }
    wxsTemp = AppConfig->Options["grid"];
    if( wxsTemp == _( "true" ) ) {
        chkGridVisible->SetValue( true );
    } else {
        chkGridVisible->SetValue( false );
    }
    wxsTemp = AppConfig->Options["ruler"];
    if( wxsTemp == _( "true" ) ) {
        chkRulerVisible->SetValue( true );
    } else {
        chkRulerVisible->SetValue( false );
    }
    wxsTemp = AppConfig->Options["interval"];
    if( wxsTemp != _( "" ) && wxsTemp.IsNumber() ) {
        Slider1->SetValue( wxAtoi( wxsTemp ) );
    } else {
        Slider1->SetValue( 15 );
    }
    char buf[64];
    sprintf( buf, "Update interval: %d ms", Slider1->GetValue() * 50 );
    wxUpdateInterval->SetLabel( _( buf ) );
    iPrintingInterval = Slider1->GetValue() * 50;
    cboPort->SetValue( AppConfig->Options["port"] );
    if( cboPort->GetValue() != _( "" ) && AppConfig->ReadOnly() ) {
        // disable open
        cboPort->SetEditable( false );
        bCboPortEnabled = false;
    }
    bCboPortEnabled = ! AppConfig->ReadOnly();
    for( unsigned int i = 1; i < serial_port_count; i++ ) {
        int tmp = RS232_GetPortnr( serial_ports[i] );
        if( tmp > 0 ) {
            char mode[] = {'8', 'N', '1', 0};
            int bdrate = 115200;           /* 9600 baud */
            // wxMessageBox( _(serial_ports[i]), _( "comport" ) );
            if( 0 == RS232_OpenComport( tmp, bdrate, mode, 0 ) ) {
                RS232_CloseComport( tmp );
                cboPort->Append( _( serial_ports[i] ) );
            }
        }
    }
    cboPort->SetValue( AppConfig->Options["port"] );
    if( comport_nr == 0 ) {
        char mode[] = {'8', 'N', '1', 0};
        int bdrate = 115200;           /* 9600 baud */
        char mbstring[513];
        wxsComPort = cboPort->GetValue();
        wcstombs( mbstring, wxsComPort.wc_str(), 512 );
        comport_nr = RS232_GetPortnr( mbstring );
        if( RS232_OpenComport( comport_nr, bdrate, mode, 0 ) ) {
            printf( "Cannot open comport\n" );
            comport_nr = 0;
            btnConnect->SetForegroundColour( *wxBLACK );
            btnDisconnect->SetForegroundColour( wxColor( 128, 128, 128 ) );
        } else {
            btnDisconnect->SetForegroundColour( *wxBLACK );
            btnConnect->SetForegroundColour( wxColor( 128, 128, 128 ) );
        }
    }
}

void CurveTracerDialog::Outdated( uint16_t coords ) {
    Timer1.Stop();
    std::wstringstream msg;
    msg << L"Number of coordinates received ("  << fields.coords << L") differs from the number expected by the program (" << COORDS << L")" << std::endl << std::endl;
    msg << L"Either this program or the embedded code in your Arduino board is outdated!";
    msg << std::ends;
    wxMessageBox( msg.str().c_str(), _( "About CurveTracer" ) );
    //Close();        // Close does not terminate completely when the aboutbox is showing
    std::terminate(); // terminate closes all windows
}


void CurveTracerDialog::OncboPortTextUpdated( wxCommandEvent& event ) {
    wxsComPort = cboPort->GetValue();
}

void CurveTracerDialog::OnbtnConnectClick( wxCommandEvent& event ) {
    if( comport_nr == 0 ) {
        if( btnConnect->GetForegroundColour() == *wxBLACK ) {
            char mode[] = {'8', 'N', '1', 0};
            int bdrate = 115200;           // 9600 baud
            char mbstring[513];
            wcstombs( mbstring, wxsComPort.wc_str(), 512 );
            comport_nr = RS232_GetPortnr( mbstring );
            if( RS232_OpenComport( comport_nr, bdrate, mode, 0 ) ) {
                printf( "Cannot open comport\n" );
                comport_nr = 0;
                btnConnect->SetForegroundColour( *wxBLACK );
                btnDisconnect->SetForegroundColour( wxColor( 128, 128, 128 ) );
            } else {
                btnDisconnect->SetForegroundColour( *wxBLACK );
                btnConnect->SetForegroundColour( wxColor( 128, 128, 128 ) );
                wxString wxTemp = AppConfig->Options["port"];
                if( wxTemp != wxsComPort ) {
                    AppConfig->Save();
                }
            }
        }
    }
}

void CurveTracerDialog::OnbtnDisconnectClick( wxCommandEvent& event ) {
    if( comport_nr > 0 ) {
        RS232_CloseComport( comport_nr );
        comport_nr = 0;
    }
    //btnDisconnect->SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    btnConnect->SetForegroundColour( *wxBLACK );
    btnDisconnect->SetForegroundColour( wxColor( 128, 128, 128 ) );
    if( bCboPortEnabled ) {
        cboPort->Clear();
        for( unsigned int i = 0; i < serial_port_count; i++ ) {
            int tmp = RS232_GetPortnr( serial_ports[i] );
            if( tmp > 0 ) {
                char mode[] = {'8', 'N', '1', 0};
                int bdrate = 115200;           /* 9600 baud */
                if( 0 == RS232_OpenComport( tmp, bdrate, mode, 0 ) ) {
                    RS232_CloseComport( tmp );
                    cboPort->Append( _( serial_ports[i] ) );
                }
            }
        }
    }
}

void CurveTracerDialog::OnSelect( wxCommandEvent& event ) {
    if( bCboPortEnabled ) {
        OnbtnDisconnectClick( event );
        wxsComPort = event.GetString(); // get new value
        AppConfig->Options["port"] = wxsComPort;
        AppConfig->Save();
        cboPort->SetValue( wxsComPort );
    } else {
        wxString wxsTemp;
        wxsTemp = "The config file /etc/CurveTracer.conf is NOT writable,\n changes will not be stored.";
        wxMessageBox( wxsTemp, _( "Read-only config" ) );
    }
}

void CurveTracerDialog::OnchkGridVisibleClick( wxCommandEvent& event ) {
    if( chkGridVisible->IsChecked() != AppConfig->Options["grid"] ) {
        AppConfig->Options["grid"] = chkGridVisible->IsChecked() ? "true" : "false";
        AppConfig->Save();
        wxString wxsTemp = AppConfig->Options["grid"];
        graph->GridVisible =  ( wxsTemp == _( "true" ) ) ? true : false;
        Refresh();
    }
}

void CurveTracerDialog::OnchkRulerVisibleClick( wxCommandEvent& event ) {
    if( chkRulerVisible->IsChecked() != AppConfig->Options["ruler"] ) {
        AppConfig->Options["ruler"] = chkRulerVisible->IsChecked() ? "true" : "false";
        AppConfig->Save();
        wxString wxsTemp = AppConfig->Options["ruler"];
        graph->RulerVisible =  ( wxsTemp == _( "true" ) ) ? true : false;
        Refresh();
    }
}

void CurveTracerDialog::OngrpVoltageSelect( wxCommandEvent& event ) {
    wxString wxsVoltage;
    wxsVoltage = event.GetString();
    if( wxsVoltage == _( "Sine 2V" ) ) {
        graph->SetSpacingVert( 2 );
        graph->SetFactorMulVert( 1 );
        graph->SetFactorDivVert( 1 );
        graph->SetVertUnit( " V" );
    } else if( wxsVoltage == _( "Sine 12V" ) ) {
        graph->SetSpacingVert( 6 );
        graph->SetFactorMulVert( 2 );
        graph->SetFactorDivVert( 1 );
        graph->SetVertUnit( " V" );
    }
}

void CurveTracerDialog::OnSlider1CmdScrollThumbTrack( wxScrollEvent& event ) {
    char buf[64];
    int tmp = Slider1->GetValue() * 50;
    sprintf( buf, "Update interval: %d ms", tmp );
    wxUpdateInterval->SetLabel( _( buf ) );
}

void CurveTracerDialog::OnSlider1CmdScrollChanged( wxScrollEvent& event ) {
    char buf[64];
    iPrintingInterval = Slider1->GetValue() * 50;
    sprintf( buf, "Update interval: %d ms", iPrintingInterval );
    wxUpdateInterval->SetLabel( _( buf ) );
    sprintf( buf, "%d", Slider1->GetValue() );
    AppConfig->Options["interval"] = _( buf );
    AppConfig->Save();
}

void CurveTracerDialog::OnbtnEvenClick( wxCommandEvent& event ) {
    wxColourData data;
    data.SetColour( btnEven->GetBackgroundColour() );
    data.SetChooseFull( true );
    wxColourDialog dialog( this, &data );
    if( dialog.ShowModal() == wxID_OK ) {
        btnEven->SetBackgroundColour( dialog.GetColourData().GetColour() );
    }
    char buf[8];
    sprintf( buf, "%d", dialog.GetColourData().GetColour().Red() );
    AppConfig->Options["colour_even_r"] = _( buf );
    sprintf( buf, "%d", dialog.GetColourData().GetColour().Green() );
    AppConfig->Options["colour_even_g"] = _( buf );
    sprintf( buf, "%d", dialog.GetColourData().GetColour().Blue() );
    AppConfig->Options["colour_even_b"] = _( buf );
    AppConfig->Save();
    if( graph != NULL ) {
        graph->ChangeSamplesColour( btnEven->GetBackgroundColour(), btnOdd->GetBackgroundColour(), btnHeld->GetBackgroundColour(), iLineSize );
    }
}

void CurveTracerDialog::OnbtnOddClick( wxCommandEvent& event ) {
    wxColourData data;
    data.SetColour( btnOdd->GetBackgroundColour() );
    data.SetChooseFull( true );
    wxColourDialog dialog( this, &data );
    if( dialog.ShowModal() == wxID_OK ) {
        btnOdd->SetBackgroundColour( dialog.GetColourData().GetColour() );
    }
    char buf[8];
    sprintf( buf, "%d", dialog.GetColourData().GetColour().Red() );
    AppConfig->Options["colour_odd_r"] = _( buf );
    sprintf( buf, "%d", dialog.GetColourData().GetColour().Green() );
    AppConfig->Options["colour_odd_g"] = _( buf );
    sprintf( buf, "%d", dialog.GetColourData().GetColour().Blue() );
    AppConfig->Options["colour_odd_b"] = _( buf );
    AppConfig->Save();
    if( graph != NULL ) {
        graph->ChangeSamplesColour( btnEven->GetBackgroundColour(), btnOdd->GetBackgroundColour(), btnHeld->GetBackgroundColour(), iLineSize );
    }
}

void CurveTracerDialog::OnbtnHeldClick( wxCommandEvent& event ) {
    wxColourData data;
    data.SetColour( btnHeld->GetBackgroundColour() );
    data.SetChooseFull( true );
    wxColourDialog dialog( this, &data );
    if( dialog.ShowModal() == wxID_OK ) {
        btnHeld->SetBackgroundColour( dialog.GetColourData().GetColour() );
    }
    char buf[8];
    sprintf( buf, "%d", dialog.GetColourData().GetColour().Red() );
    AppConfig->Options["colour_store_r"] = _( buf );
    sprintf( buf, "%d", dialog.GetColourData().GetColour().Green() );
    AppConfig->Options["colour_store_g"] = _( buf );
    sprintf( buf, "%d", dialog.GetColourData().GetColour().Blue() );
    AppConfig->Options["colour_store_b"] = _( buf );
    AppConfig->Save();
    if( graph != NULL ) {
        graph->ChangeSamplesColour( btnEven->GetBackgroundColour(), btnOdd->GetBackgroundColour(), btnHeld->GetBackgroundColour(), iLineSize );
    }
}

void CurveTracerDialog::ProgressIndicator( void ) {
    if( lblProgress->GetLabel() == _( " /" ) ) {
        lblProgress->SetLabel( _( "\\" ) );
    } else {
        lblProgress->SetLabel( _( " /" ) );
    }
}

CurveTracerDialog::~CurveTracerDialog() {
    //(*Destroy(CurveTracerDialog)
    //*)
    RS232_CloseComport( comport_nr );
    delete graph;
}

void CurveTracerDialog::OnQuit( wxCommandEvent& event ) {
    Close();
}

#ifdef __GLIBC__
#include <gnu/libc-version.h>
#endif
void CurveTracerDialog::OnAbout( wxCommandEvent& event ) {
    wxString msg = wxbuildinfo( long_f );
    msg << _( "\r\nGNU GCC version: " ) << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
#ifdef __GLIBC__
    msg << _( "\r\nGNU libc compile-time version: " ) << __GLIBC__ << "." << __GLIBC_MINOR__;
    msg << _( "\r\nGNU libc runtime version: " ) << gnu_get_libc_version();
#endif
    msg << _( "\r\n\r\nProject:\t\t\thttps://github.com/HenniePeters/CurveTracer/\r\n\r\n" );
    msg << _( "Developers:\r\n--------------------\r\n" );
    msg << _( "Hennie Peters: \t\tC++ programming/hardware development\r\n" );
    msg << _( "Marcel Sweers:\t\tHardware development\r\n" );
    wxMessageBox( msg, _( "About CurveTracer" ) );
}

void CurveTracerDialog::OnchkFatClick( wxCommandEvent& event ) {
    iLineSize = 4;
    if( graph != NULL ) {
        delete graph;
        graph = NULL;
    }
    AppConfig->Options["linesize"] = _( "4" );
    AppConfig->Save();
}

void CurveTracerDialog::OnchkThickClick( wxCommandEvent& event ) {
    iLineSize = 2;
    if( graph != NULL ) {
        delete graph;
        graph = NULL;
    }
    AppConfig->Options["linesize"] = _( "2" );
    AppConfig->Save();
}

void CurveTracerDialog::OnchkThinClick( wxCommandEvent& event ) {
    iLineSize = 1;
    if( graph != NULL ) {
        delete graph;
        graph = NULL;
    }
    AppConfig->Options["linesize"] = _( "1" );
    AppConfig->Save();
}

void CurveTracerDialog::OnPaint( wxPaintEvent & evt ) {
    wxClientDC canvas( StaticText2 );
    int left_margin = 30, right_margin = 1, top_margin = 30, bottom_margin = 1;
    if( graph == NULL ) {
        graph = new class PaintGraphics( StaticText2 );
        graph->SetBox( 5, left_margin, right_margin, top_margin, bottom_margin );
        graph->ChangeSamplesColour( btnEven->GetBackgroundColour(), btnOdd->GetBackgroundColour(), btnHeld->GetBackgroundColour(), iLineSize );
        // set "Sine 2V"
        graph->SetSpacingHori( iFullScaleX );
        graph->SetFactorMulHori( iFullScaleX );
        graph->SetFactorDivHori( iFullScaleX );
        graph->SetHoriUnit( " V" );
        // set "1 kHz (1 ms)"
        graph->SetSpacingVert( iFullScaleY );
        graph->SetFactorMulVert( iFullScaleY );
        graph->SetFactorDivVert( iFullScaleY );
        graph->SetVertUnit( " V" );
        wxString wxsTemp = AppConfig->Options["grid"];
        graph->GridVisible =  ( wxsTemp == _( "true" ) ) ? true : false;
    }
    if( Timer1.IsRunning() ) {
        graph->Clear();
        graph->ShowGrid();
        graph->ShowRulers();
        if( bReady ) {
            bReady = false;
            graph->ShowSamples( &fields, &held, bShowHeldSample );
        }
    }
}

void CurveTracerDialog::OnTimer1Trigger( wxTimerEvent& event ) {
    class communication_package *cp_in;
    static int teller;
    static unsigned char buf[512];
    uint16_t n;
    if( comport_nr > 0 ) {
        if( ( n = RS232_PollComport( comport_nr, buf, 512 ) ) > 0 ) {
            for( uint16_t i = 0; i < n; i++ ) {
                fifo.Put( buf[i] );
            }
        }
        teller += Timer1.GetInterval();
        n = fifo.Size();
        while( n >= sizeof( struct f ) ) {
            cp_in = new class communication_package;
            cp_in->pkg_clear();
            if( ! cp_in->pkg_receive() ) { // wait until reception of full package
                for( unsigned int i = 0; i < cp_in->len; i++ ) { // copy data to union package
                    buffer[i] = cp_in->buf[i];
                }
                memcpy( ( void* )&fields, &buffer, sizeof( buffer ) );
                if( fields.coords && fields.coords == COORDS
                &&  fields.endianness == 19532   ) {
                    if( fields.coords ) {
                        if( iFullScaleX != fields.Vx / 1000 ) {
                            iFullScaleX = fields.Vx / 1000;
                            delete graph;
                            graph = NULL;
                        }
                        if( iFullScaleY != fields.Vy / 1000 ) {
                            iFullScaleY = fields.Vy / 1000;
                            delete graph;
                            graph = NULL;
                        }
                        if( teller >= iPrintingInterval ) {
                            teller = 0;
                            bReady = true;
                            Refresh();  // force repaint
                            ProgressIndicator();
                        }
                    }
                } else {
                    cp_in->pkg_clear();
                    //Outdated( fields.coords );
                }
            }
            n = fifo.Size();
            delete cp_in;
        }
    }
}

void CurveTracerDialog::OnbtnToggleHoldToggle( wxCommandEvent& event ) {
    if( btnToggleHold->GetValue() == true ) {
        bShowHeldSample = true;
        for( int i = 0; i < ( COORDS << 1 ); i++ ) {
            held.xy[i] =  fields.xy[i];
        }
        held.Vx =  fields.Vx;
        held.Vy =  fields.Vy;
    } else {
        bShowHeldSample = false;
    }
}

void CurveTracerDialog::OngrpFrequencySelect( wxCommandEvent& event ) {
    class communication_package pkg_out;
    uint16_t freq = 50;
    wxString wxsFrequency;
    wxsFrequency = event.GetString();
    if( wxsFrequency == _( "50 Hz (20 ms)" ) ) {
        graph->SetSpacingHori( 2 );
        graph->SetFactorMulHori( 10 );
        graph->SetFactorDivHori( 2 );
        freq = 50;
    } else if( wxsFrequency == _( "200 Hz (5 ms)" ) ) {
        graph->SetSpacingHori( 2 );
        graph->SetFactorMulHori( 10 );
        graph->SetFactorDivHori( 2 );
        freq = 200;
    } else if( wxsFrequency == _( "1 kHz (1 ms)" ) ) {
        graph->SetSpacingHori( 2 );
        graph->SetFactorMulHori( 1 );
        graph->SetFactorDivHori( 2 );
        freq = 1000;
    } else if( wxsFrequency == _( "2 kHz (0.5 ms)" ) ) {
        graph->SetSpacingHori( 2 );
        graph->SetFactorMulHori( 1 );
        graph->SetFactorDivHori( 2 );
        freq = 2000;
    }
    if( comport_nr > 0 ) {
        pkg_out.pkg_clear();
        pkg_out.pkg_add_uint16( freq );
        pkg_out.pkg_send();
    } else {
        //"Comport is NOT connected"
    }
}
unsigned char read_byte( void ) {
    unsigned char c;
    unsigned char buf[256];
    while( true ) {
        if( ! fifo.IsEmpty() ) {
            c = fifo.Get();
            break;
        } else { // fifo is empty start polling
            int n;
#ifdef __linux__
            usleep( 1000 );
#else
            Sleep( 1 );
#endif
            if( ( n = RS232_PollComport( comport_nr, buf, 256 ) ) > 0 ) {
                for( int i = 0; i < n; i++ ) {
                    fifo.Put( buf[i] );
                }
            }
        }
    }
    return c;
}
//######################################################################################################

uint8_t send_byte( uint8_t c ) {
    if( comport_nr > 0 ) {
        RS232_SendByte( comport_nr, c );
    }
    return 1;
}

