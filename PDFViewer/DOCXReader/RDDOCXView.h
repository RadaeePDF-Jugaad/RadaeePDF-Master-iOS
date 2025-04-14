//
//  RDDOCXView.h
//  PDFViewer
//
//  Created by Radaee on 2020/8/31.
//

#pragma once
#import "DOCXLayoutView.h"

@class DOCXLayoutView;
@interface RDDOCXCanvas : UIView
{
    DOCXLayoutView *m_view;
}
-(void)setView :(DOCXLayoutView *)view;
@end

@interface RDDOCXView : UIView
{
    DOCXLayoutView *m_view;
    RDDOCXCanvas *m_canvas;
}
-(id)initWithFrame:(CGRect)frame;
-(id)initWithCoder:(NSCoder *)aDecoder;
-(DOCXLayoutView *)view;
-(RDDOCXCanvas *)canvas;
@end

