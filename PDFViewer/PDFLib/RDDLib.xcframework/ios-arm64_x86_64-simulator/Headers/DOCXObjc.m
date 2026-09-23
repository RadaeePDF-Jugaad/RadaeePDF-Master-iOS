//
//  DOCXObjc.m
//  PDFViewer
//
//  Created by Radaee Lou on 2020/8/31.
//
#import "DOCXObjc.h"
#import "PDFObjc.h"
#import <Foundation/Foundation.h>
@implementation DOCXFinder
-(id)init:(DOCX_FINDER)handle
{
    self = [super init];
    if(self)
    {
        m_handle = handle;
    }
    return self;
}
-(void)dealloc
{
    DOCX_Page_findClose(m_handle);
    m_handle = NULL;
}
-(int)count
{
    return DOCX_Page_findGetCount(m_handle);
}
-(int)objsIndex:(int)find_index
{
    return DOCX_Page_findGetFirstChar(m_handle, find_index);
}
-(int)objsEnd:(int)find_index
{
    return DOCX_Page_findGetEndChar(m_handle, find_index);
}
@end

@implementation DOCXPage
@synthesize handle = m_page;
-(id)init:(DOCX_PAGE)page
{
    self = [super init];
    if(self)
    {
        m_page = page;
    }
    return self;
}
-(void)dealloc
{
    DOCX_Page_close(m_page);
    m_page = NULL;
}
-(void)renderPrepare:(RDPDFDIB *)dib
{
    DOCX_Page_renderPrepare(m_page, [dib handle]);
}
-(bool)render:(RDPDFDIB *)dib :(float)scale :(int)orgx :(int)orgy :(int)quality
{
    return DOCX_Page_render(m_page, [dib handle], scale, orgx, orgy, quality);
}
-(void)renderCancel
{
    DOCX_Page_renderCancel(m_page);
}
-(void)objsStart
{
    DOCX_Page_objsStart(m_page);
}
-(int)objsCount
{
    return DOCX_Page_objsGetCharCount(m_page);
}
-(int)objsAlignWord:(int)from :(int)dir
{
    return DOCX_Page_objsAlignWord(m_page, from, dir);
}
-(bool)objsCharRect:(int)index :(PDF_RECT *)rect
{
    return DOCX_Page_objsGetCharRect(m_page, index, rect);
}
-(NSString *)objsString:(int)from :(int)to
{
    return DOCX_Page_objsGetString(m_page, from, to);
}
-(int)objsGetCharIndex:(const PDF_POINT *)pt
{
    return DOCX_Page_objsGetCharIndex(m_page, pt);
}
-(int)objsGetCharIndex:(float)x :(float)y
{
    PDF_POINT pt;
    pt.x = x;
    pt.y = y;
    return DOCX_Page_objsGetCharIndex(m_page, &pt);
}
-(NSString *)getHLink:(float) x :(float) y
{
    return DOCX_Page_getHLink(m_page, x, y);
}
-(DOCXFinder *)find:(NSString *)str :(bool)match_case :(bool)whole_word
{
    DOCX_FINDER hand = DOCX_Page_findOpen(m_page, str, match_case, whole_word);
    if(!hand) return nil;
    return [[DOCXFinder alloc] init:hand];
}
-(DOCXFinder *)find:(NSString *)str :(bool)match_case :(bool)whole_word :(bool)skip_blank
{
    DOCX_FINDER hand = DOCX_Page_findOpen2(m_page, str, match_case, whole_word, skip_blank);
    if(!hand) return nil;
    return [[DOCXFinder alloc] init:hand];
}
@end

@implementation DOCXDoc
-(id)init
{
    self = [super init];
    if(self)
    {
        m_doc = NULL;
    }
    return self;
}
-(int)open:(NSString *)path :(NSString *)pswd
{
    PDF_ERR err;
    m_doc = DOCX_Document_open(path, pswd, &err);
    return err;
}
-(int)openStream:(id<PDFStream>)stream :(NSString *)pswd
{
    PDF_ERR err;
    m_doc = DOCX_Document_openStream(stream, pswd, &err);
    return err;
}
-(void)dealloc
{
    DOCX_Document_close(m_doc);
    m_doc = NULL;
}
-(PDF_SIZE)getPagesMaxSize
{
    PDF_SIZE sz;
    DOCX_Document_getPagesMaxSize(m_doc, &sz);
    return sz;
}
-(DOCXPage *)page:(int) pageno
{
    DOCX_PAGE hand = DOCX_Document_getPage(m_doc, pageno);
    if( !hand ) return NULL;
    return [[DOCXPage alloc] init:hand];
}
-(float)pageWidth:(int) pageno
{
    return DOCX_Document_getPageWidth(m_doc, pageno);
}
-(float)pageHeight:(int) pageno
{
    return DOCX_Document_getPageHeight(m_doc, pageno);
}
-(int)pageCount
{
    return DOCX_Document_getPageCount(m_doc);
}
-(bool)exportPDF:(RDPDFDoc *)pdf
{
    return DOCX_Document_exportPDF(m_doc, [pdf handle]);
}
@end
