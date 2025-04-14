//
//  DOCXReaderCtrl.m
//  PDFViewer
//
//  Created by Radaee Lou on 2020/8/31.
//

#import <Foundation/Foundation.h>
#import "DOCXReaderCtrl.h"
#import "MenuSearch.h"

@implementation DOCXReaderCtrl
- (void)show_error:(NSString *)msg
{
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Error" message:msg preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *conform = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
    }];
    [alert addAction:conform];
    [self presentViewController:alert animated:YES completion:nil];
}

-(void)enter_none
{
    [mToolBar setHidden:NO];
    [mSearchBar setHidden:YES];
    [mSearchBarBottom setHidden:YES];
}
-(void)enter_search
{
    [mToolBar setHidden:YES];
    [mSearchBar setHidden:NO];
    [mSearchBarBottom setHidden:NO];
}
-(void)enter_select
{
    [self enter_none];
    [mToolBar setHidden:YES];
    [mSearchBar setHidden:YES];
}

- (void)loadDOCX
{
    if(!m_view) m_view = [mView view];
    if(!m_view) return;
    [m_view DOCXOpen:m_doc :4 :[mView canvas] :self];
    [self enter_none];
}

- (void)setDoc:(DOCXDoc *)doc
{
    m_doc = doc;
    [self loadDOCX];
}
- (void)OnPageChanged :(int)pageno
{
}
- (void)OnPageUpdated :(int)pageno
{
}
- (void)OnLongPressed:(float)x :(float)y
{
}
- (void)OnSingleTapped:(float)x :(float)y
{
    if(mToolBar.isHidden)
        [mToolBar setHidden:NO];
    else
        [mToolBar setHidden:YES];
}
- (void)OnDoubleTapped:(float)x :(float)y
{
}
- (void)OnFound:(bool)found
{
    if(!found) [self show_error:@"no more found"];
}
- (void)OnSelStart:(float)x :(float)y
{
}
- (void)OnSelEnd:(float)x1 :(float)y1 :(float)x2 :(float)y2
{
    NSString *s = [m_view vSelGetText];
    NSLog(@"OnSelEnd select text = %@",s);
    if(s)
    {
        //popup a menu
        [selectMenu setTargetRect:CGRectMake(x2 * m_view.zoomScale, y2 * m_view.zoomScale, 0, 0) inView:m_view];
        [selectMenu setMenuVisible:YES animated:YES];
    }
}
- (void)OnPageGoto:(int)pageno
{
}
- (void)OnOpenURL:(NSString *)url
{
    //[self show_error:[@"no implement to open:" stringByAppendingString:url]];
    //open URI
    if( url ){
        UIAlertController* alert = [UIAlertController alertControllerWithTitle:NSLocalizedString(@"Alert", @"Localizable")
                                                                       message:[NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"Do you want to open:", @"Localizable"), url]
                                                                preferredStyle:UIAlertControllerStyleAlert];
        
        UIAlertAction* ok = [UIAlertAction
                             actionWithTitle:NSLocalizedString(@"OK", nil)
                             style:UIAlertActionStyleDefault
                             handler:^(UIAlertAction * action)
                             {
                                 [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url] options:@{} completionHandler:nil];
                             }];
        UIAlertAction* cancel = [UIAlertAction
                                 actionWithTitle:NSLocalizedString(@"Cancel", nil)
                                 style:UIAlertActionStyleDefault
                                 handler:nil];
        
        [alert addAction:ok];
        [alert addAction:cancel];
        [self presentViewController:alert animated:YES completion:nil];
    }
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self loadDOCX];
}

-(void)viewWillAppear:(BOOL)animated
{
    if(self.navigationController)
        self.navigationController.navigationBarHidden = YES;
    [self initialPopupView];
    fileName.title = [GLOBAL.g_pdf_name stringByDeletingPathExtension];
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    if ([m_popup isViewLoaded]) {
        [self dismissViewControllerAnimated:NO completion:^{
            self->m_popup = nil;
        }];
    }
}

- (void)PDFClose
{
    [m_view DOCXClose];
    m_view = nil;
    m_doc = nil;
    fileName.title = @"";
    if(self.navigationController) [self.navigationController popViewControllerAnimated:YES];
    else [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)back_pressed:(id)sender
{
    [self PDFClose];
    self.navigationController.navigationBarHidden = NO;
    [self.navigationController popViewControllerAnimated:YES];
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)select_pressed:(id)sender
{
    [self enter_select];
    [m_view vSelStart];
}

- (IBAction)search_pressed:(id)sender
{
    [self enter_search];
    [mSearchText becomeFirstResponder];
    [mSearchText addTarget:self action:@selector(snext_pressed:) forControlEvents:UIControlEventEditingDidEndOnExit];
    
    // This could be in an init method.
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onSearchKeyboardFrameChanged:) name:UIKeyboardDidChangeFrameNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onSearchKeyboardHiding:) name:UIKeyboardWillHideNotification object:nil];
    

    searchTapNone = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(dismissKeyboard)];
    [self.view addGestureRecognizer:searchTapNone];
    
    searchTapField = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(enter_search)];
    [mSearchText addGestureRecognizer:searchTapField];
}

