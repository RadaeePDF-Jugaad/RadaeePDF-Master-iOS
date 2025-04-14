#import "DOCXSel.h"
#import "RDVGlobal.h"
#import "RDVCanvas.h"
#import "DOCXVPage.h"

@implementation DOCXSel
@synthesize pageno = m_pgno;
-(id)init:(DOCXPage *)page :(int)pgno
{
    if( self = [super init] )
    {
        m_page = page;
        m_pgno = pgno;
        m_index1 = -1;
        m_index2 = -1;
        m_ok = false;
    }
    return self;
}
-(void)dealloc
{
    [self Clear];
}
- (DOCXPage *)pdfpage {
    return m_page;
}
-(void)Clear
{
    if( m_page != NULL )
    {
        m_page = NULL;
    }
}
-(void)SetSel:(float)x1 :(float) y1 :(float) x2 :(float) y2
{
    if( !m_ok )
    {
        [m_page objsStart];
        m_ok = true;
    }
    m_index1 = [m_page objsGetCharIndex:x1 :y1];
    m_index2 = [m_page objsGetCharIndex:x2 :y2];
    if( m_index1 > m_index2 )
    {
        int tmp = m_index1;
        m_index1 = m_index2;
        m_index2 = tmp;
    }
    m_index1 = [m_page objsAlignWord:m_index1 :-1];
    m_index2 = [m_page objsAlignWord:m_index2 :1];
}

-(NSString *)GetSelString
{
    if( m_index1 < 0 || m_index2 < 0 || !m_ok ) return NULL;
    return [m_page objsString:m_index1 :m_index2];
}

-(void)drawOffScreen :(RDVCanvas *)canvas :(DOCXVPage *)page :(int)docx :(int)docy
{
    if( m_index1 < 0 || m_index2 < 0 || !m_ok ) return;
    PDF_RECT rect;
    PDF_RECT rect_word;
    PDF_RECT rect_draw;
    [m_page objsCharRect:m_index1 :&rect];
    rect_word = rect;
    float imul = 1.0/canvas.scale_pix;
    int tmp = m_index1 + 1;
    while( tmp <= m_index2 )
    {
        [m_page objsCharRect:tmp :&rect];
        float gap = (rect.bottom - rect.top)/2;
        if( rect_word.top == rect.top && rect_word.bottom == rect.bottom &&
           rect_word.right + gap > rect.left && rect_word.left - gap < rect.right )
        {
            if( rect_word.left > rect.left ) rect_word.left = rect.left;
            if( rect_word.right < rect.right ) rect_word.right = rect.right;
        }
        else
        {
            rect_draw.left = ([page GetVX:rect_word.left] - docx) * imul;
            rect_draw.top = ([page GetVY:rect_word.top] - docy) * imul;
            rect_draw.right = ([page GetVX:rect_word.right] - docx) * imul;
            rect_draw.bottom = ([page GetVY:rect_word.bottom] - docy) * imul;
            [canvas FillRect:CGRectMake(rect_draw.left, rect_draw.top,
                                        (rect_draw.right - rect_draw.left), (rect_draw.bottom - rect_draw.top)): GLOBAL.g_sel_color ];
            rect_word = rect;
        }
        tmp++;
    }
    rect_draw.left = ([page GetVX:rect_word.left] - docx) * imul;
    rect_draw.top = ([page GetVY:rect_word.top] - docy) * imul;
    rect_draw.right = ([page GetVX:rect_word.right] - docx) * imul;
    rect_draw.bottom = ([page GetVY:rect_word.bottom] - docy) * imul;
    [canvas FillRect:CGRectMake(rect_draw.left, rect_draw.top,
                                (rect_draw.right - rect_draw.left), (rect_draw.bottom - rect_draw.top)): GLOBAL.g_sel_color ];
}

-(void)DrawSel:(RDVCanvas *)canvas :(DOCXVPage *)page
{
    if( m_index1 < 0 || m_index2 < 0 || !m_ok ) return;
    PDF_RECT rect;
    PDF_RECT rect_word;
    PDF_RECT rect_draw;
    [m_page objsCharRect:m_index1 :&rect];
    rect_word = rect;
    float imul = 1.0/canvas.scale_pix;
    int tmp = m_index1 + 1;
    while( tmp <= m_index2 )
    {
        [m_page objsCharRect:tmp :&rect];
        float gap = (rect.bottom - rect.top)/2;
        if( rect_word.top == rect.top && rect_word.bottom == rect.bottom &&
           rect_word.right + gap > rect.left && rect_word.left - gap < rect.right )
        {
            if( rect_word.left > rect.left ) rect_word.left = rect.left;
            if( rect_word.right < rect.right ) rect_word.right = rect.right;
        }
        else
        {
            rect_draw.left = [page GetVX:rect_word.left] * imul;
            rect_draw.top = [page GetVY:rect_word.top] * imul;
            rect_draw.right = [page GetVX:rect_word.right] * imul;
            rect_draw.bottom = [page GetVY:rect_word.bottom] * imul;
            [canvas FillRect:CGRectMake(rect_draw.left, rect_draw.top,
                                        (rect_draw.right - rect_draw.left), (rect_draw.bottom - rect_draw.top)): GLOBAL.g_sel_color ];
            rect_word = rect;
        }
        tmp++;
    }
    rect_draw.left = [page GetVX:rect_word.left] * imul;
    rect_draw.top = [page GetVY:rect_word.top] * imul;
    rect_draw.right = [page GetVX:rect_word.right] * imul;
    rect_draw.bottom = [page GetVY:rect_word.bottom] * imul;
    [canvas FillRect:CGRectMake(rect_draw.left, rect_draw.top,
                                (rect_draw.right - rect_draw.left), (rect_draw.bottom - rect_draw.top)): GLOBAL.g_sel_color ];
}

-(void)Reset
{
    m_index1 = -1;
    m_index2 = -1;
}

- (int)startIdx
{
    return m_index1;
}
- (int)endIdx
{
    return m_index2;
}

@end
