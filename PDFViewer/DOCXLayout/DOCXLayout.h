//
//  DOCXLayout.h
//  RDPDFReader
//
//  Created by Radaee on 16/11/20.
//  Copyright © 2016年 radaee. All rights reserved.
//
#pragma once
#import "DOCXObjc.h"

@class DOCXVPage;
@class DOCXThread;
@class RDVCanvas;
@class DOCXSel;
@class DOCXVFinder;

typedef struct _DOCXPos
{
    int pageno;
    float pdfx;
    float pdfy;
}DOCXPos;

@protocol DOCXLayoutDelegate <NSObject>
- (void)RDVOnPageRendered:(int)pageno;
- (void)RDVOnFound:(DOCXVFinder *)finder;
@end

@interface DOCXLayout :NSObject
{
    DOCXDoc *m_doc;
    NSMutableArray *m_pages;
    int m_pages_cnt;
    DOCXThread *m_thread;
    float m_scale;
    float m_scale_min;
    float m_scale_max;
    int m_w;
    int m_h;
    int m_docx;
    int m_docy;
    int m_docw;
    int m_doch;
    int m_cellw;
    int m_cellh;
    int m_page_gap;
    int m_disp_pg1;
    int m_disp_pg2;
    DOCXPos m_zoom_pos;
    bool m_zooming;
    int m_zoom_pg1;
    int m_zoom_pg2;
    DOCXVFinder *m_finder;
    id<DOCXLayoutDelegate> m_del;
    CALayer *m_rlayer;
    
    // custom scales
    float *m_scales_min;
    float *m_scales_max;
    float *m_scales;
    int m_render_cnt;
}
@property(readonly) int docx;
@property(readonly) int docy;
@property(readonly) int docw;
@property(readonly) int doch;
@property(readonly) int vw;
@property(readonly) int vh;
@property(readonly) float zoomMin;
@property(readonly) float zoomMax;
@property(readonly) float zoom;
@property(readonly) DOCXVFinder *finder;
@property(readonly) int cur_pg1;
@property(readonly) int cur_pg2;

-(id)init :(id<DOCXLayoutDelegate>)del;
-(void)vOpen :(DOCXDoc *)doc :(int)page_gap :(CALayer *)rlay;
-(void)vClose;
-(void)vResize :(int)vw :(int)vh;
-(void)vGetPos :(int)vx :(int)vy :(DOCXPos *)pos;
-(void)vSetPos :(int)vx :(int)vy :(const DOCXPos *)pos;
-(void)vMoveTo :(int)docx :(int)docy;
-(DOCXVPage *)vGetPage :(int)pageno;
-(void)vDraw :(RDVCanvas *)canvas;
-(float)vGetScaleMin:(int)page;
-(void)vZoomStart;
-(void)vZooming:(float)zoom;
-(void)vZoomConfirm;
-(void)vGotoPage:(int)pageno;
-(void)vRenderAsync:(int)pageno;
-(void)vRenderSync:(int)pageno;
-(void)vFindStart:(NSString *)pat :(bool)match_case :(bool) whole_word;
-(int)vFind:(int) dir;
-(void)vFindEnd;
-(bool)vFindGoto;
-(bool)vCanPaging;
@end

typedef enum _DOCXPAGE_ALIGN
{
    docx_align_left = 0,
    docx_align_right = 1,
    docx_align_hcenter = 2,
    docx_align_top = 3,
    docx_align_bot = 4,
    docx_align_vcenter = 5,
}DOCXPAGE_ALIGN;

@interface DOCXLayoutVert :DOCXLayout
{
    DOCXPAGE_ALIGN m_align;
    bool m_same_width;
}
-(id)init :(id<DOCXLayoutDelegate>)del :(bool)same_width;
-(void)vSetAlign :(DOCXPAGE_ALIGN) align;
@end

@interface DOCXLayoutHorz :DOCXLayout
{
    DOCXPAGE_ALIGN m_align;
    bool       m_rtol;
    bool       m_same_height;
    bool       m_thumb;
}
-(id)init :(id<DOCXLayoutDelegate>)del :(bool)rtol :(bool)same_height;
-(void)vSetAlign :(DOCXPAGE_ALIGN) align;
@end

@interface DOCXLayoutThumb :DOCXLayoutHorz
{
}
-(id)init :(id<DOCXLayoutDelegate>)del :(BOOL)rtol;
@end

@interface DOCXLayoutGrid :DOCXLayout
{
    DOCXPAGE_ALIGN m_align;
    int        m_grid_mode;
    int        m_height;
    bool       m_thumb;
}
-(id)init :(id<DOCXLayoutDelegate>)del :(int)height :(int)mode;
@end

typedef enum
{
    DOCX_SCALE_NONE = 0,//no scale, same to old layout style.
    DOCX_SCALE_SAME_WIDTH = 1,//min scale of all cells are in same width
    DOCX_SCALE_SAME_HEIGHT = 2,//min scale of all cells are in same height
    DOCX_SCALE_FIT = 3//min scale of all cells are fit screen.
}DOCX_SCALE_MODE;

@interface DOCXLayoutDual :DOCXLayout
{
    DOCXPAGE_ALIGN m_align;
    DOCX_SCALE_MODE m_smode;
    bool       m_rtol;
    int        m_cells_cnt;
    bool       *m_vert_dual;
    int        m_vert_dual_cnt;
    bool      *m_horz_dual;
    int        m_horz_dual_cnt;
    int        m_cell_w;
    struct DOCXCell
    {
        int left;
        int right;
        int page_left;
        int page_right;
    }* m_cells;
}
-(id)init :(id<DOCXLayoutDelegate>)del :(bool)rtol :(const bool *)verts :(int)verts_cnt :(const bool *)horzs :(int) horzs_cnt;
-(void)vSetScaleMode:(DOCX_SCALE_MODE)scale_mode;
-(void)vSetAlign :(DOCXPAGE_ALIGN) align;
@end

@interface DOCXLayoutSingle :DOCXLayout
{
    DOCXPAGE_ALIGN m_align;
    bool       m_rtol;
    bool       m_thumb;
    int        pageViewNo;
}
-(id)init :(id<DOCXLayoutDelegate>)del :(BOOL)rtol :(int)pageno;
-(void)vSetAlign :(DOCXPAGE_ALIGN) align;

@end
