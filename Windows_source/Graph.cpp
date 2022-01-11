//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

#include "Graph.h"
#include <stdio.h>
PaintGraphics::PaintGraphics( TImage *stArea_param ) {
    stArea = stArea_param;
    TCanvas *canvas = stArea->Canvas;

    iSpacingHori = 16;
    iSpacingVert = 6;
    iFactorMulHori = 1;
    iFactorMulVert = 1;
    iFactorDivHori = 1;
    iFactorDivVert = 1;
    iPenWidth = 2;
    wxsHoriUnit = "";
    wxsVertUnit = "";
    Resize( stArea );
    GridVisible = false;
    RulerVisible = false;
    wxBrush_Border = new TBrush;
    wxBrush_Margin = new TBrush;
    Brush_Samples = new TBrush;
    wxBrush_Ruler = new TBrush;
    wxPen_SamplesGreen = new TPen;
    wxPen_SamplesBlue = new TPen;
    Pen_Samples = new TPen;
    wxPen_Border = new TPen;
    wxPen_Margin = new TPen;
    wxPen_Ruler = new TPen;
    wxPen_Grid20 = new TPen;
    wxPen_Grid40 = new TPen;
    wxPen_Grid80 = new TPen;

    wxPen_Ruler->Width = 1;
    wxPen_Grid20->Width = 1;
    wxPen_Grid40->Width = 1;
    wxPen_Grid80->Width = 1;
    wxPen_Border->Width = 0;
    wxPen_Margin->Width = 0;
    wxPen_SamplesGreen->Width = iPenWidth;
    wxPen_SamplesBlue->Width = iPenWidth;

    wxPen_Ruler->Color = RGB(128,128,128);
    wxBrush_Ruler->Color = RGB(0,0,0);
    wxPen_Grid20->Color = RGB(32,32,32);
    wxPen_Grid40->Color = RGB(64,64,64);
    wxPen_Grid80->Color = RGB(128,128,128);
    wxPen_Border->Color = RGB(255,255,255);
    wxBrush_Border->Color = RGB(0,0,0);
    wxPen_Margin->Color = RGB(0,0,0);
    wxBrush_Margin->Color = RGB(0,0,0);
    Brush_Samples->Color = RGB(0,0,0);
    wxPen_SamplesGreen->Color = RGB(0,0xCC,0); //(TColor)0x0000CC00;
    wxPen_SamplesBlue->Color = RGB(255,64,64); //(TColor)0x00FF4040;

    // text settings
    canvas->Font->Color = RGB(128,128,128);
    canvas->Font->Size = 10;
    canvas->Font->Style = TFontStyles();
}
//---------------------------------------------------------------------------
void PaintGraphics::Resize( TImage *img ) {
    int w = img->Width;
    int h = img->Height;
    if( w < h ) {
        h = w;
    } else {
        w = h;
    }
    img->Picture->Bitmap->Width  = w;
    img->Picture->Bitmap->Height = h;
    iDrawWidth  = w;
    iDrawHeight = h;
}
//---------------------------------------------------------------------------
void PaintGraphics::SetBox( int iRulerWidth_param, int iLeftMargin_param, int iRightMargin_param, int iTopMargin_param, int iBottomMargin_param ) {
    if( RulerVisible ) {
        iRulerWidth = iRulerWidth_param;
        iLeftMargin = iLeftMargin_param + iRulerWidth_param;
        iTopMargin = iTopMargin_param + iRulerWidth_param;
    } else {
        iRulerWidth = 0;
        iLeftMargin = 5;
        iTopMargin = 5;
    }
    iRightMargin = iRightMargin_param + iRulerWidth_param;
    iBottomMargin = iBottomMargin_param + iRulerWidth_param;
    iCenterWidth = 4 + ( iDrawWidth - iRightMargin - iLeftMargin ) / 2 + iLeftMargin;
    iCenterHeight = 2 + ( iDrawHeight - iTopMargin - iBottomMargin ) / 2 + iTopMargin;
    iLeftEdge = iRulerWidth + iLeftMargin + 1;
    iRightEdge = 2 + iDrawWidth - iRightMargin ;
    iTopEdge = iRulerWidth + iTopMargin + 1;
    iBottomEdge = iDrawHeight - iBottomMargin -3;
}
//---------------------------------------------------------------------------
void PaintGraphics::ShowRulers( void ) {
    TCanvas *canvas = stArea->Canvas;
    if( RulerVisible ) {
        // ruler settings
        canvas->Pen = wxPen_Ruler;
        canvas->Brush = wxBrush_Ruler;

        signed int VV = 0, TT = 0;
        char bufRuler[32];
        TT = 0;
        for( int i = iCenterWidth; i <= iRightEdge; i++ ) {
           if( ( (i-iCenterWidth) % (iSpacingHori*10) ) == 0 ) {
                DrawLine( canvas, i, iTopEdge, i, iTopEdge - 10 );
                if( (i-iCenterWidth) > 0 ) {
                    ++TT;
                    sprintf( bufRuler, "%d", TT*iFactorMulHori/iFactorDivHori );
                    if( 10*TT*iFactorMulHori/iFactorDivHori < 10 && iFactorDivHori > 1 ) {
                        sprintf( bufRuler, "0.%d", 10*TT*iFactorMulHori/iFactorDivHori );
                        if( strlen(bufRuler) > 2 ) {
                            canvas->TextOutA( i - 14, iTopEdge - 30, bufRuler );
                        } else if( strlen(bufRuler) > 1 ) {
                            canvas->TextOutA( i - 12, iTopEdge - 30, bufRuler );
                        } else {
                            canvas->TextOutA( i - 6, iTopEdge - 30, bufRuler );
                        }
                    } else {
                        if( i > iRightEdge - 5 ) {
                            if( strlen(bufRuler) > 1 ) {
                                canvas->TextOutA( i - 16, iTopEdge - 30, bufRuler );
                            } else {
                                canvas->TextOutA( i - 10, iTopEdge - 30, bufRuler );
                            }
                        } else {
                            canvas->TextOutA( i - 3, iTopEdge - 30, bufRuler );
                        }
                    }
                }
            } else if( ( (iCenterWidth-i) % (iSpacingHori*5) ) == 0 ) {
                DrawLine( canvas, i, iTopEdge, i, iTopEdge - 7 );
            } else if( ( (iCenterWidth-i) % iSpacingHori ) == 0 ) {
                DrawLine( canvas, i, iTopEdge, i, iTopEdge - 3 );
            }
        }
        TT = 0;
        for( int i = iCenterWidth; i >= iLeftEdge; i-- ) {
            if( ( (i-iCenterWidth) % (iSpacingHori*10) ) == 0 ) {
                DrawLine( canvas, i, iTopEdge, i, iTopEdge - 10 );
                if( (i-iCenterWidth) < 0 ) {
                    TT--;
                    if( 10*TT*iFactorMulHori/iFactorDivHori > -10 && iFactorDivHori > 1 ) {
                        sprintf( bufRuler, "-0.%d", abs(10*TT*iFactorMulHori/iFactorDivHori) );
                        if( strlen(bufRuler) > 2 ) {
                            canvas->TextOutA( i - 12, iTopEdge - 30, bufRuler );
                        } else {
                            canvas->TextOutA( i - 6, iTopEdge - 30, bufRuler );
                        }
                    } else {
                        sprintf( bufRuler, "%d", TT*iFactorMulHori/iFactorDivHori );
                        if( strlen(bufRuler) > 2 ) {
                            canvas->TextOutA( i - 12, iTopEdge - 30, bufRuler );
                        } else {
                            canvas->TextOutA( i - 6, iTopEdge - 30, bufRuler );
                        }
                    }

                }
            } else if( ( (iCenterWidth-i) % (iSpacingHori*5) ) == 0 ) {
                DrawLine( canvas, i, iTopEdge, i, iTopEdge - 7 );
            } else if( ( (iCenterWidth-i) % iSpacingHori ) == 0 ) {
                DrawLine( canvas, i, iTopEdge, i, iTopEdge - 3 );
            }
        }

        VV = 0;
        for( int i = iCenterHeight; i <= iBottomEdge; i++ ) {
            if( ( (iCenterHeight-i) % (iSpacingVert*10) ) == 0 ) {
                DrawLine( canvas, iLeftEdge, i , iLeftEdge - 10, i );
                if( (i-iCenterHeight) > 0 ) {
                    --VV;
                    if( 10*VV*iFactorMulVert/iFactorDivVert > -10 && iFactorDivVert > 1 ) {
                        sprintf( bufRuler, "-0.%d", abs(10*VV*iFactorMulVert/iFactorDivVert) );
                        if( i > iBottomEdge - 10 ) {
                            canvas->TextOutA( iLeftEdge - 33, i - 15, bufRuler );
                        } else {
                            canvas->TextOutA( iLeftEdge - 33, i - 7, bufRuler );
                        }
                    } else {
                        sprintf( bufRuler, "%d", VV*iFactorMulVert/iFactorDivVert );
                        if( i > iBottomEdge - 10 ) {
                            canvas->TextOutA( iLeftEdge - 33, i - 15, bufRuler );
                        } else {
                            canvas->TextOutA( iLeftEdge - 33, i - 7, bufRuler );
                        }
                    }
                }
            } else if( ( (iCenterHeight-i) % (iSpacingVert*5) ) == 0 ) {
                DrawLine( canvas, iLeftEdge, i, iLeftEdge - 7, i );
            } else if( ( (iCenterHeight-i) % iSpacingVert ) == 0 ) {
                DrawLine( canvas, iLeftEdge, i, iLeftEdge - 3, i );
            }
        }
        VV = 0;
        for( int i = iCenterHeight; i >= iTopEdge; i-- ) {
            if( ( (iCenterHeight-i) % (iSpacingVert*10) ) == 0 ) {
                DrawLine( canvas, iLeftEdge, i, iLeftEdge - 10, i );
                if( (i-iCenterHeight) < 0 ) {
                    ++VV;
                    if( 10*VV*iFactorMulVert/iFactorDivVert < 10 && iFactorDivVert > 1 ) {
                        sprintf( bufRuler, "0.%d", 10*VV*iFactorMulVert/iFactorDivVert );
                        canvas->TextOutA( iLeftEdge - 33, i - 7, bufRuler );
                    } else {
                        sprintf( bufRuler, "%d", VV*iFactorMulVert/iFactorDivVert );
                        canvas->TextOutA( iLeftEdge - 33, i - 7, bufRuler );
                    }
                }
            } else if( ( (iCenterHeight-i) % (iSpacingVert*5) ) == 0 ) {
                DrawLine( canvas, iLeftEdge, i , iLeftEdge - 7, i );
            } else if( ( (iCenterHeight-i) % iSpacingVert ) == 0 ) {
                DrawLine( canvas, iLeftEdge, i , iLeftEdge - 3, i );
            }
        }
        sprintf( bufRuler, "%s", wxsHoriUnit );
        canvas->TextOutA( iCenterWidth - 3, iTopEdge - 30, bufRuler );
        sprintf( bufRuler, "%s", wxsVertUnit );
        canvas->TextOutA( iLeftEdge - 33, iCenterHeight - 7, bufRuler );
    }
}
//---------------------------------------------------------------------------
void PaintGraphics::ShowGrid( void ) {
    TCanvas *canvas = stArea->Canvas;
    if( GridVisible ) {
        canvas->Pen = wxPen_Grid20;
        for( int i=iCenterWidth+iSpacingHori; i<iRightEdge; i+=iSpacingHori ) {
            DrawLine( canvas, i, iTopEdge+1, i, iBottomEdge );
        }
        for( int i=iCenterWidth-iSpacingHori; i>iLeftEdge; i-=iSpacingHori ) {
            DrawLine( canvas, i, iTopEdge+1, i, iBottomEdge );
        }

        for( int i=iCenterHeight+iSpacingVert; i<iBottomEdge; i+=iSpacingVert ) {
            DrawLine( canvas, iLeftEdge+1, i, iRightEdge, i );
        }
        for( int i=iCenterHeight-iSpacingVert; i>iTopEdge; i-=iSpacingVert ) {
            DrawLine( canvas, iLeftEdge+1, i, iRightEdge, i );
        }
        canvas->Pen = wxPen_Grid40;
        for( int i=iCenterWidth+iSpacingHori*5; i<iRightEdge; i+=iSpacingHori*5 ) {
            DrawLine( canvas, i, iTopEdge+1, i, iBottomEdge );
        }
        for( int i=iCenterWidth-iSpacingHori*5; i>iLeftEdge; i-=iSpacingHori*5 ) {
            DrawLine( canvas, i, iTopEdge+1, i, iBottomEdge );
        }
        for( int i=iCenterHeight+iSpacingVert*5; i<iBottomEdge; i+=iSpacingVert*5 ) {
            DrawLine( canvas, iLeftEdge+1, i, iRightEdge, i );
        }
        for( int i=iCenterHeight-iSpacingVert*5; i>iTopEdge; i-=iSpacingVert*5 ) {
            DrawLine( canvas, iLeftEdge+1, i, iRightEdge, i );
        }
        canvas->Pen = wxPen_Grid80;
        DrawLine( canvas, iCenterWidth, iTopEdge, iCenterWidth, iBottomEdge );    // center vert line
        DrawLine( canvas, iLeftEdge, iCenterHeight, iRightEdge, iCenterHeight );  // center hori line
    }
}
//---------------------------------------------------------------------------
void PaintGraphics::Clear( void ) {
    TCanvas *canvas = stArea->Canvas;
    TPen *pen_backup = canvas->Pen;
    TBrush *brush = canvas->Brush;

    // redraw border
    canvas->Pen = wxPen_Border;
    canvas->Brush = wxBrush_Border;
    canvas->Rectangle( Rect( 0, 0, iDrawWidth , iDrawHeight  ) );

    // redraw margin
    canvas->Pen = wxPen_Margin;
    canvas->Brush = wxBrush_Margin;
    canvas->Rectangle( Rect( iRulerWidth, iRulerWidth, iDrawWidth - iRulerWidth, iDrawHeight - iRulerWidth - iRulerWidth ) );

    // ruler settings
    canvas->Pen = wxPen_Ruler;
    canvas->Brush = wxBrush_Ruler;
    // draw outline
    canvas->MoveTo( iLeftEdge, iTopEdge);         // top hori line
    canvas->LineTo( iRightEdge, iTopEdge);
    canvas->MoveTo( iLeftEdge, iBottomEdge );     // bott hori line
    canvas->LineTo( iRightEdge, iBottomEdge );
    canvas->MoveTo( iLeftEdge, iTopEdge );        // left vert line
    canvas->LineTo( iLeftEdge, iBottomEdge );
    canvas->MoveTo( iRightEdge, iTopEdge );       // right vert line
    canvas->LineTo( iRightEdge, iBottomEdge );

    canvas->Pen = pen_backup;
    canvas->Brush = brush;
}
//---------------------------------------------------------------------------
void PaintGraphics::DrawLine( TCanvas *canvas, int a, int b, int c, int d ) {
    canvas->MoveTo( a, b );
    canvas->LineTo( c, d );
}
//---------------------------------------------------------------------------
void PaintGraphics::ShowSamples( class f *fields ) {
    if( Pen_Samples == NULL ) {
     //   return;
    }
    TCanvas *canvas = stArea->Canvas;
    canvas->Pen = Pen_Samples;
    canvas->Brush = Brush_Samples;
    int a, b, c, d;
    a = 2+map( fields->x[0], 0-fields->Vx, fields->Vx, iLeftEdge+1, iRightEdge-1 );
    b = 2+map( fields->y[0], 0-fields->Vx, fields->Vy, iTopEdge+1, iBottomEdge-1 );
    for( int i = 1; i < COORDS; i++ ) {
        c = 2+map( fields->x[i], 0-fields->Vx, fields->Vx, iLeftEdge+1, iRightEdge-1 );
        d = 2+map( fields->y[i], 0-fields->Vy, fields->Vy, iTopEdge+3, iBottomEdge+1 );
        DrawLine( canvas, a, b, c, d );
        a = c;
        b = d;
    }
}
//---------------------------------------------------------------------------
void PaintGraphics::SampleColour( void ) {
    static char colour = 1;
    colour ^= 1;
    if( colour ) {
        Pen_Samples = wxPen_SamplesBlue;
    } else {
        Pen_Samples = wxPen_SamplesGreen;
    }
}
//---------------------------------------------------------------------------
void PaintGraphics::ChangeSamplesColour( unsigned long clEven, unsigned long clOdd ) {
    wxPen_SamplesGreen->Color = (TColor)clEven;
    wxPen_SamplesBlue->Color = (TColor)clOdd;
}
//---------------------------------------------------------------------------
void __fastcall PaintGraphics::SetPenWidth(int value) {
    iPenWidth = value;
    wxPen_SamplesGreen->Width = iPenWidth;
    wxPen_SamplesBlue->Width = iPenWidth;
}
//---------------------------------------------------------------------------

