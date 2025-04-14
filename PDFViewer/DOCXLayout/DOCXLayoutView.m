//
//  PDFLayoutView.m
//  RDPDFReader
//
//  Created by Radaee on 16/11/19.
//  Copyright © 2016年 radaee. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DOCXLayoutView.h"
#import "DOCXVPage.h"
#import "RDVGlobal.h"
#import "RDVCanvas.h"
#import "DOCXVFinder.h"
#import "DOCXSel.h"
#import "RDDOCXView.h"

@implementation DOCXLayoutView
{
    DOCXPos m_save_pos;
    int m_save_vmode;
}
-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if(self)
    {
        m_doc = nil;
        m_layout = nil;
        m_timer = nil;
        m_scale_pix = [[UIScreen mainScreen] scale];
        m_status = sta_docx_none;
        m_sel = nil;
        m_cur_page = -1;
        self.userInteractionEnabled = YES;
        self.multipleTouchEnabled = YES;
        self.alwaysBounceHorizontal = NO;
        self.alwaysBounceVertical = NO;
        self.delegate = self;
        m_zoom = 1;
        self.minimumZoomScale = 1;
        self.maximumZoomScale = GLOBAL.g_layout_zoom_level;
        self.bouncesZoom = NO;
        m_child = [[UIView alloc] initWithFrame
                   :CGRectMake(0, 0, frame.size.width, frame.size.height)];
        [self addSubview:m_child];
        [self resignFirstResponder];
        
        if (@available(iOS 11.0, *)) {
            self.contentInsetAdjustmentBehavior = UIScrollViewContentInsetAdjustmentNever;
        }
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if(self)
    {
        m_doc = nil;
        m_layout = nil;
        m_timer = nil;
        m_scale_pix = [[UIScreen mainScreen] scale];
        m_status = sta_docx_none;
        m_sel = nil;
        m_cur_page = -1;
        self.userInteractionEnabled = YES;
        self.multipleTouchEnabled = YES;
        self.alwaysBounceHorizontal = NO;
        self.alwaysBounceVertical = NO;
        self.delegate = self;
        m_zoom = 1;
        self.minimumZoomScale = 1;
        self.maximumZoomScale = GLOBAL.g_layout_zoom_level;
        self.bouncesZoom = NO;
        CGRect frame = self.frame;
        m_child = [[UIView alloc] initWithFrame
                   :CGRectMake(0, 0, frame.size.width, frame.size.height)];
        [self addSubview:m_child];
        [self resignFirstResponder];
        
        if (@available(iOS 11.0, *)) {
            self.contentInsetAdjustmentBehavior = UIScrollViewContentInsetAdjustmentNever;
        }
        
    }
    return self;
}

-(void)setFrame:(CGRect)frame
{
    [super setFrame:frame];
    if (!m_layout) return;
    self.zoomScale = m_zoom = 1;
    [m_layout vResize:frame.size.width * m_scale_pix :frame.size.height * m_scale_pix];
    self.contentSize = CGSizeMake([m_layout docw]/m_scale_pix, [m_layout doch]/m_scale_pix);
    [m_layout vGotoPage:m_cur_page];
    [self setContentOffset:CGPointMake([m_layout docx]/m_scale_pix, [m_layout docy]/m_scale_pix) animated:NO];
    
    self.pagingEnabled = GLOBAL.g_paging_enabled && m_layout && [m_layout vCanPaging];
}

-(void)dealloc
{
    [self DOCXClose];
}

- (void)clean {
    [self DOCXClose];
    if (!m_child) {
        m_child = [[UIView alloc] initWithFrame:self.frame];
        [self addSubview:m_child];
    }
}

