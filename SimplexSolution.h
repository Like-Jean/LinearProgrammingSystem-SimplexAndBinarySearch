//
//  SimplexSolution.h
//  Simplex
//
//  Created by 郭力克 on 07/01/17.
//  Copyright (c) 2017 EFREI. All rights reserved.
//

#ifndef __Simplex__SimplexSolution__
#define __Simplex__SimplexSolution__

#include <stdio.h>
#include  <vector>
using namespace std;

class SimplexSolution {
public:
    double optZ;//obj value
    int xN;//Number of variables
    double* x; //value of variables
    
    SimplexSolution(int n, vector<double> xx, double z){
        xN = n;
        x = new double[xN];
        
        for(int i=0; i<xN; i++) {
            x[i] = xx[i];
        }
        
        optZ = z;
    }
    
    SimplexSolution() {
        xN = 0;
    }
};

/*****Decleartions****************/
SimplexSolution Simplex(vector<double *>,int, int, bool);
void import_process(vector<double *>,int, int);
bool if_feasible();
bool if_need_two();
void first_step_of_two();
void display_first();
void display();
void solve();
pair<int ,int> exchange(vector<double>);
void pivot(int, int);
bool is_finished();
#endif /* defined(__Simplex__SimplexSolution__) */