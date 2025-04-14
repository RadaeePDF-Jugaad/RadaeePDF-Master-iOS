//
//  MenuTool.m
//  RDPDFReader
//
//  Created by Radaee Lou on 2020/5/6.
//  Copyright © 2020 Radaee. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MenuTool.h"
#import "RDVGlobal.h"
@implementation MenuTool

- (id)init:(CGPoint)point :(RDBlock)callback
{
    NSMutableArray *items = [[NSMutableArray alloc] initWithObjects:
    @{NSLocalizedString(@"Undo", nil): [UIImage imageNamed:@"btn_undo"]},
    @{NSLocalizedString(@"Redo", nil): [UIImage imageNamed:@"btn_redo"]},
    @{NSLocalizedString(@"Selection", nil): [UIImage imageNamed:@"btn_select"]},
    @{NSLocalizedString(@"Meta", nil): [UIImage imageNamed:@"btn_meta"]},
    @{NSLocalizedString(@"Outlines", nil): [UIImage imageNamed:@"btn_outline"]},
    @{NSLocalizedString(@"Slider", nil): [UIImage imageNamed:@"btn_slider"]},
    @{NSLocalizedString(@"Night mode", nil): [UIImage imageNamed:@"btn_night_mode"]},
    @{NSLocalizedString(@"Manage pages", nil): [UIImage imageNamed:@"btn_manage_page"]},
    @{NSLocalizedString(@"Page Editing", nil): [UIImage imageNamed:@"btn_edit_page"]},
    @{NSLocalizedString(@"Attachments", nil): [UIImage imageNamed:@"btn_attach"]},
                             nil];

    if (!GLOBAL.g_navigation_mode) {
        items[5] = @{NSLocalizedString(@"Thumbnail", nil): [UIImage imageNamed:@"btn_thumb"]};
    }
    if (GLOBAL.g_dark_mode) {
        items[6] = @{NSLocalizedString(@"Light mode", nil): [UIImage imageNamed:@"btn_light_mode"]};
    }
    
    return [super init:point :callback :items];
}

- (void)updateIcons:(UIImage *)iUndo :(UIImage *)iRedo :(UIImage *)iSel
{
    UIImage *icon;
    if(icon = iUndo)//undo
    {
        UIView *view = self.subviews[0];
        UIImageView *img = view.subviews[0];
        img.image = icon;
    }
    if(icon = iRedo)//redo
    {
        UIView *view = self.subviews[1];
        UIImageView *img = view.subviews[0];
        img.image = icon;
    }
    if(icon = iSel)//select
    {
        UIView *view = self.subviews[3];
        UIImageView *img = view.subviews[0];
        img.image = icon;
    }
}

- (void)updateVisible:(BOOL)hideUndo :(BOOL)hideRedo :(BOOL)hideSel
{
    int hcnt = 0;
    UIView *view = self.subviews[0];
    view.hidden = hideUndo;
    if (hideUndo) hcnt++;
    
    view = self.subviews[1];
    view.hidden = hideRedo;
    if (hideRedo) hcnt++;

    view = self.subviews[3];
    view.hidden = hideSel;
    if (hideSel) hcnt++;
    
    CGRect rect = self.frame;
    rect.size.height -= hcnt * 24;
    self.frame = rect;
}
@end
