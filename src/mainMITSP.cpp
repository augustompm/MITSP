#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <random>
#include <tuple>
#include <vector>
#include <array>
#include <string.h>

using namespace std;

class TSPProblemData {
 public:
  static mt19937 seed;
  static constexpr int N = 318;
  static float xInstance[N];
  static float yInstance[N];
  static array<int, N> Solution;

  static float EuclidianDistance(float x1, float y1, float x2, float y2) {
    return round(sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2))));
  }

  static float N2OptGain(int vi, int vj) {
    int vi_anterior = vi - 1;
    int vj_posterior = vj + 1;

    if (vi == 0) vi_anterior = N - 1;
    if (vj == N - 1) vj_posterior = 0;
    if (((vi == 0) && (vj == N - 1)) || (vi_anterior == vj_posterior)) return 0;

    return -1 *
           (EuclidianDistance(xInstance[Solution[vi]], yInstance[Solution[vi]],
                              xInstance[Solution[vj_posterior]],
                              yInstance[Solution[vj_posterior]]) +
            EuclidianDistance(xInstance[Solution[vj]], yInstance[Solution[vj]],
                              xInstance[Solution[vi_anterior]],
                              yInstance[Solution[vi_anterior]]) -
            EuclidianDistance(xInstance[Solution[vi]], yInstance[Solution[vi]],
                              xInstance[Solution[vi_anterior]],
                              yInstance[Solution[vi_anterior]]) -
            EuclidianDistance(xInstance[Solution[vj]], yInstance[Solution[vj]],
                              xInstance[Solution[vj_posterior]],
                              yInstance[Solution[vj_posterior]]));
  }

  static float SolutionCost() {
    float Custo = 0;
    for (int i = 0; i < N - 1; i++)
      Custo = Custo + EuclidianDistance(xInstance[Solution[i]],
                                        yInstance[Solution[i]],
                                        xInstance[Solution[i + 1]],
                                        yInstance[Solution[i + 1]]);

    Custo = Custo + EuclidianDistance(
                        xInstance[Solution[N - 1]], yInstance[Solution[N - 1]],
                        xInstance[Solution[0]], yInstance[Solution[0]]);
    return Custo;
  }

  static void ShowSolution() {
    printf("\nS -> ");
    for (int i = 0; i < N; i++) {
      printf("%d ", Solution[i]);
    }
    printf("\nCusto da Solução: %.0f\n", SolutionCost());
  }

  static void LoadInstance() {
   FILE* input;
   int c;
   char line[1024];

   input = fopen("instances/lin318.tsp", "r");
   if (!input) {
       printf("Erro ao abrir arquivo instances/lin318.tsp\n");
       exit(1);
   }

   while (fgets(line, sizeof(line), input)) {
       if (strstr(line, "NODE_COORD_SECTION") != NULL)
           break;
   }

   for (int i = 0; i < N; i++) {
       if (fscanf(input, "%d %f %f", &c, &xInstance[i], &yInstance[i]) != 3) {
           printf("Erro ao ler coordenadas da cidade %d\n", i+1);
           exit(1);
       }
   }

   fclose(input);
}

  static void CreateTSPSolution() {
    for (int i = 0; i < N; i++) Solution[i] = i;
    shuffle(Solution.begin(), Solution.end(), seed);
  }
};

class MiTSP2OptDynProg {
 public:
  constexpr static int N = TSPProblemData::N;
  static pair<int, int> Optimal2OptMoves[N][N];
  static list<pair<int, int>> Selected2OptMoves;

  static void N2Opt(int vi, int vj) {
   int aux;
   for (int i = 0; i < (int)(vj - vi + 1) / 2; i++) {
     aux = TSPProblemData::Solution[i + vi];
     TSPProblemData::Solution[i + vi] = TSPProblemData::Solution[vj - i];
     TSPProblemData::Solution[vj - i] = aux;
   }
 }

 static void FirstImprovement2Opt() {
   float gain;
   int i, j, flag = 1, cont = 0;
   while (flag == 1) {
     flag = 0;
     for (i = 0; i < N - 1; i++) {
       for (j = i + 1; j < N; j++) {
         gain = TSPProblemData::N2OptGain(i, j);
         if (gain > 0) {
           N2Opt(i, j);
           flag = 1;
           cont++;
           break;
         }
       }
       if (flag == 1) break;
     }
   }
 }

 static void BestImprovement2Opt() {
   float best, best2=1, actual;
   int i, j, flag = 1, bestI, bestJ, cont = 0;
   while (flag == 1) {
     flag = 0;
     best = 0;
     actual = 0;
     bestI = -1;
     for (i = 0; i < N - 1; i++) {
       for (j = i + 1; j < N; j++) {
         actual = TSPProblemData::N2OptGain(i, j);
         if (actual > best) {
           best = actual;
           bestI = i;
           bestJ = j;
           flag = 1;
         }
       }
     }
     if (bestI != -1) {
       cont++;
       cout << cont << " " << best << " " <<endl;


       N2Opt(bestI, bestJ);
     }
   }
 }

 static void update2OptInternalMoves(pair<int, int> move) {
   list<pair<int, int>>::iterator w;
   for (w = Selected2OptMoves.begin(); w != Selected2OptMoves.end(); w++)
     if ((move.first < (*w).first) && (move.second > (*w).second)) {
       (*w).first = move.second - (*w).first + move.first;
       (*w).second = move.second - (*w).second + move.first;
       if ((*w).first > (*w).second) {
         int aux = (*w).first;
         (*w).first = (*w).second;
         (*w).second = aux;
       }
     }
 }

