//
//  AppDelegate.h
//  RemoteServer
//
//  Created by Jason Dreisbach on 11/28/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SerialConnect.h"

@class RemoteHTTPServer;

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (retain) RemoteHTTPServer *httpServer;
@property (retain) SerialConnect *serialConnection;
@property (assign) IBOutlet NSTextField *nameField;
@property (retain) NSMutableString *receivedData;


- (NSDictionary *)codes;
- (void)storeCode:(NSString *)code withName:(NSString *)name;

- (IBAction)recordCode:(id)sender;
- (IBAction)playCode:(id)sender;

@end
