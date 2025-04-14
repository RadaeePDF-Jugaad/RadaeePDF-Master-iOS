//
//  RDPDFDoc.m
//  PDFViewer
//
//  Created by Radaee on 12-9-18.
//  Copyright (c) 2012 Radaee. All rights reserved.
//

#import "PDFObjc.h"
int z_verbose = 0;
void z_error(char *m)
{
}
extern uint annotHighlightColor;
extern uint annotUnderlineColor;
extern uint annotStrikeoutColor;

@implementation RDPDFSign
@synthesize handle = m_sign;
-(id)init:(PDF_SIGN)sign
{
    if( self = [super init] )
    {
        m_sign = sign;
    }
    return self;
}

-(NSString *)issue
{
	return PDF_Sign_getIssue(m_sign);
}
-(NSString *)subject
{
	return PDF_Sign_getSubject(m_sign);
}
-(long)version
{
	return PDF_Sign_getVersion(m_sign);
}
-(NSString *)name
{
    return PDF_Sign_getName(m_sign);
}
-(NSString *)reason
{
    return PDF_Sign_getReason(m_sign);
}
-(NSString *)location
{
    return PDF_Sign_getLocation(m_sign);
}
-(NSString *)contact
{
    return PDF_Sign_getContact(m_sign);
}
-(NSString *)modTime
{
    return PDF_Sign_getModDT(m_sign);
}
-(const void*)content :(int *)len
{
	return PDF_Sign_getContent(m_sign, len);
}
@end

@implementation RDPDFEditNode
@synthesize handle = m_node;
-(id)init:(PDF_EDITNODE)node
{
    if( self = [super init] )
    {
        m_node = node;
    }
    return self;
}
+(void)setDefFont:(NSString *)fname
{
    PDF_EditNode_setDefFont(fname);
}
+(void)setDefCJKFont:(NSString *)fname
{
    PDF_EditNode_setDefCJKFont(fname);
}
+(bool)caret_is_end:(long long)pos
{
    return (pos&1) != 0;
}
+(bool)caret_is_vert:(long long)pos
{
    return (pos&2) != 0;
}
+(bool)caret_is_same:(long long)pos0 :(long long)pos1
{
    if (pos0 == pos1) return true;
    int ic0 = (int)((pos0 >> 16) & 65535);
    int ic1 = (int)((pos1 >> 16) & 65535);
    return ((pos0 >> 32) == (pos1 >> 32) && ic0 + 1 == ic1 && ![RDPDFEditNode caret_is_end:pos0] && [RDPDFEditNode caret_is_end:pos1]);
}
+(long long)caret_regular_end:(long long)pos
{
    if([RDPDFEditNode caret_is_end:pos])
    {
        int ic0 = ((int)((pos >> 16) & 65535)) + 1;
        int if0 = ((int)(pos & 65535)) & (~1);
        pos &= (~0xffffffffl);
        pos += (ic0 << 16) + if0;
    }
    return pos;
}
+(bool)caret_is_first:(long long)pos
{
    return ((pos >> 32) == 0 && ((pos >> 16) & 65535) == 0 && (pos & 1) == 0);
}
-(long long)caret_regular_start:(long long)pos
{
    if([RDPDFEditNode caret_is_end:pos])
    {
        pos &= (~1l);
        pos = [self getCharNext :pos];
    }
    return pos;
}

-(long long)getCharPos:(float)pdfx :(float)pdfy
{
    return PDF_EditNode_getCharPos(m_node, pdfx, pdfy);
}
-(long long)getCharPrev:(long long)pos
{
    return PDF_EditNode_getCharPrev(m_node, pos);
}
-(long long)getCharNext:(long long)pos
{
    return PDF_EditNode_getCharNext(m_node, pos);
}
-(PDF_RECT)getCharRect:(long long)pos
{
    PDF_RECT rect;
    if(!PDF_EditNode_getCharRect(m_node, pos, &rect))
    {
        rect.left = 0;
        rect.right = 0;
        rect.top = 0;
        rect.bottom = 0;
    }
    return rect;
}
-(void)charDelete:(long long)start :(long long)end
{
    PDF_EditNode_charDelete(m_node, start, end);
}
-(NSString *)charGetString:(long long)start :(long long)end
{
    return PDF_EditNode_charGetString(m_node, start, end);
}
-(void)charReturn:(long long)pos
{
    PDF_EditNode_charReturn(m_node, pos);
}
-(long long)charInsert:(long long)pos :(NSString *)sval
{
    return PDF_EditNode_charInsert(m_node, pos, sval);
}
-(int)getType
{
    return PDF_EditNode_getType(m_node);
}
-(PDF_RECT)getRect
{
    PDF_RECT rect;
    if(!PDF_EditNode_getRect(m_node, &rect))
    {
        rect.left = 0;
        rect.right = 0;
        rect.top = 0;
        rect.bottom = 0;
    }
    return rect;
}
-(void)setRect:(PDF_RECT)rect
{
    PDF_EditNode_setRect(m_node, &rect);
}
-(void)updateRect
{
    PDF_EditNode_updateRect(m_node);
}
-(void)delete
{
    PDF_EditNode_delete(m_node);
    m_node = NULL;
}
-(void)dealloc
{
    PDF_EditNode_destroy(m_node);
    m_node = NULL;
}
@end

@implementation RDPDFDIB
@synthesize handle = m_dib;
-(id)init
{
    if( self = [super init] )
    {
	    m_dib = NULL;
    }
    return self;
}

-(id)init:(int)width :(int)height
{
    if( self = [super init] )
    {
	    m_dib = Global_dibGet(NULL, width, height);
    }
    return self;
}
-(void)resize:(int)newWidth :(int)newHeight
{
    m_dib = Global_dibGet(m_dib, newWidth, newHeight);
}

-(void *)data
{
    return Global_dibGetData(m_dib);
}

-(int)width
{
    return Global_dibGetWidth(m_dib);
}

-(int)height
{
    return Global_dibGetHeight(m_dib);
}

-(void)dealloc
{
    PDF_DIB tmp_dib = m_dib;
    m_dib = NULL;
    Global_dibFree(tmp_dib);
}

-(void)erase:(int)color
{
	int *pix = (int *)Global_dibGetData(m_dib);
	int *pix_end = pix + Global_dibGetWidth(m_dib) * Global_dibGetHeight(m_dib);
	while(pix < pix_end) *pix++ = color;
}

-(CGImageRef)image
{
    if(!m_dib) return nil;
    void *pdata = Global_dibGetData(m_dib);
    int w = Global_dibGetWidth(m_dib);
    int h = Global_dibGetHeight(m_dib);
    CGDataProviderRef provider = CGDataProviderCreateWithData( NULL, pdata, w * h * 4, NULL );
    CGColorSpaceRef cs = CGColorSpaceCreateDeviceRGB();
    CGImageRef img = CGImageCreate( w, h, 8, 32, w<<2, cs,
                                   kCGBitmapByteOrder32Little|kCGImageAlphaNoneSkipFirst,
                                   provider, NULL, FALSE, kCGRenderingIntentDefault );
    CGColorSpaceRelease(cs);
    CGDataProviderRelease(provider);
    return img;
}
@end

@implementation RDPDFMatrix
@synthesize handle = m_mat;
-(id)init
{
    if( self = [super init] )
    {
	    m_mat = Matrix_createScale(1, 1, 0, 0);
    }
    return self;
}
-(id)init:(float)scalex :(float)scaley :(float)orgx :(float)orgy
{
    if( self = [super init] )
    {
	    m_mat = Matrix_createScale(scalex, scaley, orgx, orgy);
    }
    return self;
}
-(id)init:(float)xx :(float)yx :(float)xy :(float)yy :(float)x0 :(float)y0
{
    if( self = [super init] )
    {
	    m_mat = Matrix_create(xx, yx, xy, yy, x0, y0);
    }
    return self;
}
-(void)invert
{
	Matrix_invert( m_mat );
}
-(void)transformPath:(RDPDFPath *)path
{
	Matrix_transformPath( m_mat, path.handle );
}
-(void)transformInk:(RDPDFInk *)ink
{
	Matrix_transformInk( m_mat, ink.handle );
}
-(void)transformRect:(PDF_RECT *)rect
{
	Matrix_transformRect( m_mat, rect );
}
-(void)transformPoint:(PDF_POINT *)point
{
	Matrix_transformPoint( m_mat, point );
}
-(void)dealloc
{
    PDF_MATRIX tmp_mat = m_mat;
    m_mat = NULL;
    Matrix_destroy(tmp_mat);
}
@end

@implementation RDPDFObj
@synthesize handle = m_obj;
-(id)init:(PDF_OBJ)obj
{
    if( self = [super init] )
    {
	    m_obj = obj;
		m_ref = true;
    }
    return self;
}
-(id)init
{
    if( self = [super init] )
    {
	    m_obj = PDF_Obj_create();
		m_ref = false;
    }
    return self;
}

