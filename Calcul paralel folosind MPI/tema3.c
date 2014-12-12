/*
    Bogdan Stefan 333CA 
    Tema 3 APD - Calcul paralel folosind MPI
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define NUM_COLORS 256

int** creareMatrix(int lines, int cols) {

    int i;
    int ** new_matrix = (int**) malloc (lines * sizeof(int*));

    for (i = 0; i < lines; i++)
        new_matrix[i] = (int*) calloc (cols, sizeof(int));

    return new_matrix;
}

void distrugeMatrix(int ** matrix, int lines) {

    int i;

    for (i = 0; i < lines; i++)
        free(matrix[i]);

    free(matrix);
}

void scriereFisier(char* nume_fisier, int width, int height, int** matrix) {

    int i, j;

    FILE* out;
    out = fopen(nume_fisier, "w");
    fprintf(out, "P2\n");
    fprintf(out, "%d %d\n", width, height);
    fprintf(out, "255\n");
    for (i = height - 1; i >= 0; i--) {
        for (j = 0; j < width; j++) {
            fprintf(out, "%d ", matrix[i][j]);
        }
        fprintf(out, "\n");
    }
    fclose(out);

}


int main (int argc, char* argv[]) {

	if (argc != 3) {
		printf ("Error : try ./exe_name in_file out_file\n");
		exit(0);
	}


    int numtasks, rank, rc, tag = 1; 
    MPI_Status Stat;

    rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS) {   
   		printf ("Error starting MPI program. Terminating.\n");
    	MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int i, j, tip, max_iter, width, height, pixeli, first, first_line, first_col;
    int received_pixels, start_line_recv, start_col_recv, line, col, nr = 0, iter, color;
    int * results;
    int *results_recv;
    double col_min, col_max, line_min, line_max, rezolutie, z1, z2, w, h, x0, x, y, xtemp, y0;

    // Citirea
    if (rank == 0) {

    	FILE *in;
    	in = fopen(argv[1], "r");

    	fscanf(in, "%d", &tip);

    	fscanf(in, "%lf", &col_min);
    	fscanf(in, "%lf", &col_max);
    	fscanf(in, "%lf", &line_min);
    	fscanf(in, "%lf", &line_max);

    	fscanf(in, "%lf", &rezolutie);

    	fscanf(in, "%d", &max_iter);

    	if (tip == 1) {
    		fscanf(in, "%lf", &z1);
    		fscanf(in, "%lf", &z2);    
    	}
    	else {
    		z1 = 0;
    		z2 = 0;
    	}

    	fclose(in);
    }

    // Trimitem catre toate procesele
    MPI_Bcast(&tip, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&col_min, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&col_max, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&line_min, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&line_max, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&rezolutie, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&max_iter, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&z1, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&z2, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

 	w = floor ((col_max - col_min) / rezolutie);
 	h = floor ((line_max - line_min) / rezolutie);

 	width = (int) w;
 	height = (int) h;
   
    if (width * height % numtasks == 0) {
        pixeli = width * height / numtasks;
        first = rank * pixeli;
    }
    else {
        if (rank < numtasks - 1) { 
            pixeli = width * height / numtasks + 1;
            first = rank * pixeli;
        }
        else {
            pixeli = width * height - (width * height / numtasks + 1) * (numtasks - 1);
            first = rank * (width * height / numtasks + 1);
        }
    }
 
    results = (int*) calloc (pixeli, sizeof(int));

 	first_line = first / width; 
 	first_col = first % width;

    i = first_line;
    j = first_col;

    // Mandelbrot
    if (tip == 0) 
        while (nr < pixeli) {

            y0 = line_min + i * rezolutie;
            x0 = col_min + j * rezolutie;

            x = 0; 
            y = 0;
            iter = 0;

            while (x * x + y * y < 2 * 2 && iter < max_iter) {
                xtemp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtemp;
                iter++;
            }
            color = iter % NUM_COLORS;
            results[nr] = color;

            j++;
            if (j == width) {
                i++;
                j = 0;
            }
            nr++;
        }

    // Julia
    if (tip == 1) {
        x0 = z1; 
        y0 = z2;
        while (nr < pixeli) {

            y = line_min + i * rezolutie;
            x = col_min + j * rezolutie;

            iter = 0;

            while (x * x + y * y < 2 * 2 && iter < max_iter) {
                xtemp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtemp;
                iter++;
            }
            color = iter % NUM_COLORS;
            results[nr] = color;

            j++;
            if (j == width) {
                i++;
                j = 0;
            }
            nr++;
        }
    }


 	// Trimiterea datelor catre master(rank = 0)
 	if (rank != 0) {
 		// trimitem numarul de pixeli prelucrati
 		rc = MPI_Send(&pixeli, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
 		
 		// trimitem coordonatele de unde am inceput prelucrarea 
 		rc = MPI_Send(&first_line, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
 		rc = MPI_Send(&first_col, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);

        // trimitem sirul de valori rezultate in urma prelucrarii
        rc = MPI_Send(&results[0], pixeli, MPI_INT, 0, tag, MPI_COMM_WORLD);
 	}


 	if (rank == 0) {
  		int** matrix;
        matrix = creareMatrix(height, width);

 		// scriem in matrice elementele pe care le-am prelucrat in procesul MASTER(rank = 0)
	 	for (i = 0; i < pixeli; i++) {
	 		matrix[i / (int) width][i % (int) width] = results[i];
	 	}		


	 	for (i = 1; i < numtasks; i++) {
			rc = MPI_Recv(&received_pixels, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);
		 	results_recv = (int*) calloc (received_pixels, sizeof(int));

 			rc = MPI_Recv(&start_line_recv, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);
 			rc = MPI_Recv(&start_col_recv, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);

            rc = MPI_Recv(&results_recv[0], received_pixels, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);
 
 			line = start_line_recv;
			col = start_col_recv;

 			for (j = 0; j < received_pixels; j++) {
 				matrix[line][col] = results_recv[j];
 				col++;
 				if (col == width) {
 					col = 0;
 					line++;
 				}
 			}

	 		free(results_recv);
	 	}

        scriereFisier(argv[2], width, height, matrix);
        distrugeMatrix(matrix, height);
 	} 

 	free(results);
    MPI_Finalize();    

return 0;
}