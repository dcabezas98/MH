extern "C" {
#include "cec17.h"
}
#include <iostream>
#include <vector>
#include <random>
#include <math.h>

using namespace std;

//int TOTAL_EVALS=0;

// Parámetros
float INIT_CELERITY = 1;
float DECREASE_SUCCESS = 0.99;
float DECREASE_FAIL = 0.9;

float BASE_WEIGHT = 0.2;

float MIN_DIF_SOLIS = 0.9;

float PROB_SPLIT = 0.9; // Coeficiente que multiplica para decidir si la solución se divide
float PROB_VANISH = 0.01; // Coeficiente que multiplica a la probabilidad de que una solución desaparezca

float IMPULSE = 0.5; // Impulso para los hijos cuando se hace split

float MIN_CEL_SPLIT = 0.1*INIT_CELERITY;
float MAX_CEL_SPLIT = 0.7*INIT_CELERITY;
float MIN_CEL = 0.01*INIT_CELERITY;
int MIN_EVALS_SPLIT = 400; // Mínimo de evaluaciones para dividirse. Se multiplica por la dimensión
int MAX_EVALS_TRUNCATE = 1200; // Máximo de evaluaciones restantes con las que se puede truncar una solución. Se multiplica por la dimensión

int min_evals_split = 400; // Mínimo de evaluaciones para dividirse
int max_evals_truncate = 1200; // Máximo de evaluaciones restantes con las que se puede truncar una solución

int IMPROVE_LIMIT = 10; // Máximo de intentos de obtener un vecino mejor, se multiplica por la dimensión

int EVALS_SOLIS = 1000; // Por debajo de EVALS_SOLIS*dim se hace LS

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

void increm_bias(vector<double> &bias, vector<double> dif) {
  for (unsigned i = 0; i < bias.size(); i++) {
    bias[i] = 0.2*bias[i]+0.4*(dif[i]+bias[i]);
  }
}

void decrement_bias(vector<double> &bias, vector<double> dif) {
  for (unsigned i = 0; i < bias.size(); i++) {
    bias[i] = bias[i]-0.4*(dif[i]+bias[i]);
  }
}

/**
 * Aplica el Solis Wets
 *
 * @param  sol solucion a mejorar.
 * @param fitness fitness de la solución.
 */
template <class Random>
void soliswets(vector<double> &sol, double &fitness, double delta, int maxevals, int lower, int upper, Random &random) {
  const size_t dim = sol.size();
  vector<double> bias (dim), dif (dim), newsol (dim);
  double newfit;
  size_t i;

  int evals = 0;
  int num_success = 0;
  int num_failed = 0;

  while (evals < maxevals) {
    std::uniform_real_distribution<double> distribution(0.0, delta);

    for (i = 0; i < dim; i++) {
      dif[i] = distribution(random);
      newsol[i] = sol[i] + dif[i] + bias[i];
    }

    clip(newsol, lower, upper);
    newfit = cec17_fitness(&newsol[0]);
    evals += 1;
    //TOTAL_EVALS++;

    if (newfit < fitness) {
      sol = newsol;
      fitness = newfit;
      increm_bias(bias, dif);
      num_success += 1;
      num_failed = 0;
    }
    else if (evals < maxevals) {

      for (i = 0; i < dim; i++) {
        newsol[i] = sol[i] - dif[i] - bias[i];
      }

      clip(newsol, lower, upper);
      newfit = cec17_fitness(&newsol[0]);
      evals += 1;
      //TOTAL_EVALS++;

      if (newfit < fitness) {
        sol = newsol;
        fitness = newfit;
        decrement_bias(bias, dif);
        num_success += 1;
        num_failed = 0;
      }
      else {
        for (i = 0; i < dim; i++) {
          bias[i] /= 2;
        }

        num_success = 0;
        num_failed += 1;
      }
    }

    if (num_success >= 5) {
      num_success = 0;
      delta *= 2;
    }
    else if (num_failed >= 3) {
      num_failed = 0;
      delta /= 2;
    }
  }

}

