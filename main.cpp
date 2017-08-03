#include <iostream>
#include  <fstream>
#include  <vector>
#include "SimplexSolution.h"
#include "binary_variable.h"
#include "split.h"
using namespace std;

void main(){
	ifstream fin;
	int constrainN;
	int variableN;
    fin.open("test_two_phase1.txt");
	fin>>variableN>>constrainN;

	//for simplex,and split, use vector
	//for binary_variable, use matrice
	vector<double*> consrains_v;
	double **constrains_m=new double*[constrainN];
	double *zc_m=new double[variableN];
	for(int i = 0 ; i < constrainN+1 ; i++ )
    {
		double* vectmp=new double[variableN+1];
	   constrains_m[i]=new double[variableN+1];
		for( size_t j = 0 ; j < variableN+1 ; j++)
        {
            double tmp = 0;
            fin>>tmp;
			vectmp[j]=tmp;
			if(i==0){
				if(j<variableN) zc_m[j]=tmp;
			}
			else{
				constrains_m[i-1][j]=tmp;
			}
        }
        consrains_v.push_back(vectmp);
    }
	cout<<"------------------choose LP method:-----------------:"<<endl;
	cout<<"1--simplex"<<endl;
	cout<<"2--binary variable"<<endl;
	cout<<"3--simplex split"<<endl;
	int c;
	while(cin>>c){
	cout<<endl;
	switch (c)
	{
	case 1:
	{
		cout<<"****************simplex method**************"<<endl;
		Simplex(consrains_v,  constrainN, variableN, false);
		cout<<endl;
		break;
	 }
	case 2:
	{
		cout<<"****************binary variable method**************"<<endl;
		binaryVariable(zc_m,constrains_m, variableN, constrainN);
		cout<<endl;
		break;
	}
	case 3:
	{
		cout<<"****************split method**************"<<endl;
		splitSimplex(consrains_v,variableN,constrainN);
		cout<<endl;
		break;
	}
	default:{cout<<"please input 1,2 or 3;";}
	};
	cout<<"------------------choose LP method:-----------------"<<endl;
	cout<<"1--simplex"<<endl;
	cout<<"2--binary variable"<<endl;
	cout<<"3--simplex split"<<endl;
	}
	
	
}
/////////////////////////////////////
//glpk input:
///* Object function */
//maximize z: x1 + 14*x2 + 6*x3;
///* Constrains */
//s.t. con1: x1 + x2 + x3 <= 4;
//s.t. con2: x1  <= 2;
//s.t. con3: x3  <= 3;
//s.t. con4: 3*x2 + x3  <= 6;
//s.t. con5: -x1  <= 0;
//s.t. con6: -x2  <= 0;
//s.t. con7: -x3  <= 0;
//end;
/////////////////////////////////////
//myinput:
//3 7                 ->3 is number of variable, 7 is number of constrains
//1 14 6
//1 1 1 4
//1 0 0 2
//0 0 1 3
//0 3 1 6
//-1 0 0 0
//0 -1 0 0
//0 0 -1 0
/////////////////////////////////////