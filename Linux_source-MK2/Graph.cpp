//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

#include "Graph.h"

PaintGraphics::PaintGraphics( wxStaticText *stArea_param ) {
    stArea = stArea_param;
    stArea->GetSize( &iDrawWidth, &iDrawHeight );
    GridVisible = false;
}

void PaintGraphics::ChangeSamplesColour( wxColour ColourEven, wxColour ColourOdd, wxColour ColourStorage, int iPenWidth ) {
    wxPen_Samples1.SetColour( ColourEven.Red(), ColourEven.Green(), ColourEven.Blue() );
    wxPen_Samples2.SetColour( ColourOdd.Red(), ColourOdd.Green(), ColourOdd.Blue() );
    wxPen_Storage.SetColour( ColourStorage.Red(), ColourStorage.Green(), ColourStorage.Blue() );
    wxPen_Samples1.SetWidth( iPenWidth );
    wxPen_Samples2.SetWidth( iPenWidth );
    wxPen_Storage.SetWidth( iPenWidth );
}

void PaintGraphics::SetBox( int iRulerWidth_param, int iLeftMargin_param, int iRightMargin_param, int iTopMargin_param, int iBottomMargin_param ) {
    iRulerWidth = iRulerWidth_param;
    iLeftMargin = iLeftMargin_param + iRulerWidth_param;
    iRightMargin = iRightMargin_param + iRulerWidth_param;
    iTopMargin = iTopMargin_param + iRulerWidth_param;
    iBottomMargin = iBottomMargin_param + iRulerWidth_param;
    iCenterWidth = 4 + ( iDrawWidth - iRightMargin - iLeftMargin ) / 2 + iLeftMargin;
    iCenterHeight = 2 + ( iDrawHeight - iTopMargin - iBottomMargin ) / 2 + iTopMargin;
    iLeftEdge = iRulerWidth + iLeftMargin + 1;
    iRightEdge = 2 + iDrawWidth - iRightMargin ;
    iTopEdge = iRulerWidth + iTopMargin + 1;
    iBottomEdge = iDrawHeight - iBottomMargin - 3;
}

