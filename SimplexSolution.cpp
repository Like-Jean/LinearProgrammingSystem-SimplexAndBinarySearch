//
//  SimplexSolution.cpp
//  Simplex
//
//  Created by 郭力克 on 08/01/17.
//  Copyright (c) 2017 EFREI. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include  <vector>
#include  <fstream>
#include <set>

#include "SimplexSolution.h"
using namespace std;

vector<vector<double> > A;
vector<double> b;
vector<double> c;
int m;//number of constraints
int n;//number of varibles
vector<int> B;//Based solutions
vector<int> N;//Non-based solutions
double z;

SimplexSolution Simplex(vector<double *> Matrix, int _m, int _n, bool if_split)
{
    A.clear();
    b.clear();
    c.clear();
    m = _m;
    n = _n;
    B.clear();
    N.clear();
    
    import_process(Matrix, m, n);
    
    cout<<"****************Original form***************************"<<endl<<endl;
    
    for( int i = 0 ; i < A.size() ; i++ )
    {
        for( int j = 0 ; j < A[0].size() ; j++ )
        {
            cout<<A[i][j]<<"\t";
        }
        cout<<b[i]<<"\t";
        cout<<endl;
    }
    cout<<"obj function: "<<endl;
    for (int i=0; i<c.size(); i++) {
        cout<<c[i]<<"\t";
    }
    cout<<z;
    
    cout<<endl<<endl;
    cout<<"*********************************************************"<<endl<<endl;
    
    if (if_need_two()) {
        first_step_of_two();
        solve();
    }
    else solve();
    
    if(if_split) {
        
        if(if_feasible()) {
            vector<double> variables;
            
            for (int i=0; i<n; i++) {
                bool based = false;
                for (int j=0; j<m; j++) {
                    if (B[j] == i+1) {
                        variables.push_back(b[j]);
                        based = true;
                        break;
                    }
                }
                
                if (!based) {
                    variables.push_back(0.);
                }
            }
            
            return SimplexSolution(n, variables, -z);
        }
        
        else return SimplexSolution();
        
    }
    
    else {
        cout<<"Zmax: "<<-z<<endl;
        for (int i=0; i<n; i++) {
            bool based = false;
            for (int j=0; j<m; j++) {
                if (B[j] == i+1) {
                    cout<<"x"<<i+1<<":"<<b[j]<<"(based) ";
                    based = true;
                    break;
                }
            }
            
            if (!based) {
                cout<<"x"<<i+1<<":"<<0<<" ";
            }
        }
        cout<<endl;
        
        return SimplexSolution();
    }
}

/*****Import data process*********/
void import_process(vector<double *> Matrix, int m, int n) {
    double tmp = 0;
    
    //Initialize the obj function
    for( int i = 0 ; i < n ; i++ )
    {
        tmp = *(Matrix[0]+i);
        c.push_back(tmp);
    }
    cout<<endl;
    z = *(Matrix[0]+n);
    
    for( int i = 1 ; i <= m ; i++ )
    {
        vector<double> vectmp;
        for( int j = 0 ; j < n; j++)
        {
            tmp = *(Matrix[i]+j);
            vectmp.push_back(tmp);
        }
        
        A.push_back(vectmp);
        tmp = *(Matrix[i]+n);
        b.push_back(tmp);
    }
    
    //Add slacks
    for (int i=0; i<m; i++) {
        for (int j=0; j<m; j++) {
            A[i].push_back(0.);
        }
        
        c.push_back(0.);
    }
    
    for (int i=0; i<m; i++) {
        if (b[i]>=0) {
            A[i][n+i] = 1.;
        }
        else {
            for (int j=0; j<n; j++) {
                A[i][j] = -A[i][j];
            }
            A[i][n+i] = -1.;
            b[i] = -b[i];
        }
    }
    
    for(int i=1; i<=m; i++) {
        B.push_back(i+n);
    }
    for (int i=1; i<=n; i++) {
        N.push_back(i);
    }
    
}
/*****The two-phase process*******/
bool if_need_two() {
    
    bool check = false;
    for( int i = 0 ; i < m ; i++ )
        for( int j = n ; j < n+m ; j++)
        {
            //cout<<"A"<<i+1<<j+1<<":"<<A[i][j]<<"  ";
            if (A[i][j]<0) {
                check = true;
            }
            //cout<<endl;
        }
    return check;
}

//Check if it is feasible
bool if_feasible() {
    bool result = false;
    
    for (int i=0; i<A[0].size(); i++) {
        result = false;
        
        if (c[i] <= 0) {
            result = true;
        }
        for (int j=0; j<m; j++) {
            if (A[j][i]>0) {
                result = true;
            }
        }
        
        if (!result) {
            return result;
        }
    }
    
    return result;
}

