#include <stdio.h> 
#include <stdlib.h>
void convolution(double data[], double filter[], int datalen, int filterlen,double dataf[]) 
{
	int i,j;
	int halflen = filterlen/2;
	for(i=0; i<datalen; i++)
	{
		double tmp = 0;
		for(j=-halflen; j<filterlen- halflen; j++)
		{
			if(i-j<datalen&& i-j>=0)
			{
				tmp =tmp + data[i-j]*filter[j+halflen];
			}
		}
		dataf[i] = tmp;
	}
}

void max(double data[], double cof[], int initial, int len)
{
	cof[0] = data[0+initial];
	cof[1] = 0;
	
	for(int i =1;i<len;i++)
	{
		if(data[i+initial]>cof[0])
		{
			cof[0] = data[i+initial];
			cof[1] = i;
		}
	}
}


void ppg_fearure_extractor(double data[], double highpass[], double lowpass[], int datalen, int highlen, int lowlen, double **Fm)
{
	double T = 100;
	double maxslope = 150;
	int count = 0;
	int flag = -1;
	int mark = 0;
	double peak = 0;
	int tmpT = 0;
	double maxdif=0;
	double maxindex=0;
	double feature[50][10];
	double featuretmp[10] = {0};
	double datadif[datalen];
	double *cof = (double *)malloc(2*sizeof(double));
	double *dataf = (double *)malloc(datalen*sizeof(double));
	
	convolution(data, highpass, datalen, highlen, dataf);
	convolution(dataf, lowpass, datalen, lowlen, data); 
	
	for(int i = 0; i<datalen; i++)
	{
		if(i>=4)
		{
			datadif[i] = (2.0*data[i] + data[i-1] -data[i-3] -2.0*data[i-4])/8.0;
			if(i>70)
			{
				max(datadif,cof,i-70,71);
				maxdif = cof[0];
				maxindex = cof[1];
				if(T*1.2<70)
				{
					tmpT = int(T*0.6); 
					max(datadif,cof,i-35-tmpT,tmpT*2);
					maxdif = cof[0];
					maxindex = cof[1]-tmpT+35;
				 } 
				if(maxindex == 35 && maxdif> maxslope*0.7)
				{
					maxslope = maxslope*0.9+ maxdif*0.1;
					peak = i -35;
					if(featuretmp[0]!=0)
					{
						if(flag == 4)
						{
							feature[count][8] = featuretmp[0];
							feature[count][9] = featuretmp[1];
							if(feature[count][8]-feature[count][0]>1.6*T&&feature[count][8]-feature[count][0]<0.3*T)
							{
								count--;
							}
							else
							{
								if(count>=1)
								{
									double slopek = (feature[count-1][9]-feature[count][1])/(feature[count][8]-feature[count][0]);
									Fm[count-1][0] = feature[count][2]-feature[count-1][2];
									Fm[count-1][1] = feature[count-1][2]-feature[count-1][0];
									Fm[count-1][2] = feature[count-1][8]-feature[count-1][2];
									Fm[count-1][3] = feature[count-1][4]-feature[count-1][2];
									Fm[count-1][4] = feature[count-1][6]-feature[count-1][2];
									Fm[count-1][5] = feature[count-1][3]-feature[count-1][1] - slopek*(feature[count-1][2]-feature[count-1][0]);
									Fm[count-1][6] = feature[count-1][5]-feature[count-1][1] - slopek*(feature[count-1][4]-feature[count-1][0]);
									Fm[count-1][7] = feature[count-1][7]-feature[count-1][1] - slopek*(feature[count-1][6]-feature[count-1][0]);
								}
							}
							T = T*0.9+(feature[count][8]-feature[count][0])*0.1;
							count++;
						}
						if(count>=50)
						{
							break;
						}
						feature[count][0] = featuretmp[0];
						feature[count][1] = featuretmp[1];
						flag = 1;
						mark = 1;
					}
				}
				if(datadif[i-35]>=0&&datadif[i-36]<0)
				{
					featuretmp[0] = i-35-2;
					featuretmp[1] = data[i-35-2];
				}
				if(flag==2&&double(i-35)>peak+T*0.1&&double(i-35)<peak+T*0.5)
				{
					max(datadif,cof,i-35-6,13);
					maxdif = cof[0];
					maxindex = cof[1];
					if(maxindex == 6)
					{
						if(maxdif<=0)
						{
							feature[count][4] = i-35-2;
							feature[count][5] = data[i-35-2];
							feature[count][6] = i-35-2;
							feature[count][7] = data[i-35-2];
							flag = 4;
						}
						else
						{
							feature[count][4] = featuretmp[0];
							feature[count][5] = featuretmp[1];
							flag = 3;
						}
					}
				}
				if(datadif[i-35]>=0&&datadif[i-34]<0)
				{
					if(flag==1)
					{
						feature[count][2] = i-35-2;
						feature[count][3] = data[i-35-2];
						flag = 2;
					}
					if(flag == 3)
					{
						feature[count][6] = i-35-2;
						feature[count][7] = data[i-35-2];
						flag = 4;
					}
				}
			}
		}
	}
	
	Fm[49][0] = count;
} 