void PaintGraphics::ShowRulers( void ) {
    wxClientDC canvas( stArea );
    // text settings
    wxFont font( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false );
    canvas.SetFont( font );
    canvas.SetTextForeground( wxColour( 255, 255, 255 ) );
    // ruler settings
    wxPen_Ruler.SetColour( 128, 128, 128 );
    wxPen_Ruler.SetWidth( 1 );
    canvas.SetPen( wxPen_Ruler );
    // draw outline
    canvas.DrawLine( iLeftEdge, iTopEdge, iRightEdge, iTopEdge );          // top hori line
    canvas.DrawLine( iLeftEdge, iBottomEdge, iRightEdge, iBottomEdge );    // bott hori line
    canvas.DrawLine( iLeftEdge, iTopEdge, iLeftEdge, iBottomEdge );        // left vert line
    canvas.DrawLine( iRightEdge, iTopEdge, iRightEdge, iBottomEdge );      // right vert line
    signed int VV = 0, TT = 0;
    char bufRuler[32];
    TT = 0;
    for( int i = iCenterWidth; i <= iRightEdge; i++ ) {
        if( ( ( i - iCenterWidth ) % ( iSpacingHori * 10 ) ) == 0 ) {
            canvas.DrawLine( i, iTopEdge, i, iTopEdge - 10 );
            if( ( i - iCenterWidth ) > 0 ) {
                ++TT;
                sprintf( bufRuler, "%d", TT * iFactorMulHori / iFactorDivHori );
                if( 10 * TT * iFactorMulHori / iFactorDivHori < 10 && iFactorDivHori > 1 ) {
                    sprintf( bufRuler, "0.%d", 10 * TT * iFactorMulHori / iFactorDivHori );
                    if( strlen( bufRuler ) > 2 ) {
                        canvas.DrawText( _( bufRuler ), i - 14, iTopEdge - 30 );
                    } else if( strlen( bufRuler ) > 1 ) {
                        canvas.DrawText( _( bufRuler ), i - 12, iTopEdge - 30 );
                    } else {
                        canvas.DrawText( _( bufRuler ), i - 6, iTopEdge - 30 );
                    }
                } else {
                    if( i > iRightEdge - 5 ) {
                        if( strlen( bufRuler ) > 1 ) {
                            canvas.DrawText( _( bufRuler ), i - 16, iTopEdge - 30 );
                        } else {
                            canvas.DrawText( _( bufRuler ), i - 10, iTopEdge - 30 );
                        }
                    } else {
                        canvas.DrawText( _( bufRuler ), i - 3, iTopEdge - 30 );
                    }
                }
            }
        } else if( ( ( iCenterWidth - i ) % ( iSpacingHori * 5 ) ) == 0 ) {
            canvas.DrawLine( i, iTopEdge, i, iTopEdge - 7 );
        } else if( ( ( iCenterWidth - i ) % iSpacingHori ) == 0 ) {
            canvas.DrawLine( i, iTopEdge, i, iTopEdge - 3 );
        }
    }
    TT = 0;
    for( int i = iCenterWidth; i >= iLeftEdge; i-- ) {
        if( ( ( i - iCenterWidth ) % ( iSpacingHori * 10 ) ) == 0 ) {
            canvas.DrawLine( i, iTopEdge, i, iTopEdge - 10 );
            if( ( i - iCenterWidth ) < 0 ) {
                TT--;
                if( 10 * TT * iFactorMulHori / iFactorDivHori > -10 && iFactorDivHori > 1 ) {
                    sprintf( bufRuler, "-0.%d", abs( 10 * TT * iFactorMulHori / iFactorDivHori ) );
                    if( strlen( bufRuler ) > 2 ) {
                        canvas.DrawText( _( bufRuler ), i - 12, iTopEdge - 30 );
                    } else {
                        canvas.DrawText( _( bufRuler ), i - 6, iTopEdge - 30 );
                    }
                } else {
                    sprintf( bufRuler, "%d", TT * iFactorMulHori / iFactorDivHori );
                    if( strlen( bufRuler ) > 2 ) {
                        canvas.DrawText( _( bufRuler ), i - 12, iTopEdge - 30 );
                    } else {
                        canvas.DrawText( _( bufRuler ), i - 6, iTopEdge - 30 );
                    }
                }
            }
        } else if( ( ( iCenterWidth - i ) % ( iSpacingHori * 5 ) ) == 0 ) {
            canvas.DrawLine( i, iTopEdge, i, iTopEdge - 7 );
        } else if( ( ( iCenterWidth - i ) % iSpacingHori ) == 0 ) {
            canvas.DrawLine( i, iTopEdge, i, iTopEdge - 3 );
        }
    }
    VV = 0;
    for( int i = iCenterHeight; i <= iBottomEdge; i++ ) {
        if( ( ( iCenterHeight - i ) % ( iSpacingVert * 10 ) ) == 0 ) {
            canvas.DrawLine( iLeftEdge, i, iLeftEdge - 10, i );
            if( ( i - iCenterHeight ) > 0 ) {
                --VV;
                if( 10 * VV * iFactorMulVert / iFactorDivVert > -10 && iFactorDivVert > 1 ) {
                    sprintf( bufRuler, "-0.%d", abs( 10 * VV * iFactorMulVert / iFactorDivVert ) );
                    if( i > iBottomEdge - 10 ) {
                        canvas.DrawText( _( bufRuler ), iLeftEdge - 33, i - 15 );
                    } else {
                        canvas.DrawText( _( bufRuler ), iLeftEdge - 33, i - 7 );
                    }
                } else {
                    sprintf( bufRuler, "%d", VV * iFactorMulVert / iFactorDivVert );
                    if( i > iBottomEdge - 10 ) {
                        canvas.DrawText( _( bufRuler ), iLeftEdge - 33, i - 15 );
                    } else {
                        canvas.DrawText( _( bufRuler ), iLeftEdge - 33, i - 7 );
                    }
                }
            }
        } else if( ( ( iCenterHeight - i ) % ( iSpacingVert * 5 ) ) == 0 ) {
            canvas.DrawLine( iLeftEdge, i, iLeftEdge - 7, i );
        } else if( ( ( iCenterHeight - i ) % iSpacingVert ) == 0 ) {
            canvas.DrawLine( iLeftEdge, i, iLeftEdge - 3, i );
        }
    }
    VV = 0;
    for( int i = iCenterHeight; i >= iTopEdge; i-- ) {
        if( ( ( iCenterHeight - i ) % ( iSpacingVert * 10 ) ) == 0 ) {
            canvas.DrawLine( iLeftEdge, i, iLeftEdge - 10, i );
            if( ( i - iCenterHeight ) < 0 ) {
                ++VV;
                if( 10 * VV * iFactorMulVert / iFactorDivVert < 10 && iFactorDivVert > 1 ) {
                    sprintf( bufRuler, "0.%d", 10 * VV * iFactorMulVert / iFactorDivVert );
                    canvas.DrawText( _( bufRuler ), iLeftEdge - 33, i - 7 );
                } else {
                    sprintf( bufRuler, "%d", VV * iFactorMulVert / iFactorDivVert );
                    canvas.DrawText( _( bufRuler ), iLeftEdge - 33, i - 7 );
                }
            }
        } else if( ( ( iCenterHeight - i ) % ( iSpacingVert * 5 ) ) == 0 ) {
            canvas.DrawLine( iLeftEdge, i, iLeftEdge - 7, i );
        } else if( ( ( iCenterHeight - i ) % iSpacingVert ) == 0 ) {
            canvas.DrawLine( iLeftEdge, i, iLeftEdge - 3, i );
        }
    }
    /*
        canvas.DrawLine( left_border-1, left_border-1, left_border-1, left_border - 10 );                       // top left mark
        canvas.DrawLine( CenterW, left_border-1, CenterH, left_border - 10 );                                   // top center mark
        canvas.DrawLine( iThisW - right_border, left_border-1, iThisW - right_border, left_border - 10 );       // top right mark
        canvas.SetPen( *wxWHITE_PEN );
    */
    canvas.DrawText( _( "0" ) + wxsHoriUnit, iCenterWidth - 3, iTopEdge - 30 );
    canvas.DrawText( _( "0" ) + wxsVertUnit, iLeftEdge - 33, iCenterHeight - 7 );
}

