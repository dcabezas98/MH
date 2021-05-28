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

    void neighbor(unsigned &index_out, int &elem_in, double &delta, vector<vector<double> > &mat) { // Busca una solución en el entorno
      
      index_out = rand()%m; // Índice del elemento que saco

      do {
        elem_in = rand()%n; // Elemento que meto
      } while(find(s.begin(),s.end(),elem_in)!=s.end()); // Si ya está, cogemos otro

      // Calcula la diferencia que supondría en la fitness
      double contrib_in = singleContribution(s,mat,elem_in)-mat[elem_in][s[index_out]];
      double contrib_out = singleContribution(s,mat,s[index_out]);

      delta=contrib_in-contrib_out;
      EVALS++;
    }

    void jump(unsigned index_out, int elem_in, double delta){ // Salta a nueva solución
      s[index_out]=elem_in;
      fitness+=delta;
    }

    void mutate(unsigned t, vector<vector<double> > &mat) { // Cambia la solución moderadamente
      
      random_shuffle(s.begin(),s.end(),rndGen); // Eliminaré los t primeros aleatoriamente

      random_shuffle(elements.begin(),elements.end(),rndGen);

      vector<int> new_elems;

      unsigned j = 0;
      while(new_elems.size()<t){
        if(find(s.begin(),s.end(),elements[j])==s.end()) // Si no está ya
          new_elems.push_back(elements[j]);
        j++;
      }

      for (unsigned i = 0; i < t; i++){
        s[i]=new_elems[i];
      }

      evaluate(mat);
    }

    void localSearch_es(vector<vector<double> > &mat){

      unsigned exitos=1, vecinos, max_vecinos=10*n;
      unsigned max_exitos=0.1*max_vecinos;
      unsigned M=LIMIT/max_vecinos;
      double delta;
      unsigned index_out;
      int elem_in;

      float mu=0.3, phi=0.3;

      float T=-mu*fitness/log(phi);
      float Tf=(float)1/1000;
      while (T<Tf){
        Tf=Tf/10;
      }
      float beta=(T-Tf)/(M*T*Tf);

      while(exitos>0 && EVALS<LIMIT){
        exitos=0;
        vecinos=0;
        while(vecinos<max_vecinos && exitos<max_exitos){
          neighbor(index_out, elem_in, delta, mat);
          vecinos++;
          if(delta>0 || randF() <= exp(delta/T)){
            jump(index_out,elem_in,delta);
            exitos++;
          }
        }
        T=T/(1+beta*T);
      }
    }
};

void ils_es(vector<vector<double> > &mat) {
  
  unsigned t=0.1*m;

  clock_t t_start, t_total;
  t_start = clock();

  Solution best_sol(mat);
  best_sol.localSearch_es(mat);

  for (int i=1; i<10; i++){
    EVALS=0;
    Solution sol(best_sol);
    sol.mutate(t,mat);
    sol.localSearch_es(mat);
        
    if(sol.fitness>best_sol.fitness)
      best_sol=sol;
  }

  t_total = clock() - t_start;
  // output: Diversidad - Tiempo

  cout << best_sol.fitness << "\t" << (double) t_total / CLOCKS_PER_SEC << endl;
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
  ils_es(mat);
}