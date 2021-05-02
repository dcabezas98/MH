#include <iostream>
#include <vector>
#include <stdlib.h>     // srand y rand
#include <algorithm>    // sort y random_shuffle
#include <time.h>

using namespace std;

int EVALS=0; // Llamadas a evaluación
const int LIMIT=100000; // Límite de evaluaciones

const int CHROMOSOMES=50; // Nº cromosomas

const float PROB_CROSS=0.7;
float PROB_MUT=0.1; // Hay que partir por n (nº genes por cromosoma) luego

/* Parámetros del problema, fijos durante toda la ejecución*/
unsigned n=0; // Número de posibles elementos (número de genes)
unsigned m=0; // Tam. soluciones (número de genes a 1)

// random generator function:
int rndGen (int i) { return rand()%i;}

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

class Solution {
  public:
    vector<int> s;
    double fitness;

    void operator = (const Solution &sol) {
      s=sol.s;
      fitness=sol.fitness;
    }

    void evaluate(vector<vector<double> > &mat) {
      fitness = evaluateSolution(s, mat);
    }

    void mutate() { // Muta un gen aleatorio
      
      int elem_out = rand()%m; // Índice del elemento que saco

      int elem_in;
      do {
        elem_in = rand()%n; // Elemento que meto
      } while(find(s.begin(),s.end(),elem_in)!=s.end()); // Si ya está, cogemos otro

      s[elem_out]=elem_in; // En la posición del que saco, pongo el que entra
    }
};

// Cruze basado en posición
Solution positionCross(const Solution &sol1, const Solution &sol2, vector<vector<double> > &mat){
    Solution child;
    bool inS1, inS2;
    vector<int> aux(n); // Elementos selecionados en un padre y no en el otro
    unsigned n_aux=0;

    for(unsigned i = 0; i<n; i++){
        inS1=find(sol1.s.begin(),sol1.s.end(),i)!=sol1.s.end();
        inS2=find(sol2.s.begin(),sol2.s.end(),i)!=sol2.s.end();
        if (inS1 && inS2) // Está en los dos padres
            child.s.push_back(i); 
        else if (!inS1 && !inS2){} // No está en ninguno
        else { // Está en uno de ellos
          aux[n_aux]=i;
          n_aux++;
        }
    }
    for (unsigned i = 0; i < n-n_aux; i++){ // Elimino los espacios no utilizados
      aux.pop_back();
    }

    random_shuffle(aux.begin(),aux.end(),rndGen); // Relleno con aleatorios
    while (child.s.size()<m){
      child.s.push_back(aux.back());
      aux.pop_back();
    }

    return child;
}

// Genera solución aleatoria
vector<int> randomSol(){

  vector<int> selected;
  vector<int> elements;
  for (unsigned i=0; i<n; i++) {
    elements.push_back(i);
  }

  random_shuffle(elements.begin(),elements.end(),rndGen);

  while (selected.size()<m){ 
    selected.push_back(elements.back());
    elements.pop_back();
  }

  return selected;
}

class Population {
public:
  vector<Solution> v;

  Population(bool first=false) {
    v.resize(CHROMOSOMES);
    if (first){ // La primera población de todas
      for (int i = 0; i < CHROMOSOMES; i++){
        v[i].s=randomSol();
        // v[i].fitness=evaluateSolution(v[i].s);
      }
    }
  }

  void operator = (const Population &pop) {
      v=pop.v;
  }

  void evaluate(vector<vector<double> > &mat) {
    for (unsigned i=0;i<CHROMOSOMES; i++){
      v[i].evaluate(mat);
    }
  }

  int binTournament(){ // Devuelve el mejor índice de dos aleatorios
    int s1 = rand()%CHROMOSOMES;
    int s2 = rand()%CHROMOSOMES;
    return v[s1].fitness>v[s2].fitness ? s1 : s2;
  }

  void replacement(){
    Population pop=Population();
    int best = bestSol(); // Índice de la mejor solución
    bool elitism=false;
    for (unsigned i = 0; i<CHROMOSOMES; i++){
      int s=binTournament();
      pop.v[i]=v[s];
      if (s==best) // La mejor solución sobrevive
        elitism=true;
    }

    // Elitismo
    if (!elitism) // La mejor solución no ha sobrevivido, reemplaza a la peor de las nuevas
        pop.v[pop.worstSol()]=v[best];

    *this=pop;
  }

  void mutate(){
    int mutations = PROB_MUT*n*CHROMOSOMES; // Número esperado de mutaciones
    for (int i = 0; i < mutations; i++){
      v[rand()%CHROMOSOMES].mutate(); // Una solución aleatoria muta un gen aleatorio
    }
  }

  int bestSol(){ // Índice mejor solución
    int best_index=0;
    for(unsigned i = 1; i < CHROMOSOMES; i++){
        if(v[i].fitness>v[best_index].fitness)
            best_index=i;
    }
    return best_index;
  }

  int worstSol(){ // Índice peor solución
    int worst_index=0;
    for(unsigned i = 1; i < CHROMOSOMES; i++){
        if(v[i].fitness<v[worst_index].fitness)
            worst_index=i;
    }
    return worst_index;  
  }

  void cross(vector<vector<double> > &mat){
      int n_2cross=PROB_CROSS*CHROMOSOMES; // Doble del número esperado de cruces
      Solution child1, child2;
      for(int i=0; i<n_2cross; i+=2){
          child1=positionCross(v[i],v[i+1],mat); // Genero los hijos
          child2=positionCross(v[i],v[i+1],mat);
          v[i]=child1; // Reemplazan a los padres
          v[i+1]=child2;
      }
  }
};

void agg(vector<vector<double> > &mat) {

  clock_t t_start, t_total;
  t_start = clock();

  Population population(true); // Primera población

  while(EVALS<LIMIT){
    population.cross(mat);
    population.mutate();
    population.evaluate(mat);
    EVALS+=CHROMOSOMES;
    population.replacement();
  }

  t_total = clock() - t_start;
  // output: Diversidad - Tiempo
  double diversity = population.v[population.bestSol()].fitness; // Fitness de la mejor solución

  cout << diversity << "\t" << (double) t_total / CLOCKS_PER_SEC << endl;
}

/******************* MAIN **********************/

int main( int argc, char *argv[] ) {

  cin >> n >> m; // Leemos los parámetros n y m
  PROB_MUT/=n; // La probabilidad de mutación va dividida por n (nº de genes por cromosoma)

  vector<double> v (n, 0); // Vector de ceros con n componentes
  vector<vector<double > > mat (n, v); // Matriz de nxn ceros
  readInput(mat); // Leemos la entrada

  cout << fixed;
  srand(stoi(argv[1])); // SEED as parameter

  agg(mat);
}