#pragma once
#import <CoreGraphics/CGImage.h>
#import <CoreGraphics/CGBitmapContext.h>
#import <UIKit/UIKit.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _PDF_DIB* PDF_DIB;
typedef struct _PDF_MATRIX* PDF_MATRIX;
typedef struct _PDF_PATH* PDF_PATH;
typedef struct _PDF_INK* PDF_INK;
typedef struct _PDF_DOC_* PDF_DOC;

typedef enum
{
    err_ok = 0,
    err_open = 1,
    err_password = 2,
    err_encrypt = 3,
    err_bad_file = 4,
}PDF_ERR;
typedef struct
{
    float cx;
    float cy;
}PDF_SIZE;
typedef struct
{
    float x;
    float y;
}PDF_POINT;
typedef struct
{
    float left;
    float top;
    float right;
    float bottom;
}PDF_RECT;


NSString *Global_getVersion();
int Global_active(NSString *serial);
    

/**

 *	@brief	Load font file.

 *	@param 	index 	Font file index.
 *	@param 	path 	Font path in SandBox.
 */
void Global_loadStdFont(int index, const char* path);
/**
 *	@brief	Save system font to a file.
 *
 *	@param 	fname 	font name from ios system, for example: Arial
 *	@param 	save_file 	full path name that save the font.
 *
 *	@return	true or false
 */
bool Global_SaveFont(const char* fname, const char* save_file);
/**
 *	@brief	Unload font file.
 *
 *	@param 	index 	font file index.
 */
void Global_unloadStdFont(int index);
/**

 *	@brief	load cmaps data. cmaps is code mapping struct.
 *
 *	@param 	cmaps 	full path of cmaps
 *	@param 	umaps 	full path of umaps

 */
void Global_setCMapsPath(const char* cmaps, const char* umaps);
bool Global_setCMYKProfile(const char* path);

/**
 *	@brief	create font list
 */
void Global_fontfileListStart(void);
/**
 *	@brief	add font file to list.
 *
 *	@param 	font_file 	full path of font file.
 */
void Global_fontfileListAdd(const char* font_file);
/**
 *	@brief		submit font list to PDF library.
 */
void Global_fontfileListEnd(void);
/**
 *	@brief	Set default font. the default font may be used when PDF has font not embed.
 this function valid after Global_fontfileListEnd() invoked.
 *
 *	@param 	collection 	may be: null, "GB1", "CNS1", "Japan1", "Korea1"
 *	@param 	font_name 	font name exist in font list.
 *	@param 	fixed 	set for fixed font?
 *
 *	@return	true or false
 */
bool Global_setDefaultFont(const char* collection, const char* font_name, bool fixed);

bool Global_fontfileMapping(const char* map_name, const char* name);

/**
 *	@brief	Set annot font type
 *
 *	@param 	font_name 	full path of font file.
 *
 *	@return	true or false
 */
bool Global_setAnnotFont(const char* font_name);
/**
 *	@brief	set annot transparency
 *
 *	@param 	color 	RGB color.e.g.0x200040FF
 */
void Global_setAnnotTransparency(int color);
/**
 *	@brief Get face count.
           this function valid after Global_fontfileListEnd() invoked.
 *
 *	@return	face count
 */
int Global_getFaceCount(void);
/**
 *	@brief	get face name by index.
            this function valid after Global_fontfileListEnd() invoked.
 *
 *	@param 	index 	0 based index, range : [0, Global_getFaceCount()-1].
 *
 *	@return	face name.
 */
const char* Global_getFaceName(int index);
/**
 *	@brief	alloc or realloc DIB object.
 *
 *	@param 	dib 	NULL for alloc, otherwise, realloc object.
 *	@param 	width 	width of DIB
 *	@param 	height 	height of DIB
 *
 *	@return	DIB object.
 */
PDF_DIB Global_dibGet(PDF_DIB dib, int width, int height);
/**
 *	@brief	Get dib data,return pointer of dib object
 *
 *	@param 	dib Dib object
 */
