#pragma once
#import "DOCXObjc.h"

@class DOCXThread;
@class DOCXCache;
@class DOCXCacheSet;

@interface DOCXVPage : NSObject
{
    DOCXPage *m_page;
    DOCXCacheSet *m_caches;
    DOCXCacheSet *m_caches_zoom;
    CALayer *m_layer;
    DOCXDoc *m_doc;
    int m_pageno;
    int m_x;
    int m_y;
    int m_w;
    int m_h;
    int m_cw;
    int m_ch;
    int m_x0;
    int m_y0;
    int m_x1;
    int m_y1;
    int m_xb0;
    int m_yb0;
    float m_scale;
    bool m_need_clip;
    bool m_thumb;
}
@property(readonly) int pageno;
@property(readonly) int x;
@property(readonly) int y;
@property(readonly) int w;
@property(readonly) int h;
@property(readonly) float scale;
@property bool thumbMode;

-(id)init :(DOCXDoc *) doc :(int) pageno :(int) cw :(int) ch;
-(void)vLayerInit : (CALayer *)root;
-(void)vLayerDel;
- (DOCXPage *)GetPage;
-(int)GetX;
-(int)GetY;
-(float)GetDOCXX :(int) vx;
-(float)GetDOCXY :(int) vy;
-(int)GetVX :(float) pdfx;
-(int)GetVY :(float) pdfy;
-(int)GetWidth;
-(int)GetHeight;
-(float)GetScale;
-(float)ToDOCXX :(int) x :(int) scrollx;
-(float)ToDOCXY :(int) y :(int) scrolly;
-(float)ToPDFSize :(int) val;
-(RDPDFMatrix *)CreateInvertMatrix :(float) scrollx :(float) scrolly;
-(RDPDFMatrix *)CreateIMatrix :(float) scrollx :(float) scrolly :(float)scale;
-(void)vDestroy :(DOCXThread *) thread;
-(void)vLayout :(int) x :(int) y :(float) scale :(bool) clip;
-(void)vClips :(DOCXThread *) thread :(bool) clip;
-(void)vEndPage :(DOCXThread *) thread;
-(NSMutableArray *)vBackCache;
-(void)vBackEnd :(DOCXThread *) thread :(NSMutableArray *)arr;
-(bool)vFinished;
-(void)vRenderAsync :(DOCXThread *) thread :(int) docx :(int) docy :(int) vw :(int) vh;
-(void)vRenderSync :(DOCXThread *) thread :(int) docx :(int) docy :(int) vw :(int) vh;
-(void)vDraw :(DOCXThread *) thread :(int) docx :(int) docy :(int) vw :(int) vh;
-(bool)vDrawZoom :(float)scale;
-(void)vZoomStart;
-(void)vZoomEnd :(DOCXThread *) thread;
@end
