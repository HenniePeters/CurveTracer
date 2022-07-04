//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

#ifndef Graph_H
#define Graph_H

#define COORDS 50

#include <wx/wx.h>
#include <stdint.h>

struct f {
    uint16_t endianness;
    uint16_t Vx, Vy;            // max scale value in mV (positive or negative)
    uint16_t coords;            // number of coordinates stored in x,y
    uint16_t freq;              // frequency in Hz
    int16_t xy[(COORDS<<1)];    // sample data
};

class PaintGraphics {
public:
    PaintGraphics( wxStaticText *stArea_param );
    void SetBox( int iBorderWidth, int iLeftMargin, int iRightMargin, int iTopMargin, int iBottomMargin  );
    void ChangeSamplesColour(wxColour ColourEven, wxColour ColourOdd, wxColour ColourStorage, int iPenWidth);
    void Clear(void);
    void ShowRulers(void);
    void ShowGrid(void);
    void DrawLine( wxClientDC *canvas, int a, int b, int c, int d );
    void ShowSamples( struct f *fields, struct f *held, bool bShowHeld );
    bool GridVisible, RulerVisible;
    void SetSpacingHori( uint16_t val ) {
        iSpacingHori = ( ( ((iRightEdge-iLeftEdge)/2) / val) / 10 );
    }
    void SetSpacingVert( uint16_t val ) {
        iSpacingVert = ( ( ((iBottomEdge-iTopEdge)/2) / val) / 10 );
    }
    void SetFactorMulHori( uint16_t val ) {
        iFactorMulHori = val;
    }
    void SetFactorDivHori( uint16_t val ) {
        iFactorDivHori = val;
    }
    void SetFactorMulVert( uint16_t val ) {
        iFactorMulVert = val;
    }
    void SetFactorDivVert( uint16_t val ) {
        iFactorDivVert = val;
    }
    int16_t map( int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max ) {
        if( in_max - in_min != 0 ) {
            return ( x - in_min ) * ( out_max - out_min ) / ( in_max - in_min ) + out_min;
        } else {
            return 0;
        }
    }
    void SetHoriUnit( const char *param ) {
        wxsHoriUnit = _( param );
    }
    void SetVertUnit( const char *param ) {
        wxsVertUnit = _( param );
    }
private:
    wxStaticText *stArea;
    wxBrush wxBrush_Border;
    wxBrush wxBrush_Margin;
    wxBrush wxBrush_Samples;
    wxPen wxPen_Samples, wxPen_Samples1, wxPen_Samples2;
    wxPen wxPen_Border;
    wxPen wxPen_Margin;
    wxPen wxPen_Ruler;
    wxPen wxPen_Grid;
    wxPen wxPen_Storage;
    int iRulerWidth;
    int iLeftMargin, iRightMargin;
    int iTopMargin, iBottomMargin;
    int iCenterHeight, iCenterWidth;
    int iDrawWidth, iDrawHeight;
    int iLeftEdge,  iRightEdge, iTopEdge, iBottomEdge;
    uint16_t iSpacingHori = 16, iSpacingVert = 6;
    uint16_t iFactorMulHori = 1, iFactorMulVert = 1;
    uint16_t iFactorDivHori = 1, iFactorDivVert = 1;
    wxString wxsHoriUnit = _(""), wxsVertUnit = _("");
};

#endif //Graph_H