void* Global_dibGetData(PDF_DIB dib);
/**
 *	@brief	Get dib object's width
 *
 *	@param 	dib DIB object
 *
 *	@return	DIB object's width
 */
int Global_dibGetWidth(PDF_DIB dib);
/**
 *	@brief	Get dib object's height
 *
 *	@param 	dib DIB object
 *
 *	@return	DIB object's height
 */
int Global_dibGetHeight(PDF_DIB dib);
/**
 *	@brief	delete DIB object
 *
 *	@param 	dib    DIB object
 */
void Global_dibFree(PDF_DIB dib);
/**
 *	@brief	map PDF Point to DIB point.
 *
 *	@param 	matrix 	Matrix object that passed to Page_Render.
 *	@param 	ppoint 	Point in PDF coordinate system.
 *	@param 	dpoint 	output value: Point in DIB coordinate system.
 */
void Global_toDIBPoint(PDF_MATRIX matrix, const PDF_POINT* ppoint, PDF_POINT* dpoint);
/**
 *	@brief	map DIB Point to PDF point.
 *
 *	@param 	matrix 	Matrix object that passed to Page_Render.
 *	@param 	dpoint 	Point in DIB coordinate system.
 *	@param 	ppoint 	output value: Point in PDF coordinate system.
 */
void Global_toPDFPoint(PDF_MATRIX matrix, const PDF_POINT* dpoint, PDF_POINT* ppoint);
/**
 *	@brief	map PDF rect to DIB rect.
 *
 *	@param 	matrix 	Matrix object that passed to Page_Render.
 *	@param 	prect 	Rect in PDF coordinate system.
 *	@param 	drect 	output value: Rect in DIB coordinate system.
 */
void Global_toDIBRect(PDF_MATRIX matrix, const PDF_RECT* prect, PDF_RECT* drect);
/**
 *	@brief	map DIB Rect to PDF Rect.
 *
 *	@param 	matrix 	Matrix object that passed to Page_Render.
 *	@param 	drect 	Rect in DIB coordinate system.
 *	@param 	prect 	output value: Rect in PDF coordinate system.
 */
void Global_toPDFRect(PDF_MATRIX matrix, const PDF_RECT* drect, PDF_RECT* prect);
bool Global_drawAnnotIcon(int annot_type, int icon, PDF_DIB dib);
/**
 *	@brief	create a Matrix object
 *
 *	@param 	xx 	x scale value
 *	@param 	yx 	yx-
 *	@param 	xy 	xy-
 *	@param 	yy 	y scale value
 *	@param 	x0 	x origin
 *	@param 	y0 	y origin
 *
 *	@return	Matrix object
 */
PDF_MATRIX Matrix_create(float xx, float yx, float xy, float yy, float x0, float y0);
/**
 *	@brief	create a Matrix object for scale values.
 *
 *	@param 	scalex 	x scale value
 *	@param 	scaley 	y scale value
 *	@param 	x0 	x origin
 *	@param 	y0 	y origin
 *
 *	@return	Matrix object
 */
PDF_MATRIX Matrix_createScale(float scalex, float scaley, float x0, float y0);
void Matrix_invert(PDF_MATRIX matrix);
void Matrix_transformPath(PDF_MATRIX matrix, PDF_PATH path);
void Matrix_transformInk(PDF_MATRIX matrix, PDF_INK ink);
void Matrix_transformRect(PDF_MATRIX matrix, PDF_RECT* rect);
void Matrix_transformPoint(PDF_MATRIX matrix, PDF_POINT* point);
/**
 *	@brief	free Matrix object
 *
 *	@param 	matrix 	matrix	Matrix object returned from Matrix_create or Matrix_createScale
 */
void Matrix_destroy(PDF_MATRIX matrix);
/**
 *	@brief	create ink object for hand-writing
 *
 *	@param 	line_w 	line width
 *	@param 	color 	RGB value for ink color
 *
 *	@return	Ink object
 */