-(BOOL)DOCXOpen:(DOCXDoc *)doc :(int)page_gap :(RDDOCXCanvas *)canvas :(id<DOCXViewDelegate>) del
{
    [self clean];
    m_canvas = canvas;
    m_del = del;
    // Load global var
    doublePage = GLOBAL.g_double_page_enabled;
    
    // Zoom action on double tap
    // 1: default zoom
    // 2: smart zoom
    doubleTapZoomMode = 1;
    
    m_doc = doc;
    m_page_gap = page_gap * m_scale_pix;
    
    self.backgroundColor = (GLOBAL.g_readerview_bg_color != 0) ? UIColorFromRGB(GLOBAL.g_readerview_bg_color) : [UIColor colorWithRed:0.8f green:0.8f blue:0.8f alpha:1.0f];
    
    [self DOCXSetVMode:GLOBAL.g_view_mode];
    
    m_timer = [NSTimer scheduledTimerWithTimeInterval:0.3 target:self selector:@selector(ProOnTimer:) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop]addTimer:m_timer forMode:NSDefaultRunLoopMode];
    return TRUE;
}
-(int)PDFGetVMode
{
    return m_save_vmode;
}
-(void)DOCXSetVMode:(int)vmode
{
    m_save_vmode = vmode;
    bool center_page = false;
    DOCXPos pos;
    pos.pageno = -1;
    pos.pdfx = 0;
    pos.pdfy = 0;
    if(m_layout)
    {
        [m_layout vGetPos:[m_layout vw] /2 :[m_layout vh] /2 :&pos];
        [m_layout vClose];
    }
    m_layout = nil;
    m_sel = nil;
    m_cur_page = -1;
    m_status = sta_docx_none;
    m_zoom = 1;
    self.zoomScale = 1;

    bool *horzs = (bool *)calloc( sizeof(bool), m_doc.pageCount );
    switch (m_save_vmode) {
        case 1:// Horizontal LTOR
            doublePage = NO;
            m_layout = [[DOCXLayoutHorz alloc] init:self :GLOBAL.g_layout_rtol :GLOBAL.g_auto_scale];
            break;
        case 2:// PageView RTOL
            doublePage = NO;
            memset(horzs, 0, sizeof(bool));
            m_layout = [[DOCXLayoutSingle alloc] init:self :GLOBAL.g_layout_rtol :(int)_pageViewNo];
            break;
        case 3:// Single Page (LTOR, paging enabled)
            doublePage = NO;
            memset(horzs, 0, sizeof(bool) * m_doc.pageCount);
            m_layout = [[DOCXLayoutDual alloc] init:self :GLOBAL.g_layout_rtol :NULL :0 :horzs :[m_doc pageCount]];
            center_page = true;
            //[((RDVLayoutDual *)m_layout) vSetAlign:align_top];
            if (GLOBAL.g_auto_scale) [((DOCXLayoutDual *)m_layout) vSetScaleMode:DOCX_SCALE_FIT];
            break;
        case 4: // Double Page, and first page as single (paging enabled)
            memset(horzs, 1, sizeof(bool) * m_doc.pageCount);
            horzs[0] = false;
            m_layout = [[DOCXLayoutDual alloc] init:self :GLOBAL.g_layout_rtol :NULL :0 :horzs :[m_doc pageCount]];
            center_page = true;
            //[((RDVLayoutDual *)m_layout) vSetAlign:align_top];
            if (GLOBAL.g_auto_scale) [((DOCXLayoutDual *)m_layout) vSetScaleMode:DOCX_SCALE_FIT];
            break;
        case 6:// Double Page (LTOR, paging enabled)
            memset(horzs, 1, sizeof(bool) * m_doc.pageCount);
            m_layout = [[DOCXLayoutDual alloc] init:self :GLOBAL.g_layout_rtol :NULL :0 :horzs :[m_doc pageCount]];
            center_page = true;
            //[((RDVLayoutDual *)m_layout) vSetAlign:align_top];
            if (GLOBAL.g_auto_scale) [((DOCXLayoutDual *)m_layout) vSetScaleMode:DOCX_SCALE_FIT];
            break;
        default:// 0: Vertical
            m_layout = [[DOCXLayoutVert alloc] init : self :GLOBAL.g_auto_scale];
            [((DOCXLayoutVert *)m_layout) vSetAlign:docx_align_vcenter];
            break;
    }
    free( horzs );
    [m_layout vOpen :m_doc :m_page_gap :self.layer];
    m_status = sta_docx_none;
    m_zoom = 1;
    self.zoomScale = 1;
    CGRect rect = self.frame;
    CGSize size = rect.size;
    [m_layout vResize:size.width * m_scale_pix :size.height * m_scale_pix];
    self.pagingEnabled = GLOBAL.g_paging_enabled && m_layout && [m_layout vCanPaging];
    self.contentSize = CGSizeMake([m_layout docw]/m_scale_pix, [m_layout doch]/m_scale_pix);
    if(pos.pageno >= 0)
    {
        if (center_page)
            [m_layout vGotoPage:pos.pageno];
        else
            [m_layout vSetPos:[m_layout vw] /2 :[m_layout vh] /2 :&pos];
        CGPoint pt;
        pt.x = [m_layout docx] / m_scale_pix;
        pt.y = [m_layout docy] / m_scale_pix;
        self.contentOffset = pt;
    }
    [self setNeedsDisplay];
}

