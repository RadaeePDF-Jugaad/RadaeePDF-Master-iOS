#pragma once
#import "DOCXObjc.h"

@class RDVCanvas;
@class DOCXVPage;

@interface DOCXSel : NSObject
{
    DOCXPage *m_page;
    int m_pgno;
    int m_index1;
    int m_index2;
    bool m_ok;
}
@property(readonly) int pageno;
@property(strong, nonatomic) DOCXPage *pdfpage;
-(id)init:(DOCXPage *)page :(int)pgno;
-(void)Reset;
-(void)Clear;
-(void)SetSel:(float)x1 : (float)y1 : (float)x2 : (float)y2;
-(NSString *)GetSelString;
-(void)DrawSel:(RDVCanvas *)canvas :(DOCXVPage *)page;
-(void)drawOffScreen :(RDVCanvas *)canvas :(DOCXVPage *)page :(int)docx :(int)docy;
-(int)startIdx;
-(int)endIdx;
@end