PDF_INK Ink_create(float line_w, int color);
/**
 *	@brief	destroy Ink object
 *
 *	@param 	ink 	Ink object returned from Ink_create
 */
void Ink_destroy(PDF_INK ink);
/**
 *	@brief	invoked when touch-down.
 *
 *	@param 	ink Ink object returned from Ink_create
 *	@param 	x 	x position
 *	@param 	y 	y position
 */
void Ink_onDown(PDF_INK ink, float x, float y);
/**
 *	@brief	invoked when touch-moving.
 *
 *	@param 	ink 	Ink object returned from Ink_create
 *	@param 	x 	x positon
 *	@param 	y 	y position
 */
void Ink_onMove(PDF_INK ink, float x, float y);
/**
 *	@brief	invoked when touch-up.
 *
 *	@param 	ink 	Ink object returned from Ink_create
 *	@param 	x 	x position
 *	@param 	y 	y position
 */
void Ink_onUp(PDF_INK ink, float x, float y);
/**
 *	@brief	get node count for ink.
 *
 *	@param 	ink 	Ink object returned from Ink_create
 *
 *	@return	nodes count
 */
int Ink_getNodeCount(PDF_INK ink);
/**
 *	@brief	get node by index
 *
 *	@param 	hand 	Ink object returned from Ink_create
 *	@param 	index 	0 based index, range: [0, Ink_getNodeCount() - 1]
 *	@param 	pt 	position pointer
 *
 *	@return	type of node:
            0: move to
            1: line to
            2: cubic bezier to.
 */
int Ink_getNode(PDF_INK hand, int index, PDF_POINT* pt);

/**
 *	@brief	create a contour
 *
 *	@return	PDF_PATH object
 */
PDF_PATH Path_create(void);
/**
 *	@brief	move to operation
 *
 *	@param 	path 	path create by Path_create()
 *	@param 	x 	x value
 *	@param 	y 	y value
 */
void Path_moveTo(PDF_PATH path, float x, float y);
/**
 *	@brief	move to operation
 *
 *	@param 	path 	path create by Path_create()
 *	@param 	x 	x value
 *	@param 	y 	y value
 *
 */
void Path_lineTo(PDF_PATH path, float x, float y);
/**
 *	@brief	curve to operation
 *
 *	@param 	path 	path create by Path_create()
 *	@param 	x1 	x1 value
 *	@param 	y1 	y1 value
 *	@param 	x2 	x2 value
 *	@param 	y2 	y2 value
 *	@param 	x3 	x3 value
 *	@param 	y3 	y3 value
 */
void Path_curveTo(PDF_PATH path, float x1, float y1, float x2, float y2, float x3, float y3);
/**
 *	@brief	close a contour.
 *
 *	@param 	path 	path create by Path_create()
 */
void Path_closePath(PDF_PATH path);
/**
 *	@brief	free memory
 *
 *	@param 	path 	path create by Path_create()
 */
void Path_destroy(PDF_PATH path);
/**
 *	@brief	get node count
 *
 *	@param 	path path create by Path_create()
 *
 *	@return	node count
 */
int Path_getNodeCount(PDF_PATH path);
/**
 *	@brief	get each node
 *
 *	@param 	path 	path create by Path_create()
 *	@param 	index 	range [0, GetNodeCount() - 1]
 *	@param 	pt 	output value: 2 elements coordinate point
 *
 *	@return	node type:
 *          0: move to
 *          1: line to
 *          3: curve to, index, index + 1, index + 2 are all data
 *          4: close operation
 */
int Path_getNode(PDF_PATH path, int index, PDF_POINT* pt);


@protocol PDFStream
@required
- (bool)writeable;
@required
- (int)read: (void*)buf : (int)len;
@required
- (int)write: (const void*)buf : (int)len;
@required
- (unsigned long long)position;
@required
- (unsigned long long)length;
@required
- (bool)seek:(unsigned long long)pos;
@end

#ifdef __cplusplus
}
#endif
