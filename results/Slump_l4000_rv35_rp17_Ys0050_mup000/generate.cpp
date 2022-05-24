#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;
 
static double Spacing;
static double CylinderRadius;
static double CylinderHeight;
static double SquarePlateSide;
static int    WallLayer;

static double DomainMin[3];
static double DomainMax[3];

static int *Type;
static double (*Position)[3];
static double (*Velocity)[3];

#define PI 3.14159265359



int main(int argc, char *argv[]){
	{
		FILE *rfp=fopen(argv[1],"r");
		char buf[1024];
		fgets(buf, sizeof(buf),rfp);
		if(sscanf(buf," Spacing %lf",&Spacing)!=1){fprintf(stderr,"error %d\n",__LINE__);}
		fgets(buf, sizeof(buf),rfp);
		if(sscanf(buf," CylinderRadius %lf", &CylinderRadius)!=1){fprintf(stderr,"error %d\n",__LINE__);}
		fgets(buf, sizeof(buf),rfp);
		if(sscanf(buf," CylinderHeight %lf", &CylinderHeight)!=1){fprintf(stderr,"error %d\n",__LINE__);}
		fgets(buf, sizeof(buf),rfp);
		if(sscanf(buf," SquarePlateSide %lf", &SquarePlateSide)!=1){fprintf(stderr,"error %d\n",__LINE__);}
		fgets(buf, sizeof(buf),rfp);
		if(sscanf(buf," WallLayer %d", &WallLayer)!=1){fprintf(stderr,"error %d\n",__LINE__);}
		fclose(rfp);
	}
	fprintf(stderr,"Spacing %e, CylinderRadius %e, CylinderHeight %e, SquarePlateSide %e\n", Spacing, CylinderRadius,CylinderHeight, SquarePlateSide);
	fprintf(stderr,"WallLayer %d\n", WallLayer);


	
	const int Nfluid = ((2.0*CylinderRadius)/Spacing)*((2.0*CylinderRadius)/Spacing)*(((double)CylinderHeight)/Spacing);
	const int Nwall  = ((double)SquarePlateSide/Spacing)*((double)SquarePlateSide/Spacing)*WallLayer;
		
	
	Type = (int *)malloc((Nfluid+Nwall)*sizeof(int));
	Position = (double (*)[3])malloc((Nfluid+Nwall)*sizeof(double [3]));
	Velocity = (double (*)[3])malloc((Nfluid+Nwall)*sizeof(double [3]));
	
	fprintf(stderr, "memryoy for %d particles are reserved\n", (Nfluid+Nwall));
	
	int count = 0;
	{
		int iP =0;
		//—¬‘Ì—±Žq
		for(int iX=-(CylinderRadius)/Spacing;iX<(CylinderRadius)/Spacing;++iX){
			for(int iY=-(CylinderRadius)/Spacing;iY<(CylinderRadius)/Spacing;++iY){
				
				const double x = Spacing*((double)iX+0.5);
				const double y = Spacing*((double)iY+0.5);
				if(x*x+y*y>CylinderRadius*CylinderRadius)continue;
				
				for(int iZ=0;iZ<(CylinderHeight)/Spacing;++iZ){
					const double z = Spacing*((double)iZ+0.5);
					Type[iP] = 1;
					Position[iP][0] = x;
					Position[iP][1] = y;
					Position[iP][2] = z;
					Velocity[iP][0] = 0.0;
					Velocity[iP][1] = 0.0;
					Velocity[iP][2] = 0.0;
					iP++;
				}
			}
		}
		
		fprintf(stderr, "line:%d, iP=%d\n",__LINE__,iP);
		
		//°–Ê—±Žq
		for(int iX=-(0.5*SquarePlateSide)/Spacing;iX<(0.5*SquarePlateSide)/Spacing;++iX){
			for(int iY=-(0.5*SquarePlateSide)/Spacing;iY<(0.5*SquarePlateSide)/Spacing;++iY){
				const double x = Spacing*((double)iX+0.5);
				const double y = Spacing*((double)iY+0.5);
				for(int iZ=-WallLayer;iZ<0;++iZ){
					const double z = Spacing*((double)iZ+0.5);
					
					Type[iP] = 2;
					Position[iP][0] = x;
					Position[iP][1] = y;
					Position[iP][2] = z;
					Velocity[iP][0] = 0.0;
					Velocity[iP][1] = 0.0;
					Velocity[iP][2] = 0.0;
					iP++;
				}
			}
		}
		fprintf(stderr, "line:%d, iP=%d\n",__LINE__,iP);
		
  	
    count = iP;
  	
  	
  }
	
	fprintf(stderr, "line:%d, %d particles are generated\n", __LINE__, count);
	{
	
	DomainMin[0] = -(0.5*SquarePlateSide);
	DomainMax[0] =  (0.5*SquarePlateSide);
	DomainMin[1] = -(0.5*SquarePlateSide);
	DomainMax[1] =  (0.5*SquarePlateSide);
	DomainMin[2] =   -Spacing*WallLayer - Spacing*WallLayer;
	DomainMax[2] =    CylinderHeight + Spacing*WallLayer;
	}
  {
    FILE *fp=fopen(argv[2],"w");
    fprintf(fp,"%lf\n",0.0);
    fprintf(fp,"%d %e  %e %e %e %e %e %e\n",count,Spacing, DomainMin[0], DomainMax[0], DomainMin[1], DomainMax[1], DomainMin[2], DomainMax[2]);
    for(int iP=0;iP<count;++iP){
      fprintf(fp, "%d %e %e %e %e %e %e\n", Type[iP], Position[iP][0], Position[iP][1], Position[iP][2], Velocity[iP][0], Velocity[iP][1], Velocity[iP][2]);
    }
    fclose(fp);
  }

  //free(Position);
  //free(Velocity);
  return 0;
}


    
 

      
      