-(void)dismissKeyboard
{
    [mSearchText resignFirstResponder];
}

- (void)onSearchKeyboardFrameChanged:(NSNotification*)notification
{
    NSDictionary* keyboardInfo = [notification userInfo];
    NSValue* keyboardFrameBegin = [keyboardInfo valueForKey:UIKeyboardFrameBeginUserInfoKey];
    NSValue* keyboardFrameEnd = [keyboardInfo valueForKey:UIKeyboardFrameEndUserInfoKey];
    CGRect keyboardFrameBeginRect = [keyboardFrameBegin CGRectValue];
    CGRect keyboardFrameEndRect = [keyboardFrameEnd CGRectValue];
    
    if ([mSearchText isFirstResponder]) {
        [UIView animateWithDuration:0.33 delay:0 options:UIViewAnimationOptionTransitionCurlUp animations:^{
           if (keyboardFrameBeginRect.origin.y > keyboardFrameEndRect.origin.y) {
               self->mSearchBarBottom.transform = CGAffineTransformMakeTranslation(0, - keyboardFrameEndRect.size.height);
           } else {
               self->mSearchBarBottom.transform = CGAffineTransformIdentity;
           }
        } completion:nil];
    }
}

- (void)onSearchKeyboardHiding:(NSNotification*)notification {
    [UIView animateWithDuration:0.33 delay:0 options:UIViewAnimationOptionTransitionCurlUp animations:^{
       self->mSearchBarBottom.transform = CGAffineTransformIdentity;
    } completion:nil];
}

- (IBAction)sprev_pressed:(id)sender
{
    NSString *pat = mSearchText.text;
    if(!pat || pat.length <= 0) return;
    //BOOL mwhole = mSearchWhole.state == UIControlStateSelected;
    //BOOL mcase = mSearchCase.state == UIControlStateSelected;
    //if (!m_fstr || m_whole != mwhole || m_case != mcase || [m_fstr compare:pat])
    //    [m_view vFindStart:pat :mwhole :mcase];
    if (!m_fstr || [m_fstr compare:pat])
        [m_view vFindStart:pat :GLOBAL.g_case_sensitive :GLOBAL.g_match_whole_word];
    [m_view vFind:-1];
}

- (IBAction)snext_pressed:(id)sender
{
    NSString *pat = mSearchText.text;
    if(!pat || pat.length <= 0) return;
    //BOOL mwhole = mSearchWhole.state == UIControlStateSelected;
    //BOOL mcase = mSearchCase.state == UIControlStateSelected;
    //if (!m_fstr || m_whole != mwhole || m_case != mcase || [m_fstr compare:pat])
    //    [m_view vFindStart:pat :mwhole :mcase];
    if (!m_fstr || [m_fstr compare:pat])
        [m_view vFindStart:pat :GLOBAL.g_case_sensitive :GLOBAL.g_match_whole_word];
    [m_view vFind:1];
}
- (IBAction)sclose_pressed:(id)sender
{
    [mSearchText resignFirstResponder];
    mSearchText.text = @"";
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [self.view removeGestureRecognizer:searchTapNone];
    [self.view removeGestureRecognizer:searchTapField];
    [m_view vSelEnd];
    [self enter_none];
}

- (IBAction)search_tool_pressed:(id)sender
{
    mSearchBarBottom.transform = CGAffineTransformIdentity;
    MenuSearch *view = [[MenuSearch alloc] init:CGPointMake(self.view.center.x - 125, mSearchBarBottom.frame.origin.y - 20) :nil];
    m_popup = [[PDFPopupCtrl alloc] init:view];
    [self presentViewController:m_popup animated:YES completion:nil];
}

#pragma mark - MenuController

- (BOOL)canBecomeFirstResponder
{
    return YES;
}

-(void)initialPopupView
{
    UIMenuItem *copyText = [[UIMenuItem alloc] initWithTitle:@"COPY" action:@selector(copyText:)];
    UIMenuItem *cancel = [[UIMenuItem alloc] initWithTitle:@"CANCEL" action:@selector(endSelect)];
    NSArray *itemsMC = [[NSArray alloc] initWithObjects:copyText, cancel, nil];
    selectMenu = [UIMenuController sharedMenuController];
    [selectMenu setMenuItems:itemsMC];
}
-(void)copyText:(id)sender
{
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(.1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        NSString* s = [self->m_view vSelGetText];
        UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
        pasteboard.string = s;
        [self endSelect];
    });
}

- (void)endSelect
{
    [self enter_none];
    [m_view vSelEnd];
}


@end
