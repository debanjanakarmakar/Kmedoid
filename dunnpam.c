#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<time.h>
#define MAX(a,b) (((a)>(b))?(a):(b))


double *high;


int row,col,k=2,classlable;
float **p;

int *medoid,*cluster,*newmedoid;

void input()
{
	printf("Enter k:\n");
	scanf("%d",&k);	
	medoid=(int*)malloc(sizeof(int)*k);
	newmedoid=(int*)malloc(sizeof(int)*k);
}


void read()/*reads from file*/
{
	FILE *fp1=fopen("rbreast.txt","r");
	
	if(fp1==NULL)
	{
		printf("error");
	}
	fscanf(fp1,"%d",&row);
	fscanf(fp1,"%d",&col);
//	fscanf(fp1,"%d",&classlable);
	printf("%d %d",row,col);
	col-=1;
	p=(float**)malloc(sizeof(float*)*row);
	int i,j;
	for(i=0;i<row;i++)
	{
		p[i]=(float *)malloc(sizeof(float)*col);
		
	}
	float test;
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			fscanf(fp1,"%f",&test);
			p[i][j]=test;
			//printf("%f  ",p[i][j]);
		}
	}	
}

float distance(int x,int y)/*x=object row no.  ,  y=medoid i.e row no. in dataset that is selected*/
{
	float d=0.0;
	int i,j;
	for(i=0;i<col;i++)
	{
		d+=abs(p[x][i]-p[y][i]);	
	}
	return d;
}

void cluster_assign()
{
	int pos,in,j;
	float l=0.0;
	
	 for(in=0;in<row;in++)
	 {
	 	l=distance(in,medoid[0]);
		pos=0;
		
		for(j=1;j<k;j++)
		{
			if((distance(in,medoid[j]))<l)
			{
				l=distance(in,medoid[j]);
				pos=j;
			
			}
			
		}

		cluster[in]=pos;
		
	
 	}	

}

float cal_cost()
{
	float cost=0.0;
	int i,j;
	for(i=0;i<row;i++)
	{
		cost+=distance(i,cluster[i]);
	}
	return cost;
}

int notselected(int x)
{
	
	int i;
	for(i=0;i<k;i++)
	{
		if(medoid[i]==x)
			return 0;
			
	}
	return 1;
}

float swapfindcost()
{
	float cost=0.0,temp,l=0.0;
	int i,j,m;
	printf("flufluflu");
	for(i=0;i<row;i++)
	{
		l=distance(i,newmedoid[0]);
		
		for(j=1;j<k;j++)
		{
			if(distance(i,newmedoid[j])<l)
				l=distance(i,newmedoid[j]);
		}
		cost+=l;
	}
	return cost;
}

void kmedoid()
{
		int i,j,l,m,n;
		float newcost=0.0;
		for(i=0;i<k;i++)
		{
			printf("blablabla");
			float cost1=cal_cost();
			for(j=0;j<row;j++)
			{
				if(notselected(j))
				{
				   for(m=0;m<k;m++)
				   {
				   		if(m==i)
				   			newmedoid[m]=j;
				   		else
				   			newmedoid[m]=medoid[m];
				   }
				   newcost=swapfindcost();
				   if(newcost<cost1)
				   {
						medoid[i]=j;
						cluster_assign();
				   }
				}
			}
		}
	
}








///////???????????Dunn




double EDist(int x,int y)/*x=object row no.  ,  y=medoid i.e row no. in dataset that is selected*/
{
	double d=0.0;
	int i,j;
	for(i=0;i<col;i++)
	{
		d+=pow((p[x][i]-p[y][i]),2);	
	}
	return sqrt(d);
}



double maxDia_minInter()
{
	
	int i,j;
	double v,inter=EDist(0,1);
	v=inter;
	for(i=0;i<row;i++)
	{
		for(j=i+1;j<row;j++)
		{
			v=EDist(i,j);
			
			if(cluster[i]==cluster[j])////same cluster so intra cluster
			{		
				
				if(v>high[cluster[i]])
				{
					high[cluster[i]]=v;		
				}	
			}
			else ///different cluster
			{
				if(inter>v)
				{	
					inter=v;
				}
			}
		}
	
	}
	return inter;
}
void dunn()
{
	
	int i;
	double di,inter,max;
	high=(double*)malloc(sizeof(double)*k);
	for(i=0;i<k;i++)
	{
		high[i]=0.0;
	}
	inter=maxDia_minInter();
     max=high[0];
	for(i=1;i<k;i++)
	{
		
		if(max<high[i])
			max=high[i];
	}
	
	
	for(i=0;i<k;i++)
	{
		printf("\nhigh{i}=%lf",high[i]);
	}
	printf("max %lf,min %lf",max,inter);

   di=inter/max;
	printf("\n\nThe dunn index is %lf",di);
	
}

int main()
{
	read();
	input();
	int i,j,a;
	
	//for(a=2;a<=10;a++)
	//{
	//	k=a;
	srand(time(NULL));
	
	for(i=0;i<k;i++)
	{
		
		medoid[i]=rand()%(row-0)+0;

	}
	cluster=(int *)malloc(sizeof(int)*row);


	cluster_assign();
	
	kmedoid();
	for(i=0;i<row;i++)
	{
		printf("%d \t",cluster[i]);
	}	
	dunn();
//}
	free(p);
	free(medoid);
	free(newmedoid);
	free(cluster);

	return 0;
	
}








