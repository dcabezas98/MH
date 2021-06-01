extern "C" {
#include "cec17.h"
}
#include <iostream>
#include <vector>
#include <random>

using namespace std;

// Parámetros
float INIT_CELERITY = 1;
float DECREASE = 0.98;

double best;
int n; // Número de soluciones activas

uniform_real_distribution<> prob(0, 1);
uniform_real_distribution<> u(-1, 1);

void clip(vector<double> &sol, int lower=-100, int upper=100) {
  for (auto &val : sol) {
    if (val < lower) {
      val = lower;
    }
    else if (val > upper) {
      val = upper;
    }
  }
}

int branchSearch(vector<double> &sol, vector<double> &mom, float cel, double fitness, int evals, mt19937 &gen){
  const size_t dim = sol.size();

  float p;
  float dif;


  p = prob(gen);
  dif = fitness-best;

  if(p<(fitness-best)/cel){ // TODO

  }
  else if(p<1){}


  cel = DECREASE*cel;
}

int main() {
  vector<double> sol;
  std::uniform_real_distribution<> range(-100.0, 100.0);

  int dim = 10;
  //for(int dim=10; dim<=30; dim+=20) {
    for (int seed=42; seed<=87; seed+=5){
      for (int funcid = 1; funcid <= 30; funcid++) {
        vector<double> sol(dim);
        vector<double> mom(dim); // Momento
        double fitness;

        cec17_init("branch", funcid, dim);

        //cerr <<"Warning: output by console, if you want to create the output file you have to comment cec17_print_output()" <<endl;
        //cec17_print_output(); // Comment to generate the output file

        mt19937 gen(seed); // Inicio semilla

        for (int i = 0; i < dim; i++) { // Solución inicial
          sol[i] = range(gen);
          mom[i] = u(gen); // Momento inicial
        }
        fitness = cec17_fitness(&sol[0]);

        branchSearch(sol, mom, INIT_CELERITY, fitness, dim*10000-1, gen);
      }
    }
  //}

}