-(void)dealloc
{
	if (!m_ref)
	{
		PDF_Obj_destroy(m_obj);
		m_obj = NULL;
	}
}

-(int)getType
{
	return PDF_Obj_getType(m_obj);
}
-(int)getIntVal
{
	return PDF_Obj_getInt(m_obj);
}
-(bool)getBoolVal
{
	return PDF_Obj_getBoolean(m_obj);
}
-(float)getRealVal
{
	return PDF_Obj_getReal(m_obj);
}
-(PDF_OBJ_REF)getReferenceVal
{
	return PDF_Obj_getReference(m_obj);
}
-(NSString *)getNameVal
{
	const char *name = PDF_Obj_getName(m_obj);
	if(!name) return NULL;
	return [NSString stringWithUTF8String:name];
}
-(NSString *)getAsciiStringVal
{
	return PDF_Obj_getAsciiString(m_obj);
}
-(NSString *)getTextStringVal
{
	return PDF_Obj_getTextString(m_obj);
}
-(const unsigned char *)getHexStrngVal :(int *)plen
{
	if(!plen) return NULL;
	return PDF_Obj_getHexString(m_obj, plen);
}
-(void)setIntVal:(int)v
{
	PDF_Obj_setInt(m_obj, v);
}
-(void)setBoolVal:(bool)v
{
	PDF_Obj_setBoolean(m_obj, v);
}
-(void)setRealVal:(float)v
{
	PDF_Obj_setReal(m_obj, v);
}
-(void)setReferenceVal:(PDF_OBJ_REF)v
{
	PDF_Obj_setReference(m_obj, v);
}
-(void)setNameVal:(NSString *)v
{
	if(!v) return;
	PDF_Obj_setName(m_obj, [v UTF8String]);
}
-(void)setAsciiStringVal:(NSString *)v
{
	if(!v) return;
	PDF_Obj_setAsciiString(m_obj, [v UTF8String]);
}
-(void)setTextStringVal:(NSString *)v
{
	if(!v) return;
	PDF_Obj_setTextString(m_obj, [v UTF8String]);
}
-(void)setHexStringVal:(unsigned char *)v :(int)len
{
	if(!v) return;
	PDF_Obj_setHexString(m_obj, v, len);
}
-(void)setDictionary
{
	PDF_Obj_dictGetItemCount(m_obj);
}
-(int)dictGetItemCount
{
	return PDF_Obj_dictGetItemCount(m_obj);
}
-(NSString *)dictGetItemTag:(int)index
{
	const char *tag = PDF_Obj_dictGetItemName(m_obj, index);
	if(!tag) return NULL;
	return [NSString stringWithUTF8String:tag];
}
-(RDPDFObj *)dictGetItemByIndex:(int)index
{
	PDF_OBJ obj = PDF_Obj_dictGetItemByIndex(m_obj, index);
	if(!obj) return NULL;
	return [[RDPDFObj alloc] init:obj];
}
-(RDPDFObj *)dictGetItemByTag:(NSString *)tag
{
	if(!tag) return NULL;
	PDF_OBJ obj = PDF_Obj_dictGetItemByName(m_obj, [tag UTF8String]);
	if(!obj) return NULL;
	return [[RDPDFObj alloc] init:obj];
}
-(void)dictSetItem:(NSString *)tag
{
	if(!tag) return;
	PDF_Obj_dictSetItem(m_obj, [tag UTF8String]);
}
-(void)dictRemoveItem:(NSString *)tag
{
	if(!tag) return;
	PDF_Obj_dictRemoveItem(m_obj, [tag UTF8String]);
}
-(void)setArray
{
	PDF_Obj_arrayGetItemCount(m_obj);
}
-(int)arrayGetItemCount
{
	return PDF_Obj_arrayGetItemCount(m_obj);
}
-(RDPDFObj *)arrayGetItem:(int)index
{
	PDF_OBJ obj = PDF_Obj_arrayGetItem(m_obj, index);
	if(!obj) return NULL;
	return [[RDPDFObj alloc] init:obj];
}
-(void)arrayAppendItem
{
	PDF_Obj_arrayAppendItem(m_obj);
}
-(void)arrayInsertItem:(int)index
{
	PDF_Obj_arrayInsertItem(m_obj, index);
}
-(void)arrayRemoveItem:(int)index
{
	PDF_Obj_arrayRemoveItem(m_obj, index);
}
-(void)arrayClear
{
	PDF_Obj_arrayClear(m_obj);
}
@end

@implementation RDPDFOutline
@synthesize handle = m_handle;
-(id)init
{
    if( self = [super init] )
    {
	    m_doc = NULL;
		m_handle = NULL;
    }
    return self;
}
-(id)init:(PDF_DOC)doc :(PDF_OUTLINE)handle
{
    if( self = [super init] )
    {
	    m_doc = doc;
		m_handle = handle;
    }
    return self;
}
-(RDPDFOutline *)next
{
    PDF_OUTLINE outline = PDF_Document_getOutlineNext(m_doc, m_handle);
    if( !outline ) return NULL;
    return [[RDPDFOutline alloc] init:m_doc:outline];
}
-(RDPDFOutline *)child
{
    PDF_OUTLINE outline = PDF_Document_getOutlineChild(m_doc, m_handle);
    if( !outline ) return NULL;
    return [[RDPDFOutline alloc] init:m_doc:outline];
}
-(int)dest
{
    return PDF_Document_getOutlineDest(m_doc, m_handle);
}
-(NSString *)label
{
    return PDF_Document_getOutlineLabel(m_doc, m_handle);
}
-(NSString *)fileLink
{
    return PDF_Document_getOutlineFileLink(m_doc, m_handle);
}

-(NSString *)url
{
    return PDF_Document_getOutlineURI(m_doc, m_handle);
}
-(bool)removeFromDoc
{
	return PDF_Document_removeOutline( m_doc, m_handle );
}
-(bool)addNext:(NSString *)label :(int)pageno :(float)top
{
    return PDF_Document_addOutlineNext(m_doc, m_handle, [label UTF8String], pageno, top);
}
-(bool)addChild:(NSString *)label :(int)pageno :(float)top
{
    return PDF_Document_addOutlineChild(m_doc, m_handle, [label UTF8String], pageno, top);
}
@end

@implementation RDPDFDocFont
@synthesize handle = m_handle;
-(id)init
{
    if( self = [super init] )
    {
	    m_doc = NULL;
		m_handle = NULL;
    }
    return self;
}
-(id)init:(PDF_DOC)doc :(PDF_DOC_FONT)handle
{
    if( self = [super init] )
    {
	    m_doc = doc;
		m_handle = handle;
    }
    return self;
}
-(float)ascent
{
	return PDF_Document_getFontAscent(m_doc, m_handle );
}
-(float)descent
{
	return PDF_Document_getFontDescent(m_doc, m_handle );
}
@end

@implementation RDPDFDocGState
@synthesize handle = m_handle;
-(id)init
{
    if( self = [super init] )
    {
	    m_doc = NULL;
		m_handle = NULL;
    }
    return self;
}
-(id)init:(PDF_DOC)doc :(PDF_DOC_GSTATE)handle
{
    if( self = [super init] )
    {
	    m_doc = doc;
		m_handle = handle;
    }
    return self;
}
-(bool)setStrokeAlpha:(int)alpha
{
	return PDF_Document_setGStateStrokeAlpha( m_doc, m_handle, alpha );
}
-(bool)setFillAlpha:(int)alpha
{
	return PDF_Document_setGStateFillAlpha( m_doc, m_handle, alpha );
}

-(bool)setStrokeDash:(const float *)dash :(int)dash_cnt :(float)phase
{
	return PDF_Document_setGStateStrokeDash( m_doc, m_handle, dash, dash_cnt, phase );
}

-(bool)setBlendMode :(int)bmode
{
	return PDF_Document_setGStateBlendMode( m_doc, m_handle, bmode );
}

@end

@implementation RDPDFDocImage
@synthesize handle = m_handle;
-(id)init
{
    if( self = [super init] )
    {
	    m_doc = NULL;
		m_handle = NULL;
    }
    return self;
}
-(id)init:(PDF_DOC)doc :(PDF_DOC_IMAGE)handle
{
    if( self = [super init] )
    {
	    m_doc = doc;
		m_handle = handle;
    }
    return self;
}
@end

