#include <iostream>
#include <vector>
#include <unordered_set>
#include <stdlib.h>     // srand y rand
#include <algorithm>    // sort
#include <time.h>

using namespace std;

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

/******************* Greedy ********************/

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

// Más lejano al resto
int farthestToAll(vector<vector<double> > &mat) {
  unordered_set<int> all_elements;
  for (unsigned i=0; i<mat.size(); i++) {
    all_elements.insert(i);
  }
  return farthestToSel(all_elements, all_elements, mat);
}

void greedy(vector<vector<double> > &mat, unsigned m) {
  unordered_set<int> non_selected, selected;
  int farthest;
  clock_t t_start, t_total;

  t_start = clock();
  // Inicializamos selected con el más lejano y non_selected con el resto
  for (unsigned i=0; i<mat.size(); i++) {
    non_selected.insert(i);
  }
  farthest = farthestToAll(mat);
  non_selected.erase( farthest );
  selected.insert( farthest );

  while( selected.size() < m ) {
    farthest = farthestToSel(non_selected, selected, mat); // Más lejano a los seleccionados
    non_selected.erase( farthest ); // Lo elimino de los restantes
    selected.insert( farthest ); // Lo añado a los seleccionados
  }
  t_total = clock() - t_start;

  double diversity = evaluateSolution(selected, mat); // Diversidad de la solución
  // output: Diversidad - Tiempo
  cout << diversity << "\t" << (double) t_total / CLOCKS_PER_SEC << endl;
}

/******************* MAIN **********************/

int main( int argc, char *argv[] ) {
  int n, m;

  cin >> n >> m; // Leemos los parámetros n y m
  vector<double> v (n, 0); // Vector de ceros con n componentes
  vector<vector<double > > mat (n, v); // Matriz de nxn ceros
  readInput(mat); // Leemos la entrada

  cout << fixed;

  greedy(mat, m);
}