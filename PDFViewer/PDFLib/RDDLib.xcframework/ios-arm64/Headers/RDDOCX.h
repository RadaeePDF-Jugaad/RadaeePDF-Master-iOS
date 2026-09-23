#pragma once
#import "RDComm.h"
#import <CoreGraphics/CGImage.h>
#import <CoreGraphics/CGBitmapContext.h>
#import <UIKit/UIKit.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _DOCX_DOC_* DOCX_DOC;
typedef struct _DOCX_PAGE_* DOCX_PAGE;
typedef struct _DOCX_PAGE_* DOCX_FINDER;
DOCX_DOC DOCX_Document_open(NSString * path, NSString * password, PDF_ERR * err);
DOCX_DOC DOCX_Document_openStream(id<PDFStream> str_obj, NSString * password, PDF_ERR * err);
void DOCX_Document_close(DOCX_DOC doc);
bool DOCX_Document_getPagesMaxSize(DOCX_DOC doc, PDF_SIZE * ret);
float DOCX_Document_getPageWidth(DOCX_DOC doc, int pageno);
float DOCX_Document_getPageHeight(DOCX_DOC doc, int pageno);
int DOCX_Document_getPageCount(DOCX_DOC doc);
bool DOCX_Document_exportPDF(DOCX_DOC doc, PDF_DOC pdf);
DOCX_PAGE DOCX_Document_getPage(DOCX_DOC doc, int pageno);
void DOCX_Page_renderPrepare(DOCX_PAGE page, PDF_DIB dib);
bool DOCX_Page_render(DOCX_PAGE page, PDF_DIB dib, float scale, int orgx, int orgy, int quality);
void DOCX_Page_renderCancel(DOCX_PAGE page);
bool DOCX_Page_renderIsFinished(DOCX_PAGE page);
void DOCX_Page_objsStart(DOCX_PAGE page);
NSString * DOCX_Page_getHLink(DOCX_PAGE page, float x, float y);
int DOCX_Page_objsGetCharCount(DOCX_PAGE page);
int DOCX_Page_objsGetCharIndex(DOCX_PAGE page, const PDF_POINT * pt);
NSString * DOCX_Page_objsGetString(DOCX_PAGE page, int from, int to);
int DOCX_Page_objsAlignWord(DOCX_PAGE page, int from, int dir);
bool DOCX_Page_objsGetCharRect(DOCX_PAGE page, int index, PDF_RECT * rect);
void DOCX_Page_close(DOCX_PAGE page);
DOCX_FINDER DOCX_Page_findOpen(DOCX_PAGE page, NSString * str, bool match_case, bool whole_word);
DOCX_FINDER DOCX_Page_findOpen2(DOCX_PAGE page, NSString * str, bool match_case, bool whole_word, bool skip_blanks);
int DOCX_Page_findGetCount(DOCX_FINDER finder);
int DOCX_Page_findGetFirstChar(DOCX_FINDER finder, int index);
int DOCX_Page_findGetEndChar(DOCX_FINDER finder, int index);
void DOCX_Page_findClose(DOCX_FINDER finder);


#ifdef __cplusplus
}
#endif