@implementation RDPDFDocForm
@synthesize handle = m_handle;
-(id)init
{
    if( self = [super init] )
    {
	    m_doc = NULL;
		m_handle = NULL;
    }
    return self;
}
-(id)init:(PDF_DOC)doc :(PDF_DOC_FORM)handle
{
    if( self = [super init] )
    {
	    m_doc = doc;
		m_handle = handle;
    }
    return self;
}
-(void)dealloc
{
	if(m_handle && m_doc)
	{
		PDF_Document_freeForm(m_doc, m_handle);
		m_handle = NULL;
		m_doc = NULL;
	}
}
-(PDF_PAGE_FONT)addResFont :(RDPDFDocFont *)font
{
	return PDF_Document_addFormResFont(m_doc, m_handle, font.handle);
}
-(PDF_PAGE_IMAGE)addResImage :(RDPDFDocImage *)img
{
	return PDF_Document_addFormResImage(m_doc, m_handle, img.handle);
}
-(PDF_PAGE_GSTATE)addResGState : (RDPDFDocGState *)gs
{
	return PDF_Document_addFormResGState(m_doc, m_handle, gs.handle);
}
-(PDF_PAGE_FORM)addResForm : (RDPDFDocForm *)form
{
	return PDF_Document_addFormResForm(m_doc, m_handle, form.handle);
}
-(void)setContent : (float)x : (float)y : (float)w : (float)h : (RDPDFPageContent *)content
{
	PDF_Document_setFormContent(m_doc, m_handle, x, y, w, h, content.handle);
}
-(void)setTransparency :(bool)isolate :(bool)knockout
{
	PDF_Document_setFormTransparency(m_doc, m_handle, isolate, knockout);
}
@end

@implementation RDPDFFinder
-(id)init
{
    if( self = [super init] )
    {
		m_handle = NULL;
    }
    return self;
}
-(id)init:(PDF_FINDER)handle
{
    if( self = [super init] )
    {
		m_handle = handle;
    }
    return self;
}
-(int)count
{
	return PDF_Page_findGetCount(m_handle);
}

-(int)objsIndex:(int)find_index
{
	return PDF_Page_findGetFirstChar( m_handle, find_index );
}
-(int)objsEnd:(int)find_index
{
	return PDF_Page_findGetEndChar( m_handle, find_index );
}
-(void)dealloc
{
    PDF_Page_findClose(m_handle);
    m_handle = NULL;
}
@end

@implementation RDPDFPath
@synthesize handle = m_handle;
-(id)init
{
    if( self = [super init] )
    {
		m_handle = Path_create();
    }
    return self;
}

-(id)init:(PDF_PATH)path
{
    if( self = [super init] )
    {
		m_handle = path;
    }
    return self;
}

-(void)moveTo:(float)x :(float)y
{
	Path_moveTo( m_handle, x, y );
}
-(void)lineTo:(float)x :(float)y
{
	Path_lineTo( m_handle, x, y );
}
-(void)CurveTo:(float)x1 :(float)y1 :(float)x2 :(float)y2 :(float)x3 :(float)y3
{
	Path_curveTo( m_handle, x1, y1, x2, y2, x3, y3 );
}
-(void)closePath
{
	Path_closePath(m_handle);
}
-(int)nodesCount
{
	return Path_getNodeCount(m_handle);
}
-(int)node:(int)index :(PDF_POINT *)pt
{
	return Path_getNode(m_handle, index, pt);
}
-(void)dealloc
{
    Path_destroy(m_handle);
    m_handle = NULL;
}
@end

@implementation RDPDFInk
@synthesize handle = m_handle;
-(id)init
{
    if( self = [super init] )
    {
		m_handle = NULL;
    }
    return self;
}
-(id)init:(float)line_width :(int)color
{
    if( self = [super init] )
    {
		m_handle = Ink_create(line_width, color);
    }
    return self;
}
-(void)onDown:(float)x :(float)y
{
	Ink_onDown(m_handle, x, y);
}
-(void)onMove:(float)x :(float)y
{
	Ink_onMove(m_handle, x, y);
}
-(void)onUp:(float)x :(float)y
{
	Ink_onUp(m_handle, x, y);
}
-(int)nodesCount
{
	return Ink_getNodeCount(m_handle);
}
-(int)node:(int)index :(PDF_POINT *)pt
{
	return Ink_getNode(m_handle, index, pt);
}
-(void)dealloc
{
    Ink_destroy(m_handle);
    m_handle = NULL;
}
@end

@implementation RDPDFPageContent
@synthesize handle = m_handle;
-(id)init
{
    if( self = [super init] )
    {
		m_handle = PDF_PageContent_create();
    }
    return self;
}
-(void)gsSave
{
	PDF_PageContent_gsSave( m_handle );
}
-(void)gsRestore
{
	PDF_PageContent_gsRestore( m_handle );
}
-(void)gsSet:(PDF_PAGE_GSTATE) gs
{
	PDF_PageContent_gsSet( m_handle, gs );
}
-(void)gsCatMatrix:(RDPDFMatrix *) mat
{
	PDF_PageContent_gsSetMatrix( m_handle, mat.handle );
}
-(void)textBegin
{
	PDF_PageContent_textBegin( m_handle );
}
-(void)textEnd
{
	PDF_PageContent_textEnd( m_handle );
}
-(void)drawImage:(PDF_PAGE_IMAGE) img
{
	PDF_PageContent_drawImage( m_handle, img );
}
-(void)drawForm:(PDF_PAGE_FORM) form
{
	PDF_PageContent_drawForm( m_handle, form );
}
-(void)drawText:(NSString *)text
{
	PDF_PageContent_drawText( m_handle, [text UTF8String] );
}
-(int)drawText:(NSString *)text :(int)align :(float)width
{
	return PDF_PageContent_drawText2(m_handle, [text UTF8String], align, width);
}
-(PDF_TEXT_RET)drawText:(NSString *)text :(int)align :(float)width :(int)max_lines
{
	int val = PDF_PageContent_drawText3(m_handle, [text UTF8String], align, width, max_lines);
	PDF_TEXT_RET ret;
	ret.num_unicodes = val & ((1<<20) - 1);
	ret.num_lines = val >> 20;
	return ret;
}
-(void)strokePath:(RDPDFPath *) path
{
	PDF_PageContent_strokePath( m_handle, path.handle );
}
-(void)fillPath:(RDPDFPath *)path :(bool) winding
{
	PDF_PageContent_fillPath( m_handle, path.handle, winding );
}
-(void)clipPath:(RDPDFPath *)path :(bool) winding
{
	PDF_PageContent_clipPath( m_handle, path.handle, winding );
}
-(void)setFillColor:(int) color
{
	PDF_PageContent_setFillColor( m_handle, color );
}
-(void)setStrokeColor:(int) color
{
	PDF_PageContent_setStrokeColor( m_handle, color );
}
-(void)setStrokeCap:(int) cap
{
	PDF_PageContent_setStrokeCap( m_handle, cap );
}
-(void)setStrokeJoin:(int) join
{
	PDF_PageContent_setStrokeJoin( m_handle, join );
}
-(void)setStrokeWidth:(float) w
{
	PDF_PageContent_setStrokeWidth( m_handle, w );
}
-(void)setStrokeMiter:(float) miter
{
	PDF_PageContent_setStrokeMiter( m_handle, miter );
}
-(void)setStrokeDash:(const float *)dash: (int)dash_cnt: (float)phase
{
	PDF_PageContent_setStrokeDash(m_handle, dash, dash_cnt, phase);
}
-(void)textSetCharSpace:(float) space
{
	PDF_PageContent_textSetCharSpace( m_handle, space );
}
-(void)textSetWordSpace:(float) space
{
	PDF_PageContent_textSetWordSpace( m_handle, space );
}
-(void)textSetLeading:(float) leading
{
	PDF_PageContent_textSetLeading( m_handle, leading );
}
-(void)textSetRise:(float) rise
{
	PDF_PageContent_textSetRise( m_handle, rise );
}
-(void)textSetHScale:(int) scale
{
	PDF_PageContent_textSetHScale( m_handle, scale );
}
-(void)textNextLine
{
	PDF_PageContent_textNextLine( m_handle );
}
-(void)textMove:(float) x :(float) y
{
	PDF_PageContent_textMove( m_handle, x, y );
}
-(void)textSetFont:(PDF_PAGE_FONT) font :(float) size
{
	PDF_PageContent_textSetFont( m_handle, font, size );
}
-(void)textSetRenderMode:(int) mode
{
	PDF_PageContent_textSetRenderMode( m_handle, mode );
}
-(void)dealloc
{
    PDF_PageContent_destroy(m_handle);
    m_handle = NULL;
}
@end

@implementation RDPDFAnnot
@synthesize handle = m_handle;
-(id)init
{
    if( self = [super init] )
    {
        m_page = NULL;
		m_handle = NULL;
    }
    return self;
}
-(id)init:(PDF_PAGE)page :(PDF_ANNOT)handle
{
    if( self = [super init] )
    {
	    m_page = page;
		m_handle = handle;
    }
    return self;
}
-(PDF_OBJ_REF)advanceGetRef
{
	return PDF_Page_advGetAnnotRef(m_page, m_handle);
}
-(void)advanceReload
{
	PDF_Page_advReloadAnnot(m_page, m_handle);
}
-(int)type
{
	return PDF_Page_getAnnotType( m_page, m_handle );
}
-(int)export :(unsigned char *)buf :(int)len
{
	return PDF_Page_exportAnnot(m_page, m_handle, buf, len);
}
-(int)signField :(RDPDFDocForm *)appearence :(NSString *)cert_file :(NSString *)pswd :(NSString *)name :(NSString *)reason :(NSString *)location :(NSString *)contact;
{
	return PDF_Page_signAnnotField(m_page, m_handle, [appearence handle], [cert_file UTF8String], [pswd UTF8String], [name UTF8String], [reason UTF8String], [location UTF8String], [contact UTF8String]);
}

