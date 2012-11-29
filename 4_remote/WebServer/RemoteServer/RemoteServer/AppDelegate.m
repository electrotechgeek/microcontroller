//
//  AppDelegate.m
//  RemoteServer
//
//  Created by Jason Dreisbach on 11/28/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import "AppDelegate.h"
#import "RemoteHTTPServer.h"

@implementation AppDelegate

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
        // Make our http server
        self.httpServer = [[RemoteHTTPServer alloc] init];
        [self.httpServer setPort:8888];
        NSError *err = nil;
        [self.httpServer start:&err];
        if (err != nil)
            NSLog(@"%s: %@", __PRETTY_FUNCTION__, err);

}

@end
