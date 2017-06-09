#include<stdio.h>
#include<time.h>

int main() {

  FILE *fp_C,*fp_D,*fp_Rp;
     fp_C = fopen("C.txt", "r");
     double C_array[99911];
     for (int i = 0; i < 99911 ; i++) {
         fscanf(fp_C, "%lf", &C_array[i]);
     }

     fclose(fp_C);

     fp_D = fopen("D.txt","r");
     double D_array[99911];
     for (int j = 0; j < 99911; j++) {
       fscanf(fp_D,"%lf",&D_array[j]);
     }

     fclose(fp_D);

    int Rp_array[10001];
     fp_Rp = fopen("Rp.txt", "r");
    for (int i = 0; i < 10001 ; i++) {
        fscanf(fp_Rp, "%d", &Rp_array[i]);
    }

    fclose(fp_Rp);

    double V[1][10000];
    for (int i = 0; i < 10000; i++) {
      V[0][i] = 1.0;
    }

double sonuc[1][10000];
 for (int  i = 0; i < 10000; i++) {
   sonuc[0][i]=0;
 }

clock_t start = clock(), diff;

for (int k = 0; k < 99912; k++) {
  int satir = 0,counter=0;
 for (int  j = 0; j < 99911; j++) {
   if (C_array[j]==k) {
    while(satir < j){
      satir = satir + (Rp_array[counter+1]-Rp_array[counter]);
      counter++;
    }
    sonuc[0][k-1] += D_array[j] * V[0][counter];
    }
   }
  }
FILE *f =fopen("sonuc.txt","w");

diff = clock() - start;
int msec = diff*1000/CLOCKS_PER_SEC;
fprintf(f, "Bu program %d saniyede islemini tamamlamistir \n", msec/1000);

for (int i = 0; i < 10000; i++) {
  /* code */
  fprintf(f,"%.3f ",sonuc[0][i]);
}
 fclose(f);

  return 0;
}
