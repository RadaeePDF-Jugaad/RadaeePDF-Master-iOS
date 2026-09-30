//
//  RDSceneDelegate.m
//  PDFViewer
//

#import "RDSceneDelegate.h"
#import "RDFileCollectionViewController.h"
#import "MoreViewController.h"
#import "RDUtils.h"

@implementation RDSceneDelegate

@synthesize window = _window;
@synthesize navController;
@synthesize tabBarController;

- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)connectionOptions
{
    if (![scene isKindOfClass:[UIWindowScene class]]) return;

    self.window = [[UIWindow alloc] initWithWindowScene:(UIWindowScene *)scene];
    self.window.backgroundColor = [RDUtils radaeeWhiteColor];
    NSMutableArray *localControllesArray = [[NSMutableArray alloc]initWithCapacity:4];
    RDFileCollectionViewController *ctl = [[RDFileCollectionViewController alloc] initWithNibName:@"RDFileCollectionViewController" bundle:nil];
    navController = [[UINavigationController alloc] initWithRootViewController:ctl];
    [localControllesArray addObject:navController];

    NSString *title4 =[[NSString alloc]initWithFormat:NSLocalizedString(@"More", @"Localizable")];
    MoreViewController *moreCtl = [[MoreViewController alloc]initWithNibName:@"MoreViewController" bundle:nil];
    navController = [[UINavigationController alloc] initWithRootViewController:moreCtl];
    UITabBarItem *item3 = [[UITabBarItem alloc] initWithTitle:title4 image:[UIImage imageNamed:@"btn_info"] tag:3 ];
    moreCtl.tabBarItem = item3;
    [localControllesArray addObject:navController];

    tabBarController = [[UITabBarController alloc]init];
    tabBarController.viewControllers =localControllesArray;

    // Preloads keyboard so there's no lag on initial keyboard appearance.
    UITextField *lagFreeField = [[UITextField alloc] init];
    [self.window addSubview:lagFreeField];
    [lagFreeField becomeFirstResponder];
    [lagFreeField resignFirstResponder];
    [lagFreeField removeFromSuperview];

    self.window.rootViewController = self.tabBarController;
    [self.window makeKeyAndVisible];
}

@end