void first_step_of_two() {
    
    int art_var_num = 0;
    vector<pair<int,int> > artificial_var;
    
    for( int i = 0 ; i < m ; i++ )
        for( int j = n ; j < n+m ; j++)
            if (A[i][j]<0) {
                pair<int,int> temp(i,m+n+art_var_num);
                artificial_var.push_back(temp);
                
                //Replace the negative var by the artificial variable
                B[i] = m+n+art_var_num+1;
                N.push_back(j+1);
                
                art_var_num++;
            }
    
    for (int i=0; i < m; i++) {
        for (int j=n+m; j < n+m+art_var_num; j++) {
            
            //Check if the A[i][j] is a position for artificial variable
            bool found = false;
            for (int k=0; k < art_var_num; k++) {
                pair<int,int> temp = artificial_var[k];
                if (temp.first == i && temp.second == j) {
                    found = true;
                }
            }
            
            if (found) A[i].push_back(1.);
            else A[i].push_back(0.);
            
        }
    }
    
    for (int i=0; i<art_var_num; i++) {
        c.push_back(0.);
    }
    //Make the new obj function
    vector<double> new_obj;
    
    for (int i = 0; i<n+m; i++) {
        new_obj.push_back(0.);
    }
    
    for (int i = 0; i<art_var_num; i++) {
        new_obj.push_back(-1.);
    }
    cout<<"***********************Start the first phase of two-phase***********************"<<endl<<endl;
    
    cout<<"Based: ";
    for(int i=0; i<B.size(); i++) cout<<B[i]<<" ";
    cout<<endl;
    cout<<"Non-based: ";
    for(int i=0; i<N.size(); i++) cout<<N[i]<<" ";
    cout<<endl;
    cout<<"Artificial variables: ";
    for (int i=0; i<art_var_num; i++) cout<<artificial_var[i].second+1<<" ";
    cout<<endl;
    
    for( int i = 0 ; i < A.size() ; i++ )
    {
        for( int j = 0 ; j < A[0].size() ; j++ )
        {
            cout<<A[i][j]<<"\t";
        }
        cout<<b[i]<<"\t";
        cout<<endl;
    }
    
    cout<<"obj function: "<<endl;
    for (int i=0; i<c.size(); i++) {
        cout<<c[i]<<"\t";
    }
    cout<<z<<endl;
    
    for (int i=0; i<art_var_num; i++) {
        for (int j=0; j<new_obj.size(); j++) {
            new_obj[j] += A[artificial_var[i].first][j];
        }
    }
    
    cout<<"new obj function: "<<endl;
    for (int i=0; i<new_obj.size(); i++) {
        cout<<new_obj[i]<<"\t";
    }
    cout<<"U"<<endl<<endl;
    
    cout<<"*********************************************************"<<endl<<endl;
    
    bool finished = false;
    
    while (!finished) {
        
        if (!if_feasible()) {
            cout<<"No feasible solution"<<endl;
            return;
        }
        
        //Move in and move out
        pair<int,int> in_out = exchange(new_obj);
        
        //Pivot
        pivot(in_out.first, in_out.second);
        
        double factor = new_obj[B[in_out.second]-1];
        for (int i=0; i<new_obj.size(); i++) {
            new_obj[i] -= A[in_out.second][i]*factor;
        }
        
        cout<<"Based: ";
        for(int i=0; i<B.size(); i++) cout<<B[i]<<" ";
        cout<<endl;
        cout<<"Non-based: ";
        for(int i=0; i<N.size(); i++) cout<<N[i]<<" ";
        cout<<endl;
        cout<<"Artificial variables: ";
        for (int i=0; i<art_var_num; i++) cout<<artificial_var[i].second+1<<" ";
        cout<<endl;
        
        for( int i = 0 ; i < A.size() ; i++ )
        {
            for( int j = 0 ; j < A[0].size() ; j++ )
            {
                cout<<A[i][j]<<"\t";
            }
            cout<<b[i]<<"\t";
            cout<<endl;
        }
        
        cout<<"obj function: "<<endl;
        for (int i=0; i<c.size(); i++) {
            cout<<c[i]<<"\t";
        }
        cout<<z<<endl;
        
        cout<<"new obj function: "<<endl;
        for (int i=0; i<new_obj.size(); i++) {
            cout<<new_obj[i]<<"\t";
        }
        cout<<"U";
        
        cout<<endl<<endl;
        
        cout<<"*********************************************************"<<endl<<endl;
        
        finished = true;
        for (int i=0; i<art_var_num; i++) {
            for (int j=0; j<B.size(); j++) {
                if (B[j] == artificial_var[i].second+1) {
                    finished = false;
                }
            }
        }
    }
    
    cout<<"*******Remove all the artificial variables and new obj function****************"<<endl<<endl;
    //Remove the artificial variables and the obj function
    for (int i=0; i<m; i++) {
        for (int j=0; j<art_var_num; j++) {
            A[i].pop_back();
        }
    }
    
    for (int i=0; i<art_var_num; i++) {
        c.pop_back();
    }

	for(int i=0; i<art_var_num; i++) {
		for(int j=0; j<N.size(); j++) {
			if(N[j] == artificial_var[i].second+1) {
				N.erase(N.begin()+j);
				break;
			}
		}
	}
}
void display() {
    cout<<"Based: ";
    for(int i=0; i<B.size(); i++) cout<<B[i]<<" ";
    cout<<endl;
    cout<<"Non-based: ";
    for(int i=0; i<N.size(); i++) cout<<N[i]<<" ";
    cout<<endl;
    for( int i = 0 ; i < A.size() ; i++ )
    {
        for( int j = 0 ; j < A[0].size() ; j++ )
        {
            cout<<A[i][j]<<"\t";
        }
        cout<<b[i]<<"\t";
        cout<<endl;
    }
    cout<<"obj function: "<<endl;
    for (int i=0; i<c.size(); i++) {
        cout<<c[i]<<"\t";
    }
    cout<<z;
    
    cout<<endl<<endl;
}

