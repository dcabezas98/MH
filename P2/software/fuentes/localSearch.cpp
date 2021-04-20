#include <iostream>
#include <vector>
#include <stdlib.h>     // srand y rand
#include <algorithm>    // sort y random_shuffle
#include <time.h>

using namespace std;

int CALLS=0; // Llamadas a evaluación
const int LIMIT=100000; // Límite de evaluaciones

/* Parámetros del problema, fijos durante toda la ejecución*/
int n=0;
int m=0;

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

    void evaluate(vector<vector<double> > &mat) {
      fitness = evaluateSolution(s, mat);
    }

    void mutate() { // Muta un gen aleatorio
      int elem_in, elem_out;

      elem_out = rand()%m;

      do {
        elem_in = rand()%n;
      } while(find(s.begin(),s.end(),elem_in)!=s.end()); // Si ya está, cogemos otro
    }

    // Elemento que menos contribuye
    int lowestContributor(vector<vector<double> > &mat, double &min_contrib){

      int lowest;
      double current_contrib;
      vector<int>::iterator it;

      it = s.begin();
      lowest = *it;
      min_contrib = singleContribution(s, mat, lowest);
      it++;
      for ( ; it!=s.end(); it++) {
        current_contrib = singleContribution(s, mat, *it); // Suma distancias del elemento a los seleccionados
        if (current_contrib < min_contrib) { // Si la suma es menor, lo reemplaza
          min_contrib = current_contrib;
          lowest = *it;
        }
      }

      return lowest;
    }

    int greatestContributor(vector<vector<double> > &mat, double &max_contrib, bool from_out=false){

      int greatest;
      double current_contrib;

      if(from_out){ // fuera de la solución
        max_contrib=0;
        greatest = 0;
        for (int i = 0; i < n; i++){
          if (find(s.begin(),s.end(),i)==s.end()) { // Si no está en la solución
            current_contrib = singleContribution(s, mat, i); // Suma distancias del elemento a los seleccionados
            if (current_contrib < max_contrib) { // Si la suma es menor, lo reemplaza
              max_contrib = current_contrib;
              greatest = i;
            }
          }
        }

      } else { // dentro de la solución
        vector<int>::iterator it;
        it = s.begin();
        greatest = *it;
        max_contrib = singleContribution(s, mat, greatest);
        it++;
        for ( ; it!=s.end(); it++) {
          current_contrib = singleContribution(s, mat, *it); // Suma distancias del elemento a los seleccionados
          if (current_contrib < max_contrib) { // Si la suma es menor, lo reemplaza
            max_contrib = current_contrib;
            greatest = *it;
          }
        }
      }

      return greatest;
    }

    void repair(vector<vector<double> > &mat){
      while (s.size()>m){ // Sobran elementos, quito de la solución
        double contrib;
        int g = greatestContributor(mat, contrib);
        s.erase(find(s.begin(),s.end(),g));
      }

      while (s.size()<m){ // Faltan elementos, añado de fuera
        double contrib;
        int g = greatestContributor(mat, contrib, true);
        s.push_back(g);
      }
    }
};

// Genera solución aleatoria
vector<int> randomSol(){

  int elem,rnd;

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

  Population(int tam, bool first=false) {
    v.resize(tam);
    if (first){ // La primera población de todas
      for (int i = 0; i < tam; i++){
        v[i].s=randomSol();
        // v[i].fitness=evaluateSolution(v[i].s);
      }
    }
  }

  void evaluate(vector<vector<double> > &mat) {
    for (int i=0;i<v.size(); i++){
      v[i].evaluate(mat);
    }
  }

  int binTournament(){ // Devuelve el mejor índice de dos aleatorios
    int s1 = rand()%v.size();
    int s2 = rand()%v.size();
    return v[s1].fitness>v[s2].fitness ? s1 : s2;
  }

  Population selection(){
    Population pop=Population(v.size());

    for (int i = 0; i<v.size(); i++){
      int s=binTournament();
      pop.v[i]=v[s];
    }
  }

  void mutate(double prob){
    int mutations = prob*m*v.size(); // Número esperado de mutaciones
    for (int i = 0; i < mutations; i++){
      v[rand()%v.size()].mutate(); // Una solución aleatoria muta un gen aleatorio
    }
  }
};

void localSearch(vector<vector<double> > &mat, unsigned m) {

  clock_t t_start, t_total;
  t_start = clock();

  unordered_set<int> complement; // Elementos no seleccionados
  unordered_set<int> solution = randomSol(m,mat.size());//, complement);
  double diversity=evaluateSolution(solution, mat);

  // Pasamos los no seleccionados a un vector
  vector<int> non_selected;
  non_selected.insert(non_selected.end(),complement.begin(),complement.end());

  vector<int>::iterator it; // Para iterar sobre los candidatos
  int lowest, candidate;
  double contrib, min_contrib;

  bool carryon = true;
  while (carryon){
    carryon=false;
    lowest=lowestContributor(solution, mat, min_contrib); // Menor contribuyente
    solution.erase(lowest); // Lo elimino

    // Barajo los candidatos
    random_shuffle(non_selected.begin(), non_selected.end(), rndGen);

    for(it=non_selected.begin(); !carryon && it!=non_selected.end() && CALLS<LIMIT; it++){
      candidate= *it;
      contrib = singleContribution(solution, mat, candidate); // Contribución del candidato
      CALLS++; // He evaluado una nueva solución (factorizada)

      if (contrib > min_contrib){ // Si encuentra una mejor en el entorno, se actualiza y continúa
        diversity = diversity + contrib - min_contrib; // Modificamos diversidad (sólo el factor que cambia)
        carryon=true;
        solution.insert(candidate); // Insertamos el candidato
        non_selected.erase(it); // Lo borramos de la lista de candidatos
        non_selected.push_back(lowest); // Ahora el menor contribuyente es un candidato más
      }
    } // Si se sale del for pero no del while es porque se ha actualizado
  } // Si se sale del for y tb del while es por una de las condiciones de finalización
  /*
  if (solution.size() < m) 
    solution.insert(lowest); // Si no se encontró uno mejor, recuperamos la solución
    */

  t_total = clock() - t_start;
  // output: Diversidad - Tiempo
  cout << diversity << "\t" << (double) t_total / CLOCKS_PER_SEC << "\t" << CALLS << endl;
}

/******************* MAIN **********************/

int main( int argc, char *argv[] ) {
  int n, m;

  cin >> n >> m; // Leemos los parámetros n y m
  vector<double> v (n, 0); // Vector de ceros con n componentes
  vector<vector<double > > mat (n, v); // Matriz de nxn ceros
  readInput(mat); // Leemos la entrada

  cout << fixed;
  srand(stoi(argv[1])); // SEED as parameter

  localSearch(mat, m);
}