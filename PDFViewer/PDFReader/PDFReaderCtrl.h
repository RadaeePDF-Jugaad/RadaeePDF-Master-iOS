//
//  PDFReaderCtrl.h
//  RDPDFReader
//
//  Created by Radaee on 2020/5/5.
//  Copyright © 2020 Radaee. All rights reserved.
//

#pragma once
#import <UIKit/UIKit.h>
#import "PDFLayoutView.h"
#import "PDFThumbView.h"
#import "RDPDFView.h"
#import "PDFObjc.h"
#import "RDToolbar.h"
#import "SearchResultViewController.h"
#import "RDPopupTextViewController.h"
#import "RDTreeViewController.h"
#import "SignatureViewController.h"

#ifdef FTS_ENABLED
#import "FTSSearchTableViewController.h"
#import "RDUtils.h"
#endif

@protocol PDFReaderDelegate<NSObject>
// define protocol functions that can be used in any class using this delegate
- (void)willShowReader;
- (void)didShowReader;
- (void)willCloseReader;
- (void)didCloseReader;
- (void)didChangePage:(int)page;
- (void)didSearchTerm:(NSString *)term found:(BOOL)found;
- (void)didTapOnPage:(int)page atPoint:(CGPoint)point;
- (void)didDoubleTapOnPage:(int)page atPoint:(CGPoint)point;
- (void)didLongPressOnPage:(int)page atPoint:(CGPoint)point;
- (void)didTapOnAnnotationOfType:(int)type atPage:(int)page atPoint:(CGPoint)point;
@end;


@class PDFPopupCtrl;
@class MenuAnnotOp;

#ifndef FTS_ENABLED
@interface PDFReaderCtrl : UIViewController <PDFLayoutDelegate, PDFThumbViewDelegate, SearchResultViewControllerDelegate, RDPopupTextViewControllerDelegate, RDTreeViewControllerDelegate, SignatureDelegate>
#else
@interface PDFReaderCtrl : UIViewController <PDFLayoutDelegate, PDFThumbViewDelegate, SearchResultViewControllerDelegate, RDPopupTextViewControllerDelegate, RDTreeViewControllerDelegate, SignatureDelegate, FTSSearchResultDelegate>
#endif
{
    __weak IBOutlet RDPDFView *mView;
    __weak IBOutlet RDPDFThumb *mThumb;
    __weak IBOutlet UIView *mSliderView;
    __weak IBOutlet UISlider *mSlider;
    __weak IBOutlet UILabel *mSliderLabel;
    __weak IBOutlet UIToolbar *mBarNoneTop;
    __weak IBOutlet RDToolbar *mBarNoneBottom;
    __weak IBOutlet UIBarButtonItem *mBarThumbButton;
    __weak IBOutlet RDToolbar *mBarAnnot;
    __weak IBOutlet UIBarButtonItem *mBarAnnotColorButton;
    __weak IBOutlet UIBarButtonItem *mBarAnnotDoneButton;
    __weak IBOutlet UIToolbar *mBarSearchTop;
    __weak IBOutlet RDToolbar *mBarSearchBottom;
    __weak IBOutlet UIBarButtonItem *mBarSearchResults;
    __weak IBOutlet UITextField *mSearchText;
    __weak IBOutlet UIButton *mSearchWhole;
    __weak IBOutlet UIButton *mSearchCase;
    __weak IBOutlet UILabel *fileName;
    __weak IBOutlet UIBarButtonItem *mBtnBack;
    __weak IBOutlet UIBarButtonItem *mBtnCancel;
    __weak IBOutlet UIBarButtonItem *mBtnDone;
    __weak IBOutlet UIBarButtonItem *mBtnPrev;
    __weak IBOutlet UIBarButtonItem *mBtnNext;
    
    PDFLayoutView *m_view;
    PDFThumbView *m_thumb;
    BOOL m_readonly;
    int m_page_no;
    int m_page_cnt;
    RDPDFDoc *m_doc;
    PDFPopupCtrl *m_popup;
    MenuAnnotOp *m_menu_op;
    UIMenuController *selectMenu;
    int m_annot_type;
    NSString *m_fstr;
    BOOL m_whole;
    BOOL m_case;
    BOOL showingThumb;
    BOOL findStart;
    
    UITapGestureRecognizer *searchTapNone;
    UITapGestureRecognizer *searchTapField;
    
#ifdef FTS_ENABLED
    FTSSearchTableViewController *ftsTableViewController;
#endif

}
@property (nonatomic, assign) id <PDFReaderDelegate> delegate;
@property (strong, nonatomic) UIImage *closeImage;
@property (strong, nonatomic) UIImage *viewModeImage;
@property (strong, nonatomic) UIImage *searchImage;
@property (strong, nonatomic) UIImage *bookmarkImage;
@property (strong, nonatomic) UIImage *addBookmarkImage;
@property (strong, nonatomic) UIImage *outlineImage;
@property (strong, nonatomic) UIImage *lineImage;
@property (strong, nonatomic) UIImage *rowImage;
@property (strong, nonatomic) UIImage *rectImage;
@property (strong, nonatomic) UIImage *ellipseImage;
@property (strong, nonatomic) UIImage *bitmapImage;
@property (strong, nonatomic) UIImage *noteImage;
@property (strong, nonatomic) UIImage *signatureImage;
@property (strong, nonatomic) UIImage *printImage;
@property (strong, nonatomic) UIImage *shareImage;
@property (strong, nonatomic) UIImage *gridImage;
@property (strong, nonatomic) UIImage *deleteImage;
@property (strong, nonatomic) UIImage *doneImage;
@property (strong, nonatomic) UIImage *removeImage;
@property (strong, nonatomic) UIImage *prevImage;
@property (strong, nonatomic) UIImage *nextImage;
@property (strong, nonatomic) UIImage *undoImage;
@property (strong, nonatomic) UIImage *redoImage;
@property (strong, nonatomic) UIImage *performImage;
@property (strong, nonatomic) UIImage *moreImage;
@property (strong, nonatomic) UIImage *drawImage;
@property (strong, nonatomic) UIImage *selectImage;
@property (strong, nonatomic) UIImage *saveImage;
@property (strong, nonatomic) UIImage *metaImage;

