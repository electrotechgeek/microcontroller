//
//  AppDelegate.m
//  RemoteServer
//
//  Created by Jason Dreisbach on 11/28/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import "AppDelegate.h"
#import "RemoteHTTPServer.h"

#define CODE_PLIST_PATH [NSHomeDirectory() stringByAppendingPathComponent:@"ir_codes.plist"]
@implementation AppDelegate

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    self.serialConnection = [[SerialConnect alloc] init];
    [self.serialConnection activate];
    [self.serialConnection release];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(serialRecv:) name:@"serialMessageReceived" object:nil];
    
    // Make our http server
    self.httpServer = [[RemoteHTTPServer alloc] init];
    self.httpServer.appDelgate = self;
    [self.httpServer setPort:8888];
    NSError *err = nil;
    [self.httpServer start:&err];
    if (err != nil)
        NSLog(@"%s: %@", __PRETTY_FUNCTION__, err);
    
        
}

- (NSDictionary *)codes
{
    return [NSDictionary dictionaryWithContentsOfFile:CODE_PLIST_PATH]  ?: [NSDictionary dictionary];
}

- (void)storeCode:(NSString *)code withName:(NSString *)name
{
    NSMutableDictionary *codeDict = [NSMutableDictionary dictionaryWithContentsOfFile:CODE_PLIST_PATH] ?: [NSMutableDictionary dictionary];
    [codeDict setObject:code forKey:name];
    [codeDict writeToFile:CODE_PLIST_PATH atomically:YES];
}

- (IBAction)recordCode:(id)sender
{
    // Check the name box
    if (self.nameField.stringValue.length > 0) {
        
        self.receivedData = [NSMutableString string];
        //  send r the arduino
        [self.serialConnection sendSerialMessage:@"r"];

    }
    else {
        // Hey you need to have a name for the code first
    }
}

- (void)serialRecv:(NSNotification*)notification
{
    NSString *receivedString = [notification object];
    [self.receivedData appendString:receivedString];
    if ([self.receivedData rangeOfString:@"err"].location != NSNotFound) {
        
    }
    else if ([self.receivedData rangeOfString:@"|"].location != NSNotFound) {
        // We have a complete code !!
        NSArray *array = [self.receivedData componentsSeparatedByString:@"|"];
        
        if (array.count > 0) {
            NSString *name = self.nameField.stringValue;
            NSString *code = [array objectAtIndex:0];
            NSLog(@"%@ - %@", name, code);
            [self storeCode:code withName:name];
        }
    }
}

- (IBAction)printData:(id)sender
{
    NSLog(@"%@", self.receivedData);
}

- (IBAction)playCode:(id)sender
{
    NSString *code = nil;
    if ([sender isKindOfClass:[NSString class]]) {
        code = [self.codes objectForKey:sender];
    }
    else {
        // get name from name box
        code = [self.codes objectForKey:self.nameField.stringValue];
    }
    if (code.length > 0) {
        self.receivedData = [NSMutableString string];
        NSUInteger componentCount = [[code componentsSeparatedByString:@","] count] - 1;
        [self.serialConnection sendSerialMessage:@"p"];
        [self.serialConnection writeByte:(unsigned char)componentCount];
        [self.serialConnection sendSerialMessage:code];
    }
}

@end
