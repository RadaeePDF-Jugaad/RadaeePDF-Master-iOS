#pragma once
#import "RDComm.h"
#import <CoreGraphics/CGImage.h>
#import <CoreGraphics/CGBitmapContext.h>
#import <UIKit/UIKit.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _PDF_OUTLINE* PDF_OUTLINE;
typedef struct _PDF_PAGE* PDF_PAGE;
typedef struct _PDF_FINDER* PDF_FINDER;
typedef struct _PDF_ANNOT* PDF_ANNOT;
typedef struct _PDF_IMPORTCTX* PDF_IMPORTCTX;
typedef struct _PDF_PAGECONTENT* PDF_PAGECONTENT;
typedef struct _PDF_DOC_FONT* PDF_DOC_FONT;
typedef struct _PDF_PAGE_FONT* PDF_PAGE_FONT;
typedef struct _PDF_DOC_GSTATE* PDF_DOC_GSTATE;
typedef struct _PDF_PAGE_GSTATE* PDF_PAGE_GSTATE;
typedef struct _PDF_DOC_IMAGE* PDF_DOC_IMAGE;
typedef struct _PDF_PAGE_IMAGE* PDF_PAGE_IMAGE;
typedef struct _PDF_DOC_FORM* PDF_DOC_FORM;
typedef struct _PDF_PAGE_FORM* PDF_PAGE_FORM;
typedef struct _PDF_OBJ* PDF_OBJ;
typedef struct _PDF_SIGN* PDF_SIGN;
typedef struct _PDF_TAG* PDF_TAG;
typedef struct _PDF_HTML_EXPORTER* PDF_HTML_EXPORTER;
typedef unsigned long long PDF_OBJ_REF;
typedef struct _PDF_EDITNODE_* PDF_EDITNODE;
typedef enum
{
    mode_poor = 0,
    mode_normal = 1,
    mode_best = 2,
}PDF_RENDER_MODE;

typedef struct
{
    unsigned short num_pals;//0 or 256, if this field is 0, and num_bits == 8, that mean, the image is a mask image.
    unsigned short num_bits;//8 or 32
    int stride;//bytes per scan line.
    int width;
    int height;
    int *palette;//pallete data, if not null, it must be format as 0xAARRGGBB.
    unsigned char *pixels;//pixels data
}PDF_IMAGE_DATA;
/**
 *  @brief  free image data
 *
 *  @param  data image data to free
 */
void PDF_ImageData_Free(PDF_IMAGE_DATA* data);
/**
 *  @brief  draw icon to dib object
 *
 *  @param  annot_type  1(text note) or 17(file attachment)
 *  @param  icon  same as Annotation.GetIcon()
 *  @param  dib dib object
 *
 *  @return true or false.
 */
bool PDF_Document_drawAnnotIcon(int annot_type, int icon, PDF_DIB dib);
/**
 *  @brief  a static method set open flag.
 *          the flag is a global setting, which effect all Document_OpenXXX() methods.
 * 
 *  @param  flag    (flag&1) : load linearzied hint table.
 *                  (flag&2) : if bit set, mean all pages considered as same size, and SDK will only read first page object in open time, and set all pages size same to first page.
 *                  (flag&2) only works when (flag&1) is set.
 */
void PDF_Document_setOpenFlag(int flag);
/**
 *	@brief	open document and return Document object.
 *
 *	@param 	path 	 full path of PDF file.
 *	@param 	password password try both user password and owner password
 *	@param 	err 	 output value: error code.
 *
 *	@return	NULL if failed, and developers should check err code for some reason.
            otherwize return Document object.
 */
PDF_DOC PDF_Document_open(const char* path, const char* password, PDF_ERR* err);
/**
 *  @brief  open document in memory.
 *          first time, SDK try password as user password, and then try password as owner password.
 * 
 *  @param  data    data for whole PDF file in byte array. developers should retain array data, till document closed.
 *  @param  data_size   data array length
 *  @param  password    password or null.
 *  @param  err error code:
 *              0:succeeded, and continue
 *              -1:need input password
 *              -2:unknown encryption
 *              -3:damaged or invalid format
 *              -10:access denied or invalid file path
 *              others:unknown error
 * 
 *  @return NULL if failed, and developers should check err code for some reason.
 *          otherwise return Document object.
 */
PDF_DOC PDF_Document_openMem(void* data, int data_size, const char* password, PDF_ERR* err);
/**
 *  @brief  open document from stream.
 *          first time, SDK try password as user password, and then try password as owner password.
 * 
 *  @param  stream PDFStream object.
 *  @param  password password or null.
 *  @param  err error code:
 *              0:succeeded, and continue
 *              -1:need input password
 *              -2:unknown encryption
 *              -3:damaged or invalid format
 *              -10:access denied or invalid file path
 *              others:unknown error
 * 
 *  @return NULL if failed, and developers should check err code for some reason.
 *          otherwise return Document object.
 */
PDF_DOC PDF_Document_openStream(id<PDFStream> stream, const char* password, PDF_ERR* err);
/**
 *  @brief  open PDF and decrypt PDF using public-key.
 *          this feature only enabled on signed feature version. which native libs has bigger size.
 *  
 *  @param  path    PDF file path
 *  @param  cert_file   a cert file like .p12 or .pfx file, DER encoded cert file.
 *  @param  password    password to open cert file.
 *  @param  err error code:
 *              0:succeeded, and continue
 *              -1:need input password
 *              -2:unknown encryption
 *              -3:damaged or invalid format
 *              -10:access denied or invalid file path
 *              others:unknown error
 * 
 *  @return same as password version.
 */
PDF_DOC PDF_Document_openWithCert(const char* path, const char* cert_file, const char* password, PDF_ERR* err);
/**
 *  @brief  open document from memory and decrypt PDF using public-key.
 *          this feature only enabled on signed feature version. which native libs has bigger size.
 * 
 *  @param  data    data for whole PDF file in byte array. developers should retain array data, till document closed.
 *  @param  data_size   data array length
 *  @param  cert_file   a cert file like .p12 or .pfx file, DER encoded cert file.
 *  @param  password    password to open cert file.
 *  @param  err error code:
 *              0:succeeded, and continue
 *              -1:need input password
 *              -2:unknown encryption
 *              -3:damaged or invalid format
 *              -10:access denied or invalid file path
 *              others:unknown error
 * 
 *  @return NULL if failed, and developers should check err code for some reason.
 *          otherwise return Document object.
 */
PDF_DOC PDF_Document_openMemWithCert(void* data, int data_size, const char* cert_file, const char* password, PDF_ERR* err);
/**
 *  @brief  open document from stream and decrypt PDF using public-key.
 *          first time, SDK try password as user password, and then try password as owner password.
 * 
 *  @param  stream  PDFStream object.
 *  @param  password    password or null.
 *  @param  cert_file   a cert file like .p12 or .pfx file, DER encoded cert file.
 *  @param  err error code:
 *              0:succeeded, and continue
 *              -1:need input password
 *              -2:unknown encryption
 *              -3:damaged or invalid format
 *              -10:access denied or invalid file path
 *              others:unknown error
 * 
 *  @return NULL if failed, and developers should check err code for some reason.
 *          otherwise return Document object.
 */
PDF_DOC PDF_Document_openStreamWithCert(id<PDFStream> stream, const char* cert_file, const char* password, PDF_ERR* err);

/**
 *	@brief	create document and return Document object.
 *
 *	@param 	path 	 full path of PDF file.
 *	@param 	err 	 output value: error code.
 *
 *	@return	NULL if failed, and developers should check err code for some reason.
 otherwize return Document object.
 */
PDF_DOC PDF_Document_create(const char* path, PDF_ERR* err);

/**
  *	@brief	set cache file for the document object.
  *
  *	@param 	doc 	     document object from open or create.
  *	@param 	cache_file 	 absolute path-name for cache file.
  *
  *	@return	true or false.
  */
bool PDF_Document_setCache(PDF_DOC doc, const char* cache_file);

@protocol PDFJSDelegate
@required
- (int)OnAlert:(int)nbtn : (NSString*)msg : (NSString*)title;
@required
- (void)OnConsole:(int)ccmd : (NSString*)para;
@required
- (bool)OnDocClose;
@required
- (NSString*)OnTmpFile;
@required
- (void)OnUncaughtException:(int)code : (NSString*)para;
@end
bool PDF_Document_runJS(PDF_DOC doc, const char* js, id<PDFJSDelegate> del);
/**
 *	@brief	create HTML exporter.
 *
 *	@param 	doc Document object returned from Doument_open
 *  @param  path base path for html directory, if path = "/sdcard/exp", then the html, javascript file, and css file shall generate to this directory.
 *
 *	@return	html exporter object
 */
PDF_HTML_EXPORTER PDF_Document_htmExpStart(PDF_DOC doc, NSString* path);
bool PDF_Document_htmExpPage(PDF_DOC doc, PDF_HTML_EXPORTER exp, int pageno);
void PDF_Document_htmExpEnd(PDF_DOC doc, PDF_HTML_EXPORTER exp);
void PDF_Document_htmExpBinEnd(PDF_DOC doc, PDF_HTML_EXPORTER exp);
/**
 *	@brief	get permission of PDF, this value defined in PDF reference 1.7
 *
 *	@param 	doc Document object returned from PDF_Document_open
 *
 *	@return	permission flags
 *          bit 1-2 reserved
 *          bit 3(0x4) print
 *          bit 4(0x8) modify
 *          bit 5(0x10) extract text or image
 *          others: see PDF reference
 */
int PDF_Document_getPermission(PDF_DOC doc);
/**
 *  @brief  set page rotate.
 *          a premium license is required for this method.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  pageno  0 based page NO.
 *  @param  degree  rotate angle in degree, must be 90 * n.
 * 
 *  @return true or false
 */
bool PDF_Document_setPageRotate(PDF_DOC doc, int pageno, int degree);
/**
 *  @brief  change page rect.
 *          a premium license is required for this method.
 * 
 *	@param 	doc  Document object returned from PDF_Document_open
 *  @param  pageno  0 based page NO.
 *  @param  dl  delta to left, page_left += dl;
 *  @param  dt  delta to top, page_top += dt;
 *  @param  dr  delta to right, page_right += dr;
 *  @param  db  delta to bottom, page_bottom += db;
 * 
 *  @return true or false.
 */
bool PDF_Document_changePageRect(PDF_DOC doc, int pageno, float dl, float dt, float dr, float db);
/**
 *	@brief	is Document editable?
 *
 *	@param 	doc  Document object returned from PDF_Document_open
 *
 *	@return	true or false
 */
bool PDF_Document_canSave(PDF_DOC doc);
/**
 *  @brief  create group tag.
 *          NOTICE:"Document" is auto created by native library.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  parent  parent group. if it is null, the created tag is top level tag.
 *  @param  tag predefined tag name, defined in PDF-Reference-1.7, chapter 10.7.3, TABLE 10.20, except "Document" tag.
 * 
 *  @return group tag object relate to Document object, and the block tag relate to page object.
 */
PDF_TAG PDF_Document_newTagGroup(PDF_DOC doc, PDF_TAG parent, NSString* tag);

/**
 *	@brief	Get title of outline item.
 *
 *	@param 	doc 	        Document object returned from PDF_Document_open
 *	@param 	outlinenode 	Outline Item returned from PDF_Document_getOutlineChild or PDF_Document_getOutlineNext
 *
 *	@return	label string.
 */
NSString* PDF_Document_getOutlineLabel(PDF_DOC doc, PDF_OUTLINE outlinenode);
/**
 *	@brief	Get destination of Outline item.
 *
 *	@param 	doc 	        Document object returned from PDF_Document_open
 *	@param 	outlinenode 	Outline Item returned from PDF_Document_getOutlineChild or PDF_Document_getOutlineNext
 *
 *	@return		0 based page NO.
 */
int PDF_Document_getOutlineDest(PDF_DOC doc, PDF_OUTLINE outlinenode);
struct PDF_DEST
{
    int pageno;//0 based page index.

    //fit 0: page from page label
    //fit 1: [page /XYZ left top zoom]
    //fit 2: [page /Fit]
    //fit 3: [page /FitH top]
    //fit 4: [page /FitV left]
    //fit 5: [page /FitR left bottom right top]
    //fit 6: [page /FitB]
    //fit 7: [page /FitBH top]
    //fit 8: [page /FitBV left]
    int fit;

    float left;
    float top;
    float right;
    float bottom;
    float zoom;
};
void PDF_Document_getOutlineDest2(PDF_DOC doc, PDF_OUTLINE outlinenode, struct PDF_DEST* dest);
/**
 *	@brief	get first child Outline item.
 *
 *	@param 	doc 	        Document object returned from PDF_Document_open
 *	@param 	outlinenode 	Outline Item returned from PDF_Document_getOutlineChild or PDF_Document_getOutlineNext
 *
 *	@return	Root Outline item if outlinenode == NULL.
            return NULL if no children.
 */
NSString* PDF_Document_getOutlineURI(PDF_DOC doc, PDF_OUTLINE outlinenode);
/**
 *  @brief  get file link path of Outline
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *	@param 	outlinenode Outline Item returned from PDF_Document_getOutlineChild or PDF_Document_getOutlineNext
 * 
 *  @return file link path string or null.
 */
NSString* PDF_Document_getOutlineFileLink(PDF_DOC doc, PDF_OUTLINE outlinenode);
/**
 *	@brief	get first child Outline item.
 *
 *	@param 	doc Document object returned from PDF_Document_open
 *	@param 	outlinenode Outline Item returned from PDF_Document_getOutlineChild or PDF_Document_getOutlineNext
 *
 *	@return	Root Outline item if outlinenode == NULL.
 *          return NULL if no children.
 */
PDF_OUTLINE PDF_Document_getOutlineChild(PDF_DOC doc, PDF_OUTLINE outlinenode);
/**
 *	@brief	get next Outline item.
 *
 *	@param 	doc 	        Document object returned from PDF_Document_open
 *	@param 	outlinenode 	Outline Item returned from PDF_Document_getOutlineChild or PDF_Document_getOutlineNext
 *
 *	@return	Root Outline item if outlinenode == NULL.
            return NULL if no next item.
 */
PDF_OUTLINE PDF_Document_getOutlineNext(PDF_DOC doc, PDF_OUTLINE outlinenode);
/**
 *	@brief	insert outline as first child of this Outline.
 *          a premium license is needed for this method.
 *
 *	@param 	doc 	        Document object returned from PDF_Document_open
 *	@param 	outlinenode 	Outline Item returned from PDF_Document_getOutlineChild or PDF_Document_getOutlineNext
 *	@param 	label 	        output value: label text ot outline item.
 *	@param 	pageno 	 0 based page NO.
 *	@param 	top 	y in PDF coordinate
 *
 *	@return	true or false
 */
bool PDF_Document_addOutlineChild(PDF_DOC doc, PDF_OUTLINE outlinenode, const char* label, int pageno, float top);
/**
 *  @brief  new a root outline to document, it insert first root outline to Document.
 *          the old first root outline, shall be next of this outline.
 * 
 *	@param 	doc 	Document object returned from PDF_Document_open
 *  @param  label   label to display
 *  @param  pageno  pageno to jump
 *  @param  top y position in PDF coordinate
 * 
 *  @return true or false
 */
bool PDF_Document_newRootOutline(PDF_DOC doc, const char* label, int pageno, float top);
/**
 *	@brief	insert outline after of this Outline.
 *          a premium license is needed for this method.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	outlinenode 	Outline Item returned from PDF_Document_getOutlineChild or PDF_Document_getOutlineNext
 *	@param 	label 	output value: label text ot outline item.
 *	@param 	pageno 	0 based page NO.
 *	@param 	top 	y in PDF coordinate
 *
 *	@return	true or false
 */
bool PDF_Document_addOutlineNext(PDF_DOC doc, PDF_OUTLINE outlinenode, const char* label, int pageno, float top);
/**
 *	@brief	remove Outline
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	outlinenode 	Outline Item returned from PDF_Document_getOutlineChild or PDF_Document_getOutlineNext
 *
 *	@return	true or false
 */
bool PDF_Document_removeOutline(PDF_DOC doc, PDF_OUTLINE outlinenode);
/**
 *	@brief	get meta data by tag.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	tag 	Predefined values:"Title", "Author", "Subject", "Keywords", "Creator", "Producer", "CreationDate","ModDate".

 *	@return	String value.
 */
NSString* PDF_Document_getMeta(PDF_DOC doc, const char* tag);
/**
 *  @brief  set meta data for document.
 *          this method valid only in premium version.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  tag Predefined values:"Title", "Author", "Subject", "Keywords", "Creator", "Producer", "CreationDate", "ModDate".
 *              or you can pass any key that self-defined.
 *  @param  meta    string value.
 * 
 *  @return true or false.
 */
