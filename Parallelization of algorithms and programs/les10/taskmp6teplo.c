#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main(int argc, char *argv[]){
    double *u, *unew, delta, maxdelta;
    double eps=1.e-6;
    double h, tau;
    int N;
    int i;
    int count = 0;
    FILE *ff;
    omp_set_num_threads(omp_get_num_procs());
    if(argc !=2 ){
	    printf("Usage: exefile npoints\n");
	    exit(-1);
    }
    N = atoi(argv[1]);
    if (N == 0) {
	    printf("Set N to 1000\n");
	    N = 1000;
    } else {
	    printf("Set N to %d\n", N);
    }
    if((u = malloc((N+1)*sizeof(double))) == NULL){
	    printf("Can't allocate memory for u\n");
	    exit(-1);
    }
    if((unew = malloc((N+1)*sizeof(double))) == NULL){
        printf("Can't allocate memory for unew\n");
        free(u);
        exit(-1);
    }
    // begin & bound values
    for(i=1; i<N; i++){
	    u[i] = 0;
    }
    unew[0] = u[0] = 1.0;
    unew[N] = u[N] = 0;
    h = 1.0/N;
    tau = 0.5*(h*h);
#pragma omp parallel
    {
        while(1){
#pragma omp for
            for (i=1; i<N; i++){
                unew[i] = u[i]+(tau/(h*h))*(u[i-1]-2*u[i]+u[i+1]);
            }   	
#pragma omp single
            {
                maxdelta = 0;
                for (i=1; i<N; i++){
                    delta = fabs(unew[i]-u[i]);
                    if(delta > maxdelta) maxdelta = delta;
                }
            }
            if(maxdelta < eps) break;
#pragma omp single
            {
                count++;
            }
#pragma omp for
            for(i = 1; i < N; i++){
    	        u[i] = unew[i];
            }
        }
    }
    printf("%d steps\n",count);
    if ((ff = fopen("seqres", "w+")) == NULL){
       printf("Can't open file\n"); free(u); free(unew);
       exit(-1);
    }
    for(i=0; i<N+1; i++)
       fprintf(ff, "%f\n", unew[i]);
    fclose(ff); free(u); free(unew);
    return 0;  
}