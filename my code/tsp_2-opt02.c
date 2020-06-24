#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define inputfile "input_0.csv"
#define outputfile "solution_yours_0.csv"

double norm(double x0, double y0, double x1, double y1) {
    return sqrt(pow(x0-x1,2) + pow(y0-y1,2));
}

void tsplib(int size, double* xs, double* ys, int* next, int* itermax) {

    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            if (i==j || i==j[next] || j==i[next]) {
                continue;
            }
            double abcd = norm(i[xs], i[ys], i[next][xs], i[next][ys]) + norm(j[xs], j[ys], j[next][xs], j[next][ys]);
            double acbd = norm(i[xs], i[ys], j[xs], j[ys]) + norm(j[next][xs], j[next][ys], i[next][xs], i[next][ys]);
            if (abcd > acbd) {
                int len=0;
                for (int k=i[next]; j!=k; k=k[next]) {
                    len++;
                }
                int tmp[len];
                for (int k=i[next], l=0; j!=k; k=k[next], l++) {
                    l[tmp] = k;
                }
                i[next][next] = j[next];
                for (int k=0, tmpj=j; k<len; k++, tmpj=tmpj[next]) {
                    tmpj[next] = (len-k-1)[tmp];
                }
                i[next] = j;

                double total_d = 0.0;
                for (int i=0; i<size; i++) {
                    total_d += norm(i[xs], i[ys], i[next][xs], i[next][ys]);
                }

                printf("iteration: %6d, total distance: %f\n", ++*itermax, total_d);
            }
        }
    }
}


// main program
int main(void) {

    // read tsp file
    FILE* fp;

    fp = fopen(inputfile, "r");

    // 行数のカウント
    int size = -1;//最初の行
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') size++;
    }
    fclose(fp);

    fp = fopen(inputfile, "r");

    double xs[size];
    double ys[size];
	char buf[3];

    // 座標の取り込み
	//fgets(buf,sizeof(buf),fp);//1行め飛ばし>うまくいかず
	while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n')
			break;
    }
    for (int i=0; i<size; i++) {
        fscanf(fp, "%lf,%lf", xs+i, ys+i);
    }
    fclose(fp);

    // 読み込み順の順番作成 [0->1->2->3->4->...]
    int next[size];
    for (int i=1; i<size; i++) {
        (i-1)[next] = i;
    }
    (size-1)[next] = 0;

    // 計算
    int iteration = 0;
    tsplib(size, xs, ys, next, &iteration);

	//ファイル出力
    if((fp=fopen(outputfile,"w"))!=NULL){
		fprintf(fp,"index\n");
        for(int i=0, j=0; i<size; i++, j=j[next]){
            fprintf(fp,"%d\n",j);
        }
        fclose(fp);
    }

    return 0;
}