void branchSearch(vector<double> &sol, vector<double> &mom, float cel, int evals, mt19937 &gen){
  const size_t dim = sol.size();

  float p;
  double dif; // Diferencia respecto a la mejor

  vector<double> modify (dim), neighbor(dim);
  double neighbor_fitness, improvement;

  float improvement_norm, dif_norm;

  double fitness = cec17_fitness(&sol[0]);
  evals--;
  //TOTAL_EVALS++;
  //if(fitness < best) cout << TOTAL_EVALS << ", " << cec17_error(fitness) << endl;
  best=min(best, fitness);

  while(evals>0){
    dif = fitness-best;
    p = prob(gen);

    dif_norm=dif/(1+dif);

    if((p<PROB_VANISH*dif_norm/cel && evals <= max_evals_truncate) or cel<MIN_CEL){ // La solución es truncada o mejorada con LS
      if (evals/dif>=1000){
        soliswets(sol, fitness, 0.2, evals, -100, 100, gen);
        //if(fitness < best) cout << TOTAL_EVALS << ", " << cec17_error(fitness) << endl;
        best=min(best, fitness);
      }
      else
        spare_evals+=evals; // Evaluaciones no consumidas

      break;
    }
    else if(p>PROB_SPLIT*dif_norm/cel && MIN_CEL_SPLIT <= cel && cel<= MAX_CEL_SPLIT && evals >= min_evals_split){ // La solución se divide en dos
      vector<double> child1(dim);
      vector<double> mom1(dim);
      vector<double> child2(dim);
      vector<double> mom2(dim);
      for (int i = 0; i < dim; i++) { // Dos hijos en direcciones opuestas
        modify[i] = u(gen);
        mom1[i]= mom[i]+modify[i];
        mom2[i]= mom[i]-modify[i];
        child1[i]=sol[i]+(cel+IMPULSE*(INIT_CELERITY-cel))*mom1[i];
        child2[i]=sol[i]+(cel+IMPULSE*(INIT_CELERITY-cel))*mom2[i];
      }
      clip(child1);
      clip(child2);
      if (evals%2==1)
        spare_evals +=1;
      branchSearch(child1, mom1, cel+IMPULSE*(1-cel), evals/2, gen);
      branchSearch(child2, mom2, cel+IMPULSE*(1-cel), evals/2, gen);
      break;
    }
    else { // La solución avanza

      if(evals<=EVALS_SOLIS*dim){
        soliswets(sol, fitness, 0.2, evals, -100, 100, gen);
        //if(fitness < best) cout << TOTAL_EVALS << ", " << cec17_error(fitness) << endl;
        best=min(best, fitness);
        break;
      }

      bool carryon = true;
      for (int j = 0; carryon && evals>0 && j < IMPROVE_LIMIT*dim; j++){
        for (int i = 0; i < dim; i++) { // Generar vecino aleatorio
          modify[i] = sqrt(cel)*u(gen);
          neighbor[i]= sol[i]+modify[i];
        }
        clip(neighbor);
        neighbor_fitness=cec17_fitness(&neighbor[0]);
        evals--;
        //TOTAL_EVALS++;
        if(neighbor_fitness<fitness){
          carryon=false;
          //if(neighbor_fitness < best) cout << TOTAL_EVALS << ", " << cec17_error(neighbor_fitness) << endl;
          best=min(best, neighbor_fitness);
        }
      }
      if (!carryon){ // Se encontró un vecino mejor
        improvement=fitness-neighbor_fitness;
        improvement_norm=improvement/(1+improvement);
        float weight_neighbor=(1-BASE_WEIGHT)*improvement_norm+BASE_WEIGHT;
        for (int i = 0; i < dim; i++)  // Actualizar momento
          mom[i]=(1-weight_neighbor)*mom[i]+weight_neighbor*modify[i];
        
        for (int i = 0; i < dim; i++) // Actualizar solución
          sol[i]=sol[i]+cel*mom[i];
        
        clip(sol);
        fitness=fitness = cec17_fitness(&sol[0]);
        evals--;
        //TOTAL_EVALS++;
        //if(fitness < best) cout << TOTAL_EVALS << ", " << cec17_error(fitness) << endl;
        best=min(best, fitness);

        cel = DECREASE_SUCCESS*cel;
      }
      else // Cerca de un máximo local o se acabaron las evaluaciones
        cel = DECREASE_FAIL*cel;
    }
  }
}

int main(int argc, char *argv[]) { 
  std::uniform_real_distribution<> range(-100.0, 100.0);

  //int dim = 30;
  for(int dim=10; dim<=30; dim+=20) {
    //int seed = stoi(argv[2]); //42;
    for (int seed=42; seed<=87; seed+=5){
      //int funcid = stoi(argv[1]); //4;
      for (int funcid = 1; funcid <= 30; funcid++) {
        vector<double> sol(dim);
        vector<double> mom(dim); // Momento
        double fitness;

        //cout << "\nINIT: " << funcid << "\n" << endl;
        cec17_init("hybrid2-2", funcid, dim);
        spare_evals=dim*10000;

        best=__DBL_MAX__;

        //cerr <<"Warning: output by console, if you want to create the output file you have to comment cec17_print_output()" <<endl;
        //cec17_print_output(); // Comment to generate the output file

        mt19937 gen(seed); // Inicio semilla

        min_evals_split=dim*MIN_EVALS_SPLIT;
        max_evals_truncate=dim*MAX_EVALS_TRUNCATE;
        
        while(spare_evals>0){
          int e = spare_evals;
          spare_evals=0;

          for (int i = 0; i < dim; i++) { 
            sol[i] = range(gen); // Solución inicial
            mom[i] = u(gen); // Momento inicial
          }

          branchSearch(sol, mom, INIT_CELERITY, e, gen);
        }
      }
    }
  }

}