- (UIImage *)imageWithImage:(UIImage *)image scaledToSize:(CGSize)newSize {
    //UIGraphicsBeginImageContext(newSize);
    // In next line, pass 0.0 to use the current device's pixel scaling factor (and thus account for Retina resolution).
    // Pass 1.0 to force exact pixel size.
    UIGraphicsBeginImageContextWithOptions(newSize, NO, 1.0);
    [image drawInRect:CGRectMake(0, 0, newSize.width, newSize.height)];
    UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return newImage;
}

-(void)DOCXClose
{
    if(m_timer)
    {
        [m_timer invalidate];
        m_timer = NULL;
    }
    m_doc = nil;
    if(m_layout)
        [m_layout vClose];
    m_layout = nil;
    m_sel = nil;
    m_cur_page = -1;
    m_status = sta_docx_none;
    m_zoom = 1;
    self.zoomScale = 1;
    [m_child removeFromSuperview];
    m_child = nil;
    m_del = nil;
}

-(void)ProRedrawOS
{
    [m_canvas setNeedsDisplay];
}
-(void)ProUpdatePage:(int) pageno
{
    [m_layout vRenderSync:pageno];
    if(m_del) [m_del OnPageUpdated:pageno];
}

- (void)RDVOnPageRendered:(int)pageno
{
    
}

-(void)RDVOnFound :(DOCXVFinder *)finder
{
    [m_layout vFindGoto];
    CGPoint pt;
    pt.x = [m_layout docx] / m_scale_pix;
    pt.y = [m_layout docy] / m_scale_pix;
    self.contentOffset = pt;
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [self refresh];
    });
    
    if( m_del )
    {
        int pageno = [finder find_get_page];
        [m_del OnFound: (pageno >= 0 && pageno < [m_doc pageCount])];
    }
}

-(void)ProOnTimer:(NSTimer *)sender
{
    [self setNeedsDisplay];
}

-(void)osDrawFind:(CGContextRef)context
{
    if(m_status != sta_docx_none) return;
    DOCXVFinder *finder = [m_layout finder];
    if(!finder) return;
    int pgno = [finder find_get_page];
    if(pgno < [m_layout cur_pg1] || pgno >= [m_layout cur_pg2]) return;
    [finder drawOffScreen :[[RDVCanvas alloc] init :context :m_scale_pix]
                          :[m_layout vGetPage:pgno]
                          :[m_layout docx]
                          :[m_layout docy]];
}

-(void)osDrawSel:(CGContextRef)context
{
    if(m_status != sta_docx_sel) return;
    [m_sel drawOffScreen :[[RDVCanvas alloc] init :context :m_scale_pix]
                         :[m_layout vGetPage:[m_sel pageno]]
                         :[m_layout docx]
                         :[m_layout docy]];
}

