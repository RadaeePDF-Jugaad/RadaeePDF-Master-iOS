//
//  RDAppDelegate.m
//  PDFViewer
//
//  Created by Radaee on 12-10-29.
//  Copyright (c) 2012年 __Radaee__. All rights reserved.
//

#import "RDAppDelegate.h"
#import "RDComm.h"
#import "RDVGlobal.h"

@implementation RDAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [[UIBarButtonItem appearance] setTintColor:[RDUtils radaeeIconColor]];
    [[UIButton appearance] setTintColor:[RDUtils radaeeIconColor]];
    [[UIButton appearance] setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    [[UIImageView appearance] setTintColor:[RDUtils radaeeIconColor]];
    [[UITableViewCell appearance] setTintColor:[RDUtils radaeeIconColor]];

		//binding to app ID "com.radaee.reader", can active version before "2025".
		//the version string can be retrieved by Global_getVersion().
    g_serial = @"A4DBD93776CD8C5CF8C77A3B163AA1262BD9AC5C8F24D696588988ECC6706EBE62620F71124A557686A2811D38A2FD22";

    [RDVGlobal Init];

    return YES;
}

- (UISceneConfiguration *)application:(UIApplication *)application configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession options:(UISceneConnectionOptions *)options
{
    UISceneConfiguration *configuration = [[UISceneConfiguration alloc] initWithName:@"Default Configuration" sessionRole:connectingSceneSession.role];
    configuration.delegateClass = NSClassFromString(@"RDSceneDelegate");
    return configuration;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}
@end
