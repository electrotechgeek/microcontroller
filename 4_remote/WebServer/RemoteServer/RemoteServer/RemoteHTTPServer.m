//
//  RemoteHTTPServer.m
//  RemoteServer
//
//  Created by Jason Dreisbach on 11/28/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import "RemoteHTTPServer.h"

@implementation RemoteHTTPServer

- (id)init
{
    self = [super init];
    if (self != nil) {
        
        [self get:@"/getCodes"
        withBlock:^(RouteRequest *request, RouteResponse *response) {
            NSString *responseString = [self.appDelgate.codes.allKeys componentsJoinedByString:@","];
            [response respondWithString:responseString];
        }];
        
        [self get:@"/doCode/:codeName"
         withBlock:^(RouteRequest *request, RouteResponse *response) {
             [self.appDelgate playCode:[request param:@"codeName"]];
             [response respondWithString:@"ok"];
         }];
        
    }
    return self;
}

@end