 static void Mount2OptOptimalList(int a, int b) {
   if ((a > b) || (a < 0) || (b >= N)) return;
   if (Optimal2OptMoves[a][b].first == -1)
     Mount2OptOptimalList(a, b - 1);
   else {
     if (TSPProblemData::N2OptGain(Optimal2OptMoves[a][b].first,
                                   Optimal2OptMoves[a][b].second) >= 0)
       Selected2OptMoves.push_back(Optimal2OptMoves[a][b]);
     Mount2OptOptimalList(a, Optimal2OptMoves[a][b].first - 2);
     Mount2OptOptimalList(Optimal2OptMoves[a][b].first + 2,
                          Optimal2OptMoves[a][b].second - 2);
   }
 }

 static void DynamicProgramming2Opt() {
   float L[N][N];
   int a, b;
   int i, k;
   float best, actual, left, diagonal, cost;
   int bestI, bestJ;

   for (k = 0; k < N; k++) {
     for (i = 0; i < N; i++) L[k][i] = 0;
   }

   for (k = 0; k < N; k++) {
     for (a = 0; a < N - k; a++) {
       b = a + k;
       if (a >= b) continue;
       best = 0, bestI = -1, bestJ = -1;
       if (L[a][b - 1] >= best) {
         best = L[a][b - 1];
         bestI = -1;
         bestJ = 0;
       }
       for (i = a; i < b; i++) {
         cost = 0;
         actual = TSPProblemData::N2OptGain(i, b);
         left = (((i - 2 >= a) && (i - 2 >= 0)) ? L[a][i - 2] : 0);
         diagonal = (((i + 2 < N) && (b - 2 >= i + 2)) ? L[i + 2][b - 2] : 0);
         cost = (actual >= 0 ? actual : 0) + left + diagonal;
         if (cost >= best) {
           best = cost;
           bestI = i;
           bestJ = b;
         }
       }
       if (best >= L[a][b]) {
         L[a][b] = best;
         Optimal2OptMoves[a][b] = make_pair(bestI, bestJ);
       }
     }
   }
   Selected2OptMoves.clear();

   if ((L[1][N - 1] <= 0) && (L[0][N - 2] <= 0)) return;

   if (L[1][N - 1] >= L[0][N - 2])
     Mount2OptOptimalList(1, N - 1);
   else
     Mount2OptOptimalList(0, N - 2);
 }

 static void MultiImprovement2Opt() {
   int cont = 0;
   float acc_gain =0, acc_gain2=1;
   while (1) {
     acc_gain = 0;
     DynamicProgramming2Opt();
     if (!Selected2OptMoves.empty()) {
       cont++;
     } else break;

     list<pair<int, int>>::iterator w;
     for (w = Selected2OptMoves.begin(); w != Selected2OptMoves.end(); w++) {
       acc_gain = acc_gain + TSPProblemData::N2OptGain((*w).first, (*w).second);
       N2Opt((*w).first, (*w).second);
       update2OptInternalMoves(*w);
     }
     cout << cont << " " << acc_gain << " " << acc_gain2 << " " << acc_gain / acc_gain2  <<" "<< endl;
     acc_gain2 = acc_gain;      
   }
 }
};

mt19937 TSPProblemData::seed;
array<int, TSPProblemData::N> TSPProblemData::Solution;
float TSPProblemData::xInstance[TSPProblemData::N];
float TSPProblemData::yInstance[TSPProblemData::N];

pair<int, int> MiTSP2OptDynProg::Optimal2OptMoves[MiTSP2OptDynProg::N][MiTSP2OptDynProg::N];
list<pair<int, int>> MiTSP2OptDynProg::Selected2OptMoves;

int main() {
 cout << "Begin MI experiments (N = " << MiTSP2OptDynProg::N << ")" << endl;
 TSPProblemData::seed = (mt19937)time(0);
 clock_t ini_t_global = clock();

 TSPProblemData::LoadInstance();
 TSPProblemData::CreateTSPSolution();
 array<int, TSPProblemData::N> InitSolution;
 InitSolution = TSPProblemData::Solution;

 printf("::: TESTE 0 :::\n");
 printf("\nNova solução inicial gerada\n");

 printf("\nRealizando Busca Local First Improvement\n");
 MiTSP2OptDynProg::FirstImprovement2Opt();
 printf("Encontrada solução de custo: %f\n", TSPProblemData::SolutionCost());

 TSPProblemData::Solution = InitSolution;
 printf("\nRealizando Busca Local Best Improvement\n");
 MiTSP2OptDynProg::BestImprovement2Opt();
 printf("Encontrada solução de custo: %f\n", TSPProblemData::SolutionCost());

 TSPProblemData::Solution = InitSolution;
 printf("\nRealizando Busca Local Multi Improvement\n");
 MiTSP2OptDynProg::MultiImprovement2Opt();
 printf("Encontrada solução de custo: %f\n", TSPProblemData::SolutionCost());

 clock_t end_t_global = clock();
 long double time_elapsed_ms = 1000.0 * (end_t_global - ini_t_global) / CLOCKS_PER_SEC;
 printf("\nTempo da Busca Local: %.2Lf", time_elapsed_ms);

 cout << "\n\nFim" << endl;
 cout << "finished successfully" << endl;
 return 0;
}