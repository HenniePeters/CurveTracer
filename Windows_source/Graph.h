//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

#ifndef GraphH
#define GraphH

#define COORDS 48

#include <vcl.h>
#include <stdint.h>

class f {
public:
    uint16_t Vx, Vy;                // max value in mV
    uint16_t coords;                // number of coordinates stored in x,y
    int16_t x[COORDS], y[COORDS];   // sample data
};

class PaintGraphics {
public:
    TPen *Pen_Samples;
    TBrush *Brush_Samples;
    PaintGraphics( TImage *stArea_param );
    void SetBox( int iBorderWidth, int iLeftMargin, int iRightMargin, int iTopMargin, int iBottomMargin  );
    void Clear(void);
    void Resize( TImage *img );
    void ShowRulers(void);
    void ShowGrid(void);
    void DrawLine( TCanvas *canvas, int a, int b, int c, int d );
    void SampleColour( void );
    void ShowSamples( class f *fields );
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
    int map( int x, int in_min, int in_max, int out_min, int out_max ) {
        return ( x - in_min ) * ( out_max - out_min ) / ( in_max - in_min ) + out_min;
    }
    void SetHoriUnit( const char *param ) {
        wxsHoriUnit = param;
    }
    void SetVertUnit( const char *param ) {
        wxsVertUnit = param;
    }
    void ChangeSamplesColour(unsigned long clEven, unsigned long clOdd); 
    __property int PenWidth  = { read=iPenWidth, write=SetPenWidth };
private:
    TImage *stArea;
    TBrush *wxBrush_Border;
    TBrush *wxBrush_Margin;
    TBrush *wxBrush_Ruler;
    TPen *wxPen_SamplesGreen;
    TPen *wxPen_SamplesBlue;
    TPen *wxPen_Border;
    TPen *wxPen_Margin;
    TPen *wxPen_Ruler;
    TPen *wxPen_Grid20, *wxPen_Grid40, *wxPen_Grid80; 
    int iPenWidth;
    int iRulerWidth;
    int iLeftMargin, iRightMargin;
    int iTopMargin, iBottomMargin;
    int iCenterHeight, iCenterWidth;
    int iDrawWidth, iDrawHeight;
    int iLeftEdge,  iRightEdge, iTopEdge, iBottomEdge;
    uint16_t iSpacingHori;
    uint16_t iSpacingVert;
    uint16_t iFactorMulHori;
    uint16_t iFactorMulVert;
    uint16_t iFactorDivHori;
    uint16_t iFactorDivVert;
    AnsiString wxsHoriUnit, wxsVertUnit;
    void __fastcall SetPenWidth(int value);
};

#endif //Graph_H