/***********Simplex***********/
void solve() {
    
    cout<<"*********************************Start the normal simplex*******************"<<endl<<endl;
    cout<<"Based: ";
    for(int i=0; i<B.size(); i++) cout<<B[i]<<" ";
    cout<<endl;
    cout<<"Non-based: ";
    for(int i=0; i<N.size(); i++) cout<<N[i]<<" ";
    cout<<endl;
    for( int i = 0 ; i < A.size() ; i++ )
    {
        for( int j = 0 ; j < A[0].size() ; j++ )
        {
            cout<<A[i][j]<<"\t";
        }
        cout<<b[i]<<"\t";
        cout<<endl;
    }
    cout<<"obj function: "<<endl;
    for (int i=0; i<c.size(); i++) {
        cout<<c[i]<<"\t";
    }
    cout<<z;
    
    cout<<endl<<endl;
    
    cout<<"*********************************************************"<<endl<<endl;
    
    while (!is_finished()) {
        
        
        if (!if_feasible()) {
            cout<<"No feasible solution"<<endl;
            return;
        }
        
        //Find the non-based to move in and the based to move out
        pair<int,int> in_out = exchange(c);
        
        //Pivot
        pivot(in_out.first, in_out.second);
        
        //Display the status
        display();
        
        cout<<"*********************************************************"<<endl<<endl;
    }
}

//Exchange the based solution and non-based solution
pair<int ,int> exchange(vector<double> obj) {
    //Choose the biggest one in C to move to based solution
    int temp = 0;
    int move_in_index = 0;
    for (int i=0; i<N.size(); i++) {
        if (obj[N[i]-1]>temp) {
            temp = obj[N[i]-1];
            move_in_index = i;
        }
    }
    cout<<"Move in: "<<N[move_in_index]<<endl;
    
    //Choose the one to move out
    int move_out_line = 0;
    double constraint = 999999999.;
    
    for (int i=0; i<m; i++) {
        if (A[i][N[move_in_index]-1]!=0 && b[i]/A[i][N[move_in_index]-1] < constraint && b[i]/A[i][N[move_in_index]-1] >= 0) {
            move_out_line = i;
            constraint = b[i]/A[i][N[move_in_index]-1];
        }
    }
    
    cout<<"Move out: "<<B[move_out_line]<<endl;
    
    return pair<int, int > (move_in_index, move_out_line);
}

//Check if it terminated
bool is_finished() {
    bool finished = true;
	for(int i=0;i<N.size();i++) {
        if (c[N[i]-1]>0) {
            finished = false;
        }
    }
    
    return finished;
}

void pivot(int in_index, int out_index) {
    
    int temp;
    
    temp = B[out_index];
    
    B[out_index] = N[in_index];
    N[in_index] = temp;
    
    double factor = 1./A[out_index][B[out_index]-1];
    cout<<"factor: "<<factor<<endl<<endl;
    
    for (int i=0; i<A[out_index].size(); i++) {
        A[out_index][i] *= factor;
        //cout<<A[out_index][i]<<"\t";
    }
    b[out_index] *= factor;
    //cout<<b[out_index]<<endl;
    
    //Fix A and c
    for (int i=0; i<m; i++) {
        if (i!= out_index && A[i][B[out_index]-1] != 0) {
            factor = A[i][B[out_index]-1];
            
            for (int j=0; j<A[i].size(); j++) {
                A[i][j] -= A[out_index][j]*factor;
            }
            
            b[i] -= b[out_index] * factor;
        }
    }
    
    factor = c[B[out_index]-1];
    for (int i=0; i<c.size(); i++) {
        c[i] -= A[out_index][i]*factor;
    }
    z -= b[out_index]*factor;
}
