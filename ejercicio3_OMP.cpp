#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <omp.h>

#define forn(n) for(int i = 0 ; i < n ; i++ ) 
#define fjn(j, n) for(j = 0 ; j < n ; j++ )
using namespace std;

int main(){
	int N, aleat,  M, view;
	srand(time(NULL));
	cin>>M;
	cin>>N;
	cin>>aleat;
	cin>>view;
	cout<<" "<<M<<" "<<N<<" "<<aleat<<" "<<view<<endl;
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
				cout<<matriz_2[i][j]<<" ";						
			}
			cout<<endl;
		}
		forn(M){
			int j;
			fjn(j, N){
				cin>>matriz_1[i][j];				
				cout<<matriz_1[i][j]<<" ";						
			}
			cout<<endl;
		}
	}
	if (view)
		cout<<"Multiplicacion"<<endl;
	
	forn(M){
		int j , k;	
		#pragma omp parallel for private (j,k)
		fjn(k,M){
			matriz_3[i][k] = 0;
			fjn(j, N){
				matriz_3[i][k] =  matriz_1[i][j] * matriz_2[j][k] + matriz_3[i][k];
			}
   		}   
   	}
	if (view)
	    cout<<"Matriz A"<<endl;
    forn(N){
		int j;
		fjn(j, M){
			cout<<matriz_2[i][j]<<" ";						
		}
		cout<<endl;
	}
	if (view)
		cout<<"Matriz B"<<endl;
   	forn(M){
		int j;
		fjn(j, N){
			cout<<matriz_1[i][j]<<" ";			
		}
		cout<<endl;
	}
	if (view)
		cout<<"Matriz BA"<<endl;
	forn(M){
		int j;
		fjn(j, M){
			cout<<matriz_3[i][j]<<" ";					
		}
		cout<<endl;
	}	
	return 0;

}