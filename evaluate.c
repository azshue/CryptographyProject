#include <stdio.h>
#include <string.h>
#include <math.h>
#include <python.h>
/*
int main()
{
	char str1[5]="foo";
	char str2[10]="hellowood";
	printf("%d\n",strcmp(str1,str2));
	return 0;
}
*/ 

double evaluate_random(char* key);
double evaluate_length(char* key);
double evaluate_unique(char* key);
double evaluate_vary(char* key);

int save2file(double f1,double f2,double f3,double f4,double f5);

int main(int argc,char* argv[])
{
	double random;//not including more than 3 consecutive the same character
	double length;//the length
	double unique;//not matching with the password.txt
	double vary;//include capitaled word, number, and symble...
	char key[20];
	
	printf("> please input a password, we will evaluate the safety of it:\n");
	scanf("%s",key);
	
	FILE* fp;
 	if((fp = fopen("password.txt","w"))==NULL)
	{
		printf("cannot open file!\n");
		return 0;
	}
	fprintf(fp,"%s",key);
	fclose(fp);

 	random = evaluate_random(key);
 	length = evaluate_length(key);
 	unique = evaluate_unique(key);
 	vary = evaluate_vary(key);
 	double average=(random+length+unique+vary)/4;

 	printf("> Result: random:%lf;\tlength:%lf;\tunique:%lf \t vary:%lf \t average:%lf\n",random,length,unique,vary,average);
 	
 	int ok;
 	ok=save2file(random,length,unique,vary,average);
 	if(ok==1)
 		printf("saved!\n");
	else
		printf("saving error!\n");
	
	Py_SetProgramName(argv[0]);
	Py_Initialize();
	PyRun_SimpleString("execfile('graph.py')");
	Py_Finalize();
 
 
 	return 0;
}


double evaluate_vary(char* key)
{
	double score;

	int flag_num=0;
	int flag_Cap=0;
	int flag_alpha=0;
	int flag_sym=0;
	int i;
	for(i=0;key[i]!='\0';i++)
	{
		if(key[i]>=48&&key[i]<=57)
			flag_num++;
		else if(key[i]>=65&&key[i]<=90)
			flag_Cap++;
		else if(key[i]>=97&&key[i]<=122)
			flag_alpha++;
		else
			flag_sym++;
	}
	
	score = 0.9*flag_sym+0.7*flag_alpha+0.9*flag_Cap+0.7*flag_num;
	score/=strlen(key);
	score*=10;
	//printf("random ok\n");
	return score;
}

double evaluate_length(char* key)
{
	//modifying to sigmoid
	double score;
	double length;
	length=strlen(key);		
	score = exp((length-8)/2)/(exp((length-8)/2)+1);
	score = score*10;
	//printf("length OK!\n");
	return score;
}


double evaluate_unique(char* key)
{
	//debug unique case
	double score;
	FILE* fp;
	if((fp = fopen("dict.txt","r"))==NULL)
	{
		printf("cannot open file1\n");
		exit(0);
	}

	char temp[100];
	int i=0;
	//ÓëÃÜÂë×ÖµäÆ¥Åä½á¹û 
	long int k=1;
	while(!feof(fp))
	{
		fscanf(fp,"%s",temp);
		//printf("%s",temp); 
		if(strncmp(key,temp,strlen(key))==0)
		{
			score=3.0;	
			//printf("iteration: %d\n",k);
			//printf("unique ok\n");
			fclose(fp);		
			return score;
		}
		else
			score=10.0;
		k++;
		memset(temp,'\0',sizeof(temp));
	}	
	//printf("iteration: %d\n",k);
	//printf("unique ok\n");
	fclose(fp);
 	return score;
}


double evaluate_random(char* key)
{
	//maybe normallized standard error is better
	double score=0;	
	double avg=0;
	double sigma=0;
	int itr;
	itr = strlen(key)-2;
	 
	int k;
	int i;
	for(k=0;k<itr;k++)
	{
		for(i=k*3;i<k*3+3;i++)
			avg+=key[i];
		avg/=3;
		for(i=k*3;i<k*3+3;i++)
			sigma+=(key[i]-avg)*(key[i]-avg);
		sigma=sqrt(sigma);
		sigma/=3;
		score+=sigma;
		avg=0;
	}
	score/=strlen(key)-2;;
	score/=33;
	score*=10;
	//printf("sigma:%lf\n",sigma);
	//printf("vary ok\n");
	return score;
}

int save2file(double f1,double f2,double f3,double f4,double f5)
{
	FILE* fp;
	if((fp = fopen("score.txt","w"))==NULL)
	{
		printf("cannot open file!\n");
		return 0;
	}
	double temp[5]={f1,f2,f3,f4,f5};
	int i=0;
	for(i=0;i<5;i++)
		fprintf(fp,"%lf ",temp[i]);
	fclose(fp);
	if((fp = fopen("average.txt","w"))==NULL)
	{
		printf("cannot open file!\n");
		return 0;
	}
	fprintf(fp,"%lf ",f5);
	fclose(fp);
	if((fp = fopen("randomscore.txt","w"))==NULL)
	{
		printf("cannot open file!\n");
		return 0;
	}
	fprintf(fp,"%lf ",f1);
	fclose(fp);
	return 1;
}
