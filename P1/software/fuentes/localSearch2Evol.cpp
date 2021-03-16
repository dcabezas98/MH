#include <iostream>
#include <vector>
#include <unordered_set>
#include <stdlib.h>     // srand y rand
#include <algorithm>    // advance
#include <time.h>

using namespace std;

int CALLS=0; // Llamadas a evaluación
const int LIMIT=100000; // Límite de evaluaciones

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
double singleContribution(unordered_set<int> &solution, vector<vector<double> > &mat, int elem) {
  double result = 0;
  unordered_set<int>::iterator it;
  for (it = solution.begin(); it != solution.end(); it++) {
    result += mat[ elem ][ *it ];
  }
  return result;
}

// Variedad de una solución
double evaluateSolution(unordered_set<int> &solution, vector<vector<double> > &mat) {
  double fitness = 0;
  unordered_set<int>::iterator it;
  for (it = solution.begin(); it != solution.end(); it++) {
    fitness += singleContribution(solution, mat, *it);
  }
  return fitness /= 2; // Las cuenta doble
}

/******************* Local Search ********************/

// Más lejano a los seleccionados (de los que quedan por elegir)
int farthestToSel(unordered_set<int> &non_selected, unordered_set<int> &selected, vector<vector<double> > &mat) {
  int farthest;
  double max_sum_dist, current_sum_dist;
  unordered_set<int>::iterator it;

  it = non_selected.begin();
  farthest = *it;
  max_sum_dist = singleContribution(selected, mat, farthest);

  for ( ; it!=non_selected.end(); it++) {
    current_sum_dist = singleContribution(selected, mat, *it); // Suma distancias del elemento a los seleccionados
    if (current_sum_dist > max_sum_dist) { // Si la suma es mayor, lo reemplaza
      max_sum_dist = current_sum_dist;
      farthest = *it;
    }
  }

  return farthest;
}

// Genera solución aleatoria
unordered_set<int> randomSol(unsigned m, unsigned n, unordered_set<int> &complement){

  int elem,rnd;

  unordered_set<int> selected;
  unordered_set<int> elements;
  for (unsigned i=0; i<n; i++) {
    elements.insert(i);
  }

  unordered_set<int>::iterator it;

  while (selected.size()<m){ 
    rnd=rand()%elements.size(); // Elijo un número aleatorio
    it=elements.begin(); 
    advance(it,rnd); // Selecciono el elemento correspondiente
    elem=*it;
    elements.erase( elem );
    selected.insert( elem );
  }

  complement=elements; // Elementos que no están en la solución

  return selected;
}

int lowestContributor(unordered_set<int> &solution, vector<vector<double> > &mat, double &min_contrib){

  int lowest;
  double current_contrib;
  unordered_set<int>::iterator it;

  it = solution.begin();
  lowest = *it;
  min_contrib = singleContribution(solution, mat, lowest);
  it++;
  for ( ; it!=solution.end(); it++) {
    current_contrib = singleContribution(solution, mat, *it); // Suma distancias del elemento a los seleccionados
    if (current_contrib < min_contrib) { // Si la suma es menor, lo reemplaza
      min_contrib = current_contrib;
      lowest = *it;
    }
  }

  return lowest;
}

// random generator function:
int rndGen (int i) { return rand()%i;}

void localSearch(vector<vector<double> > &mat, unsigned m) {

  unordered_set<int> non_selected; // Elementos no seleccionados
  unordered_set<int> solution = randomSol(m,mat.size(), non_selected);
  double diversity=evaluateSolution(solution, mat);

  cout << diversity << ", ";

  vector<int>::iterator it; // Para iterar sobre los candidatos
  int lowest, best_candidate;
  double contrib, min_contrib;

  bool carryon = true;
  while (carryon && CALLS < LIMIT){
    carryon=false;
    lowest=lowestContributor(solution, mat, min_contrib); // Menor contribuyente
    solution.erase(lowest); // Lo elimino

    best_candidate = farthestToSel(non_selected, solution,mat); // Candidato más lejano a los seleccionados
    CALLS+=non_selected.size(); // Elije la mejor entre todas estas soluciones
    contrib = singleContribution(solution, mat, best_candidate);

    if (contrib > min_contrib){ // Si encuentra una mejor en el entorno, se actualiza y continúa
        diversity = diversity + contrib - min_contrib; // Modificamos diversidad (sólo el factor que cambia)
        carryon=true;
        solution.insert(best_candidate); // Insertamos el candidato
        non_selected.erase(best_candidate); // Lo borramos de la lista de candidatos
        non_selected.insert(lowest); // Ahora el menor contribuyente es un candidato más
        cout << diversity << ", "; // Cada vez que actualiza, imprime la diversidad
    } 
  }
  /*
  if (solution.size() < m) 
    solution.insert(lowest); // Si no se encontró uno mejor, recuperamos la solución
  */

 cout << endl;
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