#include <stdio.h>  
#include <memory.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <python.h>
#define RN 16

typedef char ElemType;  

int DES_MakeSubKeys(ElemType key[64],ElemType **subKeys,int round);  
int DES_PC1_Transform(ElemType key[64], ElemType tempbts[56]);  
int DES_PC2_Transform(ElemType key[56], ElemType tempbts[]);  
int DES_ROL(ElemType data[56], int time);  
int DES_IP_Transform(ElemType data[64]);  
int DES_IP_1_Transform(ElemType data[64]);  
int DES_E_Transform(ElemType data[48]);  
int DES_P_Transform(ElemType data[32]);  
int DES_SBOX(ElemType data[48]);  
int DES_XOR(ElemType R[48], ElemType L[48],int count);  
int DES_Swap(ElemType left[32],ElemType right[32]);   
int DES_Encrypt(char *plainFile, char *keyStr,char *cipherFile,int round);  
void analysis();

//initial perbutation
int IP_Table[64] = { 
			57,49,41,33,25,17,9,1,  
   			59,51,43,35,27,19,11,3,  
      		61,53,45,37,29,21,13,5,  
        	63,55,47,39,31,23,15,7,  
   			56,48,40,32,24,16,8,0,  
			58,50,42,34,26,18,10,2,  
       		60,52,44,36,28,20,12,4,  
 			62,54,46,38,30,22,14,6};   
//inverse initial perbutation 
int IP_1_Table[64] = {
           39,7,47,15,55,23,63,31,  
           38,6,46,14,54,22,62,30,  
           37,5,45,13,53,21,61,29,  
           36,4,44,12,52,20,60,28,  
           35,3,43,11,51,19,59,27,  
           34,2,42,10,50,18,58,26,  
           33,1,41,9,49,17,57,25,  
           32,0,40,8,48,16,56,24};  
  
//扩展置换表
int E_Table[48] = {31, 0, 1, 2, 3, 4,  
                  3,  4, 5, 6, 7, 8,  
                  7,  8,9,10,11,12,  
                  11,12,13,14,15,16,  
                  15,16,17,18,19,20,  
                  19,20,21,22,23,24,  
                  23,24,25,26,27,28,  
                  27,28,29,30,31, 0};  
  
//置换函数P 
int P_Table[32] = {15,6,19,20,28,11,27,16,  
                  0,14,22,25,4,17,30,9,  
                  1,7,23,13,31,26,2,8,  
                  18,12,29,5,21,10,3,24};  
  