@property (nonatomic) BOOL hideSearchImage;
@property (nonatomic) BOOL hideDrawImage;
@property (nonatomic) BOOL hideSelImage;
@property (nonatomic) BOOL hideUndoImage;
@property (nonatomic) BOOL hideRedoImage;
@property (nonatomic) BOOL hideMoreImage;
@property (nonatomic) BOOL hideGridImage;

- (void)setDoc:(RDPDFDoc *)doc;
- (void)setDoc:(RDPDFDoc *)doc :(BOOL)readonly;
- (void)setDoc:(RDPDFDoc *)doc :(int)pageno :(BOOL)readonly;
- (RDPDFDoc *)getDoc;
- (void)PDFGoto:(int)pageno;
- (int)PDFCurPage;
- (void)setImmersive:(BOOL)immersive;
- (void)setFirstPageCover:(BOOL)cover;
- (void)setDoubleTapZoomMode:(int)mode;
- (void)setThumbnailBGColor:(int)color;
- (void)setReaderBGColor:(int)color;
- (BOOL)addAttachmentFromPath:(NSString *)path;
- (BOOL)saveImageFromAnnotAtIndex:(int)index atPage:(int)pageno savePath:(NSString *)path size:(CGSize )size;
+ (bool)flatAnnotAtPage:(int)page doc:(RDPDFDoc *)doc;
- (bool)flatAnnots;
- (bool)saveDocumentToPath:(NSString *)path;
- (void)viewDidLoad;
- (IBAction)back_pressed:(id)sender;
- (IBAction)mode_pressed:(id)sender;
- (IBAction)thumb_pressed:(id)sender;
- (IBAction)tool_pressed:(id)sender;
- (IBAction)annot_pressed:(id)sender;
- (IBAction)search_pressed:(id)sender;
- (IBAction)annot_ok:(id)sender;
- (IBAction)annot_cancel:(id)sender;
- (IBAction)annot_color:(id)sender;
- (IBAction)search_cancel:(id)sender;
- (IBAction)search_backward:(id)sender;
- (IBAction)search_forward:(id)sender;
@end