bool PDF_Document_setMeta(PDF_DOC doc, const char* tag, const char* meta);
/**
 *  @brief  get id of document.
 * 
 *	@param 	doc Document object returned from Document_open
 *  @param  index   must 0 or 1, 0 means first 16 bytes, 1 means last 16 bytes.
 * 
 *  @return bytes or null if no id for this document.
 */ 
bool PDF_Document_getID(PDF_DOC doc, unsigned char* fid);
/**
 *	@brief	get page width.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	pageno 	0 based page number.
 *
 *	@return	width of page
 */
float PDF_Document_getPageWidth(PDF_DOC doc, int pageno);
/**
 *	@brief	get page height.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	pageno 	0 based page number.
 *
 *	@return	height of page.
 */
float PDF_Document_getPageHeight(PDF_DOC doc, int pageno);
/**
 *  @brief  get label of page
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  pageno 0 based page index number
 * 
 *  @return json string or pure text. for json: name is style name of number.
 *          for example:
 *          {"D":2} is "2"
 *          {"R":3} is "III"
 *          {"r":4} is "iv"
 *          {"A":5} is "E"
 *          {"a":6} is "f"
 *          for pure text: the text is the label.
 */
NSString* PDF_Document_getPageLabel(PDF_DOC doc, int pageno);
/**
 *	@brief	get page count.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *
 *	@return	count of pages.
 */
int PDF_Document_getPageCount(PDF_DOC doc);
/**
 *  @brief  get max width and max height of all pages.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  sz  2 elements container width and height values, or null if failed.
 */
void PDF_Document_getPagesMaxSize(PDF_DOC doc, PDF_SIZE* sz);
/**
 *  @brief  get XMP string from document.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 * 
 *  @return null or XML string.
 */
NSString* PDF_Document_getXMP(PDF_DOC doc);
/**
 *  @brief  set XMP string from document.
 *          this method require premium license.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  xmp xmp string to set.
 * 
 *  @return true or false.
 */
bool PDF_Document_setXMP(PDF_DOC doc, NSString* xmp);
/**
 *	@brief	save PDF file.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *
 *	@return		true or false.
 */
bool PDF_Document_save(PDF_DOC doc);
/**
 *	@brief	save PDF file as another file.
            this function remove all encrypt information and save to dst.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	dst 	full path to save.
 *
 *	@return	true or false.
 */
bool PDF_Document_saveAs(PDF_DOC doc, const char* dst, bool rem_sec);
/**
 *  @brief  save to file using optimize flags
 * 
 *  @param  doc Document object returned from PDF_Document_open
 *  @param  path dst path to save.
 *  @param  opts  10 elements set flags, it can pass null, if using default flag.
 *                opts[0]: 1-convert transparency group to image, this may make poor quality.
 *                opts[1]: convert flag for colored images, 0-non, 1-to jpg, 2-to jpx(reserved), 3-to zlib.
 *                opts[2]: convert flag for grayed images, 0-non, 1-to zlib compressed
 *                opts[3]: convert for mask and mono images, 0-non, 1-to zlib, 2-to jbig2
 *                opts[4]: remove unsed objects?(ignored, set to 0 or 1 has same effect)
 *                opts[5]: remove tags?(ignored, set to 0 or 1 has same effect)
 *                opts[6]: remove thumbnail for each page?
 *                opts[7]: merge little piece of images in same area to an big image?(not support yet)
 *                opts[8]: generate linearized header? this mean web optimizing.
 *                opts[9]: generate pdf/a
 *  @param img_dpi  if dpi of image in page is greater than this value, image will be resized.
 * 
 *  @return true or false
 */
bool PDF_Document_optimizeAs(PDF_DOC doc, const char* dst, const unsigned char* opts, float img_dpi);
/**
*	@brief	encrypt PDF file as another file. this function need premium license.
*
*	@param 	doc 	Document object returned from PDF_Document_open
*	@param 	dst 	full path to save.
*	@param  upswd	user password.
*	@param  opswd	owner password.
*	@param  perm	permission to set, see PDF reference or PDF_Document_getPermission().
*	@param  method	reserved.
*	@param	fid		file ID to be set. must be 32 bytes long.
*	@return	true or false.
*/
bool PDF_Document_encryptAs(PDF_DOC doc, NSString* dst, NSString* upswd, NSString* opswd, int perm, int method, unsigned char* fid);
/**
 *	@brief	is document encrypted?
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *
 *	@return		true or false.
 */
bool PDF_Document_isEncrypted(PDF_DOC doc);
/**
 *  @brief  set flag to generate PDFA header when PDF_Document_Save(), PDF_Document_Save() or PDF_Document_encryptAs() invoking.
 * 
 *	@param  doc Document object returned from PDF_Document_open
 *  @param  gen  flag to set to generate PDFA header
 */
void PDF_Document_setGenPDFA(PDF_DOC doc, bool gen);
/**
 *  @brief  verify the signature
 *          a premium license is required for this method.
 * 
 *	@param  doc Document object returned from PDF_Document_open
 *  @param  sign  signature object from PDF_Page_getAnnotSign()
 * 
 *  @return 0 if verify OK, others are error.
 */
int PDF_Document_verifySign(PDF_DOC doc, PDF_SIGN sign);
/**
 *  @brief  get embed files count, for document level.
 *          this method require premium license, it always return 0 if using other license type.
 * 
 *	@param  doc Document object returned from PDF_Document_open
 * 
 *  @return embed files count
 */
int PDF_Document_getEFCount(PDF_DOC doc);
/**
 *  @brief  get name of embed file.
 * 
 *	@param  doc Document object returned from PDF_Document_open
 *  @param  index   range in [0, PDF_Document_getEFCount())
 * 
 *  @return name of embed file
 */
NSString* PDF_Document_getEFName(PDF_DOC doc, int index);
/**
 *  @brief  get Description of embed file.
 * 
 *	@param  doc Document object returned from PDF_Document_open
 *  @param  index   range in [0, PDF_Document_getEFCount())
 * 
 *  @return Description of embed file
 */
NSString* PDF_Document_getEFDesc(PDF_DOC doc, int index);
/**
 *  @brief  get embed file data, and save to save_path
 * 
 *	@param  doc Document object returned from PDF_Document_open
 *  @param  index   range in [0, PDF_Document_getEFCount())
 *  @param  save_path   absolute path to save embed file.
 * 
 *  @return true or false.
 */
bool PDF_Document_getEFData(PDF_DOC doc, int index, NSString* path);
/**
 *  @brief  delete embed file.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  index range in [0, PDF_Document_getEFCount())
 * 
 *  @return true or false
 */
bool PDF_Document_delEF(PDF_DOC doc, int index);
/**
 *  @brief  new embed file.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  path absolute path to the file to be embed.
 * 
 *  @return true or false
 */
bool PDF_Document_newEF(PDF_DOC doc, NSString* path);
/**
 *  @brief  get java script count, for document level.
 *          this method require premium license, it always return 0 if using other license type.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 * 
 *  @return count
 */
int PDF_Document_getJSCount(PDF_DOC doc);
/**
 *  @brief  get name of javascript.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  index   range in [0, PDF_Document_getJSCount())
 * 
 *  @return name of javascript
 */
NSString* PDF_Document_getJSName(PDF_DOC doc, int index);
/**
 *  @brief  get javascript.
 * 
 *	@param 	doc 	Document object returned from PDF_Document_open
 *  @param  index   range in [0, PDF_Document_getJSCount())
 * 
 *  @return javascript string
 */
NSString* PDF_Document_getJS(PDF_DOC doc, int index);
/**
 *  @brief  export form data as xml string.
 *          this method require premium license.
 * 
 *	@param 	doc 	Document object returned from PDF_Document_open
 * 
 *  @return xml string or null.
 */
NSString* PDF_Document_exportForm(PDF_DOC doc);
/**
 *  @brief  Export XFDF format to string(xml).
 * 
 *	@param 	doc 	Document object returned from PDF_Document_open
 *  @param  href  reference to source PDF file, it can be null.
 * 
 *  @return a xml string with encoding header, defined by XFDF format.
 */
NSString* PDF_Document_exportXFDF(PDF_DOC doc, NSString *href);
/**
 *  @brief  import XFDF format data from string.
 * 
 *	@param 	doc 	Document object returned from PDF_Document_open
 *  @param  xfdf XFDF format string, that mean a xml string with encoding header.
 * 
 *  @return true or false.
 */
bool PDF_Document_importXFDF(PDF_DOC doc, NSString *xfdf);
/**
 *  @brief  get linearizied status.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 * 
 *  @return 0: linearized header not loaded or no linearized header.(if setOpenFlag(0); cause always return 0)
 *          1: there is linearized header, but linearized entry checked as failed.
 *          2: there is linearized header, linearized entry checked succeeded, but hint table is damaged.
 *          3. linearized header loaded succeeded.
 */
int PDF_Document_getLinearizedStatus(PDF_DOC doc);
/**
 *	@brief	close document.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 */
void PDF_Document_close(PDF_DOC doc);
/**
 *	@brief	get page object by page NO.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	pageno 	0 based page NO.
 *
 *	@return	page object.
 */
PDF_PAGE PDF_Document_getPage(PDF_DOC doc, int pageno);
/**
 *	@brief	create a font object, used to write texts.
 *          a premium license is needed for this method.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	name 	font name exists in font list.
 *                  using Global.getFaceCount(), Global.getFaceName() to enumerate fonts.
 *	@param 	style 	(style&1) means bold,
                    (style&2) means Italic,
                    (style&8) means embed,
                    (style&16) means vertical writing, mostly used in Asia fonts.
 *
 *	@return	DocFont object or null is failed.
 */
PDF_DOC_FONT PDF_Document_newFontCID(PDF_DOC doc, const char* name, int style);
/**
 *	@brief	get font ascent
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	font 	font object created by PDF_Document_NewFontCID()
 *
 *	@return	<#return value description#>
 */
float PDF_Document_getFontAscent(PDF_DOC doc, PDF_DOC_FONT font);
/**
 *	@brief	get font descent
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	font 	font object created by PDF_Document_NewFontCID()
 *
 *	@return	<#return value description#>
 */
float PDF_Document_getFontDescent(PDF_DOC doc, PDF_DOC_FONT font);
/**
 *	@brief	create a ExtGraphicState object, used to set alpha values.
 *          a premium license is needed for this method.
 *
 *	@param 	doc Document object returned from PDF_Document_open
 *
 *	@return	PDF_DOC_GSTATE objecet or NULL is failed.
 */
PDF_DOC_GSTATE PDF_Document_newGState(PDF_DOC doc);
/**
 *	@brief	set GraphicState object stroke alpha
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	state 	PDF Graphicstate create by PDF_Document_newGState
 *	@param 	alpha 	alpha value
 *
 *	@return	true or false
 */
bool PDF_Document_setGStateStrokeAlpha(PDF_DOC doc, PDF_DOC_GSTATE state, int alpha);
/**
 *	@brief	set GraphicState object fill alpha
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	state 	PDF Graphicstate create by PDF_Document_newGState
 *	@param 	alpha 	alpha value
 *
 *	@return	true or false
 */
bool PDF_Document_setGStateFillAlpha(PDF_DOC doc, PDF_DOC_GSTATE state, int alpha);
/**
 *  @brief  set dash for stroke operation.
 * 
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	state 	PDF Graphicstate create by PDF_Document_newGState
 *  @param  dash    dash array, if null, means set to solid.
 *  @param  dash_cnt    dash array length    
 *  @param  phase   phase value, mostly, it is 0.
 * 
 *  @return true or false.
 *          eaxmple:
 *          [2, 1], 0  means 2 on, 1 off, 2 on, 1 off, …
 *          [2, 1], 0.5 means 1.5 on, 1 off, 2 on 1 off, …
 *          for more details, plz see PDF-Reference 1.7 (4.3.2) Line Dash Pattern.
 */
bool PDF_Document_setGStateStrokeDash(PDF_DOC doc, PDF_DOC_GSTATE state, const float* dash, int dash_cnt, float phase);
/**
 *  @brief  set blend mode to graphic state.
 * 
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	state 	PDF Graphicstate create by PDF_Document_newGState
 *  @param  bmode   2:Multipy
 *                  3:Screen
 *                  4:Overlay
 *                  5:Darken
 *                  6:Lighten
 *                  7:ColorDodge
 *                  8:ColorBurn
 *                  9:Difference
 *                  10:Exclusion
 *                  11:Hue
 *                  12:Saturation
 *                  13:Color
 *                  14:Luminosity
 *                  others:Normal
 * 
 *  @return true or false.
 */
bool PDF_Document_setGStateBlendMode(PDF_DOC doc, PDF_DOC_GSTATE state, int bmode);
/**
 *  @brief  new a form from Document level.
 *          this method require PDF_Document_setCache invoked.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 * 
 *  @return DocForm object or null.
 */
PDF_DOC_FORM PDF_Document_newForm(PDF_DOC doc);
/**
 *  @brief  add font as resource of form.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  form    Form created by PDF_Document_NewForm
 *  @param  font   returned by PDF_Document_NewFontCID()
 * 
 *  @return resource handle
 */
PDF_PAGE_FONT PDF_Document_addFormResFont(PDF_DOC doc, PDF_DOC_FORM form, PDF_DOC_FONT font);
/**
 *  @brief   add image as resource of form.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  form    Form created by PDF_Document_NewForm
 *  @param  image    returned by PDF_Document_NewForm
 * 
 *  @return resource handle
 */
PDF_PAGE_IMAGE PDF_Document_addFormResImage(PDF_DOC doc, PDF_DOC_FORM form, PDF_DOC_IMAGE image);
/**
 *  @brief  add Graphic State as resource of form.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  form    Form created by PDF_Document_NewForm
 *  @param  gstate  returned by PDF_Document_newGState()
 * 
 *  @return resource handle
 */
PDF_PAGE_GSTATE PDF_Document_addFormResGState(PDF_DOC doc, PDF_DOC_FORM form, PDF_DOC_GSTATE gstate);
/**
 *  @brief  add sub-form as resource of form.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  form    Form created by PDF_Document_NewForm
 *  @param  dform   returned by PDF_Document_NewForm
 * 
 *  @return resource handle
 */
PDF_PAGE_FORM PDF_Document_addFormResForm(PDF_DOC doc, PDF_DOC_FORM form, PDF_DOC_FORM sub);
/**
 *  @brief	set content of form, need a box defined in form
 *          the box define edge of form area, which PageContent object includes.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  form    Form created by PDF_Document_NewForm
 *  @param  x   x of form's box
 *  @param  y   y of form's box
 *  @param  w   width of form's box
 *  @param  h   height of form's box
 *  @param  content PageContent object.
 */
void PDF_Document_setFormContent(PDF_DOC doc, PDF_DOC_FORM form, float x, float y, float w, float h, PDF_PAGECONTENT content);
/**
 *  @brief	set this form as transparency.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  form    Form created by PDF_Document_NewForm
 *  @param  isolate set to isolate, mostly are false.
 *  @param  knockout    set to knockout, mostly are false.
 */
void PDF_Document_setFormTransparency(PDF_DOC doc, PDF_DOC_FORM form, bool isolate, bool knockout);
void PDF_Document_freeForm(PDF_DOC doc, PDF_DOC_FORM form);
/**
 *	@brief	insert a page to Document
 *          if pagheno >= PDF_Page_count, it do same as append.
 *          otherwise, insert to pageno.
 *          a premium license is needed for this method.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	pageno 	0 based page NO.
 *	@param 	w 	page width in PDF coordinate
 *	@param 	h 	page height in PDF coordinate
 *
 *	@return	Page object or null means failed.
 */
PDF_PAGE PDF_Document_newPage(PDF_DOC doc, int pageno, float w, float h);

/**
 *	@brief	create an import context.
 *          a premium license is needed for this method.
 *	@param 	doc 	dest Document object returned from PDF_Document_openXXX or PDF_Document_CreateXXX
 *	@param 	doc_src	source Document object returned from PDF_Document_openXXX or PDF_Document_CreateXXX
 *
 *	@return	context object or NULL.
 */
PDF_IMPORTCTX PDF_Document_importStart(PDF_DOC doc, PDF_DOC doc_src);

/**
 *	@brief	import 1 page from source document
 *          a premium license is needed for this method.
 *	@param 	doc 	dest Document object returned from PDF_Document_openXXX or PDF_Document_CreateXXX
 *	@param 	ctx		context object returned from PDF_Document_importStart
 *	@param 	srcno	0 based page NO. from source Document object
 *	@param 	dstno	0 based page NO. for dest Document object
 *
 *	@return	true or false.
 */
