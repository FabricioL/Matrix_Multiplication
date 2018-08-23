//mpic++ -o ejer3 ejercicio3_MPI.cpp
//  mpiexec -np 4 ./ejer3 entrada.in

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <mpi.h>

#define forn(n) for(int i = 0 ; i < n ; i++ ) 
#define fjn(j, n) for(j = 0 ; j < n ; j++ )

using namespace std;

MPI_Status status;
int main(int argc,  char* argv[]){
	int N, aleat,  M, view;
	srand(time(NULL));
	std::ifstream in( argv[1]);
    std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(in.rdbuf()); 
	cin>>M;
	cin>>N;
	cin>>aleat;
	cin>>view;
	
	int matriz_1[M][N];
	int matriz_2[N][M];
	int matriz_3[M][M];
	if(aleat){
		forn(M){
			int j;
			fjn(j, N){
				matriz_1[i][j] = rand() % 4;
				matriz_2[j][i] = rand() % 4;
			}
		}
	}else{
		forn(N){
			int j;
			fjn(j, M){
				cin>>matriz_2[i][j];				
			}
		}
		forn(M){
			int j;
			fjn(j, N){
				cin>>matriz_1[i][j];									
			}
		}
	}
	if (view)
		cout<<"Multiplicacion"<<endl;
	int nproc, mytid;
	int dest,fila,offset, source, i, j,k, averow, extra;

	int succ = MPI_Init(&argc,&argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &mytid);
	
	if (mytid == 0) {    
	    fila = M/nproc;
	    averow = M/(nproc -1);
	    extra = M%(nproc-1);
	    offset = 0;
	    
		for (dest=1; dest<=(nproc -1); dest++){
			fila = (dest <= extra) ? averow+1 : averow;     
			MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
			MPI_Send(&fila, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
			MPI_Send(&matriz_1[offset][0], fila*N, MPI_INT, dest, 1, MPI_COMM_WORLD);
			MPI_Send(&matriz_2, N*M, MPI_INT, dest, 1, MPI_COMM_WORLD);
			offset = offset + fila;
		}

	
		for (i=1; i<=(nproc-1); i++){
			source = i;
			MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&fila, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&matriz_3[offset][0], fila*M, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
		}

	    if (view){
		    cout<<"Matriz A"<<endl;
		    forn(N){
				int j;
				fjn(j, M){
					cout<<matriz_2[i][j]<<" ";						
				}
				cout<<endl;
			}
			cout<<"Matriz B"<<endl;
		   	forn(M){
				int j;
				fjn(j, N){
					cout<<matriz_1[i][j]<<" ";			
				}
				cout<<endl;
			}
			
			cout<<"Matriz BA"<<endl;
			forn(M){
				int j;
				fjn(j, M){
					cout<<matriz_3[i][j]<<" ";					
				}
				cout<<endl;
			}	
	    }    
  	}
  	if (mytid > 0) {
	    MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&fila, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&matriz_1, fila*N, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&matriz_2, N*M, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

		for (k=0; k<M; k++){
			for (i=0; i<fila; i++){
				matriz_3[i][k] = 0;
				for (j=0; j<N; j++)
					matriz_3[i][k] = matriz_3[i][k] + matriz_1[i][j] * matriz_2[j][k];
			}
		}
		MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&fila, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&matriz_3, fila*M, MPI_INT, 0, 2, MPI_COMM_WORLD);
	}


   	MPI_Finalize();
	
	return 0;

}