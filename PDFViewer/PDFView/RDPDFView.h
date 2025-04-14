//
//  PDFOffScreenView.h
//  PDFViewer
//
//  Created by Radaee on 2016/12/6.
//
//
#pragma once
#import "PDFObjc.h"
#import "RDVLayout.h"
#import "RDVSel.h"
#import "RDVFinder.h"
#import "PDFDelegate.h"

@class PDFLayoutView;
@interface RDPDFCanvas : UIView
{
    PDFLayoutView *m_view;
}
-(void)setView :(PDFLayoutView *)view;
@end

@interface RDPDFView : UIView
{
    PDFLayoutView *m_view;
    RDPDFCanvas *m_canvas;
}
-(id)initWithFrame:(CGRect)frame;
-(id)initWithCoder:(NSCoder *)aDecoder;
-(PDFLayoutView *)view;
-(RDPDFCanvas *)canvas;
@end

@interface RDPDFThumb :RDPDFView
{
}
@end
