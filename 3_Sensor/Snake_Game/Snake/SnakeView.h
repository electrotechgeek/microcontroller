//
//  SnakeView.h
//  Snake
//
//  Created by zhang xiaodong on 12-3-20.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SnakeState.h"

@class Snake;
@class SerialConnect;
@interface SnakeView: NSView <SnakeState>
{
    Snake *snake;
    NSTimer *timer;
    SerialConnect *serialConnection;
    NSMutableString *packet;
}

@property (nonatomic,retain) Snake *snake;

- (void)gameInit;
- (void)gameOver;
@end
