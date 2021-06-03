extern "C" {
#include "cec17.h"
}
#include <iostream>
#include <vector>
#include <random>

using namespace std;

// Parámetros
float INIT_CELERITY = 1;
float DECREASE_SUCCESS = 0.99;
float DECREASE_FAIL = 0.9;
float PROB_SPLIT = 100; // Coeficiente que multiplica a dif para decidir si la solución se divide

float ALPHA = 0.5; // Máxima importancia al vecino (un valor alto le da menos peso al momento)

float LOW_CELERITY = 0.25*INIT_CELERITY; // Por debajo de este valor, la solución puede desaparecer
int MIN_EVALS_SPLIT = 4000; // Mínimo de evaluaciones para dividirse. Se multiplica por la dimensión

int IMPROVE_LIMIT = 50; // Máximo de intentos de obtener un vecino mejor

int spare_evals=0;

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

void branchSearch(vector<double> &sol, vector<double> &mom, float cel, int evals, mt19937 &gen){
  const size_t dim = sol.size();

  float p;
  double dif; // Diferencia respecto a la mejor

  vector<double> modify (dim), neighbor(dim);
  double neighbor_fitness, improvement;

  double fitness = cec17_fitness(&sol[0]);
  evals--;
  if (fitness < best) best=fitness;

  while(evals>0){
    dif = cec17_error(fitness);
    p = prob(gen);

    cout << "Prob Truncar: " << dif/(best*cel) << endl; 
    cout << "Prob Dividir: " << PROB_SPLIT*dif/fitness << endl;
    /*if(p<dif/(best*cel) && cel<LOW_CELERITY){ // La solución es truncada
      spare_evals+=evals; // Evaluaciones no consumidas
      break;
    }
    else*/ if((dif<=0 || p>PROB_SPLIT*dif/fitness) && evals >= MIN_EVALS_SPLIT){ // La solución se divide en dos
      vector<double> child1(dim);
      vector<double> mom1(dim);
      vector<double> child2(dim);
      vector<double> mom2(dim);
      for (int i = 0; i < dim; i++) { // Dos hijos en direcciones opuestas
        modify[i] = u(gen);
        mom1[i]= mom[i]+modify[i];
        mom2[i]= mom[i]-modify[i];
        child1[i]=sol[i]+cel*mom1[i];
        child2[i]=sol[i]+cel*mom2[i];
      }
      clip(child1);
      clip(child2);
      branchSearch(child1, mom1, cel, evals/2, gen);
      branchSearch(child2, mom2, cel, evals/2, gen);
      break;
    }
    else { // La solución avanza
      bool carryon = true;
      for (int j = 0; carryon && evals>0 && j < IMPROVE_LIMIT; j++){
        for (int i = 0; i < dim; i++) { // Generar vecino aleatorio
          modify[i] = u(gen);
          neighbor[i]= sol[i]+modify[i];
        }
        neighbor_fitness=cec17_fitness(&neighbor[0]);
        evals--;
        if(neighbor_fitness<fitness)
          carryon=false;
      }
      if (!carryon){ // Se encontró un vecino mejor
        improvement=fitness-neighbor_fitness;
        float aux=improvement/(1+improvement);
        for (int i = 0; i < dim; i++)  // Actualizar momento
          mom[i]=(1-ALPHA*aux)*mom[i]+ALPHA*aux*modify[i];
        
        for (int i = 0; i < dim; i++){ // Actualizar solución
          sol[i]=sol[i]+cel*mom[i];
        }
        clip(sol);
        fitness=fitness = cec17_fitness(&sol[0]);
        evals--;
        if (fitness < best) best=fitness;

        cel = DECREASE_SUCCESS*cel;
      }
      else // Cerca de un máximo local o se acabaron las evaluaciones
        cel = DECREASE_FAIL*cel;
    }
  }
}

int main() {
  vector<double> sol;
  std::uniform_real_distribution<> range(-100.0, 100.0);

  int dim = 10;
  //for(int dim=10; dim<=30; dim+=20) {
    int seed = 42;
    //for (int seed=42; seed<=87; seed+=5){
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

        MIN_EVALS_SPLIT*=dim;

        branchSearch(sol, mom, INIT_CELERITY, dim*10000, gen);
      }
    //}
  //}

}
