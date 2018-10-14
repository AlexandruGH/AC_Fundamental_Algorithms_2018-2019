#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

/**
 * Buna tuturor!
 *
 * Mai jos aveti un exemplu de folosire al profilerului pe o metoda de sortare.
 * Va rog sa urmariti comentariile si sa incepeti sa cititi codul incepand cu  functia main()
 *
*/

//Declarare profiler
Profiler profiler("Direct Sorting");

//Prototipurile functiilor folosite in program
void read_vector(int **, int *);

void print_vector(int *, int);

void bubble_sort(int *, int);

void swap(int *, int*);

void best_case();

/* Pentru inceput este recomandat sa rulam algoritmul pe un exemplu simplu.
 * Astfel citim un vector si o lungime dintr-un fisier, dupa care aplicam
 *algoritmul de sortare, iar mai apoi printam vectorul sortat.
 * Pentru a realiza acest pas trebuie sa decomentam liniile urmate de  Pasul 1 si sa comantam liniile cu Pasul 2.
 * Dupa ce ne-am asigurat ca algoritmul ruleaza pe un exemplu simplu,
 *rulam statistica de best_case, in cazul nostru.
 * Pentru asta comentam din nou partea ce contine Pasul 1 si sa decomentam partea cu Pasul 2
 *
*/
int main()
{
	int n, i;
	int *v;

	//read_vector(&v, &n);          Pasul 1

	//print_vector(v, n);           Pasul 1

	//bubble_sort(v, n);		    Pasul 1

	//print_vector(v, n);			Pasul 1

	best_case();  //				Pasul 2
	//average_case();               TODO
	//worst_case();					TODO

	profiler.showReport();    //	Pasul 2

	
	getchar();
	return 0;
}
//Functia citeste din fisier atat dimensiunea vectorului reprezentata de n
// cat si vectorul in aceiasi functie, urmand sa returneze aceste 2 entitati in functia main
// cu ajutorul pointarilor.
void read_vector(int **v, int *n)
{
	FILE *fIn;
	int i;
	errno_t error;

	if (0 != (error = fopen_s(&fIn, "input.txt", "r")))
	{
		perror("Eroare la citire fisier.\n");
		getchar();
		exit(error);
	}

	fscanf_s(fIn, "%d", n);

	if (NULL == (*v = (int*)calloc(sizeof(int), *n)))
	{
		perror("Eroare la alocare memorie.\n");
		exit(1);
	}

	for (i = 0; i < *n; i++)
	{
		fscanf_s(fIn, "%d", &(*v)[i]);
	}

	fclose(fIn);
}

// Functie extrem de utila in momentul in care vrem sa verificam sirurile generate cu FillRandomArray,
// sirurile dupa sortare sau orice alt sir de numere intermediar.
void print_vector(int *v, int n)
{
	int i;

	printf_s("Vectorul este: \n");

	for (i = 0; i < n; i++)
	{
		printf_s("%d ", v[i]);
	}

	printf_s("\n");
}


//Metoda bubble sort imbunatatita.
// Am ales aceasta metoda deoarece metoda clasica necesita un timp de rulare prea mare.

void bubble_sort(int *v, int n)
{
	int i, j, temp, notSorted;

	notSorted = 1;

	while(notSorted)
	{
		notSorted = 0;
		for (i = 0; i < n - 1; i++)
		{
			profiler.countOperation("Bubble_sort_comparissons", n);
			if (v[i] > v[i + 1])
			{
				profiler.countOperation("Bubble_sort_assignments", n, 3);
				swap(&v[i], &v[i + 1]);
				notSorted = 1;
			}
		}
	}
}

void swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}


//Metoda ce analizeaza cazul cel mai favorabil.
//Metodele de average si worst sunt similare cu urmatoarele mentiuni:
//Trebuie schimbata configuratia sirului de numere si 
//in cazul average, trebuie repetat procesul de 5 ori.
// Cu alte cuvinte vom avea 2 for-uri in loc de unul.
void best_case()
{
	int *v, n;
	/* Atentie! 
	* Trebuie sa alocam memorie pentru vector chiar daca il generam cu FillRandomArray
	* Este mai rapid sa alocam o singura zona de memorie de dimensiune maxima,
	* decat sa alocam zone de memorie la fiecare iteratie din for.
	*/
	if (NULL == (v = (int*)calloc(sizeof(int), 10000)))
	{
		perror("Error at allocating memory.\n");
		getchar();
		exit(1);
	}

	for (n = 100; n <= 10000; n += 500)
	{
		//Initializare operatii
		// In cazul in care nu are loc nicio asignare sau comparatie, graficul va sti sa indice 0.
		profiler.countOperation("Bubble_sort_comparissons", n, 0);
		profiler.countOperation("Bubble_sort_assignments", n, 0);

		// Generare sir de numere. Semnatura si parametrii functiei sunt in fisierul Profiler de pe moodle.
		FillRandomArray(v, n, 0, 999, false, 1);

		//Atentie!
		// Vom avea nevoie de acelasi sir initial de numere pentru toti 3 algoritmii.
		// Acest lucru se poate realiza cu ajutorul functiei: memcpy(vector_destinatie, vector_sura, lungime_vector_sursa) - vezi functie pe net
		// Vom avea nevoie de inca 2 vectori alocati dinamic.
		// Si vom executa algoritmii pe vectori separati, insa cu aceleasi date initiale.


		bubble_sort(v, n);
		//Tot aici v-om executa si ceilalti algoritmi.
		//selection_sort(...);
		//insertion_sort(...);

	
		//Dorim sa vedem si numarul total de operatii si nu doar numarul de asignari sau comparatii individual
		profiler.addSeries("Bubble_sort_sum", "Bubble_sort_comparissons", "Bubble_sort_assignments");
		// La fel pentru 
		// Insertion_sort
		// Selection_sort

	}

	// In final vrem sa grupam toti algoritmii pe acelasi grafic si totodata comparatiile si asignarile pentru fiecare in parte.
	// Atentie!
	// Numele dintre ghilimele este foarte important si trebuie sa coincida exact cu cel din codul vostru.
	profiler.createGroup("Best_Case_Assignments", "Bubble_sort_assignments", "Insertion_sort_assignments", "Selection_sort_assignments");
	profiler.createGroup("Best_Case_Comparissons", "Bubble_sort_comparissons", "Insertion_sort_comparissons","Selection_sort_comparissons");
	profiler.createGroup("Best_Case_Sum", "Bubble_sort_sum", "Selection_sort_sum", "Insertion_sort_sum");

	//Este frumos ca dupa executie sa eliberam zonele de memorie alocate
	free(v);
}