-(void)drawRect:(CGRect)rect
{
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    //layout will check which part is rendered, and which is not.
    //it only refresh rendering block.
    [m_layout vDraw:[[RDVCanvas alloc] init :ctx :m_scale_pix]];
    
    //check current page changed?
    DOCXPos pos;
    [m_layout vGetPos :((doublePage) ? [m_layout vw] >> 2 : [m_layout vw] >> 1) :[m_layout vh] >> 1 :&pos];
    if( m_cur_page != pos.pageno )
    {
        m_cur_page = pos.pageno;
        if( m_del )
            [m_del OnPageChanged:m_cur_page];
    }
}

-(void)onDrawOffScreen:(CGContextRef)ctx
{
    //draw all other status.
    [self osDrawFind:ctx];
    [self osDrawSel:ctx];
}

-(BOOL)scrollViewShouldScrollToTop:(UIScrollView *)scrollView
{
    return YES;
}

-(void)scrollViewDidScrollToTop:(UIScrollView *)scrollView
{
}

-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if(scrollView != self) return;
    /*
     if(GLOBAL.g_view_mode == 3 || GLOBAL.g_view_mode == 4) {
     //Vertical block
     if (self.contentOffset.y <= 0)
     self.contentOffset = CGPointMake(self.contentOffset.x, 0);
     if (self.contentOffset.y > 0 && self.contentOffset.y >= self.contentSize.height - self.frame.size.height) {
     self.contentOffset = CGPointMake(self.contentOffset.x, self.contentSize.height - self.frame.size.height);
     }
     }*/
    
    //NSLog(@"POS:%f,%f", self.contentOffset.x, self.contentOffset.y);
    if(m_status == sta_docx_zoom)
    {
        self.contentOffset = CGPointMake([m_layout docx]/m_scale_pix, [m_layout docy]/m_scale_pix);
        [self setNeedsDisplay];
    }
    else
    {
        int xval = self.contentOffset.x * m_scale_pix;
        int yval = self.contentOffset.y * m_scale_pix;
        int xlay = [m_layout docx];
        int ylay = [m_layout docy];
        int vw = [m_layout vw];
        int vh = [m_layout vh];
        if(xval > xlay - vw && xval < xlay + vw && yval > ylay - vh && yval < ylay + vh)
        {
            if(xval < 0) xval = 0;
            if(yval < 0) yval = 0;
            [m_layout vMoveTo:xval :yval];
        }
        else self.contentOffset = CGPointMake([m_layout docx]/m_scale_pix, [m_layout docy]/m_scale_pix);
        [self setNeedsDisplay];
        //NSLog(@"ZPOS4:%f,%f", self.contentOffset.x, self.contentOffset.y);
    }
    [self ProRedrawOS];
}

- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
}

- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
{
}

- (void)scrollViewWillBeginDecelerating:(UIScrollView *)scrollView
{
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
}


