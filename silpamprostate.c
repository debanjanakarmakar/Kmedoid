#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#include<time.h>
#define MAX(a,b) (((a)>(b))?(a):(b))

float dist[80],count[80];



int row,col,k,classlable;
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






float intra(int r)
{
	int i,j,clusterno=cluster[r],c=0,in=0;
	float s=0.0,sum=0.0;
	for(i=0;i<row;i++)
	{
		if(cluster[i]==clusterno)
		{
			c++;
			s=0;
			for(j=0;j<col;j++)
			{
				s+=(p[r][j]-p[i][j]);
			}
			
			sum+=s;
		}
	}
	return fabsf(sum/c);

}

double inter(int r)
{
	int i,j;
	double s=0.0;
	for(i=0;i<k;i++)
	{
		dist[i]=0;
		count[i]=0;
	}
	count[cluster[r]]=1;
	for(i=0;i<row;i++)
	{
		if(cluster[i]==cluster[r])
			continue;
		for(j=0;j<col;j++)
			dist[cluster[i]]+=fabs(p[i][j]-p[r][j]);
		count[cluster[i]]++;
	}
	double min;
	min=HUGE_VAL;
	
	
	int index=0;
	for(i=0;i<k;i++)
	{
		if(cluster[r]==i)
			continue;
		dist[i]=dist[i]/count[i];
	
		if(min>dist[i])
		{
			min=dist[i];
			index=i;
		}
	}
	
	return min;

}

void silhoutte()
{
	int i;
	double s=0.0,sh,ai=0,bi=0,m,d;
	for(i=0;i<row;i++)
	{
		ai=inter(i);  
		bi=intra(i);
		d=ai-bi;
		printf("d=%lf ",d);
		m=MAX(ai,bi);
		printf("max=%lf ",m);
		sh=d/m;
		printf("sh=%lf \n",sh);
		s+=sh;
	}
	s/=row;
	printf("%lf",s);
}

int main()
{
	read();
	input();
	int i,j;
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
	silhoutte();
	
	free(p);
	free(medoid);
	free(newmedoid);
	free(cluster);
	return 0;
	
}
