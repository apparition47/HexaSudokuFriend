//
//  ViewController.m
//  HexaSudokuFriend
//
//  Created by Aaron Lee on 2013-08-03.
//  Copyright (c) 2013 Aaron Lee. All rights reserved.
//

#import "ViewController.h"

#define kSolveButtonTag 69

@interface ViewController ()
{
    @private
    sudoku board;
    NSTimer *solveCompletePoller;
}
@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]
                                   initWithTarget:self
                                   action:@selector(dismissKeyboard)];
    
    [self.view addGestureRecognizer:tap];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
 * Read input fields, invoke solving engine, start timer to check
 * when compute is done.
 */
- (IBAction) doSolve:(id)sender {
    // get input from view
    [self readInput];
    printf("%d free spots detected.", board.freecount);
    
    // disable solve button
    [((UITextField*)[self.view viewWithTag:kSolveButtonTag]) setEnabled:NO];
    
    // start poller to check whether solution has been calculated
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
    solveCompletePoller = [[NSTimer alloc] init];
    solveCompletePoller = [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(isReady)  userInfo:nil repeats:YES];
    
    // start computation and signal when dnoe
    int a[DIMENSION*DIMENSION+1];
    backtrack(a,0,&board);
}

/*
 * Wait for solution to compute then update view.
 */
- (void) isReady {
    if (finished) {
        [self updateView:&board];
    
        // unschedule
        [solveCompletePoller invalidate];
        solveCompletePoller = nil;
        [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
        
        // popup
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"HexaSudokuFriend" message:@"Solved!" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [alert show];
    }
}

/*
 * Read all textfields then feed into struct object.
 */
- (void) readInput {
	int i,j;
	int value;
    NSString *textField;
    NSArray *valuesOfLine;
    
	init_board(&board);
    
	for (i=0; i<DIMENSION; i++) {
        // process each textfield by line
        
        textField = ((UITextField*)[self.view viewWithTag:i+1]).text;
        valuesOfLine = [textField componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        valuesOfLine = [valuesOfLine filteredArrayUsingPredicate:[NSPredicate predicateWithFormat:@"SELF != ''"]];
        
        // disable textbox
        [((UITextField*)[self.view viewWithTag:i+1]) setEnabled:NO];
        [((UITextField*)[self.view viewWithTag:i+1]) setTextColor:[UIColor lightGrayColor]];
        
		for (j=0; j<DIMENSION; j++) {
            
            // process each line by space-separated unit
            value = [[valuesOfLine objectAtIndex:j] intValue];

			if (value != 0) {
				fill_square(i,j,value,&(board));
			}
		}
    }
}

/*
 * Update button and textfields
 */
- (void) updateView:(sudoku*)b {
    int i,j;
    UITextField *textField;
    
    // reenable button
    [((UITextField*)[self.view viewWithTag:kSolveButtonTag]) setEnabled:YES];
    
    for (i=0; i<DIMENSION; i++) {
        // process each textfield by line
        textField = (UITextField*)[self.view viewWithTag:i+1];
        
        // reenable textbox
        [((UITextField*)[self.view viewWithTag:i+1]) setEnabled:YES];
        [((UITextField*)[self.view viewWithTag:i+1]) setTextColor:[UIColor blackColor]];
        
        NSMutableString *newLine = [[NSMutableString alloc] init];
        
        for (j=0; j<DIMENSION; j++) {
            if (b->m[i][j] == 0) {
                [newLine appendString:@"0"];
            }
            else {
                [newLine appendFormat:@"%d ",b->m[i][j]];
            }
        }
        
        // signal update view
        [textField setText:newLine];
        [textField sendActionsForControlEvents:UIControlEventEditingChanged];
    }
    
    
}

- (IBAction) dismissKeyboard {
    [self.view endEditing:YES];
}

@end