void PaintGraphics::ShowGrid( void ) {
    wxClientDC canvas( stArea );
    if( GridVisible ) {
        wxPen_Grid.SetColour( 32, 32, 32 );
        wxPen_Grid.SetWidth( 1 );
        canvas.SetPen( wxPen_Grid );
        for( int i = iCenterWidth + iSpacingHori; i < iRightEdge; i += iSpacingHori ) {
            canvas.DrawLine( i, iTopEdge, i, iBottomEdge );
        }
        for( int i = iCenterWidth - iSpacingHori; i > iLeftEdge; i -= iSpacingHori ) {
            canvas.DrawLine( i, iTopEdge, i, iBottomEdge );
        }
        for( int i = iCenterHeight + iSpacingVert; i < iBottomEdge; i += iSpacingVert ) {
            canvas.DrawLine( iLeftEdge, i, iRightEdge, i );
        }
        for( int i = iCenterHeight - iSpacingVert; i > iTopEdge; i -= iSpacingVert ) {
            canvas.DrawLine( iLeftEdge, i, iRightEdge, i );
        }
        wxPen_Grid.SetColour( 64, 64, 64 );
        wxPen_Grid.SetWidth( 1 );
        canvas.SetPen( wxPen_Grid );
        for( int i = iCenterWidth + iSpacingHori * 5; i < iRightEdge; i += iSpacingHori * 5 ) {
            canvas.DrawLine( i, iTopEdge, i, iBottomEdge );
        }
        for( int i = iCenterWidth - iSpacingHori * 5; i > iLeftEdge; i -= iSpacingHori * 5 ) {
            canvas.DrawLine( i, iTopEdge, i, iBottomEdge );
        }
        for( int i = iCenterHeight + iSpacingVert * 5; i < iBottomEdge; i += iSpacingVert * 5 ) {
            canvas.DrawLine( iLeftEdge, i, iRightEdge, i );
        }
        for( int i = iCenterHeight - iSpacingVert * 5; i > iTopEdge; i -= iSpacingVert * 5 ) {
            canvas.DrawLine( iLeftEdge, i, iRightEdge, i );
        }
        wxPen_Grid.SetColour( 128, 128, 128 );
        wxPen_Grid.SetWidth( 1 );
        canvas.SetPen( wxPen_Grid );
        canvas.DrawLine( iCenterWidth, iTopEdge, iCenterWidth, iBottomEdge );          // center vert line
        canvas.DrawLine( iLeftEdge, iCenterHeight, iRightEdge, iCenterHeight );        // center hori line
    }
}

