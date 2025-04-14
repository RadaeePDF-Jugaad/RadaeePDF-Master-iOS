//
//  ODCXObjc.h
//  PDFViewer
//
//  Created by Radaee Lou on 2020/8/31.
//

#pragma once
#import "RDComm.h"
#import "RDDOCX.h"


@interface DOCXFinder : NSObject
{
    DOCX_FINDER m_handle;
}
/**
 *    @brief    create an finder object.
 *            this method is not supplied for developers, but invoked inner.
 *
 *    @param    handle    FINDER handle.
 *
 */
-(id)init:(DOCX_FINDER)handle;
/**
 *    @brief    get found count.
 *
 *    @return    how many times found for special key string.
 */
-(int)count;
/**
 *    @brief get found location by index.
 *
 *    @param find_index    index value in range [0, DOCXFinder.count - 1].
 *
 *    @return    the index value in range [0, RDPDFPage.objsCount - 1].
 *
 */
-(int)objsIndex:(int)find_index;
-(int)objsEnd:(int)find_index;
@end

@class RDPDFDIB;
@interface DOCXPage : NSObject
{
    DOCX_PAGE m_page;
}
@property (readonly) DOCX_PAGE handle;
-(void)renderPrepare:(RDPDFDIB *)dib;
-(bool)render:(RDPDFDIB *)dib :(float)scale :(int)orgx :(int)orgy :(int)quality;
-(void)renderCancel;
-(void)objsStart;
-(int)objsCount;
-(int)objsAlignWord:(int)from :(int)dir;
-(bool)objsCharRect:(int)index :(PDF_RECT *)rect;
-(NSString *)objsString:(int)from :(int)to;
-(int)objsGetCharIndex:(const PDF_POINT *)pt;
-(int)objsGetCharIndex:(float)x :(float)y;
-(NSString *)getHLink:(float) x :(float) y;
-(DOCXFinder *)find:(NSString *)str :(bool)match_case :(bool)whole_word;
-(DOCXFinder *)find:(NSString *)str :(bool)match_case :(bool)whole_word :(bool)skip_blank;
@end

@interface DOCXDoc : NSObject
{
    DOCX_DOC m_doc;
}
-(int)open:(NSString *)path :(NSString *)pswd;
-(int)openStream:(id<PDFStream>)stream :(NSString *)pswd;
-(PDF_SIZE)getPagesMaxSize;
-(DOCXPage *)page:(int) pageno;
-(float)pageWidth:(int) pageno;
-(float)pageHeight:(int) pageno;
-(int)pageCount;
-(bool)exportPDF:(RDPDFDoc *)pdf;
@end