-(int)fieldType
{
	return PDF_Page_getAnnotFieldType( m_page, m_handle );
}
-(int)fieldFlag
{
	return PDF_Page_getAnnotFieldFlag( m_page, m_handle );
}
-(NSString *)fieldName
{
	char buf[512];
	int len = PDF_Page_getAnnotFieldName( m_page, m_handle, buf, 511 );
	if( len <= 0 ) return NULL;
	return [NSString stringWithUTF8String:buf];
}
-(NSString *)fieldNameWithNO
{
	char buf[512];
	int len = PDF_Page_getAnnotFieldNameWithNO( m_page, m_handle, buf, 511 );
	if( len <= 0 ) return NULL;
	return [NSString stringWithUTF8String:buf];
}
-(NSString *)fieldFullName
{
	char buf[512];
	int len = PDF_Page_getAnnotFieldFullName( m_page, m_handle, buf, 511 );
	if( len <= 0 ) return NULL;
	return [NSString stringWithUTF8String:buf];
}
-(NSString *)fieldFullName2
{
	char buf[512];
	int len = PDF_Page_getAnnotFieldFullName2( m_page, m_handle, buf, 511 );
	if( len <= 0 ) return NULL;
	return [NSString stringWithUTF8String:buf];
}
-(bool)isLocked
{
	return PDF_Page_isAnnotLocked( m_page, m_handle );
}
-(void)setLocked:(bool)lock
{
    PDF_Page_setAnnotLock( m_page, m_handle, lock );
}
-(NSString *)getName
{
    return PDF_Page_getAnnotName(m_page, m_handle);
}
-(bool)setName:(NSString *)name
{
	if(!name) return false;
	return PDF_Page_setAnnotName(m_page, m_handle, [name UTF8String]);
}
-(bool)isReadonly
{
	return PDF_Page_isAnnotReadonly( m_page, m_handle );
}
-(void)setReadonly:(bool)readonly
{
	PDF_Page_setAnnotReadonly( m_page, m_handle, readonly );
}
-(bool)isHidden
{
	return PDF_Page_isAnnotHide( m_page, m_handle );
}
-(bool)setHidden:(bool)hide
{
	PDF_Page_setAnnotHide( m_page, m_handle, hide );
	return true;
}
-(bool)render:(RDPDFDIB *)dib :(int)back_color
{
	[dib erase:back_color];
	return PDF_Page_renderAnnot(m_page, m_handle, [dib handle]);
}
-(void)getRect:(PDF_RECT *)rect
{
	PDF_Page_getAnnotRect( m_page, m_handle, rect );
}
-(void)setRect:(const PDF_RECT *)rect
{
	PDF_Page_setAnnotRect( m_page, m_handle, rect );
}

-(NSString *)getModDate
{
	const char *sval = PDF_Page_getAnnotModifyDate(m_page, m_handle);
	if(!sval) return nil;
	return [NSString stringWithUTF8String:sval];
}

-(bool)setModDate:(NSString *)mdate
{
	return PDF_Page_setAnnotModifyDate(m_page, m_handle, [mdate UTF8String]);
}

-(int)getMarkupRects:(PDF_RECT *)rects :(int)cnt
{
	return PDF_Page_getAnnotMarkupRects(m_page, m_handle, rects, cnt);
}
-(int)getIndex
{
	int cnt = PDF_Page_getAnnotCount(m_page);
	int cur = 0;
	while( cur < cnt )
	{
		if( m_handle == PDF_Page_getAnnot(m_page, cur) )
			return cur;
		cur++;
	}
	return -1;
}
-(RDPDFPath *)getInkPath
{
	PDF_PATH path = PDF_Page_getAnnotInkPath( m_page, m_handle );
	if( !path ) return NULL;
	return [[RDPDFPath alloc] init: path];
}
-(bool)setInkPath:(RDPDFPath *)path
{
	return PDF_Page_setAnnotInkPath( m_page, m_handle, [path handle] );
}

-(RDPDFPath *)getPolygonPath
{
	PDF_PATH path = PDF_Page_getAnnotPolygonPath( m_page, m_handle );
	if( !path ) return NULL;
	return [[RDPDFPath alloc] init: path];
}
-(bool)setPolygonPath:(RDPDFPath *)path
{
	return PDF_Page_setAnnotPolygonPath( m_page, m_handle, [path handle] );
}

-(RDPDFPath *)getPolylinePath
{
	PDF_PATH path = PDF_Page_getAnnotPolylinePath( m_page, m_handle );
	if( !path ) return NULL;
	return [[RDPDFPath alloc] init: path];
}
-(bool)setPolylinePath:(RDPDFPath *)path
{
	return PDF_Page_setAnnotPolylinePath( m_page, m_handle, [path handle] );
}

-(int)getLineStyle
{
    return PDF_Page_getAnnotLineStyle(m_page, m_handle);
}

-(bool)setLineStyle:(int)style
{
    return PDF_Page_setAnnotLineStyle(m_page, m_handle, style);
}

-(PDF_POINT)getLinePoint:(int)idx
{
	PDF_POINT pt;
	pt.x = 0;
	pt.y = 0;
	PDF_Page_getAnnotLinePoint(m_page, m_handle, idx, &pt);
	return pt;
}

-(bool)setLinePoint:(float)x1 :(float)y1 :(float)x2 :(float)y2
{
	return PDF_Page_setAnnotLinePoint(m_page, m_handle, x1, y1, x2, y2);
}

-(int)getFillColor
{
	return PDF_Page_getAnnotFillColor( m_page, m_handle );
}
-(bool)setFillColor:(int)color
{
	return PDF_Page_setAnnotFillColor( m_page, m_handle, color );
}
-(int)getStrokeColor
{
	return PDF_Page_getAnnotStrokeColor( m_page, m_handle );
}
-(bool)setStrokeColor:(int)color
{
	return PDF_Page_setAnnotStrokeColor( m_page, m_handle, color );
}
-(float)getStrokeWidth
{
	return PDF_Page_getAnnotStrokeWidth( m_page, m_handle );
}
-(bool)setStrokeWidth:(float)width
{
	return PDF_Page_setAnnotStrokeWidth( m_page, m_handle, width );
}
-(int)getStrokeDash:(float*)dashs : (int)dashs_max
{
	return PDF_Page_getAnnotStrokeDash(m_page, m_handle, dashs, dashs_max);
}
-(bool)setStrokeDash:(float *)dash : (int)cnt
{
	return PDF_Page_setAnnotStrokeDash( m_page, m_handle, dash, cnt );
}
-(int)getIcon
{
	return PDF_Page_getAnnotIcon( m_page, m_handle );
}
-(bool)setIcon:(int)icon
{
	return PDF_Page_setAnnotIcon(m_page, m_handle, icon);
}
-(bool)setIcon2:(NSString *)icon_name :(RDPDFDocForm *)icon
{
	return PDF_Page_setAnnotIcon2(m_page, m_handle, [icon_name UTF8String], [icon handle]);
}

-(int)getDest
{
	return PDF_Page_getAnnotDest( m_page, m_handle );
}
-(NSString *)getURI
{
	return PDF_Page_getAnnotURI( m_page, m_handle );
}
-(NSString *)getJS
{
	return PDF_Page_getAnnotJS( m_page, m_handle );
}

-(NSString *)getAdditionalJS :(int)idx
{
	return PDF_Page_getAnnotAdditionalJS(m_page, m_handle, idx);
}

-(NSString *)get3D
{
	return PDF_Page_getAnnot3D( m_page, m_handle );
}
-(NSString *)getMovie
{
	return PDF_Page_getAnnotMovie( m_page, m_handle );
}
-(NSString *)getSound
{
	return PDF_Page_getAnnotSound( m_page, m_handle );
}
-(NSString *)getAttachment
{
	return PDF_Page_getAnnotAttachment( m_page, m_handle );
}
-(NSString *)getRendition
{
    return PDF_Page_getAnnotRendition( m_page, m_handle );
}
-(bool)get3DData:(NSString *)save_file
{
	return PDF_Page_getAnnot3DData( m_page, m_handle, [save_file UTF8String] );
}
-(bool)getMovieData:(NSString *)save_file
{
	return PDF_Page_getAnnotMovieData( m_page, m_handle, [save_file UTF8String] );
}
-(bool)getSoundData:(int *)paras :(NSString *)save_file
{
	return PDF_Page_getAnnotSoundData( m_page, m_handle, paras, [save_file UTF8String] );
}
-(bool)getAttachmentData:(NSString *)save_file
{
	return PDF_Page_getAnnotAttachmentData( m_page, m_handle, [save_file UTF8String] );
}


