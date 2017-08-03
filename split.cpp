#include<iostream>
#include<vector>
#include"SimplexSolution.h"
using namespace std;

bool no_opt=false;
const float erl=pow(10,-6.0);
class SSnode{
public:
	SimplexSolution ss;
	int xN;
	vector<double*> constrains;
	SSnode *childs[2];
	bool feasible;
	bool final;
	SSnode *parent;
	SSnode *brother;
	SSnode(){
		childs[0]=NULL;
		childs[1]=NULL;
		parent=NULL;
		brother=NULL;
		feasible=true;
		final=false;
	}
	SSnode(int xn){
		childs[0]=NULL;
		childs[1]=NULL;
		brother=NULL;
		parent=NULL;
		feasible=true;
		final=false;
		xN=xn;
	}
	/*---set root node--*/
	void setNode(vector<double*> cconstrains){
		for(int i=0;i<cconstrains.size();i++){
			constrains.push_back(cconstrains[i]);
		}
		ss=Simplex(cconstrains, cconstrains.size()-1, xN, true);
		//for(int i=0;i<xN;i++){ cout<<ss.x[i]<<endl;}
		if(convexIsEmpty()){
			feasible=false;
			final=true;
			return;
		}
	}
		
	/*---set generated node--*/
	/*---upper boundery: boundery>0--*/
	/*---lower boundery: boundery<0--*/
	void setNode(vector<double *> cconstrains,SSnode *parent,int xIndex,int boundery){
		for(int i=0;i<parent->constrains.size();i++){
			constrains.push_back(parent->constrains[i]);
		}
		double *new_constrain=new double[xN+1];
		for(int i=0;i<xN;i++){
			if(i==xIndex){
				if(boundery<0) new_constrain[i]=-1;
				else new_constrain[i]=1;
			}
			else  new_constrain[i]=0;
		}
		new_constrain[xN]=boundery;
		constrains.push_back(new_constrain);
		double *a=constrains[0];
		double *b=constrains[1];
		double *c=constrains[2];
		double *d=constrains[3];
		double *e=constrains[4];
		ss=Simplex(constrains,  constrains.size()-1, xN, true);
		if(convexIsEmpty()){
			feasible=false;
			final=true;
			return;
		}
	}
	bool convexIsEmpty(){
		if(ss.xN==0){
			cout<<"no optimization"<<endl;
			no_opt=true;
			return true;
		}
	    return false;
	}
};

class SStree{
	vector<double*> constrains;
	int n;//number of variable
	int cn;//number of constrains
public:
	SSnode *optimize;
	SSnode root;
	SStree(vector<double*> cconstrains,int nn,int ccn){
		n=nn;
		cn=ccn;
		constrains=cconstrains;
		optimize=NULL;
		root=SSnode(n);
		root.setNode(constrains);
		SSnode *troot=&root;
		generateNode(troot);
		
	}
	void generateNode(SSnode *root){
		if(root->feasible==false ||root->final||no_opt) return;
		bool final=root->final;
		bool feasible=root->feasible;
		/*------check node whether need split----*/
		bool hasFloat=false;
		double point;
		int floatIndex;
		//find float value
		for(int i=0;i<root->ss.xN;i++){
			if(!((root->ss.x[i]-(int)(root->ss.x[i]+0.5)<erl) && (root->ss.x[i]-(int)(root->ss.x[i]+0.5)>-erl))){
				hasFloat=true;
				point=int(root->ss.x[i]);
				floatIndex=i;
				break;
			}
		}
		if(!hasFloat){
			final=true;
		}
		/*------split node----*/
		if(feasible && !final){
			SSnode *child0=new SSnode(n);
			child0->setNode(constrains,root,floatIndex,point);
			child0->parent=root;
			root->childs[0]=child0;
			SSnode *child1=new SSnode(n);
			child1->setNode(constrains,root,floatIndex,-(point+1));
			child1->parent=root;
			root->childs[1]=child0;
			child0->brother=child1;
			child1->brother=child0;
			if(child0->ss.optZ>child1->ss.optZ){
				generateNode(child0);
				generateNode(child1);
			}
			else{
				generateNode(child1);
				generateNode(child0);
			}
			
		}
		if(final){
			if(feasible){
				if(optimize==NULL||(optimize->ss.optZ<root->ss.optZ)) optimize=root;
			}
			return;
			
		}
		
	}
};
void splitSimplex(vector<double*> constrains,int n,int cn){
	
	SStree SS=SStree(constrains,n,cn);
	SSnode root=SSnode(n);
	SSnode *r=&root;
	SS.generateNode(r);
	if(SS.optimize==NULL){
		cout<<"no integer optimun"<<endl;
		return;
	}
	cout<<"the maximum z is "<<SS.optimize->ss.optZ<<endl;
	for(int i=0;i<n;i++){
		cout<<"x"<<i+1<<"="<<SS.optimize->ss.x[i]<<" ";
	}
	cout<<endl;

}

