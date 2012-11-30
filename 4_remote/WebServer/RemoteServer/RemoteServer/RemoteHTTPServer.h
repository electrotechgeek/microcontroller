//
//  RemoteHTTPServer.h
//  RemoteServer
//
//  Created by Jason Dreisbach on 11/28/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <HTTPServer/RoutingHTTPServer.h>
#import "AppDelegate.h"

@interface RemoteHTTPServer : RoutingHTTPServer

@property (assign) AppDelegate *appDelgate;

@end