-(int)getRichMediaItemCount
{
	return PDF_Page_getAnnotRichMediaItemCount(m_page, m_handle);
}

-(int)getRichMediaItemActived
{
	return PDF_Page_getAnnotRichMediaItemActived(m_page, m_handle);
}

-(int)getRichMediaItemType:(int) idx
{
	return PDF_Page_getAnnotRichMediaItemType(m_page, m_handle, idx);
}

-(NSString *)getRichMediaItemAsset:(int) idx
{
	return PDF_Page_getAnnotRichMediaItemAsset(m_page, m_handle, idx);
}

-(NSString *)getRichMediaItemPara:(int) idx
{
	return PDF_Page_getAnnotRichMediaItemPara(m_page, m_handle, idx);
}

-(NSString *)getRichMediaItemSource:(int) idx
{
	return PDF_Page_getAnnotRichMediaItemSource(m_page, m_handle, idx);
}

-(bool)getRichMediaItemSourceData:(int) idx :(NSString *)save_path
{
	return PDF_Page_getAnnotRichMediaItemSourceData(m_page, m_handle, idx, save_path);
}

-(bool)getRichMediaData :(NSString *)asset :(NSString *)save_path
{
	return PDF_Page_getAnnotRichMediaData(m_page, m_handle, asset, save_path);
}
-(NSString*)getFileLink
{
	return PDF_Page_getAnnotFileLink(m_page, m_handle);
}
-(RDPDFAnnot *)getPopup
{
    PDF_ANNOT annot = PDF_Page_getAnnotPopup(m_page, m_handle);
    if(!annot) return nil;
    return [[RDPDFAnnot alloc] init:m_page :annot];
}

-(bool)getPopupOpen
{
	return PDF_Page_getAnnotPopupOpen(m_page, m_handle);
}

-(int)getReplyCount
{
    return PDF_Page_getAnnotReplyCount(m_page, m_handle);
}

-(RDPDFAnnot*)getReply :(int)idx
{
    PDF_ANNOT annot = PDF_Page_getAnnotReply(m_page, m_handle, idx);
    if(!annot) return nil;
    return [[RDPDFAnnot alloc] init:m_page :annot];
}

-(NSString *)getPopupSubject
{
    return PDF_Page_getAnnotPopupSubject( m_page, m_handle );
}
-(NSString *)getPopupText
{
    return PDF_Page_getAnnotPopupText( m_page, m_handle );
}
-(NSString *)getPopupLabel
{
    return PDF_Page_getAnnotPopupLabel( m_page, m_handle );
}
-(bool)setPopupOpen :(bool)open
{
	return PDF_Page_setAnnotPopupOpen( m_page, m_handle, open );
}
-(bool)setPopupSubject:(NSString *)val
{
	return PDF_Page_setAnnotPopupSubject( m_page, m_handle, [val UTF8String] );
}
-(bool)setPopupText:(NSString *)val
{
	return PDF_Page_setAnnotPopupText( m_page, m_handle, [val UTF8String] );
}
-(bool)setPopupLabel:(NSString *)val
{
	return PDF_Page_setAnnotPopupLabel( m_page, m_handle, [val UTF8String] );
}
-(int)getEditType
{
	return PDF_Page_getAnnotEditType( m_page, m_handle );
}
-(bool)getEditRect:(PDF_RECT *)rect
{
	return PDF_Page_getAnnotEditTextRect( m_page, m_handle, rect );
}
-(float)getEditTextSize
{
	return PDF_Page_getAnnotEditTextSize( m_page, m_handle );
}
-(bool)setEditTextSize:(float)fsize
{
	return PDF_Page_setAnnotEditTextSize( m_page, m_handle, fsize );
}
-(int)getEditTextAlign
{
	return PDF_Page_getAnnotEditTextAlign( m_page, m_handle );
}
-(bool)setEditTextAlign:(int)align
{
	return PDF_Page_setAnnotEditTextAlign( m_page, m_handle, align );
}
-(NSString *)getEditText
{
    return PDF_Page_getAnnotEditText( m_page, m_handle );
}

-(NSString *)getFieldJS:(int)idx
{
    return PDF_Page_getAnnotFieldJS( m_page, m_handle, idx );
}

-(bool)setEditText:(NSString *)val
{
	return PDF_Page_setAnnotEditText( m_page, m_handle, [val UTF8String] );
}
-(bool)setEditFont:(RDPDFDocFont *)font
{
	if(!font) return false;
	return PDF_Page_setAnnotEditFont( m_page, m_handle, font.handle );
}

-(int)getEditTextColor
{
	return PDF_Page_getAnnotEditTextColor( m_page, m_handle );
}
-(bool)setEditTextColor:(int)color
{
	return PDF_Page_setAnnotEditTextColor( m_page, m_handle, color );
}

-(int)getComboItemCount
{
	return PDF_Page_getAnnotComboItemCount( m_page, m_handle );
}
-(NSString *)getComboItem:(int)index
{
    return PDF_Page_getAnnotComboItem( m_page, m_handle, index );
}
-(NSString *)getComboItemVal:(int)index
{
    return PDF_Page_getAnnotComboItemVal( m_page, m_handle, index );
}
-(int)getComboSel
{
	return PDF_Page_getAnnotComboItemSel( m_page, m_handle );
}
-(bool)setComboSel:(int)index
{
	return PDF_Page_setAnnotComboItem( m_page, m_handle, index );
}
-(bool)isMultiSel
{
	return PDF_Page_isAnnotListMultiSel(m_page, m_handle);
}
-(int)getListItemCount
{
	return PDF_Page_getAnnotListItemCount( m_page, m_handle );
}
-(NSString *)getListItem:(int)index
{
    return PDF_Page_getAnnotListItem( m_page, m_handle, index );
}
-(NSString *)getListItemVal:(int)index
{
    return PDF_Page_getAnnotListItemVal( m_page, m_handle, index );
}
-(int)getListSels:(int *)sels :(int)sels_max
{
	return PDF_Page_getAnnotListSels( m_page, m_handle, sels, sels_max );
}
-(bool)setListSels:(const int *)sels :(int)sels_cnt
{
	return PDF_Page_setAnnotListSels( m_page, m_handle, sels, sels_cnt );
}
-(int)getCheckStatus
{
	return PDF_Page_getAnnotCheckStatus( m_page, m_handle );
}
-(bool)setCheckValue:(bool)check
{
	return PDF_Page_setAnnotCheckValue( m_page, m_handle, check );
}
-(bool)setRadio
{
	return PDF_Page_setAnnotRadio( m_page, m_handle );
}
-(bool)getReset
{
	return PDF_Page_getAnnotReset( m_page, m_handle );
}
-(bool)setReset
{
	return PDF_Page_setAnnotReset( m_page, m_handle );
}
-(NSString *)getSubmitTarget
{
    return PDF_Page_getAnnotSubmitTarget( m_page, m_handle );
}
-(NSString *)getSubmitPara
{
	char buf[1024];
	if( !PDF_Page_getAnnotSubmitPara( m_page, m_handle, buf, 1023 ) )
		return NULL;
	return [NSString stringWithUTF8String:buf];
}
-(bool)removeFromPage
{
	bool ret = PDF_Page_removeAnnot( m_page, m_handle );
	m_handle = NULL;
	m_page = NULL;
	return ret;
}
-(bool)flateFromPage
{
	bool ret = PDF_Page_flateAnnot( m_page, m_handle );
	m_handle = NULL;
	m_page = NULL;
	return ret;
}
-(int)getSignStatus
{
	return PDF_Page_getAnnotSignStatus( m_page, m_handle );
}
-(RDPDFSign *)getSign
{
	PDF_SIGN sign = PDF_Page_getAnnotSign(m_page, m_handle);
	if(!sign) return NULL;
	return [[RDPDFSign alloc] init:sign];
}
-(RDPDFObj*)getSignLock
{
	PDF_OBJ lock = PDF_Page_getAnnotSignLock(m_page, m_handle);
	if (!lock) return NULL;
	return [[RDPDFObj alloc] init:lock];
}

-(bool)setSignLock :(RDPDFObj*)obj
{
	return PDF_Page_setAnnotSignLock(m_page, m_handle, [obj handle]);
}

-(bool)MoveToPage:(RDPDFPage *)page :(const PDF_RECT *)rect
{
    return PDF_Page_moveAnnot(m_page, [page handle], m_handle, rect);
}

- (BOOL)canMoveAnnot
{
    int type = self.type;
    return (type != 2 && type != 9 && type != 10 && type != 11 && type != 12 && type != 20);
}

-(PDF_OBJ_REF)getRef
{
	return PDF_Page_getAnnotRef(m_page, m_handle);
}
@end

