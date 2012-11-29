//
//  ViewController.h
//  UniversalRemote
//
//  Created by Jason Dreisbach on 11/28/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>

@property (strong) NSArray *codeNames;
@property IBOutlet UINavigationBar *navBar;
@property IBOutlet UITableView *tableView;

- (IBAction)refresh:(id)sender;

@end
