#include <iostream>
#include <vector>
#include <stdlib.h>     // srand y rand
#include <algorithm>    // sort y random_shuffle
#include <time.h>

using namespace std;

int EVALS=0; // Llamadas a evaluación
const int LIMIT=100000; // Límite de evaluaciones

const int CHROMOSOMES=50; // Nº cromosomas

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

    void removeGreatest(vector<vector<double> > &mat){ // Borra el mayor contribuyente de la solución
      double current_contrib, max_contrib=0;
      vector<int>::iterator it, it_g;
      for (it=s.begin(); it!=s.end(); it++){
        current_contrib = singleContribution(s, mat, *it); // Suma distancias del elemento a los seleccionados
        if (current_contrib > max_contrib) { // Si la suma es mayor, lo reemplaza
          max_contrib = current_contrib;
          it_g = it;
        }
      }

      s.erase(it_g);
    }

    void addGreatest(vector<vector<double> > &mat){ // Añade el mayor contribuyente de fuera de la solución
      int greatest=-1;
      double current_contrib, max_contrib=0;

      for (unsigned i = 0; i < n; i++){
        if (find(s.begin(),s.end(),i)==s.end()) { // Si no está en la solución
          current_contrib = singleContribution(s, mat, i); // Suma distancias del elemento a los seleccionados
          if (current_contrib > max_contrib) { // Si la suma es mayor, lo reemplaza
            max_contrib = current_contrib;
            greatest = i;
          }
        }
      }

      s.push_back(greatest);
    }

    void repair(vector<vector<double> > &mat){
      while (s.size()>m){ // Sobran elementos, quito de la solución
        removeGreatest(mat);
      }

      while (s.size()<m){ // Faltan elementos, añado de fuera
        addGreatest(mat);
      }
    }
};

// Cruze uniforme
Solution uniformCross(const Solution &sol1, const Solution &sol2, vector<vector<double> > &mat){
    Solution child;
    bool inS1, inS2;

    for(unsigned i = 0; i<n; i++){
        inS1=find(sol1.s.begin(),sol1.s.end(),i)!=sol1.s.end();
        inS2=find(sol2.s.begin(),sol2.s.end(),i)!=sol2.s.end();
        if (inS1 && inS2) // Está en los dos padres
            child.s.push_back(i); 
        else if (!inS1 && !inS2){} // No está en ninguno
        else { // Está en uno de ellos, lo selecciono con un 50% de probabilidad
            if (rand()%2==1)
                child.s.push_back(i);
        }
    }
    child.repair(mat); // Reparamos el hijo, puede ser no factible
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

  void selection(int &parent1, int &parent2){
    parent1=binTournament();
    parent2=binTournament();
  }
    
  int bestSol(){ // Índice mejor solución
    int best_index=0;
    for(unsigned i = 1; i < CHROMOSOMES; i++){
        if(v[i].fitness>v[best_index].fitness)
            best_index=i;
    }
    return best_index;
  }

  void worst2Sol(unsigned &worst_index, unsigned &second_worst_index){ // Índices de las dos peores soluciones
    worst_index=0;
    for(unsigned i = 1; i < CHROMOSOMES; i++){
        if(v[i].fitness<v[worst_index].fitness)
            worst_index=i;
    }
    second_worst_index= worst_index==0 ? 1 : 0;
    for(unsigned i = 1; i < CHROMOSOMES; i++){
        if(v[i].fitness<v[second_worst_index].fitness && i!=worst_index)
            second_worst_index=i;
    } 
  }

  void replacement(const Solution &child1, const Solution &child2){
    unsigned w1=0, w2=0;
    worst2Sol(w1,w2);
    //cout << w1 << " " << w2 << endl;
    /* Hay que quedarse con las dos mejores de las 4 */
    if(child1.fitness>v[w2].fitness){
      v[w1]=v[w2];
      v[w2]=child1;
    }else if (child1.fitness>v[w1].fitness)
      v[w1]=child1;
    if(child2.fitness>v[w1].fitness)
      v[w1]=child2;
  }

  void cross(int p1, int p2, Solution &c1, Solution &c2, vector<vector<double> > &mat){
    c1 = uniformCross(v[p1], v[p2], mat);
    c2 = uniformCross(v[p1], v[p2], mat);
  }
};

void age(vector<vector<double> > &mat) {

  clock_t t_start, t_total;
  t_start = clock();

  Population population(true); // Primera población
  population.evaluate(mat); // Evaluación de todos
  EVALS+=CHROMOSOMES;
 
  int p1, p2; // Índices de los padres
  Solution c1, c2; // Hijos

  float prob_mut_chrom = PROB_MUT*n; // Probabilidad de mutación por cromosoma
  float x; // Para ver si se muta

  while(EVALS<LIMIT){
    population.selection(p1,p2);
    population.cross(p1,p2,c1,c2, mat);

    x=static_cast<float>(rand())/static_cast<float>(RAND_MAX); // Número aleatorio en [0,1]
    if (x<prob_mut_chrom)
      c1.mutate();
    x=static_cast<float>(rand())/static_cast<float>(RAND_MAX); // Número aleatorio en [0,1]
    if (x<prob_mut_chrom)
      c2.mutate();
    
    c1.evaluate(mat);
    c2.evaluate(mat);
    EVALS+=2;
    population.replacement(c1,c2);

    //cout << population.v[population.bestSol()].fitness << endl;
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

  age(mat);
}