//
//  PDFLayoutView.h
//  RDPDFReader
//
//  Created by Radaee on 16/11/19.
//  Copyright © 2016年 radaee. All rights reserved.
//
#pragma once
#import "DOCXObjc.h"
#import "DOCXLayout.h"
#import "DOCXVFinder.h"
#import "PDFDelegate.h"

#define UIColorFromRGB(rgbValue) \
[UIColor colorWithRed:((float)((rgbValue & 0x00FF0000) >> 16))/255.0 \
green:((float)((rgbValue & 0x0000FF00) >>  8))/255.0 \
blue:((float)((rgbValue & 0x000000FF) >>  0))/255.0 \
alpha:((float)((rgbValue & 0xFF000000) >>  24))/255.0]

@protocol DOCXViewDelegate <NSObject>
- (void)OnPageChanged :(int)pageno;
- (void)OnPageUpdated :(int)pageno;
- (void)OnLongPressed:(float)x :(float)y;
- (void)OnSingleTapped:(float)x :(float)y;
- (void)OnDoubleTapped:(float)x :(float)y;
- (void)OnFound:(bool)found;
- (void)OnSelStart:(float)x :(float)y;
- (void)OnSelEnd:(float)x1 :(float)y1 :(float)x2 :(float)y2;
- (void)OnPageGoto:(int)pageno;
- (void)OnOpenURL:(NSString *)url;
@end

//UIKeyInput
//UITextInputTraits
@class RDDOCXCanvas;
@interface DOCXLayoutView : UIScrollView <UIScrollViewDelegate, PDFOffScreenDelegate, DOCXLayoutDelegate>
{
    DOCXDoc *m_doc;
    DOCXLayout *m_layout;
    NSTimer *m_timer;
    enum DOCXLAYOUT_STATUS
    {
        sta_docx_none,
        sta_docx_zoom,
        sta_docx_sel,
    };
    enum DOCXLAYOUT_STATUS m_status;
    float m_scale_pix;
    float m_zoom;
    DOCXPos m_zoom_pos;
    CGPoint zoomPoint;
    UIView *m_child;
    RDDOCXCanvas *m_canvas;
    id<DOCXViewDelegate> m_del;
    NSTimeInterval m_tstamp;
    NSTimeInterval m_tstamp_tap;
    float m_tx;
    float m_ty;
    float m_px;
    float m_py;
    int m_page_gap;
    
    int m_w;
    int m_h;
    int m_cur_page;
    
    DOCXSel *m_sel;
    DOCXPos m_sel_pos;

    int m_note_cur;

    PDF_POINT *m_lines;
    int m_lines_cnt;
    int m_lines_max;
    bool m_lines_drawing;
    
    PDF_POINT *m_rects;
    int m_rects_cnt;
    int m_rects_max;
    bool m_rects_drawing;
    
    PDF_POINT *m_ellipse;
    int m_ellipse_cnt;
    int m_ellipse_max;
    bool m_ellipse_drawing;
    
    BOOL doublePage;
    
    UIImageView *imgAnnot;
    NSString *tmpImage;
    double lastAngle;
    
    BOOL isResizing;
    BOOL isRotating;
    BOOL coverPage;
    
    int doubleTapZoomMode;
    int readerBackgroundColor;
    
    DOCXPage *tappedPage;
    
    bool isDoubleTapping;
}

@property (nonatomic) NSUInteger pageViewNo;

-(id)initWithFrame:(CGRect)frame;
- (id)initWithCoder:(NSCoder *)aDecoder;
-(BOOL)DOCXOpen :(DOCXDoc *)doc :(int)page_gap :(RDDOCXCanvas *)canvas :(id<DOCXViewDelegate>) del;
-(void)DOCXClose;
-(void)DOCXSetVMode:(int)vmode;

//start find.
-(bool)vFindStart:(NSString *)pat :(bool)match_case :(bool)whole_word;
//find it.
-(void)vFind:(int)dir;
//end find
-(void)vFindEnd;


//invoke this method to set select mode, once you set this mode, you can select texts by touch and moving.
-(void)vSelStart;
//you should invoke this method in select mode.
-(NSString *)vSelGetText;
//invoke this method to leave select mode
-(void)vSelEnd;

- (void)vGetPos:(DOCXPos *)pos;
- (void)vGetPos:(DOCXPos *)pos x:(int)x y:(int)y;

/**
 goto page
 @param pageno page number
 */
-(void)vGoto:(int)pageno;
- (int)vGetCurrentPage;
- (CGFloat)vGetScale;
- (CGFloat)vGetPixSize;
@end