- (UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView
{
    if( m_status == sta_docx_none || m_status == sta_docx_zoom )
        return m_child;
    else
        return NULL;
}

- (void)scrollViewWillBeginZooming:(UIScrollView *)scrollView withView:(UIView *)view
{
    if( m_status != sta_docx_none ) return;
    self.pagingEnabled = NO;
    CGPoint point = [scrollView.pinchGestureRecognizer locationInView:m_canvas];
    [m_layout vZoomStart];
    m_status = sta_docx_zoom;
    if ([[[UIDevice currentDevice] systemVersion] floatValue] < 8.0 && [[[UIDevice currentDevice] systemVersion] floatValue] > 6.0 && [[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
    {
        CGFloat buffer = point.y;
        point.y = point.x;
        point.x = [m_layout vw]/m_scale_pix - buffer;
    }
    
    zoomPoint = CGPointMake((point.x - ([m_layout vw]/(m_scale_pix * 2))), (point.y - ([m_layout vh]/(m_scale_pix * 2))));
    zoomPoint.x = (zoomPoint.x < 0) ? (zoomPoint.x * -1) : zoomPoint.x;
    zoomPoint.y = (zoomPoint.y < 0) ? (zoomPoint.y * -1) : zoomPoint.y;
    [m_layout vGetPos :(point.x - (zoomPoint.x * self.zoomScale)) * m_scale_pix :(point.y - (zoomPoint.y * self.zoomScale)) * m_scale_pix :&m_zoom_pos];
}

- (void)scrollViewDidZoom:(UIScrollView *)scrollView
{
    if( m_status != sta_docx_zoom ) return;
    m_zoom = self.zoomScale;
    CGPoint point = [scrollView.pinchGestureRecognizer locationInView:m_canvas];
    if ([[[UIDevice currentDevice] systemVersion] floatValue] < 8.0 && [[[UIDevice currentDevice] systemVersion] floatValue] > 6.0 && [[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
    {
        CGFloat buffer = point.y;
        point.y = point.x;
        point.x = [m_layout vw]/m_scale_pix - buffer;
    }
    
    [m_layout vZooming:m_zoom];
    self.contentSize = CGSizeMake([m_layout docw]/m_scale_pix, [m_layout doch]/m_scale_pix);
    [m_layout vSetPos:(point.x - (zoomPoint.x * m_zoom)) * m_scale_pix :(point.y - (zoomPoint.y * m_zoom)) * m_scale_pix :&m_zoom_pos];
    self.contentOffset = CGPointMake([m_layout docx]/m_scale_pix, [m_layout docy]/m_scale_pix);
    [self refresh];
    self.pagingEnabled = GLOBAL.g_paging_enabled && m_layout && [m_layout vCanPaging];
}

- (void)scrollViewDidEndZooming:(UIScrollView *)scrollView withView:(UIView *)view atScale:(CGFloat)scale
{
    if( m_status != sta_docx_zoom ) return;
    m_zoom = scale;
    [m_layout vZooming:m_zoom];
    [m_layout vZoomConfirm];
    self.contentSize = CGSizeMake([m_layout docw]/m_scale_pix, [m_layout doch]/m_scale_pix);
    self.contentOffset  = CGPointMake([m_layout docx]/m_scale_pix, [m_layout docy]/m_scale_pix);
    //NSLog(@"ZPOS7:%f,%f", self.contentOffset.x, self.contentOffset.y);
    m_status = sta_docx_none;
    [self refresh];
    
    self.pagingEnabled = GLOBAL.g_paging_enabled && m_layout && [m_layout vCanPaging];
}

- (void)zoomPageToScale:(CGFloat)scale atPoint:(CGPoint)point {
    [m_layout vZoomStart];
    m_status = sta_docx_zoom;
    CGPoint pt;
    pt.x = point.x * m_scale_pix;
    pt.y = point.y * m_scale_pix;
    [m_layout vGetPos:pt.x :pt.y :&m_zoom_pos];
    [m_layout vZooming:scale];
    self.zoomScale = scale;
    m_zoom = scale;
    self.contentSize = CGSizeMake([m_layout docw]/m_scale_pix, [m_layout doch]/m_scale_pix);
    [m_layout vSetPos:pt.x :pt.y :&m_zoom_pos];
    self.contentOffset = CGPointMake([m_layout docx]/m_scale_pix, [m_layout docy]/m_scale_pix);
    [m_layout vZoomConfirm];
    self.pagingEnabled = GLOBAL.g_paging_enabled && m_layout && [m_layout vCanPaging];

    m_status = sta_docx_none;
    [self refresh];
    
    if(m_zoom == 1) {
        [self vGoto:m_cur_page];
    }
    
    self.pagingEnabled = GLOBAL.g_paging_enabled && m_layout && [m_layout vCanPaging];
}

- (void)refresh
{
    //[self setNeedsLayout];
    [self setNeedsDisplay];
    [self ProRedrawOS];
}

- (void)centerPage
{
    if(GLOBAL.g_view_mode == 3 || GLOBAL.g_view_mode == 6)
    {
        //[self resetZoomLevel];
    }
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet *allTouches = [event allTouches];
    NSUInteger cnt = [allTouches count];
    if( cnt == 1 )
    {
        UITouch *touch = [[allTouches allObjects] objectAtIndex:0];
        CGPoint point=[touch locationInView:m_canvas];
        if( [self OnSelTouchBegin:point] ) return;
        [self OnNoneTouchBegin:point:touch.timestamp];
    }
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSSet *allTouches = [event allTouches];
    NSUInteger cnt = [allTouches count];
    
    if( cnt == 1 )
    {
        UITouch *touch = [[allTouches allObjects] objectAtIndex:0];
        CGPoint point=[touch locationInView:m_canvas];
        if( [self OnSelTouchMove :point] ) return;
        [self OnNoneTouchMove:point:touch.timestamp];
    }
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    if (touch.tapCount == 2 && m_status == sta_docx_none) {
        //this is the double tap action
        [self OnDoubleTap:touch];
    }
    else
    {
        NSSet *allTouches = [event allTouches];
        NSUInteger cnt = [allTouches count];
        if( cnt == 1 )
        {
            UITouch *touch = [[allTouches allObjects] objectAtIndex:0];
            CGPoint point=[touch locationInView:m_canvas];
            if( [self OnSelTouchEnd:[touch locationInView:self]] ) return;
            [self OnNoneTouchEnd:point:touch.timestamp];
        }
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self touchesEnded:touches withEvent:event];
}

- (void)OnDoubleTap:(UITouch *)touch
{
    isDoubleTapping = YES;
    NSLog(@"double tap");
    
    if (doubleTapZoomMode > 0)
    {
        if (m_zoom > GLOBAL.g_tap_zoom_level)
        {
            [self defaultZoom:touch];
            self.pagingEnabled = GLOBAL.g_paging_enabled && m_layout && [m_layout vCanPaging];
        }
        else
        {
            self.pagingEnabled = NO;
            if (doubleTapZoomMode == 1) [self defaultZoom:touch];
        }
    }
    
    if (m_del) [m_del OnDoubleTapped:[touch locationInView:self.window].x :[touch locationInView:m_canvas].y];
    
    [self performSelector:@selector(delayedDoubleTapping) withObject:nil afterDelay:0.5];
    
}

- (void)delayedOnSingleTapping:(NSArray *)a
{
    if (!isDoubleTapping && a && m_del) {
        [m_del OnSingleTapped:[[a objectAtIndex:0] floatValue]:[[a objectAtIndex:1] floatValue]];
    }
}

- (void)delayedDoubleTapping
{
    isDoubleTapping = NO;
}

- (void)resetZoomLevel
{
    enum DOCXLAYOUT_STATUS save_status = m_status;
    [self zoomPageToScale:1.0 atPoint:CGPointMake([m_layout vw] / (m_scale_pix * 2), [m_layout vh] / (m_scale_pix * 2))];
    
    if ([imgAnnot isDescendantOfView:self]) {
        CGPoint center = self.center;
        center.x += self.contentOffset.x;
        center.y += self.contentOffset.y;
        imgAnnot.center = center;
    }
    m_status = save_status;
}

- (void)defaultZoom:(UITouch *)touch
{
    if (self.zoomScale == GLOBAL.g_tap_zoom_level && GLOBAL.g_zoom_step > 0) {
        GLOBAL.g_zoom_step *= -1;
    } else if (self.zoomScale <= self.minimumZoomScale && GLOBAL.g_zoom_step) {
        GLOBAL.g_zoom_step = 1;
    }
    if (self.zoomScale > GLOBAL.g_tap_zoom_level) m_zoom = 1;
    else m_zoom = (self.zoomScale + GLOBAL.g_zoom_step > GLOBAL.g_tap_zoom_level) ? GLOBAL.g_tap_zoom_level : self.zoomScale + GLOBAL.g_zoom_step;
    [self zoomPageToScale:m_zoom atPoint:[touch locationInView:m_canvas]];
}

-(void)OnSingleTap:(float)x :(float)y
{
    float nx = x * m_scale_pix;
    float ny = y * m_scale_pix;
    DOCXPos pos;
    [m_layout vGetPos :nx :ny :&pos];
    
    if( pos.pageno >= 0 )
    {
        DOCXVPage *vpage = [m_layout vGetPage:pos.pageno];
        if( !vpage )//shall not happen
        {
            if(m_del) [m_del OnSingleTapped:x:y];
            return;
        }
        DOCXPage *page = [vpage GetPage];
        if( !page ) return;
        float docx = [vpage GetDOCXX:nx + [m_layout docx]];
        float docy = [vpage GetDOCXY:ny + [m_layout docy]];
        NSString *url = [page getHLink:docx :docy];
        if(url)
        {
            if(m_del) [m_del OnOpenURL:url];
        }
        else
        {
            if(m_del) {
                NSArray *a = [NSArray arrayWithObjects:[NSNumber numberWithFloat:x], [NSNumber numberWithFloat:y], nil];
                [self performSelector:@selector(delayedOnSingleTapping:) withObject:a afterDelay:0.3];
            }
            m_status = sta_docx_none;
        }
    }
}

-(bool)OnSelTouchBegin:(CGPoint)point
{
    if( m_status != sta_docx_sel ) return false;
    m_tx = point.x * m_scale_pix;
    m_ty = point.y * m_scale_pix;
    [m_layout vGetPos : m_tx: m_ty :&m_sel_pos];
    
    m_sel = [[DOCXSel alloc] init:[m_doc page :m_sel_pos.pageno] :m_sel_pos.pageno];
    if( m_del )
        [m_del OnSelStart:point.x: point.y];
    return true;
}

-(bool)OnSelTouchMove:(CGPoint)point
{
    if( m_status != sta_docx_sel ) return false;
    DOCXVPage *vp = [m_layout vGetPage:m_sel_pos.pageno];
    float pdfx = [vp GetDOCXX :[m_layout docx] + point.x * m_scale_pix];
    float pdfy = [vp GetDOCXY :[m_layout docy] + point.y * m_scale_pix];
    [m_sel SetSel :m_sel_pos.pdfx :m_sel_pos.pdfy :pdfx :pdfy];
    
    [self ProRedrawOS];
    return true;
}

-(bool)OnSelTouchEnd:(CGPoint)point
{
    if( m_status != sta_docx_sel ) return false;
    
    if( m_del )
        [m_del OnSelEnd :m_tx/m_scale_pix :m_ty/m_scale_pix :point.x :point.y];
    return true;
}

-(void)OnNoneTouchBegin:(CGPoint)point :(NSTimeInterval)timeStamp
{
    m_tstamp = timeStamp;
    m_tstamp_tap = m_tstamp;
    m_tx = point.x * m_scale_pix;
    m_ty = point.y * m_scale_pix;
    m_px = m_tx;
    m_py = m_ty;
}

-(void)OnNoneTouchMove:(CGPoint)point :(NSTimeInterval)timeStamp
{
    NSTimeInterval del = timeStamp - m_tstamp;
    if( del > 0 )
    {
        float dx = point.x * m_scale_pix - m_px;
        float dy = point.y * m_scale_pix - m_py;
        float vx = dx/del;
        float vy = dy/del;
        dx = 0;
        dy = 0;
        if( vx > 50 || vx < -50 )
            dx = vx;
        if( vy > 50 || vy < -50 )
            dy = vy;
        else if( timeStamp - m_tstamp_tap > 1 )//long pressed
        {
            dx = point.x * m_scale_pix - m_tx;
            dy = point.y * m_scale_pix - m_ty;
            if( dx < 10 && dx > -10 && dy < 10 && dy > -10 )
            {
                m_status = sta_docx_none;
                if( m_del )
                    [m_del OnLongPressed :point.x :point.y];
            }
        }
    }
    m_px = point.x * m_scale_pix;
    m_py = point.y * m_scale_pix;
}

-(void)OnNoneTouchEnd:(CGPoint)point :(NSTimeInterval)timeStamp
{
    float dx = point.x - m_tx / m_scale_pix;
    float dy = point.y - m_ty / m_scale_pix;
    if( timeStamp - m_tstamp_tap < 0.20 )//single tap
    {
        bool single_tap = true;
        if( dx > 5 || dx < -5 )
            single_tap = false;
        if( dy > 5 || dy < -5 )
            single_tap = false;
        if( single_tap )
        {
            [self OnSingleTap :point.x :point.y];
        }
    }
    else
    {
        bool long_press = true;
        if( dx > 5 || dx < -5 )
            long_press = false;
        if( dy > 5 || dy < -5 )
            long_press = false;
        if( long_press )
        {
            if( m_del )
                [m_del OnLongPressed:point.x :point.y];
        }
    }
}
- (CGFloat)vGetScale
{
    return [[m_layout vGetPage:m_cur_page] scale];
}

- (CGFloat)vGetPixSize
{
    return m_scale_pix;
}

- (void)PDFSetGBColor:(int)color
{
    GLOBAL.g_readerview_bg_color = color;
    
    if (GLOBAL.g_readerview_bg_color != 0) {
        self.backgroundColor = UIColorFromRGB(color);
    }
}

- (void)setFirstPageCover:(BOOL)cover
{
    coverPage = cover;
}

- (void)setDoubleTapZoomMode:(int)mode
{
    doubleTapZoomMode = mode;
}

#pragma mark - VFind

-(bool)vFindStart:(NSString *)pat :(bool)match_case :(bool)whole_word
{
    if( !pat ) return false;
    [m_layout vFindStart:pat :match_case :whole_word];
    [self ProRedrawOS];
    return true;
}

-(void)vFind:(int)dir
{
    if( [m_layout vFind:dir] < 0 )//no more found.
    {
        if( m_del ) [m_del OnFound:false];
    }
    
    [self ProRedrawOS];
}
-(void)vFindEnd
{
    [m_layout vFindEnd];
    [self setNeedsDisplay];
}


-(void)vSelStart
{
    if( m_status == sta_docx_none )
    {
        m_status = sta_docx_sel;
        self.scrollEnabled = false;
    }
}

-(void)vSelEnd
{
    if( m_status == sta_docx_sel )
    {
        self.scrollEnabled = true;
        m_status = sta_docx_none;
        m_sel = nil;
    }
}

-(NSString *)vSelGetText
{
    if( m_status != sta_docx_sel || !m_sel ) return nil;
    return [m_sel GetSelString];
}

- (void)enableScroll
{
    self.scrollEnabled = true;
}

#pragma mark view Method
-(void)vGetPos:(DOCXPos *)pos
{
    [m_layout vGetPos:[m_layout vw]/2 :[m_layout vh]/2 :pos];
}

- (void)vGetPos:(DOCXPos *)pos x:(int)x y:(int)y
{
    [m_layout vGetPos:x * m_scale_pix :y * m_scale_pix :pos];
}

-(void)vGoto:(int)pageno
{
    [m_layout vGotoPage:pageno];
    CGPoint pt;
    pt.x = m_layout.docx/m_scale_pix;
    pt.y = m_layout.docy/m_scale_pix;
    self.contentOffset = pt;
    [self ProRedrawOS];
    [self setNeedsDisplay];
}

- (int)vGetCurrentPage
{
    return m_cur_page;
}

-(void)OnUncaughtException:(int)code :(NSString *)para
{}

@end
