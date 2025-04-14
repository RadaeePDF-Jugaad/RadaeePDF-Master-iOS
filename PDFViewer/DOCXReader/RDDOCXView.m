//
//  RDDOCXView.m
//  PDFViewer
//
//  Created by Radaee Lou on 2020/8/31.
//

#import <Foundation/Foundation.h>
#import "RDDOCXView.h"

@implementation RDDOCXCanvas
-(void)setView :(DOCXLayoutView *)view
{
    m_view = view;
    [self setBackgroundColor:[UIColor colorWithRed:0 green:0 blue:0 alpha:0]];
    [self setUserInteractionEnabled:NO];
}

-(void)drawRect:(CGRect)rect
{
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    [m_view onDrawOffScreen:ctx];
}

@end

@implementation RDDOCXView
-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if(self)
    {
        m_view = [[DOCXLayoutView alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)];
        m_canvas = [[RDDOCXCanvas alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)];
        [m_canvas setView:m_view];
        m_canvas.autoresizesSubviews = YES;
        m_canvas.autoresizingMask = UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight;
        m_view.autoresizesSubviews = YES;
        m_view.autoresizingMask = UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight;
        [self addSubview:m_view];
        [self addSubview:m_canvas];
    }
    return self;
}
-(id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if(self)
    {
        CGRect frame = self.frame;
        m_view = [[DOCXLayoutView alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)];
        m_canvas = [[RDDOCXCanvas alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)];
        [m_canvas setView:m_view];
        m_canvas.autoresizesSubviews = YES;
        m_canvas.autoresizingMask = UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight;
        m_view.autoresizesSubviews = YES;
        m_view.autoresizingMask = UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight;
        [self addSubview:m_view];
        [self addSubview:m_canvas];
    }
    return self;
}

-(DOCXLayoutView *)view
{
    return m_view;
}
-(RDDOCXCanvas *)canvas
{
    return m_canvas;
}
@end