bool PDF_Document_importPage(PDF_DOC doc, PDF_IMPORTCTX ctx, int srcno, int dstno);
/**
 *  @brief  import a page to another page.
 *          a premium license is required for this method.
 *          do not forget to invoke PDF_Document_importEnd() after all pages are imported.
 *	@param 	doc dest Document object returned from PDF_Document_openXXX or PDF_Document_CreateXXX
 *	@param 	ctx context object returned from PDF_Document_importStart
 *  @param  srcno 0 based page NO. from source Document that passed to PDF_Document_importStart.
 *  @param  dstno 0 based page NO. to insert in this document object.
 *	@param 	rect 	rect of dstno page, in PDF coordinate.
 *  @return true or false.
 */
bool PDF_Document_importPage2Page(PDF_DOC doc, PDF_IMPORTCTX ctx, int srcno, int dstno, const PDF_RECT* rect);

/**
 *	@brief	destroy context object
 *          a premium license is needed for this method.
 *	@param 	doc 	dest Document object returned from PDF_Document_openXXX or PDF_Document_CreateXXX
 *	@param 	ctx		context object returned from PDF_Document_importStart
 */
void PDF_Document_importEnd(PDF_DOC doc, PDF_IMPORTCTX ctx);

/**
 *	@brief	move page by page NO.
 *          a premium license is needed for this method.
 *
 *	@param 	doc 	Document object returned from PDF_Document_openXXX or PDF_Document_CreateXXX
 *	@param 	pageno1	0 based page NO for origin page NO.
 *	@param 	pageno2	0 based page NO for dest page NO.
 *
 *	@return	true or false
 */
bool PDF_Document_movePage(PDF_DOC doc, int pageno1, int pageno2);

/**
 *	@brief	remove page by page NO.
 *          a premium license is needed for this method.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	pageno 	0 based page NO.
 *
 *	@return	true or false
 */
bool PDF_Document_removePage(PDF_DOC doc, int pageno);
/**
 *  @brief  create an image from CGImageRef Bitmap object.
 * 
 *	@param 	doc 	Document object returned from PDF_Document_open
 *  @param  img CGImageRef Bitmap object
 *  @param  has_alpha   generate alpha channel information?
 *  @param  interpolate interpolate pixels when rendering image scale large than origin image size.
 * 
 *  @return PDF_DOC_IMAGE object or null.
 */
PDF_DOC_IMAGE PDF_Document_newImage(PDF_DOC doc, CGImageRef img, bool has_alpha, bool interpolate);
/**
 *  @brief  create an image from CGImageRef Bitmap object.
 *          this method will generate an image with alpha channel.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  img CGImageRef Bitmap object
 *  @param  matte matte value.
 *  @param  interpolate interpolate pixels when rendering image scale large than origin image size.
 * 
 *  @return PDF_DOC_IMAGE object or null.
 */
PDF_DOC_IMAGE PDF_Document_newImage2(PDF_DOC doc, CGImageRef img, unsigned int matte, bool interpolate);
/**
 *	@brief	create an image from JPEG/JPG file.
 *          supported image color space:
 *          --GRAY
 *          --RGB
 *          --CMYK
 *          a premium license is needed for this method.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	path 	path to JPEG file
 *
 *	@return	DocImage object or null.
 */
PDF_DOC_IMAGE PDF_Document_newImageJPEG(PDF_DOC doc, const char* path, bool interpolate);
/**
 *	@brief	create an image from JPX/JPEG 2k file.
 *          a premium license is needed for this method.
 *
 *	@param 	doc 	Document object returned from PDF_Document_open
 *	@param 	path 	path to JPX file.
 *
 *	@return	DocImage object or null.
 */
PDF_DOC_IMAGE PDF_Document_newImageJPX(PDF_DOC doc, const char* path, bool interpolate);
/**
 *  @brief  sign date time
 * 
 *  @param  sign    sign annotation object, returned from PDF_Page_getAnnotSign()
 * 
 *  @return date time.
 */
NSString *PDF_Sign_getIssue(PDF_SIGN sign);
/**
 *  @brief  get sign subject
 * 
 *  @param  sign    sign annotation object, returned from PDF_Page_getAnnotSign()
 * 
 *  @return sign subject
 */
NSString *PDF_Sign_getSubject(PDF_SIGN sign);
/**
 *  @brief  get sign version
 * 
 *  @param  sign    sign annotation object, returned from PDF_Page_getAnnotSign()
 * 
 *  @return sign version
 */
long PDF_Sign_getVersion(PDF_SIGN sign);
/**
 *  @brief  get signer name.
 * 
 *  @param  sign    sign annotation object, returned from PDF_Page_getAnnotSign()
 * 
 *  @return name string.
 */
NSString *PDF_Sign_getName(PDF_SIGN sign);
/**
 *  @brief  get sign location.
 * 
 *  @param  sign    sign annotation object, returned from PDF_Page_getAnnotSign()
 * 
 *  @return location description.
 */
NSString *PDF_Sign_getLocation(PDF_SIGN sign);
/**
 *  @brief  get sign reason
 * 
 *  @param  sign    sign annotation object, returned from PDF_Page_getAnnotSign()
 * 
 *  @return reason string
 */
NSString *PDF_Sign_getReason(PDF_SIGN sign);
/**
 *  @brief  get sign contact string
 * 
 *  @param  sign    sign annotation object, returned from PDF_Page_getAnnotSign()
 * 
 *  @return contact address or phone.
 */
NSString *PDF_Sign_getContact(PDF_SIGN sign);
/**
 *  @brief  get sign date time
 * 
 *  @param  sign    sign annotation object, returned from PDF_Page_getAnnotSign()
 * 
 *  @return date time.
 */
NSString *PDF_Sign_getModDT(PDF_SIGN sign);
const void *PDF_Sign_getContent(PDF_SIGN sign, int *len);
/**
 *  @brief  Sign and save the PDF file.
 *          this method required premium license, and signed feature native libs, which has bigger size.
 * 
 *	@param 	page    returned from PDF_Document_getPage
 *  @param  appearence  appearance icon for signature
 *                      DocForm object return from PDF_Document_NewForm    
 *  @param  box RECT for sign field
 *  @param  cert_file   a cert file like .p12 or .pfx file, DER encoded cert file.
 *  @param  pswd    password to open cert file.
 *  @param  name    signer name string.
 *  @param  reason  sign reason will write to signature.
 *  @param  location    signature location will write to signature.
 *  @param  contact contact info will write to signature.
 * 
 *  @return 0 mean OK
 *          -1: generate parameters error.
 *          -2: it is not signature field, or field has already signed.
 *          -3: invalid annotation data.
 *          -4: save PDF file failed.
 *          -5: cert file open failed.
 */
int PDF_Page_sign(PDF_PAGE page, PDF_DOC_FORM appearence, const PDF_RECT* box, const char* cert_file, const char* pswd, const char *name, const char* reason, const char* location, const char* contact);
/**
 *  @brief  get rotated CropBox, this method need an any type of license.
 * 
 *	@param 	page    returned from PDF_Document_getPage
 *  @param  box PDF_RECT array as [left, top, right, bottom] in PDF coordinate.
 * 
 *  @return true or false
 */
bool PDF_Page_getCropBox(PDF_PAGE page, PDF_RECT* box);
/**
 *  @brief  get rotated MediaBox, this method need an any type of license.
 * 
 *	@param 	page    returned from PDF_Document_getPage
 *  @param  box PDF_RECT array as [left, top, right, bottom] in PDF coordinate.
 * 
 *  @return true or false
 */
bool PDF_Page_getMediaBox(PDF_PAGE page, PDF_RECT* box);
/**
 *	@brief	close page.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 */
void PDF_Page_close(PDF_PAGE page);
/**
 *  @brief  render thumb image to dib object.
 *          the image always scale and displayed in center of dib.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *  @param  dib DIB to render
 * 
 *  @return true if the page has thumb image, or false.
 */
bool PDF_Page_renderThumb(PDF_PAGE page, PDF_DIB dib);
/**
 *	@brief	reset status and erase wihite for dib.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	dib 	returned from Global_dibGet
 */
void PDF_Page_renderPrepare(PDF_PAGE page, PDF_DIB dib);
/**
 *	@brief	render page to dib.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	dib 	returned from Global_dibGet
 *	@param 	matrix 	returned from Matrix_create or Matrix_createScale
 *	@param 	show_annots 	show annotations?
 *	@param 	mode 	render mode.
 *
 *	@return	true or false.
 */
bool PDF_Page_render(PDF_PAGE page, PDF_DIB dib, PDF_MATRIX matrix, bool show_annots, PDF_RENDER_MODE mode);

/**
 *	@brief	cancel render, in mostly, this function called by UI thread, and PDF_Page_render called by another thread.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 */
void PDF_Page_renderCancel(PDF_PAGE page);
/**
 *	@brief	check if page render finished.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *
 *	@return	true or false.
 */
bool PDF_Page_renderIsFinished(PDF_PAGE page);
/**
 *  @brief  get rotate degree for page, example: 0 or 90
 * 
 *	@param 	page    returned from PDF_Document_getPage
 * 
 *  @return rotate degree for page
 */
int PDF_Page_getRotate(PDF_PAGE page);
/**
 *  @brief  remove all annotations and display it as normal content on page.
 *          this method require premium license.
 * 
 *	@param 	page    returned from PDF_Document_getPage
 * 
 *  @return true or false
 */
bool PDF_Page_flate(PDF_PAGE page);
/**
 *  @brief  flate single annotation
 *          you should render page again to display modified data.
 *          this method require professional or premium license
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *	@param 	annot annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *  @return true or false
 */