@implementation RDPDFPage
@synthesize handle = m_page;
-(id)init;
{
    if( self = [super init] )
    {
	    m_page = NULL;
    }
    return self;
}
-(id)init:(PDF_PAGE) hand
{
    if( self = [super init] )
    {
	    m_page = hand;
    }
    return self;
}
-(PDF_OBJ_REF)advanceGetRef
{
	return PDF_Page_advGetRef(m_page);
}
-(void)advanceReload
{
	PDF_Page_advReload(m_page);
}
-(bool)importAnnot:(const PDF_RECT *)rect :(const unsigned char *)dat :(int)dat_len
{
	return PDF_Page_importAnnot(m_page, rect, dat, dat_len);
}

-(bool)renderThumb:(RDPDFDIB *)dib
{
	return PDF_Page_renderThumb(m_page, [dib handle]);
}

-(void)renderPrepare:(RDPDFDIB *)dib
{
    PDF_Page_renderPrepare(m_page, [dib handle]);
}
-(bool)render:(RDPDFDIB *)dib :(RDPDFMatrix *)mat :(int)quality
{
    return PDF_Page_render(m_page, [dib handle], [mat handle], true, quality);
}
-(void)renderCancel
{
    return PDF_Page_renderCancel(m_page);
}
-(bool)renderIsFinished
{
    return PDF_Page_renderIsFinished(m_page);
}
-(float)reflowPrepare:(float)width :(float)scale
{
    return PDF_Page_reflowStart( m_page, width,  scale );
}
-(bool)reflow:(RDPDFDIB *)dib :(float)orgx :(float)orgy
{
    return PDF_Page_reflow( m_page, [dib handle], orgx, orgy );
}
-(int)getRotate
{
	return PDF_Page_getRotate(m_page);
}
-(bool)flatAnnots
{
	return PDF_Page_flate(m_page);
}

-(int)sign:(RDPDFDocForm *)appearence :(const PDF_RECT *)box :(NSString *)cert_file :(NSString *)pswd :(NSString *)name :(NSString *)reason :(NSString *)location :(NSString *)contact
{
	return PDF_Page_sign(m_page, [appearence handle], box, [cert_file UTF8String], [pswd UTF8String], [name UTF8String], [reason UTF8String], [location UTF8String], [contact UTF8String]);
}

-(void)objsStart:(bool)rtol
{
    PDF_Page_objsStart(m_page, rtol);
}
-(int)objsCount
{
    return PDF_Page_objsGetCharCount(m_page);
}
-(NSString *)objsString:(int)from :(int)to
{
    return PDF_Page_objsGetString(m_page, from, to);
}
-(int)objsAlignWord:(int)index :(int)dir
{
    return PDF_Page_objsAlignWord(m_page, index, dir);
}
-(void)objsCharRect:(int)index :(PDF_RECT *)rect
{
    PDF_Page_objsGetCharRect(m_page, index, rect);
}
-(int)objsGetCharIndex:(float)x :(float)y
{
    return PDF_Page_objsGetCharIndex(m_page, x, y);
}
-(bool)objsGetImageInfo:(int)index :(int *)info
{
	return PDF_Page_objsGetImageInfo(m_page, index, info);
}
-(bool)objsGetImageData:(int)index :(PDF_IMAGE_DATA *)data
{
	return PDF_Page_objsGetImageData(m_page, index, data);
}
+(void)objsFreeImageData:(PDF_IMAGE_DATA*)data
{
	PDF_ImageData_Free(data);
}
-(bool)objsSetImageData:(int)index :(unsigned char *)pixels :(int)w :(int)h :(int)stride :(bool)has_alpha :(bool)interpolate
{
	return PDF_Page_objsSetImage(m_page, index, pixels, w, h, stride, has_alpha, interpolate);
}
-(bool)objsSetImageJPEG:(int)index :(NSString *)path :(bool)interpolate
{
	return PDF_Page_objsSetImageJPEG(m_page, index, path, interpolate);
}
-(bool)objsSetImageJPX:(int)index :(NSString *)path :(bool)interpolate
{
	return PDF_Page_objsSetImageJPX(m_page, index, path, interpolate);
}

-(RDPDFFinder *)find:(NSString *)key :(bool)match_case :(bool)whole_word
{
	PDF_FINDER hand = PDF_Page_findOpen( m_page, [key UTF8String], match_case, whole_word );
	if( !hand ) return NULL;
	return [[RDPDFFinder alloc] init:hand];
}
-(RDPDFFinder *)find2:(NSString *)key :(bool)match_case :(bool)whole_word :(bool)skip_blanks
{
	PDF_FINDER hand = PDF_Page_findOpen2( m_page, [key UTF8String], match_case, whole_word, skip_blanks );
	if( !hand ) return NULL;
	return [[RDPDFFinder alloc] init:hand];
}
-(bool)objsRemove:(const int *)range :(int)range_cnt :(bool)reload
{
    return PDF_Page_objsRemove(m_page, range, range_cnt, reload);
}
-(int)annotCount
{
	return PDF_Page_getAnnotCount( m_page );
}
-(RDPDFAnnot *)annotAtIndex:(int)index
{
	PDF_ANNOT hand = PDF_Page_getAnnot( m_page, index );
	if( !hand ) return NULL;
	return [[RDPDFAnnot alloc] init:m_page:hand];
}
-(RDPDFAnnot *)annotAtPoint : (float)x : (float)y
{
	PDF_ANNOT hand = PDF_Page_getAnnotFromPoint( m_page, x, y );
	if( !hand ) return NULL;
	return [[RDPDFAnnot alloc] init:m_page:hand];
}
-(RDPDFAnnot *)annotByName:(NSString *)name
{
	if(!name) return NULL;
	PDF_ANNOT hand = PDF_Page_getAnnotByName(m_page, [name UTF8String]);
	if( !hand ) return NULL;
	return [[RDPDFAnnot alloc] init:m_page:hand];
}
-(bool)copyAnnot:(RDPDFAnnot *)annot :(const PDF_RECT *)rect
{
	return PDF_Page_copyAnnot( m_page, [annot handle], rect );
}
-(bool)addAnnot:(PDF_OBJ_REF)ref :(int)index
{
	return PDF_Page_addAnnot2(m_page, ref, index);
}

-(bool)addAnnotPopup:(RDPDFAnnot *)parent :(const PDF_RECT *)rect :(bool)open
{
	return PDF_Page_addAnnotPopup( m_page, [parent handle], rect, open);
}

-(bool)addAnnotMarkup : (int)index1 : (int)index2 : (int)type :(int) color
{
    return PDF_Page_addAnnotMarkup2(m_page, index1, index2, color, type);
}
-(bool)addAnnotInk:(RDPDFInk *)ink
{
	return PDF_Page_addAnnotInk2( m_page, ink.handle );
}
-(bool)addAnnotGoto :(const PDF_RECT *)rect :(int)dest :(float)top
{
	return PDF_Page_addAnnotGoto2( m_page, rect, dest, top );
}
-(bool)addAnnotURI :(NSString *)uri :(const PDF_RECT *)rect
{
	return PDF_Page_addAnnotURI2( m_page, rect, [uri UTF8String] );
}
-(bool)addAnnotLine :(const PDF_POINT *)pt1 :(const PDF_POINT *)pt2 :(int) style1 : (int) style2 : (float) width : (int) color : (int) icolor
{
	return PDF_Page_addAnnotLine2( m_page, pt1, pt2, style1, style2, width, color, icolor );
}
-(bool)addAnnotRect:(const PDF_RECT *)rect :(float) width :(int) color :(int) icolor
{
	return PDF_Page_addAnnotRect2( m_page, rect, width, color, icolor );
}
-(bool)addAnnotEllipse:(const PDF_RECT *)rect :(float) width :(int) color :(int) icolor
{
	return PDF_Page_addAnnotEllipse2( m_page, rect, width, color, icolor );
}
-(bool)addAnnotPolygon:(RDPDFPath *)path :(int) color :(int) fill_color :(float) width
{
	return PDF_Page_addAnnotPolygon(m_page, [path handle], color, fill_color, width);
}
-(bool)addAnnotPolyline:(RDPDFPath *)path :(int) style1 :(int) style2 :(int) color :(int) fill_color :(float) width
{
	return PDF_Page_addAnnotPolyline(m_page, [path handle], style1, style2, color, fill_color, width);
}
-(bool)addAnnotNote:(const PDF_POINT *)pt
{
	return PDF_Page_addAnnotText2( m_page, pt->x, pt->y );
}
-(bool)addAnnotAttachment:(NSString *)att :(int)icon :(const PDF_RECT *)rect
{
	return PDF_Page_addAnnotAttachment( m_page, [att UTF8String], icon, rect );
}
-(bool)addAnnotBitmap0:(RDPDFMatrix *)mat :(RDPDFDocImage *) dimage :(const PDF_RECT *) rect
{
	return PDF_Page_addAnnotBitmap( m_page, [mat handle], [dimage handle], rect );
}
-(bool)addAnnotBitmap:(RDPDFDocImage *) dimage :(const PDF_RECT *) rect
{
	return PDF_Page_addAnnotBitmap2( m_page, [dimage handle], rect );
}

