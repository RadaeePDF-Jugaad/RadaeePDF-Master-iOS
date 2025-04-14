//
//  VCache.h
//  RDPDFReader
//
//  Created by Radaee on 16/11/19.
//  Copyright © 2016年 radaee. All rights reserved.
//
#pragma once
#import "DOCXObjc.h"

@interface DOCXCache : NSObject
{
    DOCXDoc *m_doc;
    DOCXPage *m_page;
    int m_pageno;
    float m_scale_pix;
    float m_scale;
    int m_dibx;
    int m_diby;
    int m_dibw;
    int m_dibh;
    int m_status;//2 mean render finished without cancel, -1 mean render cancelled, 1 is rendering, others: 0
    bool m_thumb;
    PDF_DIB m_dib;//use PDF_DIB in direct may has better performance.
    CALayer *m_layer;
}
@property int x;
@property int y;
@property int w;
@property int h;
@property int pageno;
@property bool thumbMode;

-(id)init:(DOCXDoc *)doc :(int)pageno :(float) scale :(int)dibx :(int)diby :(int)dibw :(int)dibh;
-(DOCXCache *)vClone;
-(bool)vStart;
-(bool)vEnd;
-(bool)vIsRenderFinished;
-(bool)vIsRendering;
-(void)vRender;
-(void)vDestroy;
-(void)vDestroyLayer;
-(void)vDraw :(CALayer *)canvas;
-(void)vDrawZoom :(CALayer *)parent :(float)scale;
@end

@interface DOCXCacheSet : NSObject
{
    NSMutableArray *m_dat;
    int m_cols;
    int m_rows;
}
@property(readonly) int cols;
@property(readonly) int rows;
-(id)init :(int)cols :(int) rows;
-(DOCXCache *)get :(int)col :(int) row;
-(void)set :(int)col :(int) row :(DOCXCache *)cache;
@end