//S盒子 
int S[8][4][16] =//S1  
        /*    {{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},  
              {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},  
                {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},  
                {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},  
                //S2  
              {{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},  
              {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},  
              {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},  
              {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},  
              //S3  
              {{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},  
              {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},  
                {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},  
              {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},  
              //S4  
              {{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},  
              {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},  
              {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},  
              {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},  
              //S5  
              {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},  
              {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},  
              {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},  
              {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},  
              //S6  
              {{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},  
              {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},  
              {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},  
              {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},  
              //S7  
              {{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},  
              {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},  
              {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},  
              {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},  
              //S8  
              {{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},  
              {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},  
              {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},  
              {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}};  */
              {{{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
              {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}},
				 
               {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
              {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}},
		 
        {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
              {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}},
				 
		       {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
              {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}}, 
                
			        {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
              {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}},
				 
	       {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
              {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}},
				  
		        {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
              {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}},
				 
			        {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
              {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
                {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}}};
 
int PC_1[56] = {56,48,40,32,24,16,8,  
              0,57,49,41,33,25,17,  
              9,1,58,50,42,34,26,  
              18,10,2,59,51,43,35,  
              62,54,46,38,30,22,14,  
              6,61,53,45,37,29,21,  
              13,5,60,52,44,36,28,  
              20,12,4,27,19,11,3};  
  
//置换 
int PC_2[48] = {13,16,10,23,0,4,2,27,  
              14,5,20,9,22,18,11,3,  
              25,7,15,6,26,19,12,1,  
              40,51,30,36,46,54,29,39,  
              50,44,32,46,43,48,38,55,  
              33,52,45,41,49,35,28,31};  
  
//左移位置每一次 
int MOVE_TIMES[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};  

void Encrypt(ElemType* p1,ElemType*p2,ElemType*p3,int round);
void bitchange(char* plainText,char* Keys,char* Cyphers,int round[]);
void cleanCypher(char* Cyphers);
void diff(char* file1,char* file2,char* result);

int main(int argc,char* argv[])
{

	int i;
	FILE* fp;
	
/****************************load round*******************************/	
	
	printf("> Input each round number:[total %d rounds]\n",RN);
	int round[RN];
		
	if((fp = fopen("round.txt","w"))==NULL)
	{
		printf("cannot open file!\n");
		exit(0);
	}
	for(i=0;i<RN;i++)
	{
		scanf("%d",round+i);
		fprintf(fp,"%d ",round[i]);
	}		
	fclose(fp);
	
/************************clean the Cypher file**************************/
	
	cleanCypher("Cypher1.txt");
	cleanCypher("Cypher2.txt");
	cleanCypher("Cypher11.txt");
	cleanCypher("Cypher21.txt");
	cleanCypher("Cypher12.txt");
	cleanCypher("Cypher22.txt");

/***************************original Encryption*************************/
	
	bitchange("plainText1.txt","Keys1.txt","Cypher1.txt",round);
	bitchange("plainText11.txt","Keys11.txt","Cypher11.txt",round);
	bitchange("plainText12.txt","Keys12.txt","Cypher12.txt",round);

/*********************substitue 1 bit in Keys***************************/	

	bitchange("plainText2.txt","Keys2.txt","Cypher2.txt",round);
	bitchange("plainText21.txt","Keys21.txt","Cypher21.txt",round);
	bitchange("plainText22.txt","Keys22.txt","Cypher22.txt",round);

/******************************Analysis*********************************/
	diff("plainText1.txt","plainText2.txt","change1.txt");
	//diff("plainText11.txt","plainText21.txt","change2.txt");
	//diff("plainText12.txt","plainText22.txt","change3.txt");
	diff("Cypher11.txt","Cypher21.txt","diff2.txt");
	//diff("Cypher11.txt","Cypher21.txt","diff2.txt");
	//diff("Cypher12.txt","Cypher22.txt","diff3.txt");
	analysis("Cypher1.txt","Cypher2.txt","Analysis1.txt");
	analysis("Cypher11.txt","Cypher21.txt","Analysis2.txt");
	analysis("Cypher12.txt","Cypher22.txt","Analysis3.txt");
	
/****************************Graphic Result******************************/
	
	Py_SetProgramName(argv[0]);
	Py_Initialize();
	PyRun_SimpleString("execfile('plot.py')");
	Py_Finalize();
 	
	return 0;	
} 
void diff(char* file1,char* file2,char* result)
{
  ElemType temp1[64*16]={0};
  ElemType temp2[64*16]={0};
  ElemType temp;
  int rslt[16][64]={0};
    int i;
  int j;
  //read file
  FILE* fp1,*fp2,*fp3;
  if((fp1 = fopen(file1,"r"))==NULL)
  {
    printf("cannot open file!\n");
    exit(0);
  }
  for(j=0;j<16;j++)
  {
    for(i=0;i<64;i++)
      fscanf(fp1,"%c ",temp1+64*j+i);
    fscanf(fp1,"\n",temp);
  }
  if((fp2 = fopen(file2,"r"))==NULL)
  {
    printf("cannot open file!\n");
    exit(0);
  }
  for(j=0;j<16;j++)
  {
    for(i=0;i<64;i++)
      fscanf(fp2,"%c ",temp2+64*j+i);
    fscanf(fp2,"\n",temp);
  }
  fclose(fp1);
  fclose(fp2);
  //write diffs into result.txt

  for(i=0;i<16;i++)
    for(j=0;j<64;j++)
      rslt[i][j]=((temp1[64*i+j]+temp2[64*i+j])%2);
  if((fp3 = fopen(result,"w"))==NULL)
  {
    printf("cannot open file!\n");
    exit(0);
  }
  for(i=0;i<16;i++)
  {
    for(j=0;j<64;j++)
      fprintf(fp3,"%d ",rslt[i][j]);
    fprintf(fp3,"\n");
  }
  
  fclose(fp3);
  return;
  
} 
void cleanCypher(char* Cyphers)
{
	FILE* fd;
	fd = fopen(Cyphers,"w");
	if(fd < 0)
    	printf("open file.txt failed\n");
	else{
    	ftruncate(fd,0);
    	lseek(fd,0,SEEK_SET);
    	close(fd);
	}
	return;
}

void bitchange(char* plainText,char* Keys,char* Cyphers,int round[])
{
	ElemType Bits[64];
	ElemType Cypher[64];
	ElemType Key[64];
	int k;
	int i;
	FILE* fp;
	if((fp = fopen(plainText,"r"))==NULL)
	{
		printf("cannot open file!\n");
		exit(0);
	}
	fread(Bits,sizeof(char),64,fp);
	fclose(fp);
	if((fp = fopen(Keys,"r"))==NULL)
	{
		printf("cannot open file1\n");
		exit(0);
	}
	fread(Key,sizeof(char),64,fp);
	fclose(fp);

	for(k=0;k<RN;k++)
	{
		Encrypt(Bits,Cypher,Key,round[k]);
		printf("> Result: \n");
		for(i=0;i<64;i++)
		{
			printf("%c",*(Cypher+i));
			if((i+1)%8==0)
				putchar(' ');		
		}
		putchar('\n');
	/*	printf("> Result2: \n");
		for(i=0;i<64;i++)
		{
			printf("%c",*(Cypher+i));
			if((i+1)%8==0)
				putchar(' ');		
		}
		*/
		if((fp = fopen(Cyphers,"a"))==NULL)
		{
			printf("cannot open file1\n");
			exit(0);
		}
		//fprintf(fp,"Cypher round %d:",round[i);
		for(i=0;i<64;i++)
			fprintf(fp,"%c ",*(Cypher+i));
		fprintf(fp,"\n");	
		fclose(fp);	
		printf("\n");
	}
	return;
} 
void analysis(char* Cypher1,char* Cypher2,char* Analysis)
{
	FILE* fp;
	if((fp = fopen(Cypher1,"r"))==NULL)
		{
			printf("cannot open file1\n");
			exit(0);
		}
	ElemType temp1[RN][64];
	int i,j;
	for(i=0;i<RN;i++)
	{
		for(j=0;j<64;j++)
			fscanf(fp,"%c ",&temp1[i][j]);
		fscanf(fp,"\n");
	}
	fclose(fp);
	if((fp = fopen(Cypher2,"r"))==NULL)
		{
			printf("cannot open file1\n");
			exit(0);
		}
	ElemType temp2[RN][64];
	for(i=0;i<RN;i++)
	{
		for(j=0;j<64;j++)
			fscanf(fp,"%c ",&temp2[i][j]);
		fscanf(fp,"\n");
	}
	fclose(fp);
	int diff[RN]={0};
	for(i=0;i<RN;i++)
	{
		for(j=0;j<64;j++)
			if(temp1[i][j]!=temp2[i][j])
				diff[i]++;
	}
	if((fp = fopen(Analysis,"w"))==NULL)
		{
			printf("cannot open file1\n");
			exit(0);
		}
	for(i=0;i<RN;i++)
		fprintf(fp,"%d ",diff[i]);
	fclose(fp);
	printf("Analysis OK!\n");
	return;
}
int DES_MakeSubKeys(ElemType key[64],ElemType **subKeys,int round){  
    ElemType temp[56];  
    int cnt;  
    DES_PC1_Transform(key,temp);//PC1置换    printf("\nover\n");
    for(cnt = 0; cnt <round; cnt++){//16轮迭代，产生16个子密钥 
        DES_ROL(temp,MOVE_TIMES[cnt]);//循环左移
        DES_PC2_Transform(temp,subKeys[cnt]);//PC2置换，产生子密        printf("\nover\n");
    }  
    
    return 0;  
}  
void Encrypt(ElemType* p1,ElemType* p2,ElemType* p3,int round)
{
	ElemType plainBits[64];
    ElemType copyRight[48]; 
    int cnt;  
    int i;
    for(i=0;i<64;i++)
    	plainBits[i] = *(p1+i);
    ElemType ** subKeys = (ElemType **)malloc(round*sizeof(ElemType *));
	for(i=0;i<round;i++){
		subKeys[i] = (ElemType *)malloc(48 * sizeof(ElemType));
	}
    DES_MakeSubKeys(p3,subKeys,round);
	DES_IP_Transform(p1);
	for(cnt = 0; cnt < round; cnt++)
	{         
        memcpy(copyRight,plainBits+32,32);  
        //扩展右半部分，从32位扩展到48位
        DES_E_Transform(copyRight);  
        //将右半部分与子密钥进行异或操作 
        DES_XOR(copyRight,subKeys[cnt],48);   
        //异或结果进入S盒子，输出32位结果 
        DES_SBOX(copyRight);  
        //P置换
        DES_P_Transform(copyRight);  
        //将明文左半部分与右半部分进行异或
        DES_XOR(plainBits,copyRight,32);  
        if(cnt != (round-1))
		{  
            //左右交换 
            DES_Swap(plainBits,plainBits+32);  
        }  
    }  
    //逆初始置换 
    DES_IP_1_Transform(plainBits); 
    for(i=0;i<64;i++)
    	*(p2+i) = plainBits[i];
   	return 0;
}
int DES_IP_Transform(ElemType data[64]){  
    int cnt;  
    ElemType temp[64];  
    for(cnt = 0; cnt < 64; cnt++){  
        temp[cnt] = data[IP_Table[cnt]];  
    }  
    memcpy(data,temp,64);  
    return 0;  
}  
  
//Initial Perbutation
int DES_IP_1_Transform(ElemType data[64]){  
    int cnt;  
    ElemType temp[64];  
    for(cnt = 0; cnt < 64; cnt++){  
        temp[cnt] = data[IP_1_Table[cnt]];  
    }  
    memcpy(data,temp,64);  
    return 0;  
}  
  
//扩展置换  
int DES_E_Transform(ElemType data[48]){  
    int cnt;  
    ElemType temp[48];  
    for(cnt = 0; cnt < 48; cnt++){  
        temp[cnt] = data[E_Table[cnt]];  
    }     
    memcpy(data,temp,48);  
    return 0;  
}  
  
//P置换
int DES_P_Transform(ElemType data[32]){  
    int cnt;  
    ElemType temp[32];  
    for(cnt = 0; cnt < 32; cnt++){  
        temp[cnt] = data[P_Table[cnt]];  
    }     
    memcpy(data,temp,32);  
    return 0;  
}  
  
//异或 
int DES_XOR(ElemType R[48], ElemType L[48] ,int count){  
    int cnt;  
    for(cnt = 0; cnt < count; cnt++){  
        R[cnt] ^= L[cnt]; 
    }  
    return 0;  
}  
  
//S盒子置换  
int DES_SBOX(ElemType data[48]){  
    int cnt;  
    int line,row,output;  
    int cur1,cur2;  
    for(cnt = 0; cnt < 8; cnt++){  
        cur1 = cnt*6;  
        cur2 = cnt<<2;  
          
        //计算在S盒子中的行与列
        line = (data[cur1]<<1) + data[cur1+5];  
        row = (data[cur1+1]<<3) + (data[cur1+2]<<2)  
            + (data[cur1+3]<<1) + data[cur1+4];  
        output = S[cnt][line][row];  
  
        //化为2进制 
        data[cur2] = (output&0X08)>>3;  
        data[cur2+1] = (output&0X04)>>2;  
        data[cur2+2] = (output&0X02)>>1;  
        data[cur2+3] = output&0x01;  
    }     
    return 0;  
}  
  
//交换  
int DES_Swap(ElemType left[32], ElemType right[32]){  
    ElemType temp[32];  
    memcpy(temp,left,32);     
    memcpy(left,right,32);    
    memcpy(right,temp,32);  
    return 0;  
}  
//密钥置换1  
int DES_PC1_Transform(ElemType key[64], ElemType tempbts[56]){  
    int cnt;      
    for(cnt = 0; cnt < 56; cnt++){  
        tempbts[cnt] = key[PC_1[cnt]];  
    }  
    return 0;  
}  
  
//密钥置换2  
int DES_PC2_Transform(ElemType key[56], ElemType tempbts[48]){  
    int i;  
    for(i = 0; i < 48; i++){  
        tempbts[i] = key[PC_2[i]];  
    }  
    return 0;  
}  
  
//循环左移 
int DES_ROL(ElemType data[56], int time){     
    ElemType temp[56];  
  
    //保存将要循环移动到右边的位 
    memcpy(temp,data,time);  
    memcpy(temp+time,data+28,time);  
      
    //前28位移动
    memcpy(data,data+time,28-time);  
    memcpy(data+28-time,temp,time);  
  
    //后28位移动
    memcpy(data+28,data+28+time,28-time);  
    memcpy(data+56-time,temp+time,time);      
  
    return 0;  
}  