bool PDF_Page_flateAnnot(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	load all objects in page.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 */
void PDF_Page_objsStart(PDF_PAGE page, bool rtol);
/**
 *	@brief	get chars count.
            to invoke this function, developers should call PDF_Page_objsStart before.
 *	@param 	page 	returned from PDF_Document_getPage
 *
 *	@return	count of chars.
 */
int PDF_Page_objsGetCharCount(PDF_PAGE page);
/**
 *	@brief	get string by index range.
            to invoke this function, developers should call PDF_Page_objsStart before.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	from 	from index, range: [0, PDF_Page_objsGetCharCount() - 1]
 *	@param 	to 	    to index, range: [0, PDF_Page_objsGetCharCount() - 1]
 *
 *	@return	String value.
 */
NSString* PDF_Page_objsGetString(PDF_PAGE page, int from, int to);
/**
 *	@brief	get area rect of char.
            to invoke this function, developers should call PDF_Page_objsStart before.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	index 	range: [0, PDF_Page_objsGetCharCount() - 1]
 *	@param 	rect 	output value: rect in PDF coordinate
 */
void PDF_Page_objsGetCharRect(PDF_PAGE page, int index, PDF_RECT* rect);
/**
 *  @brief  return unicode from char index
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *  @param  index range:[0, ObjsGetCharCount())
 * 
 *  @return -1 if char is image, others: text unicode
 */
int PDF_Page_objsGetCharUnicode(PDF_PAGE page, int index);
/**
 *  @brief  get image information
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *  @param  index range:[0, ObjsGetCharCount()-1]
 *  @param  info  null if char is not image, others: [bits of image, flags, width, height]
 *                bits of image: bits of image.
 *                flags: flags&1: mask, flags&2: has palette, flags&4: need interpolate.
 *                width is image width.
 *                height is image height.
 * 
 *  @return true or false
 */
bool PDF_Page_objsGetImageInfo(PDF_PAGE page, int index, int info[10]);
/**
 *  @brief  get image pixels and palettes
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *  @param  index range:[0, ObjsGetCharCount()-1]
 *  @param  data  null if char is not image, others: pixels data and information, array like [(bits<<16)|number_pals, stride, width, height, palette, pixels...]
 *                bits is bits number, must be 8 or 32.
 *                number_pals is number of palette entries.
 *                stride is number of ints per pixel line.
 *                palette is palette data. if number_pals is 0, this item is ignored.
 *                width is image width.
 *                height is image height.
 *                pixels: is all pixels data.
 * 
 *  @return true or false
 */
bool PDF_Page_objsGetImageData(PDF_PAGE page, int index, PDF_IMAGE_DATA* data);
/**
 *  @brief  set image data
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *  @param  pixels  pixels data
 *  @param  w width of image
 *  @param  h height of image
 *  @param  stride  bytes per row, must aligned by 4.
 *  @param  has_alpha image has alpha?
 *  @param  interpolate interpolate pixels when rendering image scale large than origin image size.
 * 
 *  @return true or false, mask image always return false.
 */
bool PDF_Page_objsSetImage(PDF_PAGE page, int index, unsigned char* pixels, int w, int h, int stride, bool has_alpha, bool interpolate);
/**
 *  @brief  set image from jpeg file.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *  @param  index char index, range:[0, ObjsGetCharCount()-1]
 *  @param  path same as Document.NewImageJPEG()
 *  @param  interpolate interpolate pixels when rendering image scale large than origin image size.
 * 
 *  @return true or false, mask image always return false.
 */
bool PDF_Page_objsSetImageJPEG(PDF_PAGE page, int index, NSString* path, bool interpolate);
/**
 *  @brief  set image from jpx file.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *  @param  index char index, range:[0, ObjsGetCharCount()-1]
 *  @param  path  same as Document.NewImageJPX()
 *  @param  interpolate interpolate pixels when rendering image scale large than origin image size.
 * 
 *  @return true or false, mask image always return false.
 */
bool PDF_Page_objsSetImageJPX(PDF_PAGE page, int index, NSString* path, bool interpolate);
/**
 *  @brief  set image from jpeg byte array.
 * 
 *  @param  index char index, range:[0, ObjsGetCharCount()-1]
 *  @param  data image byte data array
 *  @param  len data length
 *  @param  interpolate interpolate pixels when rendering image scale large than origin image size.
 * 
 *  @return true or false, mask image always return false.
 */
bool PDF_Page_objsSetImageJPEGByMem(PDF_PAGE page, int index, const unsigned char* data, int len, bool interpolate);
bool PDF_Page_objsRemove(PDF_PAGE page, const int* ranges, int ranges_cnt, bool reload);
/**
 *	@brief	get char's font name. this can be invoked after ObjsStart
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	index 	0 based unicode index.
 *
 *	@return	font name, may be null.
 */
const char* PDF_Page_objsGetCharFontName(PDF_PAGE page, int index);
/**
 *	@brief	get char index nearest to point
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	x 	x in PDF coordinate.
 *	@param 	y 	y in PDF coordinate.
 *
 *	@return	char index or -1 failed.
 */
int PDF_Page_objsGetCharIndex(PDF_PAGE page, float x, float y);
/**
 *	@brief	get index aligned by word. this can be invoked after ObjsStart
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	from 	0 based unicode index.
 *	@param 	dir 	if dir < 0, get start index of the word. otherwise get last index of the word.
 *
 *	@return	new index value.
 */
int PDF_Page_objsAlignWord(PDF_PAGE page, int from, int dir);
/**
 *	@brief	open a finder object.
            to invoke this function, developers should call PDF_Page_objsStart before.
 *
 *	@param 	page 	    returned from PDF_Document_getPage
 *	@param 	str 	    string to find.
 *	@param 	match_case 	match case?
 *	@param 	whole_word 	whole word?
 *
 *	@return	finder object ot NULL if not found.
 */
PDF_FINDER PDF_Page_findOpen(PDF_PAGE page, const char* str, bool match_case, bool whole_word);
/**
 *  @brief  create a find session. this can be invoked after ObjsStart
 *          this function treats line break as blank char.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *  @param  str key string to find.
 *  @param  match_case  match case?
 *  @param  whole_word  match whole word?
 *  @param  skip_blank  skip blank?
 * 
 *  @return handle of find session, or 0 if no found.
 */
PDF_FINDER PDF_Page_findOpen2(PDF_PAGE page, const char* str, bool match_case, bool whole_word, bool skip_blanks);
/**
 *	@brief	how many found?
            to invoke this function, developers should call PDF_Page_objsStart before.
 *
 *	@param 	finder 	finder	returned from PDF_Page_findOpen
 *	@return	0 if no found, otherwise, count of founds.
 */
int PDF_Page_findGetCount(PDF_FINDER finder);
/**
 *	@brief	get char index in page, by find index.
            to invoke this function, developers should call PDF_Page_objsStart before.
 *
 *	@param 	finder 	returned from PDF_Page_findOpen
 *	@param 	index 	find index, range: [0, PDF_Page_findGetCount() - 1].
 *
 *	@return	char index in page, range: [0, PDF_Page_objsGetCharCount() - 1].
 */
int PDF_Page_findGetFirstChar(PDF_FINDER finder, int index);
/**
 *	@brief	get char index in page, by find index.
 *          to invoke this function, developers should call Page_objsStart before.
 *
 *	@param 	finder 	returned from Page_findOpen
 *	@param 	index 	find index, range: [0, Page_findGetCount() - 1].
 *
 *	@return	char index in page, range: [0, Page_objsGetCharCount() - 1].
 */
int PDF_Page_findGetEndChar(PDF_FINDER finder, int index);
/**
 *	@brief	close finder
            to invoke this function, developers should call PDF_Page_objsStart before.
 *
 *	@param 	finder 	finder	returned from PDF_Page_findOpen
 */
void PDF_Page_findClose(PDF_FINDER finder);
/**
 *	@brief	get count of annotations.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *
 *	@param 	page returned from PDF_Document_getPage
 *
 *	@return	count of annotations.
 */
int PDF_Page_getAnnotCount(PDF_PAGE page);
/**
 *	@brief	get annotation by index.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	index 	0 based index, range: [0, PDF_Page_getAnnotCount() - 1].
 *
 *	@return	annotation object or NULL.
 */
PDF_ANNOT PDF_Page_getAnnot(PDF_PAGE page, int index);
/**
 *	@brief	get annotation by point.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	x 	x of point, in PDF coordinate
 *	@param 	y 	y of point, in PDF coordinate
 *
 *	@return	annotation object or NULL.
 */
PDF_ANNOT PDF_Page_getAnnotFromPoint(PDF_PAGE page, float x, float y);
/**
 *  @brief  get status of signature field.
 *          this method require premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return -1 if this is not signature field
 *          0 if not signed.
 *          1 if signed.
 */
int PDF_Page_getAnnotSignStatus(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  get signature field object.
 *          this method require premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return PDF Sign object
 */
PDF_SIGN PDF_Page_getAnnotSign(PDF_PAGE page, PDF_ANNOT annot);
PDF_OBJ PDF_Page_getAnnotSignLock(PDF_PAGE page, PDF_ANNOT annot);
bool PDF_Page_setAnnotSignLock(PDF_PAGE page, PDF_ANNOT annot, PDF_OBJ obj);

/**
 *	@brief	is annotation locked?
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return		true or false.
 */
bool PDF_Page_isAnnotLocked(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  set annotation lock status.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  lock    true if lock, otherwise false.
 * 
 */
void PDF_Page_setAnnotLock(PDF_PAGE page, PDF_ANNOT annot, bool lock);
/**
 *  @brief  is this annotation read-only?
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return if annotation is field, return field property. otherwise return annotation property.
 */
bool PDF_Page_isAnnotReadonly(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  if annotation is field, then set field property
 *          otherwise, set annotation property.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  read_only   true or false.
 */
void PDF_Page_setAnnotReadonly(PDF_PAGE page, PDF_ANNOT annot, bool lock);

/**
 *	@brief	is annotation locked?
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return		true or false.
 */
bool PDF_Page_isAnnotLockedContent(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	is annotation hided?
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	true or false
 */
bool PDF_Page_isAnnotHide(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	set annotation hided
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	hide 	true or false
 */
void PDF_Page_setAnnotHide(PDF_PAGE page, PDF_ANNOT annot, bool hide);
/**
 *  @brief  get annotation's name("NM" entry).
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return name string.
 */
NSString* PDF_Page_getAnnotName(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  set annotation's name("NM" entry).
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  name    name string to be set.
 * 
 *  @return true or false.
 */
bool PDF_Page_setAnnotName(PDF_PAGE page, PDF_ANNOT annot, const char* name);
/**
 *  @brief  get annotation by name.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *  @param  name    name string in "NM" entry of annotation.
 * 
 *  @return Annotation object, valid until Page.Close invoked.
 */
PDF_ANNOT PDF_Page_getAnnotByName(PDF_PAGE page, const char* name);
/**
 *	@brief	get annotation type.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method valid in professional or premium version
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	type as these values:
 *          0:  unknown
 *          1:  text
 *          2:  link
 *          3:  free text
 *          4:  line
 *          5:  square
 *          6:  circle
 *          7:  polygon
 *          8:  polyline
 *          9:  text hilight
 *          10: text under line
 *          11: text squiggly
 *          12: text strikeout
 *          13: stamp
 *          14: caret
 *          15: ink
 *          16: popup
 *          17: file attachment
 *          18: sound
 *          19: movie
 *          20: widget
 *          21: screen
 *          22: print mark
 *          23: trap net
 *          24: water mark
 *          25: 3d object
 *          26: rich media
 */
int PDF_Page_getAnnotType(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  sign the empty field and save the PDF file.
 *          if the signature field is not empty(signed), it will return failed.
 *          this method require premium license.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  appearence  appearance icon for signature
 *                      DocForm object return from Document_NewForm      
 *  @param  cert_file   a cert file like .p12 or .pfx file, DER encoded cert file.
 *  @param  pswd    password to open cert file.
 *  @param  name    signer name string.
 *  @param  reason  sign reason will write to signature.
 *  @param  location    signature location will write to signature.
 *  @param  contact contact info will write to signature.
 * 
 *  @return 0 mean OK
 *          -1: generate parameters error.
 *          -2: it is not signature field, or field has already signed.
 *          -3: invalid annotation data.
 *          -4: save PDF file failed.
 *          -5: cert file open failed.
 */
int PDF_Page_signAnnotField(PDF_PAGE page, PDF_ANNOT annot, PDF_DOC_FORM appearence, const char* cert_file, const char* pswd, const char *name, const char* reason, const char* location, const char* contact);

/**
 *	@brief	get annotation field type in acroForm.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method valid in premium version
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	type as these values:
 *          0: unknown
 *          1: button field
 *          2: text field
 *          3: choice field
 *          4: signature field
 */
int PDF_Page_getAnnotFieldType(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  get annotation field flag in acroForm.
 *          this method require premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return flag&1 : read-only
 *          flag&2 : is required
 *          flag&4 : no export.
 */
int PDF_Page_getAnnotFieldFlag(PDF_PAGE page, PDF_ANNOT annot);

/**
 *	@brief get field name of this annotation.
 *			a premium license is needed for this function.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param	buf		buffer to fill names in UTF-8 coding.
 *	@param	buf_size	size of buffer that allocated.
 *	@return	name of this annotation, like: "EditBox1[0]"
 */
int PDF_Page_getAnnotFieldName(PDF_PAGE page, PDF_ANNOT annot, char* buf, int buf_size);
/**
 *  @brief  get name of the annotation.
 *          this method require premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param	buf		buffer to fill names in UTF-8 coding.
 *	@param	buf_size	size of buffer that allocated.
 *
 *  @return null if it is not field, or name of the annotation, example: "EditBox1[0]".
 */
int PDF_Page_getAnnotFieldNameWithNO(PDF_PAGE page, PDF_ANNOT annot, char* buf, int buf_size);
/**
 *	@brief get field full name of this annotation.
 *			a premium license is needed for this function.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param	buf		buffer to fill names in UTF-8 coding.
 *	@param	buf_size	size of buffer that allocated.
 *	@return	name of this annotation, like: "form1.EditBox1"
 */
int PDF_Page_getAnnotFieldFullName(PDF_PAGE page, PDF_ANNOT annot, char* buf, int buf_size);
/**
 *	@brief get field full name of this annotation with more details.
 *			a premium license is needed for this function.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param	buf		buffer to fill names in UTF-8 coding.
 *	@param	buf_size	size of buffer that allocated.
 *	@return	name of this annotation, like: "form1[0].EditBox1[0]"
 */
int PDF_Page_getAnnotFieldFullName2(PDF_PAGE page, PDF_ANNOT annot, char* buf, int buf_size);
/**
 *  @brief  get jsvascript action of fields
 *          this method require premium license.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  idx action index:
 *          0:'K' performed when the user types a keystroke
 *          1:'F' to be performed before the field is formatted to display its current value.
 *          2:'V' to be performed when the field’s value is changed
 *          3:'C' to be performed to recalculate the value of this field when that of another field changes.
 * 
 *  @return javsscript of field's action, mostly a java-script like:
 *          AFDate_FormatEx("dd/mm/yy");
 *          most common java script function invoked as:
 *          AFNumber_Format
 *          AFDate_Format
 *          AFTime_Format
 *          AFSpecial_Format
 *          AFPercent_Format
 *          and so on.
 */
NSString* PDF_Page_getAnnotFieldJS(PDF_PAGE page, PDF_ANNOT annot, int idx);
/**
 *  @brief  render an annotation to dib. this method fully scale annotation to dib object.
 *          this method require professional or premium license.
 *          Notice 1: the render result may not correct for some annotation that not used Alpha Color blending.
 *          example: highlight annotation may not render correctly.
 *          Notice 2: you can invoke Global.hideAnnots() in Global.Init(), and invoke this method to handle Annotations by yourself.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  dib DIB object.
 * 
 *  @return true or false.
 */
bool PDF_Page_renderAnnot(PDF_PAGE page, PDF_ANNOT annot, PDF_DIB dib);
/**
 *	@brief	get annotation rect.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	rect 	rect of annotation area, in PDF coordinate.
 */
void PDF_Page_getAnnotRect(PDF_PAGE page, PDF_ANNOT annot, PDF_RECT* rect);
/**
 *	@brief	set annotation rect.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	rect 	rect of annotation area, in PDF coordinate.
 */
void PDF_Page_setAnnotRect(PDF_PAGE page, PDF_ANNOT annot, const PDF_RECT* rect);
/**
 *  @brief  get modify DateTime of Annotation.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return DateTime String object
 *          format as (D:YYYYMMDDHHmmSSOHH'mm') where:
 *          YYYY is the year
 *          MM is the month
 *          DD is the day (01–31)
 *          HH is the hour (00–23)
 *          mm is the minute (00–59)
 *          SS is the second (00–59)
 *          O is the relationship of local time to Universal Time (UT), denoted by one of the characters +, −, or Z (see below)
 *          HH followed by ' is the absolute value of the offset from UT in hours (00–23)
 *          mm followed by ' is the absolute value of the offset from UT in minutes (00–59)
 *          more details see PDF-Reference-1.7 section 3.8.3
 */
const char* PDF_Page_getAnnotModifyDate(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  get modify DateTime of Annotation.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  val DateTime String object
 *          format as (D:YYYYMMDDHHmmSSOHH'mm') where:
 *          YYYY is the year
 *          MM is the month
 *          DD is the day (01–31)
 *          HH is the hour (00–23)
 *          mm is the minute (00–59)
 *          SS is the second (00–59)
 *          O is the relationship of local time to Universal Time (UT), denoted by one of the characters +, −, or Z (see below)
 *          HH followed by ' is the absolute value of the offset from UT in hours (00–23)
 *          mm followed by ' is the absolute value of the offset from UT in minutes (00–59)
 *          more details see PDF-Reference-1.7 section 3.8.3
 * 
 *  @return true or false
 */
bool PDF_Page_setAnnotModifyDate(PDF_PAGE page, PDF_ANNOT annot, const char* val);
/**
 *  @brief  get Path object from Ink annotation.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return a new Path object, you need invoke Path_destroy() to free memory.
 */
PDF_PATH PDF_Page_getAnnotInkPath(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  set Path to Ink annotation.
 *          you need render page again to show modified annotation.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  path    Path object.
 * 
 *  @return true or false.
 */
bool PDF_Page_setAnnotInkPath(PDF_PAGE page, PDF_ANNOT annot, PDF_PATH path);
/**
 *  @brief  get Path object from Polygon annotation.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return a new Path object, you need invoke Path_destroy() to free memory.
 */
PDF_PATH PDF_Page_getAnnotPolygonPath(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  set Path to Polygon annotation.
 *          you need render page again to show modified annotation.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  path    Path object.
 * 
 *  @return true or false.
 */
bool PDF_Page_setAnnotPolygonPath(PDF_PAGE page, PDF_ANNOT annot, PDF_PATH path);
/**
 *  @brief  get Path object from Polyline annotation.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return a new Path object, you need invoke Path_destroy() to free memory.
 */
PDF_PATH PDF_Page_getAnnotPolylinePath(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  set Path to Polyline annotation.
 *          you need render page again to show modified annotation.
 *          this method require professional or premium license
 *  
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  path    PDF_PATH object.
 * 
 *  @return true or false.
 */
bool PDF_Page_setAnnotPolylinePath(PDF_PAGE page, PDF_ANNOT annot, PDF_PATH path);
/**
 *  @brief  get point of line annotation.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  idx 0: start point, others: end point.
 *	@param 	pt  PDF_POINT object, array as [x,y], or null.
 * 
 *  @return true or false
 */
bool PDF_Page_getAnnotLinePoint(PDF_PAGE page, PDF_ANNOT annot, int idx, PDF_POINT* pt);
bool PDF_Page_setAnnotLinePoint(PDF_PAGE page, PDF_ANNOT annot, float x1, float y1, float x2, float y2);
/**
 *  @brief  get line style of line or polyline annotation.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return (ret >> 16) is style of end point, (ret & 0xffff) is style of start point.
 */
int PDF_Page_getAnnotLineStyle(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  set line style of line or polyline annotation.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  style   (style >> 16) is style of end point, (style & 0xffff) is style of start point.
 * 
 *  @return true or false.
 */
bool PDF_Page_setAnnotLineStyle(PDF_PAGE page, PDF_ANNOT annot, int style);

/**
 *	@brief	get annotation fill color
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	RGB color
 */
int PDF_Page_getAnnotFillColor(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	set annotation fill color
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	color 	RGB color
 *
 *	@return	true or false
 */
bool PDF_Page_setAnnotFillColor(PDF_PAGE page, PDF_ANNOT annot, int color);
/**
 *	@brief	get annotation stroke color
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	RGB color
 */
int PDF_Page_getAnnotStrokeColor(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	set annotation stroke color
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	color 	RGBA color
 *
 *	@return	true or false
 */
bool PDF_Page_setAnnotStrokeColor(PDF_PAGE page, PDF_ANNOT annot, int color);
/**
 *	@brief	get annot stroke width
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	stroke width
 */
float PDF_Page_getAnnotStrokeWidth(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	set annot stroke width
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	width 	stroke width
 *
 *	@return	true or false
 */
bool PDF_Page_setAnnotStrokeWidth(PDF_PAGE page, PDF_ANNOT annot, float width);
int PDF_Page_getAnnotStrokeDash(PDF_PAGE page, PDF_ANNOT annot, float* dash, int max);
/**
 *  @brief  set stroke dash of square/circle/ink/line/ploygon/polyline/free text/text field annotation.
 *          for free text or text field annotation: dash of edit-box border
 *          you need render page again to show modified annotation.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  dash    stroke dash array units.
 *  @param  cnt     stroke dash array length.
 * 
 *  @return true or false
 */
bool PDF_Page_setAnnotStrokeDash(PDF_PAGE page, PDF_ANNOT annot, const float* dash, int cnt);
/**
 *	@brief	set icon for sticky text note/file attachment annotation.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          you need render page again to show modified annotation.
 *          this method valid in professional or premium version
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	icon  icon value depends on annotation type.
 *                For sticky text note:
 *                0: Note
 *                1: Comment
 *                2: Key
 *                3: Help
 *                4: NewParagraph
 *                5: Paragraph
 *                6: Insert
 *                7: Check
 *                8: Circle
 *                9: Cross
 *                For file attachment:
 *                0: PushPin
 *                1: Graph
 *                2: Paperclip
 *                3: Tag
 *                For Rubber Stamp:
 *                0: "Draft"(default icon)
 *                1: "Approved"
 *                2: "Experimental"
 *                3: "NotApproved"
 *                4: "AsIs"
 *                5: "Expired"
 *                6: "NotForPublicRelease"
 *                7: "Confidential"
 *                8: "Final"
 *                9: "Sold"
 *                10: "Departmental"
 *                11: "ForComment"
 *                12: "TopSecret"
 *                13: "ForPublicRelease"
 *                14: "Accepted"
 *                15: "Rejected"
 *                16: "Witness"
 *                17: "InitialHere"
 *                18: "SignHere"
 *                19: "Void"
 *                20: "Completed"
 *                21: "PreliminaryResults"
 *                22: "InformationOnly"
 *                23: "End"
 *	              @return	true or false
 */
bool PDF_Page_setAnnotIcon(PDF_PAGE page, PDF_ANNOT annot, int icon);
/**
 *  @brief  set customized icon for: sticky text note/file attachment annotation, and unsigned signature field.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  name    customized icon name.
 *  @param  form    DocForm object return from PDF_Document_NewForm
 * 
 *  @return true or false.
 */
bool PDF_Page_setAnnotIcon2(PDF_PAGE page, PDF_ANNOT annot, const char* name, PDF_DOC_FORM form);
/**
 *	@brief	get icon value for sticky text note/file attachment/Rubber Stamp annotation.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method valid in professional or premium version
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	icon value depends on annotation type.
 *          For sticky text note:
 *          0: Note
 *          1: Comment
 *          2: Key
 *          3: Help
 *          4: NewParagraph
 *          5: Paragraph
 *          6: Insert
 *          7: Check
 *          8: Circle
 *          9: Cross
 *          For file attachment:
 *          0: PushPin
 *          1: Graph
 *          2: Paperclip
 *          3: Tag
 *         For Rubber Stamp:
 *          0: "Draft"(default icon)
 *          1: "Approved"
 *          2: "Experimental"
 *          3: "NotApproved"
 *          4: "AsIs"
 *          5: "Expired"
 *          6: "NotForPublicRelease"
 *          7: "Confidential"
 *          8: "Final"
 *          9: "Sold"
 *         10: "Departmental"
 *         11: "ForComment"
 *         12: "TopSecret"
 *         13: "ForPublicRelease"
 *         14: "Accepted"
 *         15: "Rejected"
 *         16: "Witness"
 *         17: "InitialHere"
 *         18: "SignHere"
 *         19: "Void"
 *         20: "Completed"
 *         21: "PreliminaryResults"
 *         22: "InformationOnly"
 *         23: "End"
 */
int PDF_Page_getAnnotIcon(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	get annotation's goto page action.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	-1 if failed or 0 based page NO.
 */
int PDF_Page_getAnnotDest(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief		get annotation's goto URI action.
 *            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	String value for uri or null.
 */
NSString* PDF_Page_getAnnotURI(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  get annotation's java-script string.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *  @return string of java-script, or null.
 */
NSString* PDF_Page_getAnnotJS(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  get additional action, for java script.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  idx ievent type as below:
 *              0:(Optional; PDF 1.2) An action to be performed when the cursor enters the annotation’s active area.
 *              1:(Optional; PDF 1.2) An action to be performed when the cursor exits the annotation’s active area.
 *              2 (Optional; PDF 1.2) An action to be performed when the mouse button is pressed inside the annotation’s active area. (The name D stands for "down.")
 *              3:(Optional; PDF 1.2) An action to be performed when the mouse button is released inside the annotation’s active area. (The name U stands for "up.")
 *              4:(Optional; PDF 1.2; widget annotations only) An action to be performed when the annotation receives the input focus.
 *              5:(Optional; PDF 1.2; widget annotations only) (Uppercase B, lowercase L) An action to be performed when the annotation loses the input focus. (The name Bl stands for "blurred.")
 *              6:(Optional; PDF 1.5) An action to be performed when the page containing the annotation is opened (for example, when the user navigates to it from the next or previous page or by means of a link annotation or outline item). The action is executed after the O action in the page’s additional-actions dictionary (see Table 8.45) and the OpenAction entry in the document catalog (see Table 3.25), if such actions are present.
 *              7:(Optional; PDF 1.5) An action to be performed when the page containing the annotation is closed (for example, when the user navigates to the next or previous page, or follows a link annotation or outline item). The action is executed before the C action in the page’s additional-actions dictionary (see Table 8.45), if present.
 *              8:(Optional; PDF 1.5) An action to be performed when the page containing the annotation becomes visible in the viewer application’s user interface.
 *              9:(Optional; PDF 1.5) An action to be performed when the page containing the annotation is no longer visible in the viewer application’s user interface.
 * 
 *  @return string of java-script, or null.
 */
NSString* PDF_Page_getAnnotAdditionalJS(PDF_PAGE page, PDF_ANNOT annot, int idx);
/**
 *	@brief	get annotation's 3D play action.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in professional or premium license.
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	String value for 3D name or null.
 */
NSString* PDF_Page_getAnnot3D(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	get annotation's movie play action.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in professional or premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	String value for Movie name or null.
 */
NSString* PDF_Page_getAnnotMovie(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	get annotation's audio play action.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in professional or premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	String value for Sound Name or null.
 */
NSString* PDF_Page_getAnnotSound(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	get annotation's attachment open action.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in professional or premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	String value for attachment name or null.
 */
NSString* PDF_Page_getAnnotAttachment(PDF_PAGE page, PDF_ANNOT annot);
NSString* PDF_Page_getAnnotRendition(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	get data of annotation's 3D open action.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in professional or premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	path 	file to save.
 *
 *	@return	true or false.
 */
bool PDF_Page_getAnnot3DData(PDF_PAGE page, PDF_ANNOT annot, const char* path);
/**
 *	@brief	get data of annotation's movie open action.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in professional or premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	path 	file to save.
 *
 *	@return	true or false.
 */
bool PDF_Page_getAnnotMovieData(PDF_PAGE page, PDF_ANNOT annot, const char* path);
/**
 *	@brief	get data of annotation's Audio open action.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in professional or premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	paras 	4 element parameters, if paras[0] == 0, means the data is formatted audio, for example( *.mp3 )
 *                  otherwize it is raw sound data.
 *	@param 	path 	file to save.
 *
 *	@return	true or false.
 */
bool PDF_Page_getAnnotSoundData(PDF_PAGE page, PDF_ANNOT annot, int* paras, const char* path);
/**
 *	@brief	get data of annotation's attachment open action.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in professional or premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	path 	file to save.
 *
 *	@return	true or false.
 */
bool PDF_Page_getAnnotAttachmentData(PDF_PAGE page, PDF_ANNOT annot, const char* path);
bool PDF_Page_getAnnotRenditionData(PDF_PAGE page, PDF_ANNOT annot, const char* path);
/**
 *  @brief  get item count of RichMedia annotation.
 *          this method require professional or premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *	@param 	annot annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return count of items, or -1 if not RichMedia annotation, and no premium license actived.
 */
int PDF_Page_getAnnotRichMediaItemCount(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  get actived item of RichMedia annotation.
 *          this method require professional or premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *	@param 	annot annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return index of actived item, or -1 if not RichMedia annotation, and no premium license actived.
 */
int PDF_Page_getAnnotRichMediaItemActived(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  get content type of an item of RichMedia annotation.
 *          this method require professional or premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *	@param 	annot annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  idx range in [0, PDF_Page_getAnnotRichMediaItemCount()]
 * 
 *  @return type of item:
 *          -1: unknown or error.
 *          0: Video.
 *          1：Sound.
 *          2:Flash file object.
 *          3:3D file object.
 */
int PDF_Page_getAnnotRichMediaItemType(PDF_PAGE page, PDF_ANNOT annot, int idx);
/**
 *  @brief  return asset name of content of an item of RichMedia annotation.
 *          this method require professional or premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *	@param 	annot annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  idx range in [0, PDF_Page_getAnnotRichMediaItemCount()]
 * 
 *  @return asset name, or null.
 *          asset name example: "VideoPlayer.swf"
 */
NSString* PDF_Page_getAnnotRichMediaItemAsset(PDF_PAGE page, PDF_ANNOT annot, int idx);
/**
 *  @brief  return parameters of an item of RichMedia annotation.
 *          this method require professional or premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *	@param 	annot annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  idx range in [0, PDF_Page_getAnnotRichMediaItemCount()]
 * 
 *  @return parameter string, or null.
 *          parameter example: "source=myvideo.mp4&skin=SkinOverAllNoFullNoCaption.swf&skinAutoHide=true&skinBackgroundColor=0x5F5F5F&skinBackgroundAlpha=0.75&volume=1.00"
 */
NSString* PDF_Page_getAnnotRichMediaItemPara(PDF_PAGE page, PDF_ANNOT annot, int idx);
/**
 *  @brief  return source of an item of RichMedia annotation.
 *          this method require professional or premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *	@param 	annot annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  idx range in [0, PDF_Page_getAnnotRichMediaItemCount()]
 * 
 *  @return parameter string, or null.
 *          parameter example: "source=myvideo.mp4&skin=SkinOverAllNoFullNoCaption.swf&skinAutoHide=true&skinBackgroundColor=0x5F5F5F&skinBackgroundAlpha=0.75&volume=1.00"
 *          the source is "source=myvideo.mp4", return string is "myvideo.mp4"
 */
NSString* PDF_Page_getAnnotRichMediaItemSource(PDF_PAGE page, PDF_ANNOT annot, int idx);
/**
 *  @brief  save source of an item of RichMedia annotation to a file.
 *          this method require professional or premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *	@param 	annot annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  idx range in [0, PDF_Page_getAnnotRichMediaItemCount()]
 *  @param  save_path absolute path to save file, like "/sdcard/app_data/myvideo.mp4"
 * 
 *  @return true or false.
 */
bool PDF_Page_getAnnotRichMediaItemSourceData(PDF_PAGE page, PDF_ANNOT annot, int idx, NSString* save_path);
/**
 *  @brief  save an asset to a file.
 *          this method require professional or premium license.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  asset   asset name in RichMedia assets list.
 *  @param  save_path   absolute path to save file, like "/sdcard/app_data/myvideo.mp4"
 * 
 *  @return true or false.
 *          example:
 *          GetRichMediaItemAsset(0) return player window named as "VideoPlayer.swf"
 *          GetRichMediaItemPara(0) return "source=myvideo.mp4&skin=SkinOverAllNoFullNoCaption.swf&skinAutoHide=true&skinBackgroundColor=0x5F5F5F&skinBackgroundAlpha=0.75&volume=1.00".
 *          so we has 3 assets in item[0]:
 *          1."VideoPlayer.swf"
 *          2."myvideo.mp4"
 *          3."SkinOverAllNoFullNoCaption.swf"
 */
bool PDF_Page_getAnnotRichMediaData(PDF_PAGE page, PDF_ANNOT annot, NSString* asset, NSString* save_path);
/**
 *  @brief  get annotation's file link path string.
 *          this method require professional or premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return string of link path, or null
 */
NSString* PDF_Page_getAnnotFileLink(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  get popup Annotation associate to this annotation.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return Popup Annotation, or null, if this annotation is Popup Annotation, then return same as this.
 */
PDF_ANNOT PDF_Page_getAnnotPopup(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  get open status for Popup Annotation.
 *          if this annotation is not popup annotation, it return Popup annotation open status, which associate to this annotation.
 *          this method require professional or premium license.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 * 
 *  @return true or false.
 */
bool PDF_Page_getAnnotPopupOpen(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  set open status for Popup Annotation.
 *          if this annotation is not popup annotation, it set Popup annotation open status, which associate to this annotation.
 *          this method require professional or premium license.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  open    set annotation open status
 * 
 *  @return true or false.
 */
bool PDF_Page_setAnnotPopupOpen(PDF_PAGE page, PDF_ANNOT annot, bool open);
int PDF_Page_getAnnotReplyCount(PDF_PAGE page, PDF_ANNOT annot);
PDF_ANNOT PDF_Page_getAnnotReply(PDF_PAGE page, PDF_ANNOT annot, int idx);
/**
 *	@brief	get subject of popup text annotation.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	String value of subject or null.
 */
NSString* PDF_Page_getAnnotPopupSubject(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	set subject of popup text annotation.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in professional or premium license.
 *          you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	subj 	subject string.
 *
 *	@return	true or false
 */
bool PDF_Page_setAnnotPopupSubject(PDF_PAGE page, PDF_ANNOT annot, const char* subj);
/**
 *	@brief	get text of popup text annotation.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	String value of text or null.
 */
NSString* PDF_Page_getAnnotPopupText(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	set text of popup text annotation.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in professional or premium license.
 *          you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	text 	text string
 *
 *	@return	true or false
 */
bool PDF_Page_setAnnotPopupText(PDF_PAGE page, PDF_ANNOT annot, const char* text);
/**
 *	@brief	get text of popup label annotation.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	String value of label or null.
 */
NSString* PDF_Page_getAnnotPopupLabel(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	set text of popup label annotation.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in professional or premium license.
 *          you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	text 	text string
 *
 *	@return	true or false
 */
bool PDF_Page_setAnnotPopupLabel(PDF_PAGE page, PDF_ANNOT annot, const char* text);
/**
 *	@brief	get type of edit-box, may either in free-text annotation and widget annotation.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return -1: this annotation is not text-box.
 *           1:  normal single line.
 *           2:  password.
 *           3:  MultiLine edit area.
 */
int PDF_Page_getAnnotEditType(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	get rect area of edit-box, may either for free-text annotation and widget annotation.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	rect 	output value: rect of edit box.
 *
 *	@return	true or false.
 */
bool PDF_Page_getAnnotEditTextRect(PDF_PAGE page, PDF_ANNOT annot, PDF_RECT* rect);
/**
 *	@brief	get text size of edit-box, may either for free-text annotation and widget annotation.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	text size in PDF coordinate.
 */
float PDF_Page_getAnnotEditTextSize(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  set text size of edit-box and edit field.
 *          this method require premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  fsize   font size to set.
 * 
 *  @return true or false.
 */
bool PDF_Page_setAnnotEditTextSize(PDF_PAGE page, PDF_ANNOT annot, float fsize);
/**
 *  @brief	get text align of edit-box and edit field.
 *          this method require premium license,
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *  @return align of text, 0: left, 1: center, 2: right.
 */
int PDF_Page_getAnnotEditTextAlign(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief	set text align of edit-box and edit field.
 *          this method require premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  align   text align value, 0: left, 1: center, 2: right.
 * 
 *  @return true or false.
 */
bool PDF_Page_setAnnotEditTextAlign(PDF_PAGE page, PDF_ANNOT annot, int align);

/**
 *	@brief	get text of edit-box, may either for free-text annotation and widget annotation.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	String value of Edit text or null
 */
NSString* PDF_Page_getAnnotEditText(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	set text of edit-box, may either for free-text annotation and widget annotation.
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in premium license.
 *          you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	text 	text string
 *
 *	@return	true or false
 */
bool PDF_Page_setAnnotEditText(PDF_PAGE page, PDF_ANNOT annot, const char* text);
/**
 *  @brief  set font of edittext.
 *          you should re-render page to display modified data.
 *          this method require premium license.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  font    DocFont object from PDF_Document_NewFontCID().
 * 
 *  @return true or false.
 */
bool PDF_Page_setAnnotEditFont(PDF_PAGE page, PDF_ANNOT annot, PDF_DOC_FONT font);
/**
 *  @brief  get text color for edit-box annotation.
 *          include text field and free-text.
 *          this method require premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *  @return 0 or color, format as 0xAARRGGBB.
 */
int PDF_Page_getAnnotEditTextColor(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  set text color for edit-box annotation.include text field and free-text
 *          this method require premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  color   color format as 0xRRGGBB, alpha channel are ignored.
 *
 *  @return true or false.
 */
bool PDF_Page_setAnnotEditTextColor(PDF_PAGE page, PDF_ANNOT annot, int color);
/**
 *  @brief  data from annotation.
 *          a premium license is required for this method.
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  data    data pointer
 *  @param  data_len    data buffer size
 * 
 *  @return -1: if the export failed, otherwise: export success.
 */
int PDF_Page_exportAnnot(PDF_PAGE page, PDF_ANNOT annot, unsigned char* data, int data_len);
/**
 *  @brief  annotation from memory(byte array)
 *          a premium license is required for this method.
 * 
 *  @param 	page 	returned from PDF_Document_getPage.
 *  @param  rect    [left, top, right, bottom] in PDF coordinate. which is the import annotation's position.
 *  @param  data    data returned from Page_exportAnnot()
 *  @param  data_len    data length
 * 
 *  @return true or false.
 */
bool PDF_Page_importAnnot(PDF_PAGE page, const PDF_RECT* rect, const unsigned char* data, int data_len);
PDF_OBJ_REF PDF_Page_getAnnotRef(PDF_PAGE page, PDF_ANNOT annot);
bool PDF_Page_addAnnot(PDF_PAGE page, PDF_OBJ_REF ref);
bool PDF_Page_addAnnot2(PDF_PAGE page, PDF_OBJ_REF ref, int index);

/**
 *	@brief	add an edit-box.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage.
 *	@param 	rect 	rect in PDF coordinate.
 *	@param	line_clr border color of editbox.
 *	@param	line_w border width of editbox.
 *	@param	fill_clr background of editbox.
 *	@param 	tsize 	text size of the editbox.
 *	@param 	text_clr text color.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotEditbox2(PDF_PAGE page, const PDF_RECT* rect, int line_clr, float line_w, int fill_clr, float tsize, int text_clr);

/**
 *	@brief	add an edit-box.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage.
 *	@param 	matrix 	matrix object that passed to render.
 *	@param 	rect 	rect in PDF coordinate.
 *	@param	line_clr border color of editbox.
 *	@param	line_w border width of editbox.
 *	@param	fill_clr background of editbox.
 *	@param 	tsize 	text size of the editbox.
 *	@param 	text_clr text color.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotEditbox(PDF_PAGE page, PDF_MATRIX matrix, const PDF_RECT* rect, int line_clr, float line_w, int fill_clr, float tsize, int text_clr);
/**
 *	@brief	get items count in combo-box.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	-1: this is not combo. otherwise: items count.
 */
int PDF_Page_getAnnotComboItemCount(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	get item text by index.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	item 	0 based item index, range: [0, PDF_Page_getAnnotComboItemCount() - 1]
 *
 *	@return	String value or null
 */
NSString* PDF_Page_getAnnotComboItem(PDF_PAGE page, PDF_ANNOT annot, int item);
/**
 *  @brief  get export value of combo-box.
 *          this method require premium license
 * 
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	item 	0 based item index, range: [0, PDF_Page_getAnnotComboItemCount() - 1]
 * 
 *	@return	String value or null
 */
NSString* PDF_Page_getAnnotComboItemVal(PDF_PAGE page, PDF_ANNOT annot, int item);
/**
 *	@brief	get index of selected item.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	item index selected.
 */
int PDF_Page_getAnnotComboItemSel(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	set selected item.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
            you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	item 	index of item.
 *
 *	@return	true or false.
 */
bool PDF_Page_setAnnotComboItem(PDF_PAGE page, PDF_ANNOT annot, int item);

/**
 *	@brief	get items count in list-box.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	-1: this is not list-box. otherwise: items count.
 */
int PDF_Page_getAnnotListItemCount(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	can list select more than 1 item?
 *	
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *  @return true or false.
 */
bool PDF_Page_isAnnotListMultiSel(PDF_PAGE page, PDF_ANNOT annot);

/**
 *	@brief	get item text by index.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	item 	0 based item index, range: [0, PDF_Page_getAnnotListItemCount() - 1]
 *
 *	@return	String value or null.
 */
NSString* PDF_Page_getAnnotListItem(PDF_PAGE page, PDF_ANNOT annot, int item);
/**
 * 	@brief	get export value of list-box item.
 *          this method require premium license
 * 
 *  @param 	page 	returned from PDF_Document_getPage
 * 	@param 	annot	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  item    0 based item index. range:[0, PDF_Page_getAnnotListItemCount()-1]
 * 
 *	@return	String value or null.
 */
NSString* PDF_Page_getAnnotListItemVal(PDF_PAGE page, PDF_ANNOT annot, int item);
/**
 *	@brief	get selected items of list-box.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page		page object returned from PDF_Document_getPage
 *	@param 	annot		annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param  sels		int array to revieve selected items.
 *  @param  sels_max	array length.
 *
 *	@return	the count that filled in int array.
 */
int PDF_Page_getAnnotListSels(PDF_PAGE page, PDF_ANNOT annot, int* sels, int sels_max);
/**
 *	@brief	set selected items of list-box.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page		page object returned from PDF_Document_getPage
 *	@param 	annot		annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param  sels		int array to set selected items.
 *  @param  sels_cnt	array length.
 *
 *	@return	the count that filled in int array.
 */
bool PDF_Page_setAnnotListSels(PDF_PAGE page, PDF_ANNOT annot, const int* sels, int sels_cnt);

/**
 *	@brief	get radio or check-box check status.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	-1 if annotation is not valid control.
             0 if check-box is not checked.
             1 if check-box checked.
             2 if radio-box is not checked.
             3 if radio-box checked.
 */
int PDF_Page_getAnnotCheckStatus(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	set value to check-box.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
            you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	check 	check status.
 *
 *	@return	true or false.
 */
bool PDF_Page_setAnnotCheckValue(PDF_PAGE page, PDF_ANNOT annot, bool check);
/**
 *	@brief	set value to radio-box, and deselect all other radio-box in radio-group.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
            you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	true or false.
 */
bool PDF_Page_setAnnotRadio(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	is this annotation reset button?
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	true or false.
 */
bool PDF_Page_getAnnotReset(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	do reset action.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
            you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	true or false.
 */
bool PDF_Page_setAnnotReset(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	get submit target link.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	String value or null.
 */
NSString* PDF_Page_getAnnotSubmitTarget(PDF_PAGE page, PDF_ANNOT annot);
/**
 *	@brief	get submit parameters.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in premium license.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *	@param 	para 	output value: parameters string buffer.
                    in mail mode: return whole XML string for form data.
                    in http mode: url data likes: "para1=xxx&para2=xxx".
 *	@param 	len 	buffer length.
 *
 *	@return	true or false.
 */
bool PDF_Page_getAnnotSubmitPara(PDF_PAGE page, PDF_ANNOT annot, char* para, int len);
/**
 *  @brief  move annotation to another page.
 *          this method require professional or premium license.
 *          this method just like invoke PDF_Page_copyAnnot() and PDF_Page_removeAnnot(), but less data generated.
 *          Notice: ObjsStart shall be invoked for page_dst.
 *
 *  @param  page_src    source page. returned from PDF_Document_getPage.
 *  @param  page_dst    page destination. returned from PDF_Document_getPage.
 *  @param  annot   annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint.
 *  @param  rect    [left, top, right, bottom] in PDF coordinate in dst_page.
 *
 *  @return true or false.
 */
bool PDF_Page_moveAnnot(PDF_PAGE PDF_Page_src, PDF_PAGE PDF_Page_dst, PDF_ANNOT annot, const PDF_RECT* rect);
/**
 *  @brief  clone an annotation object to this page.
 *          this method need a professional or premium license.
 *
 *  @param  page  returned from PDF_Document_getPage
 *  @param  annot annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint.
 *                Annotation object must be in this document.
 *  @param  rect  [left, top, right, bottom] in PDF coordinate.
 *
 *  @return true or false.
 */
bool PDF_Page_copyAnnot(PDF_PAGE page, PDF_ANNOT annot, const PDF_RECT* rect);

/**
 *	@brief	remove annotation
 *          to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
 *          this function valid in professional or premium license.
 *          you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *
 *	@return	true or false.
 */
bool PDF_Page_removeAnnot(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  add a button field.
 *          this method require premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *  @param  rect [left, top, right, bottom] in PDF coordinate system.
 *  @param  name part field name, example "fld1"
 *  @param  label
 *  @param  app appearence of the button, if it is null, a transparency layer will displayed.
 *  @return true or false.
 */
bool PDF_Page_addFieldButton(PDF_PAGE page, const PDF_RECT* rect, NSString* name, NSString* label, PDF_DOC_FORM app);
/**
 *  @brief  add a checkbox field.
 *          this method require premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *  @param  rect [left, top, right, bottom] in PDF coordinate system.
 *  @param  name part field name, example "fld1"
 *  @param  val the value to export when the checkbox is selected.
 *  @param  app_on appearence for on status. if it is null, a default appearence will be generated by native.
 *  @param  app_off appearence fof off status. if it is null, a default appearence will be generated by native.
 * 
 *  @return true or false
 */
bool PDF_Page_addFieldCheck(PDF_PAGE page, const PDF_RECT* rect, NSString* name, NSString* val, PDF_DOC_FORM app_on, PDF_DOC_FORM app_off);
/**
 *  @brief  add an radiobox field.
 *          this method require premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *  @param  rect  [left, top, right, bottom] in PDF coordinate system.
 *  @param  name  part field name, example "fld1", if the name already exists, then the field is a group, and this new radio add to this radio group.
 *  @param  val the value to export when the checkbox is selected.
 *  @param  app_on  appearence for on status. if it is null, a default appearence will be generated by native.
 *  @param  app_off appearence fof off status. if it is null, a default appearence will be generated by native.
 * 
 *  @return true or false
 */
bool PDF_Page_addFieldRadio(PDF_PAGE page, const PDF_RECT* rect, NSString* name, NSString* val, PDF_DOC_FORM app_on, PDF_DOC_FORM app_off);
/**
 *  @brief  add a Combo field.
 *          this method require premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *  @param  rect  [left, top, right, bottom] in PDF coordinate system.
 *  @param  name  part field name, example "fld1"
 *  @param  opts  option items to select.
 * 
 *  @return true or false
 */
bool PDF_Page_addFieldCombo(PDF_PAGE page, const PDF_RECT* rect, NSString* name, NSArray* opts);
/**
 *  @brief  add a list field.
 *          this method require premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *  @param  rect  [left, top, right, bottom] in PDF coordinate system.
 *  @param  name  part field name, example "fld1"
 *  @param  opts  option items to select.
 *  @param  multi_sel is the list can selected more than 1 item?
 * 
 *  @return true or false
 */
bool PDF_Page_addFieldList(PDF_PAGE page, const PDF_RECT* rect, NSString* name, NSArray* opts, bool multi_sel);
/**
 *  @brief  add an editbox field.
 *          this method require premium license.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *  @param  rect  [left, top, right, bottom] in PDF coordinate system.
 *  @param  name  part field name, example "fld1"
 *  @param  multi_line  is the editbox multi-line?
 *  @param  password  is the editbox password?
 * 
 *  @return true or false.
 */
bool PDF_Page_addFieldEditbox(PDF_PAGE page, const PDF_RECT* rect, NSString* name, bool multi_line, bool password);
/**
 *  @brief  add a empty signature field.
 * 
 *	@param 	page  returned from PDF_Document_getPage
 *  @param  rect  [left, top, right, bottom] in PDF coordinate system.
 *  @param  name  part field name, example "fld1"
 * 
 *  @return true or false
 */
bool PDF_Page_addFieldSign(PDF_PAGE page, const PDF_RECT* rect, NSString* name);
/**
 *	@brief	add ink annotation to page.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in professional or premium license.
            you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	matrix 	 Matrix object passed to PDF_Page_render
 *	@param 	hand 	Ink object returned from Ink_create.
 *	@param 	orgx 	x or origin, in DIB coordinate
 *	@param 	orgy 	y or origin, in DIB coordinate
 *
 *	@return	true or false.
 */
bool PDF_Page_addAnnotInk(PDF_PAGE page, PDF_MATRIX matrix, PDF_INK hand, float orgx, float orgy);

/**
 *	@brief	add ink annotation to page.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in professional or premium license.
            you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	hand 	Ink object returned from Ink_create, must in PDF coordinate.
 *
 *	@return	true or false.
 */
bool PDF_Page_addAnnotInk2(PDF_PAGE page, PDF_INK hand);
/**
 *	@brief	add goto-page link to page.
 *          you should re-render page to display modified data.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method valid in professional or premium version

 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	matrix 	Matrix object passed to PDF_Page_render
 *	@param 	rect 	link area rect [left, top, right, bottom] in DIB coordinate.
 *	@param 	pageno 	 0 based pageno to goto.
 *	@param 	top 	y coordinate in PDF coordinate, page.height is top of page. and 0 is bottom of page.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotGoto(PDF_PAGE page, PDF_MATRIX matrix, const PDF_RECT* rect, int pageno, float top);
/**
 *	@brief	add goto-page link to page.
 *          you should re-render page to display modified data.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method valid in professional or premium version

 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	rect 	link area rect [left, top, right, bottom] in PDF coordinate.
 *	@param 	pageno 	 0 based pageno to goto.
 *	@param 	top 	y coordinate in PDF coordinate, page.height is top of page. and 0 is bottom of page.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotGoto2(PDF_PAGE page, const PDF_RECT* rect, int pageno, float top);
/**
 *	@brief	add URL link to page.
 *          you should re-render page to display modified data.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method valid in professional or premium version
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	matrix 	Matrix object passed to PDF_Page_render
 *	@param 	rect 	link area rect [left, top, right, bottom] in DIB coordinate.
 *	@param 	uri 	url address, example: "http://www.radaee.com/en"
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotUri(PDF_PAGE page, PDF_MATRIX matrix, const PDF_RECT* rect, const char* uri);
/**
 *	@brief	add URL link to page.
 *          you should re-render page to display modified data.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method valid in professional or premium version
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	rect 	link area rect [left, top, right, bottom] in PDF coordinate.
 *	@param 	uri 	url address, example: "http://www.radaee.com/en"
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotURI2(PDF_PAGE page, const PDF_RECT* rect, const char* uri);

/**
 *	@brief	add line to page.
 *          you should re-render page to display modified data.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method valid in professional or premium version
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	matrix 	Matrix object passed to PDF_Page_render
 *	@param 	pt1 	start point, 2 elements for x,y, in DIB coordinate
 *	@param 	pt2 	end point, 2 elements for x,y, in DIB coordinate
 *	@param 	style1 	tyle for start point:
 *          0: None
 *          1: Arrow
 *          2: Closed Arrow
 *          3: Square
 *          4: Circle
 *          5: Butt
 *          6: Diamond
 *          7: Reverted Arrow
 *          8: Reverted Closed Arrow
 *          9: Slash
 *	@param 	style2 	style for end point, values are same as style1.
 *	@param 	width 	line width in DIB coordinate
 *	@param 	color 	line color. same as addAnnotRect.
 *	@param 	icolor 	fill color. same as addAnnotRect.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotLine(PDF_PAGE page, PDF_MATRIX matrix, const PDF_POINT* pt1, const PDF_POINT* pt2, int style1, int style2, float width, int color, int icolor);
/**
 *	@brief	add line to page.
 *          you should re-render page to display modified data.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method valid in professional or premium version
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	pt1 	start point, 2 elements for x,y, in PDF coordinate
 *	@param 	pt2 	end point, 2 elements for x,y, in PDF coordinate
 *	@param 	style1 	tyle for start point:
 *          0: None
 *          1: Arrow
 *          2: Closed Arrow
 *          3: Square
 *          4: Circle
 *          5: Butt
 *          6: Diamond
 *          7: Reverted Arrow
 *          8: Reverted Closed Arrow
 *          9: Slash
 *	@param 	style2 	style for end point, values are same as style1.
 *	@param 	width 	line width in PDF coordinate
 *	@param 	color 	line color. same as addAnnotRect.
 *	@param 	icolor 	fill color. same as addAnnotRect.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotLine2(PDF_PAGE page, const PDF_POINT* pt1, const PDF_POINT* pt2, int style1, int style2, float width, int color, int icolor);
/**
 *	@brief	add rect annotation to page.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in professional or premium license.
            you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	matrix 	Matrix object passed to PDF_Page_render
 *	@param 	rect 	rect in DIB coordinate
 *	@param 	width 	line width in DIB coordinate
 *	@param 	color 	RGB value for rect color
 *	@param 	icolor 	fill color. same as addAnnotRect.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotRect(PDF_PAGE page, PDF_MATRIX matrix, const PDF_RECT* rect, float width, int color, int icolor);
/**
 *	@brief	add rect annotation to page.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in professional or premium license.
            you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	rect 	rect in PDF coordinate
 *	@param 	width 	line width in PDF coordinate
 *	@param 	color 	RGB value for rect color
 *	@param 	icolor 	fill color. same as addAnnotRect.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotRect2(PDF_PAGE page, const PDF_RECT* rect, float width, int color, int icolor);

/**
 *	@brief	add ellipse annotation to page.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in professional or premium license.
            you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	matrix 	Matrix object passed to PDF_Page_render
 *	@param 	rect 	rect in DIB coordinate
 *	@param 	width 	line width in DIB coordinate
 *	@param 	color 	RGB value for rect color
 *	@param 	icolor 	fill color. same as addAnnotRect.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotEllipse(PDF_PAGE page, PDF_MATRIX matrix, const PDF_RECT* rect, float width, int color, int icolor);
/**
 *	@brief	add ellipse annotation to page.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in professional or premium license.
            you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	rect 	rect in PDF coordinate
 *	@param 	width 	line width in PDF coordinate
 *	@param 	color 	RGB value for rect color
 *	@param 	icolor 	fill color. same as addAnnotRect.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotEllipse2(PDF_PAGE page, const PDF_RECT* rect, float width, int color, int icolor);
/**
 *  @brief  add polygon to page.
 *          you should re-render page to display modified data.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method require professional or premium license
 *
 *  @param  page    returned from PDF_Document_getPage
 *  @param  hand    must be a set of unclosed lines. do not container any move-to operation except the first point in the path.
 *  @param  color       stroke color formated as 0xAARRGGBB.
 *  @param  fill_color  fill color, formated as 0xAARRGGBB. if AA == 0, no fill operations, otherwise alpha value is same to stroke color.
 *  @param  width       stroke width in PDF coordinate
 * 
 *  @return true or false.
 *  the added annotation can be obtained by PDF_Page_getAnnot(PDF_Page_getAnnotCount()- 1), if this method return true.
 */
bool PDF_Page_addAnnotPolygon(PDF_PAGE page, PDF_PATH hand, int color, int fill_color, float width);
/**
 *  @brief  add polyline to page.
 *          you should re-render page to display modified data.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method require professional or premium license
 *
 *  @param  page    returned from PDF_Document_getPage
 *  @param  hand    must be a set of unclosed lines. do not container any move-to operation except the first point in the path.
 *  @param  style1    style for start point:
 *                  0: None
 *                  1: Arrow
 *                  2: Closed Arrow
 *                  3: Square
 *                  4: Circle
 *                  5: Butt
 *                  6: Diamond
 *                  7: Reverted Arrow
 *                  8: Reverted Closed Arrow
 *                  9: Slash
 *  @param  style2    style for end point, values are same as style1.
 *  @param  color       stroke color formated as 0xAARRGGBB.
 *  @param  fill_color  fill color, formated as 0xAARRGGBB. if AA == 0, no fill operations, otherwise alpha value is same to stroke color.
 *  @param  width       stroke width in PDF coordinate
 *  @return true or false.
 *  the added annotation can be obtained by PDF_Page_getAnnot(PDF_Page_getAnnotCount()- 1), if this method return true.
 */
bool PDF_Page_addAnnotPolyline(PDF_PAGE page, PDF_PATH hand, int style1, int style2, int color, int fill_color, float width);
/**
 *	@brief	add popup text annotation to page. shows as an text note icon.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in professional or premium license.
            you should re-render page to display modified data
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	matrix 	Matrix object passed to PDF_Page_render
 *	@param 	x 	x in DIB coordinate.
 *	@param 	y 	y in DIB coordinate.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotText(PDF_PAGE page, PDF_MATRIX matrix, float x, float y);
/**
 *	@brief	add popup text annotation to page. shows as an text note icon.
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in professional or premium license.
            you should re-render page to display modified data
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	x 	x in PDF coordinate.
 *	@param 	y 	y in PDF coordinate.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotText2(PDF_PAGE page, float x, float y);
/**
 *	@brief	add bitmap annotation to page
            to invoke this function, developers should call PDF_Page_objsStart or PDF_Page_render before.
            this function valid in professional or premium license.
            you should re-render page to display modified data.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	matrix 	Matrix object passed to PDF_Page_render
 *	@param 	dimg 	bitmap data, must be in RGBA color space.
 *	@param 	rect 	rect in PDF coordinate.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotBitmap(PDF_PAGE page, PDF_MATRIX matrix, PDF_DOC_IMAGE dimg, const PDF_RECT* rect);
bool PDF_Page_addAnnotBitmap2(PDF_PAGE page, PDF_DOC_IMAGE dimg, const PDF_RECT* rect);
/**
 *  @brief  add a RichMedia annotation to page.
 *          you should re-render page to display modified data.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method require professional or premium license, and PDF_Document_setCache invoked.
 *
 *  @param  page    returned from PDF_Document_getPage
 *  @param  path_player     path-name to flash player. example: "/sdcard/VideoPlayer.swf", "/sdcard/AudioPlayer.swf"
 *  @param  path_content    path-name to RichMedia content. example: "/sdcard/video.mp4", "/sdcard/audio.mp3"
 *  @param  type    0: Video, 1: Audio, 2: Flash, 3: 3D
 *                  Video like *.mpg, *.mp4 ...
 *                  Audio like *.mp3 ...
 *  @param  dimage   DocImage object return from PDF_Document_NewImage.
 *  @param  rect    4 elements: left, top, right, bottom in PDF coordinate system.
 *
 *  @return true or false.
 */
bool PDF_Page_addAnnotRichMedia(PDF_PAGE page, NSString* path_player, NSString* path_content, int type, PDF_DOC_IMAGE dimage, const PDF_RECT* rect);
bool PDF_Page_addAnnotPopup(PDF_PAGE page, PDF_ANNOT parent, const PDF_RECT* rect, bool open);
/**
 *	@brief	add a text-markup annotation to page.
 *          you should re-render page to display modified data.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method valid in professional or premium version
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	index1 	first char index
 *	@param 	index2 	second char index
 *	@param 	color 	RGB value
 *	@param 	type 	type as following:
 *                  0: Highlight
 *                  1: Underline
 *                  2: StrikeOut
 *                  3: Highlight without round corner
 *                  4: Squiggly underline.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotMarkup2(PDF_PAGE page, int index1, int index2, int color, int type);
/**
 *  @brief  get markup annotation's boxes.
 *          this method require professional or premium license
 * 
 *  @param  page    returned from PDF_Document_getPage
 *	@param 	annot 	annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  @param  rect    rects in PDF coordinate system, as out values.
 *  @param  cnt     count of rects allocated.
 *
 *  @return rects count that markup annotation has.
 */
int PDF_Page_getAnnotMarkupRects(PDF_PAGE page, PDF_ANNOT annot, PDF_RECT* rects, int cnt);

/**
 * @brief	add an Rubber Stamp to page.
 *		you should re-render page to display modified data.
 *		this can be invoked after ObjsStart or Render or RenderToBmp.
 *		this method valid in professional or premium version
 *	@param page returned from PDF_Document_getPage
 *	@param rect icon area rect [left, top, right, bottom] in PDF coordinate.
 *	@param icon predefined value as below:
 *  0: "Draft"(default icon)
 *  1: "Approved"
 *  2: "Experimental"
 *  3: "NotApproved"
 *  4: "AsIs"
 *  5: "Expired"
 *  6: "NotForPublicRelease"
 *  7: "Confidential"
 *  8: "Final"
 *  9: "Sold"
 * 10: "Departmental"
 * 11: "ForComment"
 * 12: "TopSecret"
 * 13: "ForPublicRelease"
 * 14: "Accepted"
 * 15: "Rejected"
 * 16: "Witness"
 * 17: "InitialHere"
 * 18: "SignHere"
 * 19: "Void"
 * 20: "Completed"
 * 21: "PreliminaryResults"
 * 22: "InformationOnly"
 * 23: "End"
 * @return true or false.
 * the added annotation can be obtained by Page.GetAnnot(Page.GetAnnotCount() - 1), if this method return true.
 */
bool PDF_Page_addAnnotStamp(PDF_PAGE page, const PDF_RECT* rect, int icon);

/**
 *	@brief	add a file as an attachment to page.
 *          you should re-render page to display modified data.
 *          this can be invoked after ObjsStart or Render or RenderToBmp.
 *          this method valid in professional or premium version
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	path 	absolute path name to the file.
 *	@param 	icon 	icon display to the page. values as:
 *                  0: PushPin
 *                  1: Graph
 *                  2: Paperclip
 *                  3: Tag
 *	@param 	rect 	4 elements: left, top, right, bottom in PDF coordinate system.
 *
 *	@return	true or false
 */
bool PDF_Page_addAnnotAttachment(PDF_PAGE page, const char* path, int icon, const PDF_RECT* rect);

/**
 *	@brief	create PAGECONTENT
 *
 *	@return	PDF_PAGECONTENT object
 */
PDF_PAGECONTENT PDF_PageContent_create(void);
/**
 *	@brief	PDF operator: gs_save, save current GraphicState
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 */
void PDF_PageContent_gsSave(PDF_PAGECONTENT content);
/**
 *	@brief	PDF operator: gs_restore, restore GraphicState
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 */
void PDF_PageContent_gsRestore(PDF_PAGECONTENT content);
/**
 *	@brief	PDF operator: set ExtGraphicState
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	gs 	ResGState object created by Page.AddResGState()
 */
void PDF_PageContent_gsSet(PDF_PAGECONTENT content, PDF_PAGE_GSTATE gs);
/**
 *	@brief	PDF operator: set matrix.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	mat 	Matrix object
 */
void PDF_PageContent_gsSetMatrix(PDF_PAGECONTENT content, PDF_MATRIX mat);
/**
 *	@brief	PDF operator: begin text and set text position to (0,0).
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 */
void PDF_PageContent_textBegin(PDF_PAGECONTENT content);
/**
 *	@brief	PDF operator: text end.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 */
void PDF_PageContent_textEnd(PDF_PAGECONTENT content);
/**
 *	@brief	PDF operator: show image.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	img 	 image object created by Page.AddResImage()
 */
void PDF_PageContent_drawImage(PDF_PAGECONTENT content, PDF_PAGE_IMAGE img);
/**
 *  @brief  PDF operator: show form.
 *
 *  @param  content PDF_PAGECONTENT create by PDF_PageContent_create()
 *  @param  frm Form object created by PDF_Page_addResForm()
 */
void PDF_PageContent_drawForm(PDF_PAGECONTENT content, PDF_PAGE_FORM form);
/**
 *	@brief	show text
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	text 	 text to show, '\r' or '\n' in string start a new line.
 */
void PDF_PageContent_drawText(PDF_PAGECONTENT content, const char* text);
/**
 *  @brief  show text with special width
 *
 *  @param  content     PDF_PAGECONTENT create by PDF_PageContent_create()
 *  @param  text        text to show.
 *  @param  align       0:left, 1: middle, 2:right
 *  @param  width       bounding width to draw text
 *
 *  @return line count of this text drawing.
 */
int PDF_PageContent_drawText2(PDF_PAGECONTENT content, const char* text, int align, float width);
/**
 *  @brief  show text with special width and max line count.
 *
 *  @param  content     PDF_PAGECONTENT create by PDF_PageContent_create()
 *  @param  text        text to show.
 *  @param  align       0:left, 1: middle, 2:right
 *  @param  width       bounding width to draw text
 *  @param  max_lines   max line count of this drawing
 *
 *  @return 2 element as [count of unicodes have drawn, line count have drawn].
 */
int PDF_PageContent_drawText3(PDF_PAGECONTENT content, const char* text, int align, float width, int max_lines);
/**
 *	@brief	stroke path.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	path 	Path object
 */
void PDF_PageContent_strokePath(PDF_PAGECONTENT content, PDF_PATH path);
/**
 *	@brief	fill path.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	path 	Path object
 *	@param 	winding 	winding fill rule
 */
void PDF_PageContent_fillPath(PDF_PAGECONTENT content, PDF_PATH path, bool winding);
/**
 *	@brief	set the path as clip path.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	path 	Path object
 *	@param 	winding 	winding fill rule?
 */
void PDF_PageContent_clipPath(PDF_PAGECONTENT content, PDF_PATH path, bool winding);
/**
 *	@brief	PDF operator: set fill and other operations color.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	color 	formatted as 0xRRGGBB, no alpha channel. alpha value shall set by ExtGraphicState(ResGState).
 */
void PDF_PageContent_setFillColor(PDF_PAGECONTENT content, int color);
/**
 *	@brief	PDF operator: set stroke color.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	color 	formatted as 0xRRGGBB, no alpha channel. alpha value shall set by ExtGraphicState(ResGState).
 */
void PDF_PageContent_setStrokeColor(PDF_PAGECONTENT content, int color);
/**
 *	@brief	PDF operator: set line cap
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	cap 	0:butt, 1:round: 2:square
 */
void PDF_PageContent_setStrokeCap(PDF_PAGECONTENT content, int cap);
/**
 *	@brief	PDF operator: set line join
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	join 	0:miter, 1:round, 2:bevel
 */
void PDF_PageContent_setStrokeJoin(PDF_PAGECONTENT content, int join);
/**
 *	@brief	PDF operator: set line width
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	w 	line width in PDF coordinate
 */
void PDF_PageContent_setStrokeWidth(PDF_PAGECONTENT content, float w);
/**
 *	@brief	PDF operator: set miter limit.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	miter 	 miter limit.
 */
void PDF_PageContent_setStrokeMiter(PDF_PAGECONTENT content, float miter);
/**
 *	@brief	PDF operator: set stroke dash pattern.
 *
 *	@param 	content PDF_PAGECONTENT create by PageContent_create()
 *	@param dash float array to define the dash pattern.
 *	@param dash_cnt count of the float array.
 *	@param phase phase value, plz see PDF-Reference1.7 section 4.3.2 and 4.3.3.
 */
void PDF_PageContent_setStrokeDash(PDF_PAGECONTENT content, const float* dash, int dash_cnt, float phase);
/**
 *	@brief	PDF operator: set char space(extra space between chars)
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	space 	char space
 */
void PDF_PageContent_textSetCharSpace(PDF_PAGECONTENT content, float space);
/**
 *	@brief	PDF operator: set word space(extra space between words spit by blank char ' ' ).
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	space 	word space.
 */
void PDF_PageContent_textSetWordSpace(PDF_PAGECONTENT content, float space);
/**
 *	@brief	PDF operator: set text leading, height between 2 text lines.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	leading 	leading in PDF coordinate
 */
void PDF_PageContent_textSetLeading(PDF_PAGECONTENT content, float leading);
/**
 *	@brief	PDF operator: set text rise
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	rise 	<#rise description#>
 */
void PDF_PageContent_textSetRise(PDF_PAGECONTENT content, float rise);
/**
 *	@brief	PDF operator: set horizon scale for chars.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	scale 	100 means scale value 1.0f
 */
void PDF_PageContent_textSetHScale(PDF_PAGECONTENT content, int scale);
/**
 *	@brief	PDF operator: new a text line
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 */
void PDF_PageContent_textNextLine(PDF_PAGECONTENT content);
/**
 *	@brief	PDF operator: move text position relative to previous line
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	x 	in PDF coordinate add to previous line position
 *	@param 	y 	in PDF coordinate add to previous line position
 */
void PDF_PageContent_textMove(PDF_PAGECONTENT content, float x, float y);
/**
 *	@brief	set text font
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	font 	ResFont object created by Page.AddResFont()
 *	@param 	size 	text size in PDF coordinate.
 */
void PDF_PageContent_textSetFont(PDF_PAGECONTENT content, PDF_PAGE_FONT font, float size);
/**
 *	@brief	PDF operator: set text render mode.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 *	@param 	mode -
 *          0: filling
 *          1: stroke
 *          2: fill and stroke
 *          3: do nothing
 *          4: fill and set clip path
 *          5: stroke and set clip path
 *          6: fill/stroke/clip
 *          7: set clip path.
 */
void PDF_PageContent_textSetRenderMode(PDF_PAGECONTENT content, int mode);
/**
 *	@brief	destroy and free memory.
 *
 *	@param 	content PDF_PAGECONTENT create by PDF_PageContent_create()
 */
void PDF_PageContent_destroy(PDF_PAGECONTENT content);
void PDF_PageContent_tagBlockStart(PDF_PAGECONTENT content, PDF_TAG block);
void PDF_PageContent_tagBlockEnd(PDF_PAGECONTENT content);
PDF_TAG PDF_Page_newTagBlock(PDF_PAGE page, PDF_TAG parent, NSString * tag);
/**
 *	@brief	add a font as resource of this page.
 *          a premium license is needed for this method.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	font 	font object created by Document.NewFontCID()
 *
 *	@return	PDF_PAGE_FONT object or null means failed
 */
PDF_PAGE_FONT PDF_Page_addResFont(PDF_PAGE page, PDF_DOC_FONT font);
/**
 *	@brief	add an image as resource of this page.
 *          a premium license is needed for this method.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	image 	image - image object created by Document.NewImage() or Document.NewImageJPEG()
 *
 *	@return	null means failed.
 */
PDF_PAGE_IMAGE PDF_Page_addResImage(PDF_PAGE page, PDF_DOC_IMAGE image);
/**
 *	@brief	add GraphicState as resource of this page.
 *          a premium license is needed for this method.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	gstate 	ExtGraphicState created by Document.NewGState()
 *
 *	@return	null means failed.
 */
PDF_PAGE_GSTATE PDF_Page_addResGState(PDF_PAGE page, PDF_DOC_GSTATE gstate);
/**
 *  @brief  add Form as resource of this page.
 *          a premium license is required for this method.
 *
 *  @param  page    returned from PDF_Document_getPage
 *  @param  form    Form created by PDF_Document_NewForm
 *
 *  @return null means failed.
 */
PDF_PAGE_FORM PDF_Page_addResForm(PDF_PAGE page, PDF_DOC_FORM form);
/**
 *	@brief	add content stream to this page.
 *          a premium license is needed for this method.
 *
 *	@param 	page 	returned from PDF_Document_getPage
 *	@param 	content 	PageContent object called PageContent.create().
 *
 *	@return	true or false
 */
bool PDF_Page_addContent(PDF_PAGE page, PDF_PAGECONTENT content, bool flush);
/**
 *  @brief  Start Reflow.
 *          this method require professional or premium license
 * 
 *	@param 	page    returned from PDF_Document_getPage
 *  @param  width   input width, function calculate height.
 *  @param  ratio   scale base to 72 DPI, 2.0 means 144 DPI. the reflowed text will displayed in scale
 * 
 *  @return the height that reflow needed.
 */
float PDF_Page_reflowStart(PDF_PAGE page, float width, float ratio);
/**
 *  @brief  Reflow to Bitmap object.
 *          this method require professional or premium license
 * 
 *	@param 	page    returned from PDF_Document_getPage
 *  @param  dib dib object to reflow
 *  @param  orgx    origin x coordinate
 *  @param  orgy    origin y coordinate
 * 
 *  @return true or false
 */
bool PDF_Page_reflow(PDF_PAGE page, PDF_DIB dib, float orgx, float orgy);
/**
 *  @brief  get reflow paragraph count.
 *          this method require professional or premium license
 * 
 *	@param 	page    returned from PDF_Document_getPage
 * 
 *  @return count
 */
int PDF_Page_reflowGetParaCount(PDF_PAGE page);
/**
 *  @brief  get one paragraph's char count.
 *          this method require professional or premium license
 * 
 *  @param  page    returned from PDF_Document_getPage
 *  @param  iparagraph  paragraph index range[0, PDF_Page_reflowGetParaCount()-1]
 * 
 *  @return char count
 */
int PDF_Page_reflowGetCharCount(PDF_PAGE page, int iparagraph);
/**
 *  @brief  get char's font width.
 *          this method require professional or premium license
 * 
 *  @param  page    returned from PDF_Document_getPage
 *  @param  iparagraph  paragraph index range[0, PDF_Page_reflowGetParaCount()-1]
 *  @param  ichar   char index range[0, PDF_Page_reflowGetCharCount()]
 * 
 *  @return font width for this char
 */
float PDF_Page_reflowGetCharWidth(PDF_PAGE page, int iparagraph, int ichar);
/**
 *  @brief  get char's font height.
 *          this method require professional or premium license
 * 
 *	@param 	page    returned from PDF_Document_getPage
 *  @param  iparagraph  paragraph index range[0, PDF_Page_reflowGetParaCount()-1]
 *  @param  ichar   char index range[0, PDF_Page_reflowGetCharCount()]
 * 
 *  @return font height for this char
 */
float PDF_Page_reflowGetCharHeight(PDF_PAGE page, int iparagraph, int ichar);
/**
 *  @brief  get char's fill color for display.
 *          this method require professional or premium license
 * 
 *	@param 	page    returned from PDF_Document_getPage
 *  @param  iparagraph  paragraph index range[0, PDF_Page_reflowGetParaCount()-1]
 *  @param  ichar   char index range[0, PDF_Page_reflowGetCharCount()]
 * 
 *  @return color value formatted 0xAARRGGBB, AA: alpha value, RR:red, GG:green, BB:blue
 */
int PDF_Page_reflowGetCharColor(PDF_PAGE page, int iparagraph, int ichar);
/**
 *  @brief  get char's unicode value.
 *          this method require professional or premium license
 * 
 *	@param 	page    returned from PDF_Document_getPage
 *  @param  iparagraph  paragraph index range[0, PDF_Page_reflowGetParaCount()-1]
 *  @param  ichar   char index range[0, PDF_Page_reflowGetCharCount()]
 * 
 *  @return unicode
 */
int PDF_Page_reflowGetCharUnicode(PDF_PAGE page, int iparagraph, int ichar);
/**
 *  @brief  get char's font name.
 *          this method require professional or premium license
 * 
 *	@param 	page    returned from PDF_Document_getPage
 *  @param  iparagraph  paragraph index range[0, PDF_Page_reflowGetParaCount()-1]
 *  @param  ichar   char index range[0, PDF_Page_reflowGetCharCount()]
 * 
 *  @return name string
 */
const char* PDF_Page_reflowGetCharFont(PDF_PAGE page, int iparagraph, int ichar);
/**
 *  @brief  get char's bound box.
 *          this method require professional or premium license
 * 
 *	@param 	page    returned from PDF_Document_getPage
 *  @param  iparagraph  paragraph index range[0, PDF_Page_reflowGetParaCount()-1]
 *  @param  ichar   char index range[0, PDF_Page_reflowGetCharCount()]
 *  @param  orgx    origin x coordinate
 *  @param  orgy    origin y coordinate
 *  @param  rect    output: 4 element as [left, top, right, bottom].
 */
void PDF_Page_reflowGetCharRect(PDF_PAGE page, int iparagraph, int ichar, int orgx, int orgy, PDF_RECT* rect);
/**
 *  @brief  get text from range.
 *          this method require professional or premium license
 * 
 *	@param 	page    returned from PDF_Document_getPage
 *  @param  iparagraph1 first position
 *  @param  ichar1  first position
 *  @param  iparagraph2 second position
 *  @param  ichar2  second position
 *	@param	buf buffer text.
 *	@param	buf_size    size of buffer that allocated.
 * 
 *  @return string value or null
 */
bool PDF_Page_reflowGetText(PDF_PAGE page, int iparagraph1, int ichar1, int iparagraph2, int ichar2, char* buf, int buf_len);
    
int PDF_Page_getPGEditorNodeCount(PDF_PAGE page);
void PDF_Page_setPGEditorModified(PDF_PAGE page, bool modified);
PDF_EDITNODE PDF_Page_getPGEditorNode1(PDF_PAGE page, int idx);
PDF_EDITNODE PDF_Page_getPGEditorNode2(PDF_PAGE page, float pdfx, float pdfy);
bool PDF_Page_renderWithPGEditor(PDF_PAGE page, PDF_DIB dib, PDF_MATRIX matrix, bool show_annots, int quality);
bool PDF_Page_updateWithPGEditor(PDF_PAGE page);
bool PDF_Page_cancelWithPGEditor(PDF_PAGE page);

void PDF_EditNode_destroy(PDF_EDITNODE enode);
void PDF_EditNode_delete(PDF_EDITNODE enode);
int PDF_EditNode_getType(PDF_EDITNODE enode);
void PDF_EditNode_updateRect(PDF_EDITNODE enode);
bool PDF_EditNode_getRect(PDF_EDITNODE enode, PDF_RECT* ret);
void PDF_EditNode_setRect(PDF_EDITNODE enode, const PDF_RECT* rect);
long long PDF_EditNode_getCharPos(PDF_EDITNODE enode, float pdfx, float pdfy);
long long PDF_EditNode_getCharPrev(PDF_EDITNODE enode, long long pos);
long long PDF_EditNode_getCharNext(PDF_EDITNODE enode, long long pos);
bool PDF_EditNode_getCharRect(PDF_EDITNODE enode, long long pos, PDF_RECT* ret);
void PDF_EditNode_charDelete(PDF_EDITNODE enode, long long start, long long end);
long long PDF_EditNode_charInsert(PDF_EDITNODE enode, long long pos, NSString* val);
NSString* PDF_EditNode_charGetString(PDF_EDITNODE enode, long long pos0, long long pos1);
void PDF_EditNode_charReturn(PDF_EDITNODE enode, long long pos);
void PDF_EditNode_setDefFont(NSString* fname);
void PDF_EditNode_setDefCJKFont(NSString* fname);


PDF_OBJ PDF_Obj_create();
void PDF_Obj_destroy(PDF_OBJ hand);
/**
 *  @brief  get item count of dictionary or stream obj
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 * 
 *  @return count.
 */
int PDF_Obj_dictGetItemCount(PDF_OBJ hand);
/**
 *  @brief  get item name of dictionary or stream by index
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  index   0 based index value.
 * 
 *  @return tag of item.
 */
const char* PDF_Obj_dictGetItemName(PDF_OBJ hand, int index);
/**
 *  @brief  get item of dictionary or stream by index
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  index   0 based index value.
 * 
 *  @return PDF object data.
 */
PDF_OBJ PDF_Obj_dictGetItemByIndex(PDF_OBJ hand, int index);
/**
 *  @brief  get item of dictionary or stream by tag
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  name    name same as Obj_dictGetItemName
 * 
 *  @return PDF object data.
 */
PDF_OBJ PDF_Obj_dictGetItemByName(PDF_OBJ hand, const char* name);
/**
 *  @brief  set empty object to item by tag.
 *          u can use DictGetItem(key) to get object, after DictSetItem.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  name    name same as Obj_dictGetItemName
 */
void PDF_Obj_dictSetItem(PDF_OBJ hand, const char* name);
/**
 *  @brief  remove item by tag
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  name    name of item, same as PDF_Obj_dictGetItemName
 */
void PDF_Obj_dictRemoveItem(PDF_OBJ hand, const char* name);
/**
 *  @brief  get item count of array
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 * 
 *  @return count.
 */
int PDF_Obj_arrayGetItemCount(PDF_OBJ hand);
/**
 *  @brief  get item of array by index.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  index   0 based index.
 * 
 *  @return PDF object data.
 */
PDF_OBJ PDF_Obj_arrayGetItem(PDF_OBJ hand, int index);
/**
 *  @brief  add an empty object to tail of array.
 *          you can use PDF_Obj_arrayGetItem(PDF_Obj_arrayGetItemCount() - 1) to get Object after PDF_Obj_arrayInsertItem()
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 */
void PDF_Obj_arrayAppendItem(PDF_OBJ hand);
/**
 *  @brief  insert an empty object to array by position.
 *          you can use PDF_Obj_arrayGetItem(index) to get Object after PDF_Obj_arrayInsertItem()
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  index   0 based index
 */
void PDF_Obj_arrayInsertItem(PDF_OBJ hand, int index);
/**
 *  @brief  remove an item from array
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  index   0 based index.
 */
void PDF_Obj_arrayRemoveItem(PDF_OBJ hand, int index);
/**
 *  @brief  remove all items from array.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 */
void PDF_Obj_arrayClear(PDF_OBJ hand);
/**
 *  @brief  get boolean value from object.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 * 
 *  @return boolean value.
 */
bool PDF_Obj_getBoolean(PDF_OBJ hand);
/**
 *  @brief  set boolean value to object, and set object type to boolean.
 * 
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param v boolean value
 */
void PDF_Obj_setBoolean(PDF_OBJ hand, bool v);
/**
 *  @brief  get int value from object.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 * 
 *  @return int value.
 */
int PDF_Obj_getInt(PDF_OBJ hand);
/**
 *  @brief  set int value to object, and set object type to int.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  v   int value
 */
void PDF_Obj_setInt(PDF_OBJ hand, int v);
/**
 *  @brief  get float value from object.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 * 
 *  @return float value.
 */
float PDF_Obj_getReal(PDF_OBJ hand);
/**
 *  @brief  set float value to object, and set object type to float.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  v   float value
 */
void PDF_Obj_setReal(PDF_OBJ hand, float v);
/**
 *  @brief  get name value from object.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 * 
 *  @return name value.
 */
const char* PDF_Obj_getName(PDF_OBJ hand);
/**
 *  @brief  set name value to object, and set object type to name.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  v   name value
 */
void PDF_Obj_setName(PDF_OBJ hand, const char* v);
/**
 *  @brief  get string value from object.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 * 
 *  @return ascii string value.
 */
NSString* PDF_Obj_getAsciiString(PDF_OBJ hand);
/**
 *  @brief  get string value from object.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 * 
 *  @return Unicode string value.
 */
NSString* PDF_Obj_getTextString(PDF_OBJ hand);
/**
 *  @brief  get string value from object.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  len binary string length
 * 
 *  @return binary string value.
 */
unsigned char* PDF_Obj_getHexString(PDF_OBJ hand, int* len);
/**
 *  @brief  set ascii string value to object, and set object type to string.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  v   ascii string value
 */
void PDF_Obj_setAsciiString(PDF_OBJ hand, const char* v);
/**
 *  @brief  set unicode string value to object, and set object type to string.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  v   unicode string value
 */
void PDF_Obj_setTextString(PDF_OBJ hand, const char* v);
/**
 *  @brief  set binary string value to object, and set object type to string.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  v   binary string value
 *  @param  len binary string length
 */
void PDF_Obj_setHexString(PDF_OBJ hand, unsigned char* v, int len);
/**
 *  @brief  get cross reference from object.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 * 
 *  @return cross reference item.
 */
PDF_OBJ_REF PDF_Obj_getReference(PDF_OBJ hand);
/**
 *  @brief  set cross reference to object, and set object type to reference.
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 *  @param  v   cross reference
 */
void PDF_Obj_setReference(PDF_OBJ hand, PDF_OBJ_REF v);
/**
 *  @brief  get type of object
 * 
 *  @param  hand    PDF object data, which got from:
 *                  PDF_Obj_dictGetItemByIndex()
 *                  PDF_Obj_dictGetItemByName()
 *                  PDF_Obj_arrayGetItem()
 * 
 *  @return object type as following:
 *          null = 0,
 *          boolean = 1,
 *          int = 2,
 *          real = 3,
 *          string = 4,
 *          name = 5,
 *          array = 6,
 *          dictionary = 7,
 *          reference = 8,
 *          stream = 9,
 *          others unknown.,
 */
int PDF_Obj_getType(PDF_OBJ hand);
/**
 *  @brief  advanced function to get object from Document to edit.
 *          this method require premium license.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  ref PDF cross reference ID, which got from:
 *              PDF_Document_advNewIndirectObj()
 *              PDF_Document_advGetRef()
 *              PDF_Page_advGetRef()
 *              PDF_Document_advNewFlateStream()
 *              PDF_Document_advNewRawStream()
 * 
 *  @return PDF Object or null.
 */
PDF_OBJ PDF_Document_advGetObj(PDF_DOC doc, PDF_OBJ_REF ref);
/**
 *  @brief  advanced function to create an empty indirect object to edit.
 *          this method require premium license.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 * 
 *  @return PDF cross reference to new object, using PDF_Document_advGetObj to get Object data.
 */
PDF_OBJ_REF PDF_Document_advNewIndirectObj(PDF_DOC doc);
/**
 *  @brief  advanced function to create an indirect object, and then copy source object to this indirect object.
 *          this method require premium license.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  obj_hand    source object to be copied.
 *  
 *  @return PDF cross reference to new object, using PDF_Document_advGetObj to get Object data.
 */
PDF_OBJ_REF PDF_Document_advNewIndirectObjWithData(PDF_DOC doc, PDF_OBJ PDF_Obj_hand);
/**
 *  @brief  advanced function to get reference of catalog object(root object of PDF).
 *          this method require premium license.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 * 
 *  @return PDF cross reference to new object, using PDF_Document_advGetObj to get Object data.
 */
PDF_OBJ_REF PDF_Document_advGetRef(PDF_DOC doc);
/**
 *  @brief  advanced function to reload document objects.
 *          this method require premium license.
 *          all pages object return from PDF_Document_GetPage() shall not available, after this method invoked.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 */
void PDF_Document_advReload(PDF_DOC doc);
/**
 *  @brief  advanced function to create a stream using zflate compression(zlib).
 *          stream byte contents can't modified, once created.
 *          the byte contents shall auto compress and encrypt by native library.
 *          this method require premium license, and need PDF_Document_SetCache() invoked.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  source  source data
 *  @param  len source length
 * 
 *  @return PDF cross reference to new object, using PDF_Document_advGetObj to get Object data.
 */
PDF_OBJ_REF PDF_Document_advNewFlateStream(PDF_DOC doc, const unsigned char* source, int len);
/**
 *  @brief  advanced function to create a stream using raw data.
 *          if u pass compressed data to this method, u shall modify dictionary of this stream.
 *          like "Filter" and other item from dictionary.
 *          the byte contents shall auto encrypt by native library, if document if encrypted.
 *          this method require premium license, and needPDF_ Document_setCache() invoked.
 * 
 *	@param 	doc Document object returned from PDF_Document_open
 *  @param  source  source data
 *  @param  len source length
 * 
 *  @return PDF cross reference to new object, using PDF_Document_advGetObj to get Object data.
 */
PDF_OBJ_REF PDF_Document_advNewRawStream(PDF_DOC doc, const unsigned char* source, int len);
/**
 *  @brief  advanced function to get reference of annotation object.
 *          this method require premium license.
 *  
 *  @param  page    returned from PDF_Document_getPage
 *  @param  annot   annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 *  
 *  @return reference
 */
PDF_OBJ_REF PDF_Page_advGetAnnotRef(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  advanced function to get reference of Page object.
 *          this method require premium license.
 * 
 *  @param  page    returned from PDF_Document_getPage
 * 
 *  @return reference
 */
PDF_OBJ_REF PDF_Page_advGetRef(PDF_PAGE page);
/**
 *  @brief  advanced function to reload annotation object, after advanced methods update annotation object data.
 *          this method require premium license.
 * 
 *  @param  page    returned from PDF_Document_getPage
 *	@param 	annot   annotation object returned from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint
 */
void PDF_Page_advReloadAnnot(PDF_PAGE page, PDF_ANNOT annot);
/**
 *  @brief  advanced function to reload page object, after advanced methods update Page object data.
 *          all annotations return from PDF_Page_getAnnot or PDF_Page_getAnnotFromPoint shall not available. after this method invoked.
 *          this method require premium license.
 * 
 *  @param  page    returned from PDF_Document_getPage
 */
void PDF_Page_advReload(PDF_PAGE page);

#ifdef __cplusplus
}
#endif
