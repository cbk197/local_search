#include<stdio.h>
#include<conio.h>
#include<iostream>
#include<string.h>
#include<math.h>
float readFloat(FILE *f) {
	char c;
	c = fgetc(f);
	while (c < 48 || c > 57) {
		c = fgetc(f);
	};
	float Num = 0;
	while (c > 47 && c < 58) {
		Num = Num * 10 + (c - 48);
		c = fgetc(f);
	}
	if (c == 46) {
		c = fgetc(f);
		int tmp = 10;
		while (c > 47 && c < 58) {
			Num += (float)(c - 48) / tmp;
			tmp = tmp * 10;
			c = fgetc(f);
		}
		return Num;
	}
	else {
		return Num;
	}
}

int readInteger(FILE *f) {
	char c;
	int Num = 0;
	c = fgetc(f);
	while (c < 48 || c >57) {
		c = fgetc(f);
	}

	while (c > 47 && c < 58) {
		Num = Num * 10 + (c - 48);
		c = fgetc(f);
	}
	return Num;
}

float** caculDis(float** L, int n) {
	float** R = new float*[n];
	for (int i = 0; i < n; i++) {
		R[i] = new float[n];
	}

	for (int i = 0; i < n; i++) {
		memset(R[i], 0, n * sizeof(float));
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (R[i][j] == 0) {
				R[i][j] = sqrt(pow(L[i][0] - L[j][0], 2) + pow(L[i][1] - L[j][1], 2));
				R[j][i] = R[i][j];

			}
		}
	}
	return R;
}

void printResult(FILE* f, int n, int* p, int Name) {
	char filename[6];
		filename[0] = Name/10+48;
		filename[1] = Name%10+48;
		filename[2] = '.';
		filename[3] = 't';
		filename[4] = 'x';
		filename[5] = 't';
		filename[6] = '\0';
	float y;
	int x,z;
	int* Path = new int[n-1];
	float* Time = new float[n-1];
	for (int i = 0; i < n - 1; i++) {
		Path[i] = readInteger(f);
		Time[Path[i]-1] = readFloat(f);
		
	}
	printf("\n\n");
	FILE* f1; 
	f1 = fopen(filename,"w+");
	for(int i = 1; i< n; i++){
		
		if(i % 5==0){
			fprintf(f1,"\n");
		}
		fprintf(f1,"	%d    %f ",p[i], Time[i-1] );
	}
	
	fprintf(f1,"\n");
	fprintf(f1,"	nodedie: ");
	for(int i = 0 ; i < n - 1 ; i++){
		x = readInteger(f);
		z = readInteger(f);
		if(z ==1){
			fprintf(f1,"%d    ", p[x]);
		}
	}
	fclose(f1);

}



