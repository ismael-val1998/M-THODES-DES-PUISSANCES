#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>


#define SIZE 10


int main(int argc, char *argv[])

{
	
	//Déclaration des variables 
	
	float A[SIZE][SIZE], V[SIZE], V_nouveau[SIZE];
	float temp, alpha_new, alpha_old, error, module, max = 0.0;
	int i, j, n, step=1;
	
	clock_t start, end;
	
	float Tableau[100] = {0};
	
	
	start = clock();
	
	//Saisir taille de la matrice
	printf("Enter Ordre de la Matrice A : ");
	scanf("%d", &n);
	 
	
	//Pour avoir une certaine précision
	printf("Entrer une marge de tolérance entre 0 et 1 : ");
	scanf("%f", &error);
	
	
	/* on ne parallélise pas les entrées à saisir*/
	
	//Les valeurs de la matrice
	printf("Entrer les Coefficients de la Matrice : \n");
	
	// Fonction Matrice
		
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
		   printf("A[%d][%d]=",i,j);
		   scanf("%f", &A[i][j]);
		}
	}

	
	// fonction Vecteur
	// Vecteur initial non nul de norme 1
	
	do
	{
		printf("\nLa norme du Vecteur initial doit etre égale à 1 \n");
		printf("Entrer les valeurs du vecteur \n");
		float a = 0.0;
		
		for(i=0;i<n;i++)
		{
			printf("V[%d]=",i);
			scanf("%f", &V[i]);
			a = a + V[i]*V[i];
		}
		
		module = sqrt(a);
		printf("Norme de |V| : %f \n", module);
		printf("\n");	
	}while(module != 1);
	
	 
	//Intialisation de valeur de alpha
	alpha_old = 1;
	 
	
	//Multiplication
	
	Start:
	
	#pragma omp parallel for reduction(+: temp) private(i,j)
	for(i=0;i<n;i++)
	
	{
		temp = 0.0;
		for(j=0;j<n;j++)
		{
			temp = temp + A[i][j]*V[j];
		}
		V_nouveau[i] = temp;
	}
	
	
	//Changer la valeur du vecteur
	
	#pragma omp parallel 
	{
		#pragma omp for
		
		for(i=0;i<n;i++)
		{
			V[i] = V_nouveau[i];
		}
	}
	
	
	//Recherche de la plus grande valeur de alpha
	
	#pragma omp parallel private(i)
	{
		alpha_new = fabs(V[0]);
		
		#pragma omp for
		for(i=1;i<n;i++)
		{
			if(fabs(V[i]) > alpha_new)
			{
				alpha_new = fabs(V[i]);
			}
		}
	}
	
	
	
	//Normalisation du vecteur
	
	#pragma omp parallel private(i)
	{
		#pragma omp for
		
		for(i=0;i<n;i++)
		{
			V[i] = V[i]/alpha_new;
		}
	}
	
	
	//Résultats
	
	printf("\n\n STEP : %d \n", step);
	
	printf("Valeur Propre = %f\n", alpha_new);
	
	printf("Vecteurs Associés :\n");
	
	
	for(i=0;i<n;i++)
	{
		printf("%f\t", V[i]);
	}
	printf("\n");

	
	Tableau[step] = alpha_new;
	
	printf("**************************************************\n");
	
	//Vérification de la précision 
	
	
	if(fabs(alpha_new - alpha_old) > error)
	{
		alpha_old = alpha_new;
		step++;
		goto Start;
	}
		
	// Determination de la valeur propre max
	
	int z;
		
	for(z=0;z<100;z++)
	{
		if(Tableau[z] > max)
		{
			max = Tableau[z];
		}
		
	}
	printf("la valeur propre max est : %f \n", max);
	
	end = clock();
	
	double Finaltime = ( (double)end - start / CLOCKS_PER_SEC);
	
	printf("le temps d'éxution du programme : %f secondes \n", Finaltime);
	
	return(0);
	
}
