#pragma once
#import "DOCXObjc.h"

#define DRAW_ALL 0

@class RDVEvent;
@class DOCXVPage;
@class RDVCanvas;

@interface DOCXVFinder : NSObject
{
    NSString *m_str;
    bool m_case;
    bool m_whole;
    int m_page_no;
    int m_page_find_index;
    int m_page_find_cnt;
    DOCXPage *m_page;
    DOCXDoc *m_doc;
    
    DOCXFinder *m_finder;
    
    int m_dir;
    bool is_cancel;
    bool is_notified;
    bool is_waitting;
    RDVEvent *m_eve;
}
-(void)find_start:(DOCXDoc *)doc :(int)page_start :(NSString *)str :(bool)match_case :(bool) whole;
-(int)find_prepare:(int) dir;
-(int)find;
-(bool)find_get_pos:(PDF_RECT *)rect;//get current found's bound.
-(void)find_draw_all:(RDVCanvas *)canvas :(DOCXVPage *)page;//draw all occurrences found
-(int)find_get_page;//get current found's page NO
-(void)find_end;
-(void)drawOffScreen :(RDVCanvas *)canvas :(DOCXVPage *)page :(int)docx :(int)docy;
@end
