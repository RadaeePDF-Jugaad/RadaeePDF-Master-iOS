//
//  RDSceneDelegate.h
//  PDFViewer
//

#import <UIKit/UIKit.h>
#import "RDFileCollectionViewController.h"
#import "MoreViewController.h"

@class RDFileTableController;
@interface RDSceneDelegate : UIResponder <UIWindowSceneDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) UINavigationController *navController;
@property (strong, nonatomic) UITabBarController *tabBarController;
@property (strong, nonatomic) RDFileCollectionViewController *viewController;

- (BOOL)isPortrait;

@end