void PaintGraphics::Clear( void ) {
    wxClientDC canvas( stArea );
    // redraw border
    wxPen_Border.SetColour( 255, 255, 255 ); // setting colour is mandatory but useless for pen width 0
    wxPen_Border.SetWidth( 0 );
    wxBrush_Border.SetColour( 255, 255, 255 );
    canvas.SetPen( wxPen_Border );
    canvas.SetBrush( wxBrush_Border );
    canvas.DrawRectangle( 0, 0, iDrawWidth - 1, iDrawHeight - 1 );
    // redraw margin
    wxPen_Margin.SetColour( 0, 0, 0 );
    wxPen_Margin.SetWidth( 0 );
    wxBrush_Margin.SetColour( 0, 0, 0 );
    canvas.SetPen( wxPen_Margin );
    canvas.SetBrush( wxBrush_Margin );
    canvas.DrawRectangle( iRulerWidth, iRulerWidth, iDrawWidth - iRulerWidth, iDrawHeight - iRulerWidth - iRulerWidth );
}

void PaintGraphics::DrawLine( wxClientDC *canvas, int a, int b, int c, int d ) {
    canvas->DrawLine( a, b, c, d );
}

void PaintGraphics::ShowSamples( struct f *fields, struct f *held, bool bShowHeld ) {
    wxClientDC canvas( stArea );
    int a, b, c, d, e, f;
    static char colour = 1;
    wxBrush_Samples.SetColour( 0, 0, 0 );
    canvas.SetBrush( wxBrush_Samples );
    // Show stored sample
    if( bShowHeld ) {
        canvas.SetPen( wxPen_Storage );
        a = map( held->xy[0], 0 - held->Vx, held->Vx, iLeftEdge + 2, iRightEdge - 2 );
        b = map( held->xy[1], 0 - held->Vy, held->Vy, iTopEdge + 2, iBottomEdge - 2 );
        e = a;
        f = b;
        for( int i = 2; i < (COORDS<<1); i+=2 ) {
            c = map( held->xy[i], 0 - held->Vx, held->Vx, iLeftEdge + 2, iRightEdge - 2 );
            d = map( held->xy[i+1], 0 - held->Vy, held->Vy, iTopEdge + 2, iBottomEdge - 2 );
            DrawLine( &canvas, a, b, c, d );
            a = c;
            b = d;
        }
        DrawLine( &canvas, a, b, e, f );
    }
    // Show measured sample
    colour ^= 1;
    if( colour ) {
        wxPen_Samples = wxPen_Samples1;
    } else {
        wxPen_Samples = wxPen_Samples2;
    }
    canvas.SetPen( wxPen_Samples );
    a = map( fields->xy[0], 0 - fields->Vx, fields->Vx, iLeftEdge + 2, iRightEdge - 2 );
    b = map( fields->xy[1], 0 - fields->Vy, fields->Vy, iTopEdge + 2, iBottomEdge - 2 );
    e = a;
    f = b;
    stArea->ClearBackground();
    for( int i = 2; i < (COORDS<<1); i+=2 ) {
        c = map( fields->xy[i], 0 - fields->Vx, fields->Vx, iLeftEdge + 2, iRightEdge - 2 );
        d = map( fields->xy[i+1], 0 - fields->Vy, fields->Vy, iTopEdge + 2, iBottomEdge - 2 );
        DrawLine( &canvas, a, b, c, d );
        a = c;
        b = d;
    }
    DrawLine( &canvas, a, b, e, f );
}

