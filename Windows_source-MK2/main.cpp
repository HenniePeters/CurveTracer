//------------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//------------------------------------------------------------------------------

#include <vcl.h>
#include <vector>
#pragma hdrstop
#include "CommunicationProtocol.h"
#include "Config.h"
#include "Serial.h"
#include "Fifo.h"
#include "Graph.h"
#include "main.h"
#include "byteswap.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
AppConfigData *AppConfig = new AppConfigData( "CurveTracer" );
class f fields, held;
static TPen *pen_backup;
int iFullScaleX=1, iFullScaleY=1;
unsigned char buffer[sizeof( class f )];
class PaintGraphics *graph = NULL;
int comport_nr = 0;
class FifoClass fifo;
AnsiString wxsComPort = "";
const int serial_port_count = 32;
const char *serial_ports[] = { "", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "COM10", "COM11"
                             , "COM12", "COM13", "COM14", "COM15", "COM16", "COM17", "COM18", "COM19", "COM20", "COM21", "COM22"
                             , "COM23", "COM24", "COM25", "COM26", "COM27", "COM28", "COM29", "COM30", "COM31", "COM32"
                             };

//------------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)    : TForm(Owner) {
    bReady = false;
    bShowHeld = false;
    FormPaint(this);
    frmMain->DoubleBuffered = true;
    Panel3->DoubleBuffered = true;
    lblError->Caption = "CurveTracer V.1.2.0 beta";
    AnsiString wxsTemp;
    wxsTemp = AppConfig->Options["grid"].c_str();
    chkGridVisible->Checked = ( wxsTemp == "true" );
    wxsTemp = AppConfig->Options["ruler"].c_str();
    chkRulerVisible->Checked = ( wxsTemp == "true" );
    ulColour1 = atol( AppConfig->Options["colour1"].c_str() );
    ulColour2 = atol( AppConfig->Options["colour2"].c_str() );
    ulColour3 = atol( AppConfig->Options["colour3"].c_str() );
    shpColourEven->Brush->Color = (TColor)ulColour1;
    shpColourOdd->Brush->Color = (TColor)ulColour2;
    tbFreq->Position = atol( AppConfig->Options["interval"].c_str() );
    tbFreqChange(this);
    wxsTemp = AppConfig->Options["PenWidth"].c_str();
    if( wxsTemp == "4" ) {
        optLineFat->Checked = true;
    } else if( wxsTemp == "1" ) {
        optLineThin->Checked = true;
    } else {
        optLineThick->Checked = true;
    }
    try {
        AnsiString asHeight, asWidth, asTop, asLeft;
        asWidth = AppConfig->Options["width"].c_str();
        asHeight = AppConfig->Options["height"].c_str();
        asTop = AppConfig->Options["top"].c_str();
        asLeft = AppConfig->Options["left"].c_str();
        Width = atoi( asWidth.c_str() );
        Height = atoi( asHeight.c_str() );
        Top = atoi( asTop.c_str() );
        Left = atoi( asLeft.c_str() );
    } catch(...) {
        Width = 898;
        Height = 625;
        Top = 0;
        Left = 0;
    }
    cboPort->Text = AppConfig->Options["port"].c_str();
    if( cboPort->Text != "" && AppConfig->ReadOnly() ) {
        // disable open
        cboPort->Enabled = false;
    } else {
        cboPort->Enabled = ! AppConfig->ReadOnly();
    }
    for( unsigned int i = 1; i < serial_port_count; i++ ) {
        int tmp = RS232_GetPortnr( serial_ports[i] );
        if( tmp > 0 ) {
            char mode[] = {'8', 'N', '1', 0};
            int bdrate = 115200;           /* 9600 baud */
           // wxMessageBox( _(serial_ports[i]), _( "comport" ) );
            if( 0 == RS232_OpenComport( tmp, bdrate, mode, 0 ) ) {
                RS232_CloseComport( tmp );
                cboPort->Items->Add( serial_ports[i] );
            }
        }
    }
    if( comport_nr == 0 ) {
        char mode[] = {'8', 'N', '1', 0};
        int bdrate = 115200;           /* 9600 baud */
        char mbstring[513];
        wxsComPort = cboPort->Text;
        strcpy( mbstring, wxsComPort.c_str() );
        comport_nr = RS232_GetPortnr( mbstring );
        if( RS232_OpenComport( comport_nr, bdrate, mode, 0 ) ) {
            lblError->Caption = "Cannot open comport";
            comport_nr = 0;
            cmdConnect->Font->Color = clBlack;
            cmdDisconnect->Font->Color = clGray;
        } else {
            cmdDisconnect->Font->Color = clBlack;
            cmdConnect->Font->Color = clGray;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::chkGridVisibleClick(TObject *Sender) {
    bool bTemp = (AppConfig->Options["grid"] == "true");
    if( graph != NULL ) {
        AppConfig->Options["grid"] = chkGridVisible->Checked ? "true" : "false";
    }
    if( chkGridVisible->Checked != bTemp ) {
        AppConfig->Save();
        if( graph != NULL ) {
            graph->Resize(img); // resizing img->Picture->Bitmap
            delete graph;
            graph = NULL;
            FormPaint(Sender);
        }
        Refresh();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::chkRulerVisibleClick(TObject *Sender) {
    bool bTemp = (AppConfig->Options["ruler"] == "true");
    if( graph != NULL ) {
        AppConfig->Options["ruler"] = chkRulerVisible->Checked ? "true" : "false";
    }
    if( chkRulerVisible->Checked != bTemp ) {
        AppConfig->Options["ruler"] = chkRulerVisible->Checked ? "true" : "false";
        AppConfig->Save();
        if( graph != NULL ) {
            graph->Resize(img); // resizing img->Picture->Bitmap
            delete graph;
            graph = NULL;
            FormPaint(Sender);
        }
        Refresh();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::cboPortChange(TObject *Sender) {
    AppConfig->Options["port"] = cboPort->Text.c_str();
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::cmdConnectClick(TObject *Sender) {
    if( comport_nr == 0 ) {
        if( cmdConnect->Font->Color == clBlack ) {
            char mode[] = {'8', 'N', '1', 0};
            int bdrate = 115200;           // 9600 baud
            char mbstring[513];
            strcpy( mbstring, cboPort->Text.c_str() );
            comport_nr = RS232_GetPortnr( mbstring );
            if( RS232_OpenComport( comport_nr, bdrate, mode, 0 ) ) {
                lblError->Caption = "Cannot open comport";
                comport_nr = 0;
                cmdConnect->Font->Color = clBlack;
                cmdDisconnect->Font->Color = clGray;
            } else {
                cmdDisconnect->Font->Color = clBlack;
                cmdConnect->Font->Color = clGray;
                if( cboPort->Text != AppConfig->Options["port"].c_str() ) {
                    AppConfig->Options["port"] = cboPort->Text.c_str();
                    AppConfig->Save();
                }
            }
        }
    }    
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::cmdDisconnectClick(TObject *Sender) {
    if( comport_nr > 0 ) {
        RS232_CloseComport( comport_nr );
        comport_nr = 0;
    }
    cmdConnect->Font->Color = clBlack;
    cmdDisconnect->Font->Color = clGray;
    if( cboPort->Enabled ) {
        AnsiString asTemp = cboPort->Text;
        cboPort->Clear();
        cboPort->Text = asTemp;
        for( unsigned int i = 0; i < serial_port_count; i++ ) {
            int tmp = RS232_GetPortnr( serial_ports[i] );
            if( tmp > 0 ) {
                char mode[] = {'8', 'N', '1', 0};
                int bdrate = 115200;           /* 9600 baud */
                if( 0 == RS232_OpenComport( tmp, bdrate, mode, 0 ) ) {
                    RS232_CloseComport( tmp );
                    cboPort->Items->Add( serial_ports[i] );
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::Timer1Timer(TObject *Sender) {
    class communication_package *cp_in;
    static unsigned char buf[1024];
    static int teller, iErrorTimer = 0;
    static AnsiString asLastError = "";
    if( lblError->Caption != asLastError ) {
        asLastError = lblError->Caption;
        iErrorTimer = 5000 / Timer1->Interval; // 5 seconds
    } else {
        if( iErrorTimer > 0 ) {
            iErrorTimer--;
        }
    }
    if( iErrorTimer == 0 ) {
        lblError->Caption = "";
    }
    teller += Timer1->Interval;
    if( chkDebug->Checked ) {
        if( teller < iPrintingInterval ) {
            return;
        } else {
            teller -= iPrintingInterval;
        }
        //fields.Vx = 5000;
        //fields.Vy = 5000;
        for( int i=0; i<(COORDS<<1); i+=2 ) {
            fields.xy[i] = i * 85;
            fields.xy[i+1] = i * 85;
        }
        bReady = true;
        Refresh();  // force repaint
    } else if( comport_nr > 0 ) {
        int n;
        if( ( n = RS232_PollComport( comport_nr, buf, 1024 ) ) > 0 ) {
            for( uint16_t i = 0; i < n; i++ ) {
                fifo.Put( buf[i] );
            }
        }
        n = fifo.Size();
        //lblFifoSize->Caption = n;
        while( n >= sizeof( class f ) ) {
            cp_in = new class communication_package;
            cp_in->pkg_clear();
            if( ! cp_in->pkg_receive() ) { // wait until reception of full package
                for( int i = 0; i < cp_in->len; i++ ) { // copy data to union package
                    buffer[i] = cp_in->buf[i];
                }
                memcpy( ( void* )&fields, &buffer, sizeof( buffer ) );
                int n = 1;

                if( ( *(char*)&n == 1 && *(char*)&(fields.endianness) != 'L' )
                ||  ( *(char*)&n != 1 && *(char*)&(fields.endianness) != 'B' ) ) { // if( ( *(char*)&n == 1  ) little endian if true
                    fields.Vx = BYTESWAP16( fields.Vx );
                    fields.Vy = BYTESWAP16( fields.Vy );
                    fields.coords = BYTESWAP16( fields.coords );
                    fields.freq = BYTESWAP16( fields.freq );
                    for( int i=0; i<(COORDS<<1); i+=2 ) {
                        fields.xy[i] = BYTESWAP16( fields.xy[i] );
                        fields.xy[i+1] = BYTESWAP16( fields.xy[i+1] );
                    }
                }
                if( fields.coords && fields.coords > COORDS ) {
                    Outdated( fields.coords );
                }
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
                        /*
                        Memo1->Clear();
                        Memo2->Clear();
                        for( int i=0; i<(COORDS<<1); i+=2 ) {
                            Memo1->Lines->Add( fields.x[i] );
                            if( graph != NULL ) {
                                // Memo2->Lines->Add( graph->map( fields.x[i], -10000, 10000, 0, 4096 ) );
                                Memo2->Lines->Add( fields.x[i] - fields.Vx );
                            }
                        }
                        */
                        teller = 0;
                        ProgressIndicator();
                        bReady = true;
                        Refresh();  // force repaint
                    }
                }
            }

            n = fifo.Size();
            //lblFifoSize->Caption = n;
            delete cp_in;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::Outdated( int coords ) {
    AnsiString msg;
    msg = "Number of coordinates received (";
    msg += fields.coords;
    msg += ") differs from the number expected by the program (";
    msg +=  COORDS;
    msg +=  ")\n\n";
    msg += "Either this program or the embedded code in your Arduino board is outdated!";
    Timer1->Enabled = false;
    Application->MessageBox( msg.c_str(), "About CurveTracer", 0 );
    //Close();        // Close does not terminate completely when the aboutbox is showing
    std::terminate(); // terminate closes all windows
}
//------------------------------------------------------------------------------
void TfrmMain::ProgressIndicator( void ) {
    if( lblProgress->Caption == " /" ) {
        lblProgress->Caption = "\\";
    } else {
        lblProgress->Caption = " /";
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::FormPaint(TObject *Sender) {
    if( graph == NULL ) {
        graph = new class PaintGraphics( img );
        ulColour1 = atol( AppConfig->Options["colour1"].c_str() );
        ulColour2 = atol( AppConfig->Options["colour2"].c_str() );
        ulColour3 = atol( AppConfig->Options["colour3"].c_str() );
        graph->ChangeSamplesColour( ulColour1, ulColour2, ulColour3 );
        graph->GridVisible = chkGridVisible->Checked;
        graph->RulerVisible = chkRulerVisible->Checked;
        PenWidthChange();
        int left_margin=30, right_margin=4, top_margin=30, bottom_margin=1;
        graph->SetBox( 5, left_margin, right_margin, top_margin, bottom_margin );
        // set "Sine 2V"
        graph->SetVertUnit("V");
        graph->SetSpacingHori(iFullScaleX);
        graph->SetFactorMulHori(iFullScaleX);
        graph->SetFactorDivHori(iFullScaleX);
        // set "1 kHz (1 ms)"
        graph->SetHoriUnit("V");
        graph->SetSpacingVert(iFullScaleY);
        graph->SetFactorMulVert(iFullScaleY);
        graph->SetFactorDivVert(iFullScaleY);
    }
    graph->Clear();
    graph->ShowGrid();
    graph->ShowRulers();
    if( fields.coords ) {
        if( bReady ) {
            bReady = false;
            graph->SampleColour();
            graph->ShowSamples( &fields, &held, bShowHeld );
        } else {
            graph->ShowSamples( &fields, &held, bShowHeld );
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender) {
    if( graph != NULL ) {
        pen_backup = graph->Pen_Samples;
        graph->Resize(img); // resizing img->Picture->Bitmap
        delete graph;
        graph = NULL;
        FormPaint(Sender);
        graph->Pen_Samples = pen_backup;
    }
    if( this->ClientWidth != Screen->Width ) { // do not save screen size on maximise
        char buf[8], tmp[8];
        bool bSave = false;
        sprintf( buf, "%d", Width );
        strcpy( tmp, AppConfig->Options["width"].c_str() );
        if( strcmp( buf, tmp ) != 0 ) {
            bSave = true;
            AppConfig->Options["width"] = buf;
        }
        sprintf( buf, "%d", Height );
        strcpy( tmp, AppConfig->Options["height"].c_str() );
        if( strcmp( buf, tmp ) != 0 ) {
            bSave = true;
            AppConfig->Options["height"] = buf;
        }
        if( bSave ) {
            AppConfig->Save();
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::cmdColourEvenClick(TObject *Sender) {
    ColorDialog1->Color = shpColourEven->Brush->Color;
    if( ColorDialog1->Execute() ) {
        shpColourEven->Brush->Color = ColorDialog1->Color;
        //shpColourEven->Brush->Color = 0x0000CC00;
        char buf[16];
        sprintf( buf, "%ld", ColorDialog1->Color );
        AppConfig->Options["colour1"] = buf; //even
        if( graph != NULL ) {
            graph->ChangeSamplesColour( shpColourEven->Brush->Color, shpColourOdd->Brush->Color, shpColourHeld->Brush->Color );
            AppConfig->Save();
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::cmdColourOddClick(TObject *Sender) {
    ColorDialog1->Color = shpColourOdd->Brush->Color;
    if( ColorDialog1->Execute() ) {
        shpColourOdd->Brush->Color = ColorDialog1->Color;
        //shpColourEven->Brush->Color = 39680;
        char buf[16];
        sprintf( buf, "%ld", ColorDialog1->Color );
        AppConfig->Options["colour2"] = buf; //odd
        if( graph != NULL ) {
            graph->ChangeSamplesColour( shpColourEven->Brush->Color, shpColourOdd->Brush->Color, shpColourHeld->Brush->Color );
            AppConfig->Save();
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::cmdColourHeldClick(TObject *Sender) {
    ColorDialog1->Color = shpColourHeld->Brush->Color;
    if( ColorDialog1->Execute() ) {
        shpColourHeld->Brush->Color = ColorDialog1->Color;
        char buf[16];
        sprintf( buf, "%ld", ColorDialog1->Color );
        AppConfig->Options["colour3"] = buf; //held
        if( graph != NULL ) {
            graph->ChangeSamplesColour( shpColourEven->Brush->Color, shpColourOdd->Brush->Color, shpColourHeld->Brush->Color );
            AppConfig->Save();
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::tbFreqChange(TObject *Sender) {
    AnsiString as = "Update interval: ";
    //iPrintingInterval = (tbFreq->Position + 1) * ( 1000 / (tbFreq->Max - tbFreq->Min) );
    iPrintingInterval = tbFreq->Position * ( 1000 / tbFreq->Max );
    as += iPrintingInterval;
    as += "ms";
    lblUpdateFreq->Caption = as;
    char buf[16];
    sprintf( buf, "%ld", tbFreq->Position );
    AppConfig->Options["interval"] = buf;
    AppConfig->Save();
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::optLineThickClick(TObject *Sender) {
    PenWidthChange();
    char buf[16];
    sprintf( buf, "%ld", graph->PenWidth );
    AppConfig->Options["PenWidth"] = buf;
    AppConfig->Save();
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::optLineThinClick(TObject *Sender) {
    PenWidthChange();
    char buf[16];
    sprintf( buf, "%ld", graph->PenWidth );
    AppConfig->Options["PenWidth"] = buf;
    AppConfig->Save();
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::optLineFatClick(TObject *Sender) {
    PenWidthChange(); 
    char buf[16];
    sprintf( buf, "%ld", graph->PenWidth );
    AppConfig->Options["PenWidth"] = buf;
    AppConfig->Save();
}
//------------------------------------------------------------------------------
void TfrmMain::PenWidthChange(void) {
    if( optLineFat->Checked ) {
        graph->PenWidth = 4;
    } else if( optLineThick->Checked ) {
        graph->PenWidth = 2;
    } else if( optLineThin->Checked ) {
        graph->PenWidth = 1;
    } else {
        graph->PenWidth = 2;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action) {
    char buf[16];
    sprintf( buf, "%ld", Top );
    AppConfig->Options["top"] = buf;
    sprintf( buf, "%ld", Left );
    AppConfig->Options["left"] = buf;
    AppConfig->Save();
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::cmdHoldClick(TObject *Sender) {
    if( cmdHold->Down ) {
        for( int i = 0; i < (COORDS<<1); i++ ) {
            held.xy[i] =  fields.xy[i];
        }
        held.Vx =  fields.Vx;
        held.Vy =  fields.Vy;
        bShowHeld = true;
    } else {
        bShowHeld = false;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfrmMain::grpFreqClick(TObject *Sender) {
    class communication_package pkg_out;
    uint16_t freq=50;
    AnsiString asFrequency;
    asFrequency = ((TRadioButton *)Sender)->Caption;
    if( asFrequency == "50 Hz (20 ms)" ) {
        graph->SetSpacingHori( 2 );
        graph->SetFactorMulHori( 10 );
        graph->SetFactorDivHori( 2 );
        freq = 50;
    } else if( asFrequency == "200 Hz (5 ms)" ) {
        graph->SetSpacingHori( 2 );
        graph->SetFactorMulHori( 10 );
        graph->SetFactorDivHori( 2 );
        freq = 200;
    } else if( asFrequency == "1 kHz (1 ms)" ) {
        graph->SetSpacingHori( 2 );
        graph->SetFactorMulHori( 1 );
        graph->SetFactorDivHori( 2 );
        freq = 1000;
    } else if( asFrequency == "2 kHz (0.5 ms)" ) {
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
//---------------------------------------------------------------------------
void send_byte( unsigned char c ) {
    if( comport_nr > 0 ) {
        RS232_SendByte( comport_nr, c );
    }
}
//---------------------------------------------------------------------------
unsigned char read_byte( void ) {
    unsigned char c;
    unsigned char buf[64];
    while( true ) {
        if( ! fifo.IsEmpty() ) {
            c = fifo.Get();
            break;
        } else { // fifo is empty start polling
            int n;
            #ifdef __linux__
            usleep(1000);
            #else
            Sleep(1);
            #endif
            if( (n = RS232_PollComport( comport_nr, buf, 64 )) > 0 ) {
                for( int i=0; i<n; i++ ) {
                    fifo.Put( buf[i] );
                }
            }
        }
    }
    return c;
}
//------------------------------------------------------------------------------

