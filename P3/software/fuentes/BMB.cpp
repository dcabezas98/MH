#include <iostream>
#include <vector>
#include <stdlib.h>     // srand y rand
#include <algorithm>    // sort y random_shuffle
#include <time.h>
#include <math.h>

using namespace std;

int EVALS=0; // Llamadas a evaluación
const int LIMIT=10000; // Límite de evaluaciones (por búsqueda)

/* Parámetros del problema, fijos durante toda la ejecución*/
unsigned n=0; // Número de posibles elementos (número de genes)
unsigned m=0; // Tam. soluciones (número de genes a 1)

vector<int> elements; // Todos los elementos (números del 0 al n)

// random generator function:
int rndGen (int i) { return rand()%i;}

// random float
float randF(){return static_cast<float>(rand())/static_cast<float>(RAND_MAX);}

// Lee la entrada y la almacena en una matriz
void readInput (vector<vector<double>> &mat) {
  unsigned i, j;
  double f;
  int s = mat.size();
  int iter = s*(s-1)/2;
  for (int k=0; k<iter; k++) {
      cin >> i >> j >> f;
      mat[i][j] = mat[j][i] = f;
  }
}

/********** Evaluación ***************/

// Contribución de un elemento
double singleContribution(vector<int> &solution, vector<vector<double> > &mat, int elem) {
  double result = 0;
  vector<int>::iterator it;
  for (it = solution.begin(); it != solution.end(); it++) {
    result += mat[ elem ][ *it ];
  }
  return result;
}

// Variedad de una solución
double evaluateSolution(vector<int> &solution, vector<vector<double> > &mat) {
  double fitness = 0;
  vector<int>::iterator it;
  for (it = solution.begin(); it != solution.end(); it++) {
    fitness += singleContribution(solution, mat, *it);
  }
  return fitness /= 2; // Las cuenta doble
}

// Genera solución aleatoria
vector<int> randomSol(){

  vector<int> selected;

  random_shuffle(elements.begin(),elements.end(),rndGen);

  while (selected.size()<m){ 
    selected.push_back(elements[selected.size()]);
  }

  return selected;
}

unsigned lowestContributor(vector<int> &solution, vector<vector<double> > &mat, double &min_contrib){

  unsigned lowest=0;
  double current_contrib;
  min_contrib = singleContribution(solution, mat, solution[lowest]);

  for (unsigned i=1 ; i<solution.size(); i++) {
    current_contrib = singleContribution(solution, mat, solution[i]); // Suma distancias del elemento a los seleccionados
    if (current_contrib < min_contrib) { // Si la suma es menor, lo reemplaza
      min_contrib = current_contrib;
      lowest = i;
    }
  }

  return lowest;
}

// Clase solución
class Solution {
  public:
    vector<int> s;
    double fitness;

    Solution(vector<vector<double> > &mat){
      s=randomSol();
      evaluate(mat);
    }

    void operator = (const Solution &sol) {
      s=sol.s;
      fitness=sol.fitness;
    }

    void evaluate(vector<vector<double> > &mat) {
        fitness = evaluateSolution(s, mat);
        EVALS++;
    }

    void localSearch(vector<vector<double> > &mat){

      unsigned lowest_pos;
      double contrib, min_contrib;

      vector<int>::iterator it; // Para iterar sobre los candidatos
      int candidate;

      bool carryon = true;
      while(carryon){
        carryon=false;
        lowest_pos=lowestContributor(s,mat,min_contrib);

        random_shuffle(elements.begin(),elements.end(),rndGen); // Barajo candidatos para explorar entornos en orden aleatorio
      
        for (it=elements.begin(); !carryon && it!= elements.end() && EVALS<LIMIT; it++){
          candidate=*it;
          if (find(s.begin(),s.end(),candidate)!=s.end()) // Ya está escogido
            continue; // Salto a la siguiente iteración

          contrib=singleContribution(s,mat,candidate)-mat[candidate][s[lowest_pos]]; // Calculo la contribución del candidato sin el que voy a quitar
          EVALS++; // Evalúo solución (factorizada)

          if(contrib > min_contrib){ // Salto a una mejor solución
            fitness=fitness+contrib-min_contrib;
            carryon=true;
            s[lowest_pos]=candidate;
          }
        }
      }
    }
};

void bmb(vector<vector<double> > &mat) {

  double best_fitness=0;

  clock_t t_start, t_total;
  t_start = clock();

  for (int i=0; i<10; i++){
    EVALS=0;
    Solution sol(mat);
    sol.localSearch(mat);

    if(sol.fitness>best_fitness)
      best_fitness=sol.fitness;
  }

  t_total = clock() - t_start;
  // output: Diversidad - Tiempo

  cout << best_fitness << "\t" << (double) t_total / CLOCKS_PER_SEC << endl;
}

/******************* MAIN **********************/

int main( int argc, char *argv[] ) {

  cin >> n >> m; // Leemos los parámetros n y m

  vector<double> v (n, 0); // Vector de ceros con n componentes
  vector<vector<double > > mat (n, v); // Matriz de nxn ceros
  readInput(mat); // Leemos la entrada

  for (unsigned i=0; i<n; i++) { // Todos los elementos
    elements.push_back(i);
  }
  
  cout << fixed;
  srand(stoi(argv[1])); // SEED as parameter
  bmb(mat);
}