-(bool)addAnnotRichMedia:(NSString *) path_player :(NSString *) path_content :(int) type :(RDPDFDocImage *) dimage :(const PDF_RECT *) rect
{
	return PDF_Page_addAnnotRichMedia( m_page, path_player, path_content, type, [dimage handle], rect );
}

-(bool)addAnnotStamp:(int)icon :(const PDF_RECT *)rect
{
	return PDF_Page_addAnnotStamp( m_page, rect, icon );
}
-(PDF_PAGE_FONT)addResFont:(RDPDFDocFont *)font
{
	return PDF_Page_addResFont( m_page, font.handle );
}
-(PDF_PAGE_IMAGE)addResImage:(RDPDFDocImage *)image
{
	return PDF_Page_addResImage( m_page, image.handle );
}
-(PDF_PAGE_GSTATE)addResGState:(RDPDFDocGState *)gstate
{
	return PDF_Page_addResGState( m_page, gstate.handle );
}
-(PDF_PAGE_FORM)addResForm:(RDPDFDocForm *)form
{
	return PDF_Page_addResForm( m_page, form.handle );
}
-(bool)addContent:(RDPDFPageContent *)content :(bool)flush
{
    return PDF_Page_addContent( m_page, content.handle, flush );
}
- (bool)addAnnotEditText:(const PDF_RECT *)rect
{
    return PDF_Page_addAnnotEditbox2(m_page, rect, 0xFF000000, 1, 0xFFFFFFFF, 10, 0xFF000000);
}

-(int)getPGEditorNodeCount
{
    return PDF_Page_getPGEditorNodeCount(m_page);
}
-(void)setPGEditorNodeModified:(bool)modified
{
    PDF_Page_setPGEditorModified(m_page, modified);
}
-(RDPDFEditNode *)getPGEditorNode:(int)index
{
    PDF_EDITNODE hand = PDF_Page_getPGEditorNode1(m_page, index);
    if(!hand) return nil;
    return [[RDPDFEditNode alloc] init:hand];
}
-(RDPDFEditNode *)getPGEditorNode:(float)pdfx :(float)pdfy
{
    PDF_EDITNODE hand = PDF_Page_getPGEditorNode2(m_page, pdfx, pdfy);
    if(!hand) return nil;
    return [[RDPDFEditNode alloc] init:hand];
}
-(bool)renderWithPGEditor:(RDPDFDIB *)dib :(RDPDFMatrix *)mat :(int)quality
{
    return PDF_Page_renderWithPGEditor(m_page, [dib handle], [mat handle], true, quality);
}
-(bool)updateWithPGEditor
{
    return PDF_Page_updateWithPGEditor(m_page);
}
-(bool)cancelWithPGEditor
{
    return PDF_Page_cancelWithPGEditor(m_page);
}

-(bool)addFieldButton:(const PDF_RECT *)rect :(NSString *)name :(NSString *)label :(RDPDFDocForm *)app
{
    return PDF_Page_addFieldButton(m_page, rect, name, label, [app handle]);
}
-(bool)addFieldCheck:(const PDF_RECT *)rect :(NSString *)name :(NSString *)val :(RDPDFDocForm *)app_on :(RDPDFDocForm *)app_off
{
    return PDF_Page_addFieldCheck(m_page, rect, name, val, [app_on handle], [app_off handle]);
}
-(bool)addFieldRadio:(const PDF_RECT *)rect :(NSString *)name :(NSString *)val :(RDPDFDocForm *)app_on :(RDPDFDocForm *)app_off
{
    return PDF_Page_addFieldRadio(m_page, rect, name, val, [app_on handle], [app_off handle]);
}
-(bool)addFieldCombo:(const PDF_RECT *)rect :(NSString *)name :(NSArray *)opts
{
    return PDF_Page_addFieldCombo(m_page, rect, name, opts);
}
-(bool)addFieldList:(const PDF_RECT *)rect :(NSString *)name :(NSArray *)opts :(bool)multi_sel
{
    return PDF_Page_addFieldList(m_page, rect, name, opts, multi_sel);
}
-(bool)addFieldEditbox:(const PDF_RECT *)rect :(NSString *)name :(bool)multi_line :(bool)password
{
    return PDF_Page_addFieldEditbox(m_page, rect, name, multi_line, password);
}
-(bool)addFieldSign:(const PDF_RECT *)rect :(NSString *)name
{
    return PDF_Page_addFieldSign(m_page, rect, name);
}
-(void)dealloc
{
    PDF_PAGE tmp_page = m_page;
    m_page = NULL;
    PDF_Page_close(tmp_page);
}
@end

@implementation RDPDFImportCtx
-(id)init
{
    if( self = [super init] )
    {
	    m_doc = NULL;
		m_handle = NULL;
    }
    return self;
}
-(id)init:(PDF_DOC)doc :(PDF_IMPORTCTX)handle
{
    if( self = [super init] )
    {
	    m_doc = doc;
		m_handle = handle;
    }
    return self;
}
-(bool)import:(int)src_no :(int)dst_no;
{
    return PDF_Document_importPage(m_doc, m_handle, src_no, dst_no );
}
-(bool)import2Page:(int)src_no : (int)dst_no :(const PDF_RECT*)rect
{
    return PDF_Document_importPage2Page(m_doc, m_handle, src_no, dst_no, rect);
}
-(void)importEnd
{
    PDF_Document_importEnd(m_doc, m_handle);
    m_doc = NULL;
    m_handle = NULL;
}
-(void)dealloc
{
    PDF_Document_importEnd(m_doc, m_handle);
    m_doc = NULL;
	m_handle = NULL;
}
@end

@implementation RDPDFDoc
@synthesize handle = m_doc;
-(id)init
{
    if( self = [super init] )
    {
        m_doc = NULL;
    }
    return self;
}
+(void)setOpenFlag:(int)flag
{
    PDF_Document_setOpenFlag(flag);
}
-(int)open:(NSString *)path : (NSString *)password
{
    PDF_ERR err;
    const char *cpath = [path UTF8String];
    if( !password )
        m_doc = PDF_Document_open(cpath, NULL, &err);
    else
    {
        const char *pwd = [password UTF8String];
        m_doc = PDF_Document_open(cpath, pwd, &err);
    }
    return err;
}
-(int)openMem:(void *)data : (int)data_size : (NSString *)password
{
    PDF_ERR err;
    if( !password )
        m_doc = PDF_Document_openMem(data, data_size, NULL, &err);
    else
    {
        const char *pwd = [password UTF8String];
        m_doc = PDF_Document_openMem(data, data_size, pwd, &err);
    }
    return err;
}

-(int)openStream:(id<PDFStream>)stream : (NSString *)password
{
    PDF_ERR err;
    if( !password )
        m_doc = PDF_Document_openStream(stream, NULL, &err);
    else
    {
        const char *pwd = [password UTF8String];
        m_doc = PDF_Document_openStream(stream, pwd, &err);
    }
    return err;
}

-(int)openWithCert:(NSString *)path :(NSString *)cert_file :(NSString *)password
{
    PDF_ERR err;
    m_doc = PDF_Document_openWithCert([path UTF8String], [cert_file UTF8String], [password UTF8String], &err);
    return err;
}

-(int)openMemWithCert:(void *)data :(int)data_size :(NSString *)cert_file :(NSString *)password
{
    PDF_ERR err;
    m_doc = PDF_Document_openMemWithCert(data, data_size, [cert_file UTF8String], [password UTF8String], &err);
    return err;
}

-(int)openStreamWithCert:(id<PDFStream>)stream :(NSString *)cert_file :(NSString *)password
{
    PDF_ERR err;
    m_doc = PDF_Document_openStreamWithCert(stream, [cert_file UTF8String], [password UTF8String], &err);
    return err;
}

-(int)getLinearizedStatus
{
    return PDF_Document_getLinearizedStatus(m_doc);
}

-(int)create:(NSString *)path
{
    PDF_ERR err;
    const char *cpath = [path UTF8String];
    m_doc = PDF_Document_create(cpath, &err);
    return err;
}
-(PDF_OBJ_REF)advanceGetRef
{
	return PDF_Document_advGetRef(m_doc);
}
-(void)advanceReload
{
	PDF_Document_advReload(m_doc);
}
-(PDF_OBJ_REF)advanceNewFlateStream:(const unsigned char *)source :(int)len
{
	return PDF_Document_advNewFlateStream(m_doc, source, len);
}
-(PDF_OBJ_REF)advanceNewRawStream:(const unsigned char *)source :(int)len
{
	return PDF_Document_advNewRawStream(m_doc, source, len);
}
-(PDF_OBJ_REF)advanceNewIndirectObj
{
	return PDF_Document_advNewIndirectObj(m_doc);
}
-(PDF_OBJ_REF)advanceNewIndirectObjAndCopy :(RDPDFObj *)obj
{
	if(!obj) return 0;
	return PDF_Document_advNewIndirectObjWithData(m_doc, [obj handle]);
}
-(RDPDFObj *)advanceGetObj:(PDF_OBJ_REF)ref
{
	PDF_OBJ obj = PDF_Document_advGetObj(m_doc, ref);
	if(!obj) return NULL;
	return [[RDPDFObj alloc] init:obj];
}

