//
//  DOCXReaderCtrl.h
//  PDFViewer
//
//  Created by Radaee on 2020/8/31.
//
#import "RDDOCXView.h"
#import "RDToolbar.h"
#import "PDFPopupCtrl.h"

@interface DOCXReaderCtrl : UIViewController<DOCXViewDelegate>
{
    __weak IBOutlet UIToolbar *mToolBar;
    __weak IBOutlet UIToolbar *mSearchBar;
    __weak IBOutlet UIBarButtonItem *fileName;
    __weak IBOutlet RDToolbar *mSearchBarBottom;
    __weak IBOutlet RDDOCXView *mView;
    __weak IBOutlet UITextField *mSearchText;
    DOCXDoc *m_doc;
    DOCXLayoutView *m_view;
    NSString *m_fstr;
    UIMenuController *selectMenu;
    PDFPopupCtrl *m_popup;
    UITapGestureRecognizer *searchTapNone;
    UITapGestureRecognizer *searchTapField;
}
- (IBAction)back_pressed:(id)sender;
- (IBAction)select_pressed:(id)sender;
- (IBAction)search_pressed:(id)sender;
- (IBAction)sprev_pressed:(id)sender;
- (IBAction)snext_pressed:(id)sender;
- (IBAction)sclose_pressed:(id)sender;
- (void)setDoc:(DOCXDoc *)doc;
@end
