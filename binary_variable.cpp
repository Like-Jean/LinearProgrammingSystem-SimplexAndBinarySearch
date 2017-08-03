#include <iostream>

using namespace std;
class BVnode{
public:
	short *solution;
	double z;
	double *thita;
	int solutionN;
	int constrainN;
	BVnode *childs[2];
	bool feasible;
	bool final;
	BVnode *parent;
	BVnode *brother;
	BVnode(){
		childs[0]=NULL;
		childs[1]=NULL;
		parent=NULL;
		brother=NULL;
		feasible=true;
		final=false;
	}
	BVnode(double *zc,double **constrains,int n,int cn){
		childs[0]=NULL;
		childs[1]=NULL;
		brother=NULL;
		parent=NULL;
		feasible=true;
		final=false;
		solutionN=n;
		constrainN=cn;
		solution=new short[n];
		for(int i=0;i<n;i++){
			solution[i]=-1;
		}
		z=0.0;
        thita=new double[constrainN];
	}
	void setNode(double *zc,double **constrains){
		//initialize z
		for(int i=0;i<solutionN;i++){
			if(zc[i]>=0) z+=zc[i];
		}
		//initialize thita
		for(int i=0;i<constrainN;i++){
			thita[i]=constrains[i][solutionN];
			for(int j=0;j<solutionN+1;j++){
				if(constrains[i][j]<0){
					thita[i]-=constrains[i][j];
				}
			}
		}
	}
	void setNode(short x,double *zc,double **constrains,BVnode *parent){
		z=parent->z;
		for(int i=0;i<constrainN;i++){
			thita[i]=parent->thita[i];
		}
		bool set=false;
		int step;
		for(int i=0;i<solutionN;i++){
			//give value to solution
			if(parent->solution[i]!=-1) solution[i]=parent->solution[i];
			if(solution[i]==-1&& !set){
				solution[i]=x;
				set = true;
				step=i;
				if(i==solutionN-1) final=true;
			}
		}
		//set thita and z
		if(x==0){
			if(zc[step]>=0){
					z-=zc[step];
				}
			for(int i=0;i<constrainN;i++){
				if(constrains[i][step]<=0){
					thita[i]+=constrains[i][step];
					if(thita[i]<0) feasible=false;
				}
			}
		}
		else{
			if(zc[step]<0){
					z+=zc[step];
				}
			for(int i=0;i<constrainN;i++){
					if(constrains[i][step]>0){
						thita[i]-=constrains[i][step];
						if(thita[i]<0) feasible=false;
				}
			}
		}
	}
};

class BVtree{
	double *zc;
	double **constrains;
	int n;
	int cn;
	bool getMax;
public:
	BVnode *optimize;
	BVnode root;
	BVtree(double *zzc,double **cconstrains,int nn,int ccn){
		zc=zzc;
		n=nn;
		cn=ccn;
		getMax=false;
		constrains=cconstrains;
		root=BVnode(zc,constrains,n,cn);
		root.setNode(zc,constrains);
		BVnode *troot=&root;
		generateNode(troot);
	}
	void generateNode(BVnode *root){
		if(getMax) return;
		bool final=root->final;
		bool feasible=root->feasible;
		
		if(feasible && !final){
			BVnode *child0=new BVnode(zc,constrains,n,cn);
			child0->setNode(0,zc,constrains,root);
			child0->parent=root;
			root->childs[0]=child0;
			BVnode *child1=new BVnode(zc,constrains,n,cn);
			child1->setNode(1,zc,constrains,root);
			child1->parent=root;
			root->childs[1]=child0;
			child0->brother=child1;
			child1->brother=child0;
			if(child0->z>child1->z){
				generateNode(child0);
			}
			 generateNode(child1);
		}
		if(final){
			if(feasible){
				optimize=root;
				if((root->z < root->brother->z)&& root->brother->feasible) optimize=root->brother;
			}
			else{
				if(root->brother->feasible) 
					optimize=root->brother;
				else return;
			}
			BVnode *p=optimize->parent->brother;
			for(int i=0 ;i<n-1;i++){
				if(optimize->z < p->z) return;
				p=p->parent->brother;
			}
			getMax=true;
			cout<<"the maximum z is "<<optimize->z<<endl;
			for(int i=0;i<n;i++){
				cout<<"x"<<i+1<<"="<<optimize->solution[i]<<" ";
			}
			cout<<endl;
		}
		
	}
};

void binaryVariable(double *zc,double **constrains,int n,int cn){
	
	BVtree BV=BVtree(zc,constrains,n,cn);
	BVnode root=BVnode(zc,constrains,n,cn);
	BVnode *r=&root;
	BV.generateNode(r);
}