-(bool)setCache:(NSString *)path
{
	return PDF_Document_setCache( m_doc, [path UTF8String] );
}

-(bool)setPageRotate: (int)pageno : (int)degree
{
	return PDF_Document_setPageRotate( m_doc, pageno, degree);
}

-(bool)runJS:(NSString *)js :(id<PDFJSDelegate>)del
{
    return PDF_Document_runJS(m_doc, [js UTF8String], del);
}

-(void)setGenPDFA:(bool)gen
{
	return PDF_Document_setGenPDFA(m_doc, gen);
}

-(int)verifySign:(RDPDFSign *)sign
{
	return PDF_Document_verifySign(m_doc, [sign handle]);
}

-(bool)canSave
{
    return PDF_Document_canSave(m_doc);
}
-(bool)isEncrypted
{
    return PDF_Document_isEncrypted(m_doc);
}
-(int)getEmbedFileCount
{
	return PDF_Document_getEFCount(m_doc);
}
-(NSString *)getEmbedFileName:(int)idx
{
	return PDF_Document_getEFName(m_doc, idx);
}
-(NSString *)getEmbedFileDesc:(int)idx
{
	return PDF_Document_getEFDesc(m_doc, idx);
}
-(bool)delEmbedFile:(int)idx
{
	return PDF_Document_delEF(m_doc, idx);
}
-(bool)newEmbedFile:(NSString *)path
{
	return PDF_Document_newEF(m_doc, path);
}
-(int)getJSCount
{
	return PDF_Document_getJSCount(m_doc);
}
-(NSString *)getJSName:(int)idx
{
	return PDF_Document_getJSName(m_doc, idx);
}
-(NSString *)getJS:(int)idx
{
	return PDF_Document_getJS(m_doc, idx);
}
-(bool)getEmbedFileData:(int)idx :(NSString *)path
{
	return PDF_Document_getEFData(m_doc, idx, path);
}
-(NSString *)exportForm
{
	return PDF_Document_exportForm(m_doc);
}
-(NSString *)exportXFDF:(NSString *)href
{
	return PDF_Document_exportXFDF(m_doc, href);
}
-(bool)importXFDF:(NSString *)xfdf
{
	return PDF_Document_importXFDF(m_doc, xfdf);
}
-(NSString *)getXMP
{
	return PDF_Document_getXMP(m_doc);
}
-(bool)setXMP:(NSString *)xmp
{
    return PDF_Document_setXMP(m_doc,xmp);
}
-(bool)save
{
    return PDF_Document_save(m_doc);
}
-(bool)saveAs:(NSString *)dst :(bool)rem_sec
{
    const char *fdst = [dst UTF8String];
    return PDF_Document_saveAs(m_doc, fdst, rem_sec);
}

-(bool)optimizeAs:(NSString*)dst : (const unsigned char*)opts : (float)img_dpi
{
    const char *fdst = [dst UTF8String];
	return PDF_Document_optimizeAs(m_doc, fdst, opts, img_dpi);
}

-(bool)encryptAs:(NSString *)dst :(NSString *)upswd :(NSString *)opswd :(int)perm :(int)method :(unsigned char *)fid
{
    return PDF_Document_encryptAs(m_doc, dst, upswd, opswd, perm, method, fid);
}

-(NSString *)meta:(NSString *)tag
{
    return PDF_Document_getMeta(m_doc, [tag UTF8String]);
}

-(bool)setMeta:(NSString *)tag :(NSString *)val
{
    const char *stag = [tag UTF8String];
    return PDF_Document_setMeta(m_doc, stag, [val UTF8String]);
}

-(bool)PDFID:(unsigned char *)buf
{
	return PDF_Document_getID(m_doc, buf);
}

-(int)pageCount
{
    return PDF_Document_getPageCount(m_doc);
}
-(PDF_SIZE)getPagesMaxSize
{
	PDF_SIZE sz;
	PDF_Document_getPagesMaxSize(m_doc, &sz);
	return sz;
}

-(RDPDFPage *)page:(int) pageno
{
    PDF_PAGE hand = PDF_Document_getPage(m_doc, pageno);
    if( !hand ) return NULL;
    return [[RDPDFPage alloc] init:hand];
}
-(float)pageWidth:(int) pageno
{
    return PDF_Document_getPageWidth(m_doc, pageno);
}
-(float)pageHeight:(int) pageno
{
    return PDF_Document_getPageHeight(m_doc, pageno);
}
-(NSString *)pageLabel:(int) pageno
{
    return PDF_Document_getPageLabel(m_doc, pageno);
}

-(RDPDFOutline *)rootOutline
{
    PDF_OUTLINE hand = PDF_Document_getOutlineNext(m_doc, NULL);
	if( !hand ) return NULL;
	return [[RDPDFOutline alloc] init:m_doc:hand];
}

-(bool)newRootOutline: (NSString *)label :(int) pageno :(float) top
{
    return PDF_Document_newRootOutline(m_doc, [label UTF8String], pageno, top);
}

-(RDPDFDocFont *)newFontCID: (NSString *)name :(int) style
{
	PDF_DOC_FONT hand = PDF_Document_newFontCID( m_doc, [name UTF8String], style );
	if( !hand ) return NULL;
	return [[RDPDFDocFont alloc] init:m_doc:hand];
}

-(RDPDFDocGState *)newGState
{
	PDF_DOC_GSTATE hand = PDF_Document_newGState( m_doc );
	if( !hand ) return NULL;
	return [[RDPDFDocGState alloc] init:m_doc:hand];
}
-(RDPDFDocForm *)newForm
{
	PDF_DOC_FORM hand = PDF_Document_newForm( m_doc );
	if( !hand ) return NULL;
	return [[RDPDFDocForm alloc] init:m_doc:hand];
}

-(RDPDFPage *)newPage:(int) pageno :(float) w :(float) h
{
    PDF_PAGE hand = PDF_Document_newPage(m_doc, pageno, w, h);
    if( !hand ) return NULL;
    return [[RDPDFPage alloc] init:hand];
}

-(RDPDFImportCtx *)newImportCtx:(RDPDFDoc *)src_doc
{
	PDF_IMPORTCTX hand = PDF_Document_importStart( m_doc, [src_doc handle] );
	if( !hand ) return NULL;
    return [[RDPDFImportCtx alloc] init:m_doc:hand];
}

-(bool)movePage:(int)pageno1 :(int)pageno2
{
	return PDF_Document_movePage( m_doc, pageno1, pageno2 );
}

-(bool)removePage:(int)pageno
{
	return PDF_Document_removePage( m_doc, pageno );
}
-(RDPDFDocImage *)newImage:(CGImageRef)img :(bool)has_alpha :(bool)interpolate
{
	if(!img) return nil;
	PDF_DOC_IMAGE hand = PDF_Document_newImage( m_doc, img, has_alpha, interpolate );
	if( !hand ) return NULL;
    return [[RDPDFDocImage alloc] init:m_doc:hand];
}
-(RDPDFDocImage *)newImage2:(CGImageRef)img :(unsigned int)matte :(bool)interpolate
{
	if(!img) return nil;
	PDF_DOC_IMAGE hand = PDF_Document_newImage2( m_doc, img, matte, interpolate );
	if( !hand ) return NULL;
    return [[RDPDFDocImage alloc] init:m_doc:hand];
}

-(RDPDFDocImage *)newImageJPEG:(NSString *)path :(bool)interpolate
{
	PDF_DOC_IMAGE hand = PDF_Document_newImageJPEG( m_doc, [path UTF8String], interpolate );
	if( !hand ) return NULL;
    return [[RDPDFDocImage alloc] init:m_doc:hand];
}

-(RDPDFDocImage *)newImageJPX:(NSString *)path :(bool)interpolate
{
	PDF_DOC_IMAGE hand = PDF_Document_newImageJPX( m_doc, [path UTF8String], interpolate );
	if( !hand ) return NULL;
    return [[RDPDFDocImage alloc] init:m_doc:hand];
}
-(PDF_HTML_EXPORTER)thmExpStart:(NSString *)path
{
    return PDF_Document_htmExpStart(m_doc, path);
}
-(bool)htmExpPage:(PDF_HTML_EXPORTER)exp :(int)pageno
{
    return PDF_Document_htmExpPage(m_doc, exp, pageno);
}
-(void)htmExpEnd:(PDF_HTML_EXPORTER)exp
{
    PDF_Document_htmExpEnd(m_doc, exp);
}
-(void)htmExpBinEnd:(PDF_HTML_EXPORTER)exp
{
    PDF_Document_htmExpBinEnd(m_doc, exp);
}

-(void)dealloc
{
    PDF_Document_close(m_doc);
    m_doc = NULL;
}

@end






