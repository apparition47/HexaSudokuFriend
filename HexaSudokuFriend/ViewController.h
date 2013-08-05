//
//  ViewController.h
//  HexaSudokuFriend
//
//  Created by Aaron Lee on 2013-08-03.
//  Copyright (c) 2013 Aaron Lee. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "solver.h"

@interface ViewController : UIViewController
- (IBAction) doSolve:(id)sender;
- (IBAction) dismissKeyboard;
@end