int main() {
	float T = 0;
	float Ttsp = 0;
	float Tchar = 0;
	float Emin = 540;
	float Ewce = 40000;
	float U = 5;
	float V = 5;
	float Ptravel = 1;
	float x = 0;
	FILE* f;
	int NumPath = 1; 

	f = fopen("path.txt", "rb");
	if (f == NULL) {
		printf("can not open file!");
		exit(0);
	}
	int Node = 201;
	int* Path;
	int** PathArr = new int*[NumPath];
	
	for (int j = 0; j < NumPath; j++) {
		PathArr[j] = new int[Node];
		for (int i = 0; i < Node; i++) {
			PathArr[j][i] = readInteger(f);
		}
			
	}
	
	fclose(f);
	f = fopen("data.txt", "rb");
	float** Locat = new float*[Node];
	for (int i = 0; i < Node; i++) {
		Locat[i] = new float[2];
	}

	float* P = new float[Node];
	float* Eremain = new float[Node];
	for (int i = 0; i < Node; i++) {
		Locat[i][0] = readFloat(f);
		Locat[i][1] = readFloat(f);
		P[i] = readFloat(f);

		Eremain[i] = readFloat(f);
	}
	float** D;
	D = caculDis(Locat, Node);
	for (int k = 0; k < NumPath; k++) {
		char filename[6];
		FILE* f1;
		
		
		filename[0] = k/10+48;
		filename[1] = k%10+48;
		filename[2] = '.';
		filename[3] = 'd';
		filename[4] = 'a';
		filename[5] = 't';
		filename[6] = '\0';
		f1= fopen(filename,"w+");
		Path = PathArr[k];
		for (int i = 0; i < Node - 1; i++) {
			Ttsp = Ttsp + D[Path[i]][Path[i + 1]];
		};

		Ttsp = Ttsp + D[Path[Node - 1]][0];
		Ttsp = Ttsp / V;
		
		fprintf(f1,"param Node := %d; \nparam Emin := 540; \nparam U := 5;", Node -1);
//		for (int i = 0; i < Node; i++) {
//			fprintf(f1,"(%d)%d", i, PathArr[k][i]);
//		}

		fprintf(f1,"\nparam Ttsp := %0.4f ;\n", Ttsp);
		Tchar = (Ewce - Ttsp * Ptravel) / U;
		fprintf(f1,"\nparam Tchar := %f ;\n", Tchar);
		fprintf(f1,"param Tnode := ");

		for (int i = 1; i < Node; i++) {
			x = 0;
			for (int j = 0; j < i; j++) {
				x += D[Path[j]][Path[j + 1]];
			}
			fprintf(f1," %d  %0.3f   ", i, x/V);
		}
		fprintf(f1,";");
		fprintf(f1,"\nparam P := ");
		for (int i = 1; i < Node; i++) {
			fprintf(f1," %d %0.2f  ", i, P[Path[i]]);
		}
		fprintf(f1,";");
		fprintf(f1,"\nparam Eremain := ");
		for (int i = 1; i < Node; i++) {
			fprintf(f1," %d %0.4f  ", i, Eremain[Path[i]]);
		}
		fprintf(f1,";\n\n");
		fclose(f1);

	}
	
//	for(int i = 0 ; i< NumPath; i++){
//		 Path = PathArr[i];
//		 FILE* f1 ;
//		 char filename[6];
//		
//		
//		filename[0] = i/10+48;
//		filename[1] = i%10+48;
//		filename[2] = '.';
//		filename[3] = 'o';
//		filename[4] = 'u';
//		filename[5] = 't';
//		filename[6] = '\0';
//		 
//		 f1 = fopen(filename,"rb");
//		 if(f1 == NULL){
//		 	printf("openfile error");
//		 	exit(0);
//		 }
//		 printResult(f1,Node,Path,i);
//		 fclose(f1);
//	}
		
//	float Timax = 0,Tsumchar=0;
//	int indexMax = 0;
//	int countdie = 0;
//	T = Ttsp + Tchar;
//	Path = PathArr[0];
//	int* Tstatus = new int[Node];
//	for(int i = 0 ;i< Node; i++){
//		Tstatus[i] = 0;
//	}
//	float* Tmin = new float[Node];
//		for(int i = 0 ; i< Node; i++){
//			if((Eremain[Path[i]]- Emin)/P[Path[i]] > T){
//				Tmin[i] = 0;
//			}else{
//				Tmin[i] = (T*P[Path[i]] - Eremain[Path[i]] +Emin)/U; 	
//			}
//			
//			printf(" %d   tmin = %f        ", Path[i], Tmin[i]);
//		} 
//		printf("\n\n");
//		for (int i = 1; i < Node; i++) {
//			x = 0;
//			for (int j = 0; j < i; j++) {
//				x += D[Path[j]][Path[j + 1]];
//			}
//			x = x / 5;
//			printf(" %d=%d   ttsp = %f    Tsumchar : %f     Tcan = %f",i, Path[i], x, Tsumchar, (Eremain[Path[i]]- Emin)/P[Path[i]]);
//			if(Eremain[Path[i]] - (P[Path[i]]*(Tsumchar + x)) < Emin){
//				
//				if(Timax > Tmin[i]){
//					if(Eremain[i] - (P[Path[i]]*(Tsumchar + x - Timax)) < Emin ){
//						Tstatus[i] = 1;
//						printf("(die node)%d			",i);
//					
//					}else{
//						printf("(die node)%d			",indexMax);
//						Tstatus[indexMax] = 1;
//						Tsumchar =Tsumchar +  Tmin[i] - Timax;
//						Timax = Tmin[i];
//						indexMax = i;
//						for(int j=1 ; j< i ; j++){
//							if(Tstatus[j] == 0){
//								if(Timax < Tmin[j]){
//									Timax = Tmin[j];
//									indexMax = j;
//								}
//							}
//						}
//					}
//				}else{
//					printf("(die node)%d			",i);
//					Tstatus[i] = 1;
//				}
//				countdie++;
//			}else{
//				if(Timax < Tmin[i]){
//					Timax = Tmin[i];
//					indexMax = i;
//				}
//				Tsumchar = Tsumchar+ Tmin[i];
//			}
//			
//		}
//	printf("\n\n countdie : %d", countdie);
//	
	
	
}
