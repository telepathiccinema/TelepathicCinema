//
//  IntroViewController.m
//  Telepathic Cinema
//
//  Created by user on 11/18/14.
//  Copyright (c) 2014 Ket.ai. All rights reserved.
//

#import "IntroViewController.h"

@interface IntroViewController ()

@end

@implementation IntroViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor blackColor];
    // Do any additional setup after loading the view.
    UIImage* home = [UIImage imageNamed:@"02Home.png"];
    UIImageView* h = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 1024, 768)];
    [h setImage:home];
    
    UIImage* intro0 = [UIImage imageNamed:@"00Load.png"];
    UIImageView* iView0 = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 1024, 768)];
    [iView0 setImage:intro0];
    
    UIImage* intro1 = [UIImage imageNamed:@"01Load.png"];
    UIImageView* iView1 = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 1024, 768)];
    [iView1 setImage:intro1];
    
    [self.view addSubview:h];
    [self.view addSubview:iView0];
    [self.view addSubview:iView1];
    h.hidden = NO;
    h.alpha = 0.0f;
    iView0.hidden = NO;
    iView0.alpha = 0.0f;
    iView1.alpha = 0.0f;

    [UIView animateWithDuration:1.0f delay:0 options:0 animations:^{
        //fade in the first screen
        iView0.alpha = 1.0;
    } completion:^(BOOL finished){
        [UIView animateWithDuration:1.0f delay:1.5 options:0 animations:^{
            //fade in second screen
            iView0.alpha = 0;
            iView1.alpha = 1.0;
        } completion:^(BOOL finished){
            [UIView animateWithDuration:1.0f delay:1.5 options:0 animations:^{
                //fade out the intro
                iView1.alpha = 0;
            } completion:^(BOOL finished){
                //fade in the start screen
                iView1.hidden = YES;
                iView0.hidden = YES;
                [UIView animateWithDuration:1.0f delay:0.0 options:0 animations:^{
                    h.alpha = 1.0f;
                } completion:nil];
                
            }];
            
        }];
        
    }];
}

-(BOOL)prefersStatusBarHidden
{
    return YES;
}

@end