//for classfication of data according to their quality
int isppgsignal(int data[], int initial, int len,int step,int win, double weight[], int wlen)
{
	double maxcoef=0;
	int coeflen = (len-win)/step+2;
	if(coeflen!=wlen)
	{
		return 0;
	}
	double coef[coeflen]={0};
	int count = 0;
	coef[coeflen-1] = 10000000;
	
	for(int i = 0; i+win<len; i = i+step)
	{
		for(int j = 0; j<win; j++)
		{
			coef[count] = coef[count] + data[initial+j]*data[initial+j+i];
		}
		if(coef[count]>maxcoef)
		{
			maxcoef = coef[count];
		}
		count++;
	}
	
	double labelpred = 0;
	for(int i=0; i<wlen;i++)
	{
		labelpred = labelpred + coef[i]*weight[i]/maxcoef;
	}
	
	return labelpred;
 } 
int MAXDATALEN = 100000;
int main()
{
	double **Fm =(double **)malloc(50*sizeof(double *));
	for (int i=0;i<50;i++)
        Fm[i]=(double *)malloc(8*sizeof(double));
        
	double buf[MAXDATALEN];  /*������*/
 	FILE *fp;            /*�ļ�ָ��*/
 	int count=0;             /*���ַ�����*/
 	double temp;
 	int i = 0;
 	
 	if((fp = fopen("highpass.txt","r")) == NULL)
 	{
 		perror("fail to read");
 		exit (1) ;
 		}
 	for(i=0;i<MAXDATALEN;i++)
 	{	
 		if(fscanf(fp,"%lf\t",&temp)==-1)
 		{
 			break;
		 }
 		buf[i] = temp;
 		}
 	printf("highpass len: %d\n",i);
 	fclose(fp);
 	//
 	int highlen =i;
 	double highpass[i];
 	for(int j = 0; j<i;j++)
 	{
 		highpass[j] = buf[j];
	 }
	 
	if((fp = fopen("lowpass.txt","r")) == NULL)
 	{
 		perror("fail to read");
 		exit (1) ;
 		}
 	for(i=0;i<MAXDATALEN;i++)
 	{	
 		if(fscanf(fp,"%lf\t",&temp)==-1)
 		{
 			break;
		 }
 		buf[i] = temp;
 		}
 	printf("lowpass len: %d\n",i);
 	fclose(fp);
 	//
 	int lowlen = i;
 	double lowpass[i]; 
 	for(int j = 0; j<i;j++)
 	{
 		lowpass[j] = buf[j];
	 }
	
		if((fp = fopen("input.txt","r")) == NULL)
 	{
 		perror("fail to read");
 		exit (1) ;
 		}
 	for(i=0;i<MAXDATALEN;i++)
 	{	
 		if(fscanf(fp,"%lf\t",&temp)==-1)
 		{
 			break;
		 }
 		buf[i] = temp;
 		}
 	printf("input len: %d\n",i);
 	fclose(fp);
 	//
 	int datalen = i;
 	double data[i]; 
 	for(int j = 0; j<i;j++)
 	{
 		data[j] = buf[j];
	 }
	 
	 if((fp = fopen("output.txt","r")) == NULL)
 	{
 		perror("fail to read");
 		exit (1) ;
 		}
 	for(i=0;i<MAXDATALEN;i++)
 	{	
 		if(fscanf(fp,"%lf\t",&temp)==-1)
 		{
 			break;
		 }
 		buf[i] = temp;
 		}
 	printf("output len: %d\n",i);
 	fclose(fp);
 	
 	double output[i/8][8]; 
 	for(int j = 0; j<i/8;j++)
 	{
 		for(int k = 0 ; k<8;k++)
 		{
 			output[j][k] = buf[j*8+k];
 			//printf("%lf",output[j][k]);
		 }
		//printf("\n");
	 }
	printf("start!!\n");
	ppg_fearure_extractor(data, highpass, lowpass, datalen, highlen, lowlen,Fm);
	
	double len = Fm[49][0];
	printf("test2!!%d\n",len);
	
	for(int j = 1; j<len;j++)
 	{
 		for(int k = 0 ; k<8;k++)
 		{
 			printf("%lf\t",Fm[j][k]);
		 }
		printf("\n");
		for(int k = 0 ; k<8;k++)
 		{
 			printf("%lf\t",output[j-1][k]);
		 }
		printf("\n");
		system("pause");
	 }
  	return 0;
} 


