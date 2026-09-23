//
//  RDSceneDelegate.m
//  PDFViewer
//

#import "RDSceneDelegate.h"
#import "RDUtils.h"

@implementation RDSceneDelegate

- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)connectionOptions
{
    if (![scene isKindOfClass:[UIWindowScene class]]) {
        return;
    }
    UIWindowScene *windowScene = (UIWindowScene *)scene;

    self.window = [[UIWindow alloc] initWithWindowScene:windowScene];
    if ([self isPortrait]) {
        NSLog(@"portrait");
    }
    else
    {
        NSLog(@"landscape");
    }
    // Override point for customization after application launch.
    self.window.backgroundColor = [RDUtils radaeeWhiteColor];
    NSMutableArray *localControllesArray = [[NSMutableArray alloc]initWithCapacity:4];
    RDFileCollectionViewController *ctl = [[RDFileCollectionViewController alloc] initWithNibName:@"RDFileCollectionViewController" bundle:nil];
    self.navController = [[UINavigationController alloc] initWithRootViewController:ctl];
    [localControllesArray addObject:self.navController];

    NSString *title4 =[[NSString alloc]initWithFormat:NSLocalizedString(@"More", @"Localizable")];
    // Do any additional setup after loading the view from its nib.
    MoreViewController *moreCtl = [[MoreViewController alloc]initWithNibName:@"MoreViewController" bundle:nil];
    self.navController = [[UINavigationController alloc] initWithRootViewController:moreCtl];
    UITabBarItem *item3 = [[UITabBarItem alloc] initWithTitle:title4 image:[UIImage imageNamed:@"btn_info"] tag:3 ];
    moreCtl.tabBarItem = item3;
    [localControllesArray addObject:self.navController];

    self.tabBarController = [[UITabBarController alloc]init];
    self.tabBarController.viewControllers = localControllesArray;

    // Preloads keyboard so there's no lag on initial keyboard appearance.
    UITextField *lagFreeField = [[UITextField alloc] init];
    [self.window addSubview:lagFreeField];
    [lagFreeField becomeFirstResponder];
    [lagFreeField resignFirstResponder];
    [lagFreeField removeFromSuperview];

    [self customizeAppearance];

    self.window.rootViewController = self.tabBarController;
    [self.window makeKeyAndVisible];
}

- (void)customizeAppearance
{
    [[UINavigationBar appearance] setTintColor:[RDUtils radaeeIconColor]];
    [[UITabBar appearance] setTintColor:[RDUtils radaeeIconColor]];
    [[UIToolbar appearance] setTintColor:[RDUtils radaeeIconColor]];
    [[NSClassFromString(@"UICalloutBarButton") appearance] setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
}

- (void)sceneDidDisconnect:(UIScene *)scene
{
}

- (void)sceneDidBecomeActive:(UIScene *)scene
{
}

- (void)sceneWillResignActive:(UIScene *)scene
{
}

- (void)sceneWillEnterForeground:(UIScene *)scene
{
}

- (void)sceneDidEnterBackground:(UIScene *)scene
{
}

#pragma mark - Device orientation

- (BOOL)isPortrait
{
    return ([[UIApplication sharedApplication] statusBarOrientation] == UIInterfaceOrientationPortrait ||
            [[UIApplication sharedApplication] statusBarOrientation] == UIInterfaceOrientationPortraitUpsideDown);
}

@end
