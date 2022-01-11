//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------

class TfrmMain : public TForm {
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TImage *img;
    TLabel *lblProgress;
    TLabel *Label2;
    TComboBox *cboPort;
    TLabel *Label3;
    TSpeedButton *cmdConnect;
    TSpeedButton *cmdDisconnect;
    TCheckBox *chkGridVisible;
    TTimer *Timer1;
    TLabel *Label1;
    TLabel *Label4;
    TCheckBox *chkRulerVisible;
    TStatusBar *StatusBar1;
    TLabel *lblError;
    TShape *shpColourEven;
    TLabel *Label6;
    TLabel *Label8;
    TLabel *Label9;
    TShape *shpColourOdd;
    TButton *cmdColourEven;
    TButton *cmdColourOdd;
    TColorDialog *ColorDialog1;
    TLabel *lblUpdateFreq;
    TTrackBar *tbFreq;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label10;
    TRadioButton *optLineThick;
    TRadioButton *optLineThin;
    TRadioButton *optLineFat;
    TCheckBox *chkDebug;
    TMemo *Memo1;
    TMemo *Memo2;
    TShape *Shape1;
    TShape *Shape2;
    TShape *Shape3;
    TShape *Shape4;
    void __fastcall chkGridVisibleClick(TObject *Sender);
    void __fastcall cboPortChange(TObject *Sender);
    void __fastcall cmdConnectClick(TObject *Sender);
    void __fastcall cmdDisconnectClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall chkRulerVisibleClick(TObject *Sender);
    void __fastcall cmdColourEvenClick(TObject *Sender);
    void __fastcall cmdColourOddClick(TObject *Sender);
    void __fastcall tbFreqChange(TObject *Sender);
    void __fastcall optLineThickClick(TObject *Sender);
    void __fastcall optLineThinClick(TObject *Sender);
    void __fastcall optLineFatClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    bool bReady;
    int iPrintingInterval;
    unsigned long ulColour1;
    unsigned long ulColour2;
    void ProgressIndicator( void );
    void __fastcall Outdated( int coords );
    void PenWidthChange(void);
public:		// User declarations
    __fastcall TfrmMain(TComponent* Owner);
    __property unsigned long Colour1  = { read=ulColour1, write=ulColour1 };
    __property unsigned long Colour2  = { read=ulColour2, write=ulColour2 };
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
