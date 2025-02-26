#include<iostream>
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include<vector>
#include<sstream>
#include<algorithm>

using namespace std;

#define N 51
//#define N 1000
bool M[N][N];
int vx[N+1];
int vy[N+1];
int S[N]; // solution
int fS;

int DP[N][N]; // dynamic programming table

int countMoveCost[N][N];

struct optmove {
  optmove(){
    cost = 0;
    move = make_pair(-1,-2);
  }
  int cost;
  pair<int,int> move;
};
optmove isOptMove[N][N];

vector<optmove> voptmoves;

vector<string> DT[N][N]; // dependency table (DT[i][j] points which functions depend on this)
vector<string> RDT[N][N]; // reverse dependency table (RDT[i][j] points which functions i j depends on)
int opt[N][N]; // optimal values

void swap(int& i, int& j) {
   int aux = i;    i = j;    j = aux;
}

bool blockswap(int m1x, int m1y, int m2x, int m2y) {
    assert(m1x <= m1y); assert(m2x <= m2y);
    if(m1x > m2x) { // ensure m1x <= m2x
       swap(m1x, m2x);
       swap(m1y, m2y);
    }
    return ((abs(m1y - m2x) <= 1) ||
            (abs(m1x - m2y) <= 1) ||
            (abs(m1x - m2x) <= 1) ||
            (abs(m1y - m2y) <= 1));
}
bool block2opt(int m1x, int m1y, int m2x, int m2y) {
    assert(m1x <= m1y); assert(m2x <= m2y);
    //assert(m1y <= m2x);
    if(m1x > m2x) { // ensure m1x <= m2x
       swap(m1x, m2x);
       swap(m1y, m2y);
    }
    return ( ((m1y >= m2x) && (m1y <= m2y)) ||  // no interlaced moves
            (abs(m1y - m2x) <= 1)
            || (abs(m1y - m2y) <= 1) || (abs(m1x - m2x) <= 1) );//|| (m1x==m1y) || (m2x==m2y) ); // added later (is it correct?)
}

void printM() {
   cout << "N=" << N << endl;
   cout << " *";
   for(int j=0; j<N; j++)
	cout << " " << (j<=9?" ":"") << j;
   cout << endl;
   for(int i=0; i<N; i++) {
      cout << (i<=9?" ":"") << i << " ";
      for(int j=0; j<N; j++) {
         cout << " ";
         if(i<j)
             cout << (M[i][j]?"x":".");
         else
             cout << (M[j][i]?"x":".");

         cout << " ";
     }
//         cout << " " << (i>=j?" ":(M[i][j]?"x":".")) << " ";
//         cout << M[i][j] << " ";
      cout << endl;
   }
}

int edist(long long x1, long long y1, long long x2, long long y2) {
  return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

// no limits imposed to 'a' and 'b'.
//Can be any elements from solution to calculate distance.
int distab(int a, int b) {
  if(a > b)
     swap(a,b);
  int Sa = (a == -1)?0:S[a];
  int Sb = (b == N)?0:S[b];
  return edist(vx[Sa], vy[Sa], vx[Sb], vy[Sb]);
}

int fcost() {
  int f = 0;
  for(unsigned i=0; i<N-1; i++) {
    f+=distab(i,i+1);//edist(vx[i],vy[i],vx[i+1],vy[i+1]);
    //printf("+= distab(%d,%d)=%d\n",i,i+1,distab(i,i+1));
  }
  f+= distab(-1,N-1);//edist(vx[N-1],vy[N-1],vx[0],vy[0]);
  //printf("+= distab(%d,%d)=%d\n",-1,N-1,distab(-1,N-1));
  f+= distab(-1,0);//edist(vx[N-1],vy[N-1],vx[0],vy[0]);
  //printf("+= distab(%d,%d)=%d\n",-1,0,distab(-1,0));
  return f;
}



int costmove(int a, int b) {
  assert(a<=b);
  countMoveCost[a][b]++;
  //if((a==0)&&(b==N-1)) // no extreme move
  //  return 0;
  int f = 0;
  // gives 50%!!! STUDY!
  /*
  f -= distab(S[a-1], S[a]);
  f -= distab(S[b-1], S[b]);
  f += distab(S[a-1], S[b]);
  f += distab(S[b-1], S[a]);
  */
  f -= distab(a-1, a);
  f -= distab(b, b+1);
  f += distab(a-1, b);
  f += distab(a, b+1);
  f*= -1;
  //printf("costmove(%d,%d)=%d\n",a,b,f);
  return f;
}

void showSolution() {
  printf("S: 0 | ");
  for(unsigned i=0; i<N; i++)
     printf("%d ", S[i]);
  printf("| 0\n");
  fS = fcost();
  printf("cost=%d\n",fS);
}

void printTab(int t) {
  for(unsigned i=0; i<t; i++)
     printf("\t");
}

void insertUnique(vector<string>& vs, string s) {
    for(unsigned i=0; i<vs.size(); i++)
      if(vs[i] == s)
         return;
    vs.push_back(s);
}

// passo 2 Fabiano
// dentro de um vetor [a,b], como computar a soluçao otima que usa o movimento i,j?
int recValue(int a, int b, int numTab=0) {
  if (a >= b)
     return 0; // nao há movimentos possíveis
  printTab(numTab); printf("start recValue(a=%d,b=%d)\n",a,b);
  int best = 0; // best option

  for(int i=a; i<=b-1; i++)
    for(int j=i+1;j<=b; j++){
      int mx = 0; // aggregate this move
      int mx2 = costmove(i,j);
      printTab(numTab); printf("[a=%d,b=%d]:i=%d j=%d =>%d cost\n",a,b,i,j,mx2);
      if(mx2 >= 0)
        mx += mx2;
      //mx2 = solveDP2Opt(a, i-1);
      int mx3 = 0;
      if((i-2 >= 0) && (a <= i-2)) {
         //stringstream ss; ss << "(a="<<a<<";b="<<b<<";i="<<i<<";j="<<j<<")";
         stringstream ss; ss << "(a="<<a<<";b="<<b<<")";
         //DT[a][i-2].push_back(ss.str());
         insertUnique(DT[a][i-2],ss.str());
         stringstream ss2; ss2 << "(a="<<a<<";b="<<i-2<<")";
         insertUnique(RDT[a][b],ss2.str());
         mx3 = recValue(a,i-2, numTab+1);
      }
      assert(mx3 >= 0);
      mx += mx3;
      //mx2 = solveDP2Opt(b+1, N-1);
      int mx4 = 0;
      if((j+2 < N) && (j+2<=b)) {
        stringstream ss; ss << "(a="<<a<<";b="<<b<<")";
        //DT[j+2][b].push_back(ss.str());
        insertUnique(DT[j+2][b],ss.str());
        stringstream ss2; ss2 << "(a="<<j+2<<";b="<<b<<")";
        insertUnique(RDT[a][b],ss2.str());
        mx4 = recValue(j+2,b, numTab+1);
      }
      assert(mx4 >= 0);
      mx += mx4;
      printTab(numTab); printf("[a=%d,b=%d]:i=%d j=%d => %d + %d + %d = %d \n",a,b,i,j,mx2, mx3, mx4, mx);
      if(mx > best) {
        best = mx;
        //printTab(numTab); printf("*BEST = (i=%d,j=%d)\n",i,j);
      }
    }

  int mx = 0;
  mx = recValue(a+1,b); // try to ignore first element
  if(mx > best)
     best = mx;
  mx = recValue(a,b-1); // try to ignore last element
  if(mx > best)
     best = mx;

  if(best > opt[a][b])
     opt[a][b] = best;
  printTab(numTab); printf("finished recValue(a=%d,b=%d) = %d\n",a,b, best);
  return best;
}



// solve 2opt problem in interval [a,b]
int solveDP2Opt(int a, int b) {
  if (a >= b)
     return 0; // nao há movimentos possíveis neste intervalo

  int best = 0; // best option
  int best_i = -1, best_j = -1; // best option

  int mx1 = DP[a][b-1]; //solveDP2Opt(a,b-1); // try to ignore last element
  if(mx1 >= best) {
    best = mx1;
    best_i = -1; best_j = 0;
  }

  for(int i=a; i<=b-1; i++)
    //for(int j=i+1;j<=b; j++)
    {
      int j = b;
      //j = b;
      //i = a;
      int mx = 0; // aggregate this move
      int mx2 = costmove(i,j);
      if(mx2 >= 0)
        mx += mx2;
      //mx2 = solveDP2Opt(a, i-1);
      int mx3 = 0;
      if((i-2 >= 0) && (a <= i-2)) {
         mx3 = DP[a][i-2];
         //mx3 = solveDP2Opt(a,i-2, numTab+1);
      }
      assert(mx3 >= 0);
      mx += mx3;
      //mx2 = solveDP2Opt(b+1, N-1);
      int mx4 = 0;
      if((j+2 < N) && (j+2<=b)) {
        mx4 = DP[j+2][b];
        //mx4 = solveDP2Opt(j+2,b, numTab+1);
      }
      assert(mx4 >= 0);
      mx += mx4;


      int mx5 = 0; // internal moves
      if((i+2 < N) && (j-2>=i+2)) {
        mx5 = DP[i+2][j-2];
        //mx4 = solveDP2Opt(j+2,b, numTab+1);
      }
      assert(mx5 >= 0);
      mx += mx5;

      if(mx > best) {
        best = mx;
        best_i = i;
        best_j = j;
      }


    }


  if(best >= DP[a][b]) { // update DP table
     DP[a][b] = best;
     if(best_i != -1)
       isOptMove[a][b].cost = costmove(best_i,best_j);
     isOptMove[a][b].move = make_pair(best_i,best_j);
   }

  return best;
}

void applySwap(int i, int j) {
  int x = S[i];
  S[i] = S[j];
  S[j] = x;
}

void apply2Opt(int a, int b) {
  int sz = (a+b)/2 - a;
  for(int k=0; k<=sz; k++)
    applySwap(a+k, b-k);
}

// readDPTable AND update solution!
int readDPTable(int a, int b) {
  if((a > b) || (a < 0) || (b >= N))
    return 0; // nothing to do
  //printf("reading DP Table %d %d, opt is %d\n", a, b, DP[a][b]);
  //int numMoves = 0;
  int costMoves = 0;
  optmove r = isOptMove[a][b];
  if(r.move.first == -1) {
    costMoves = readDPTable(a, b-1);
  }
  else {
    costMoves = costmove(r.move.first, r.move.second); // WARNING: COST MAY CHANGE DUE TO OTHER MOVES! REALLY????

    printf("move(%d,%d) = %d\n", r.move.first, r.move.second, costmove(r.move.first, r.move.second));

    voptmoves.push_back(r);
    //apply2Opt(r.move.first, r.move.second);

    costMoves += readDPTable(a, r.move.first-2);
    costMoves += readDPTable(r.move.second+2, b);
    costMoves += readDPTable(r.move.first+2, r.move.second-2);
      //mx4 = solveDP2Opt(j+2,b, numTab+1);
  }

  //printf("reading DP Table %d %d = %d\n", a, b, numMoves);
  return costMoves;
}

void makeRand() {
  printf("N=%d (%d coords)\n",N, N+1);
  for(int i=0; i<N+1; i++)
    vx[i] = rand()%100;//100000;
  for(int i=0; i<N+1; i++)
    vy[i] = rand()%100;//100000;

  for(int i=0; i<N+1; i++)
    printf("(%d,%d) ",vx[i],vy[i]);
  printf("\n");

   for(int i=0; i<N; i++)
      S[i] = i+1;
   fS = fcost();
}

void makeTSPBerlin() {
  FILE* fTSP = fopen("berlin52.tsp","r");
  int n;
  int r = fscanf(fTSP,"%d\n",&n);
  printf("n=%d (%d coords)\n",n, N+1);
  assert(n==N+1);
  for(unsigned i=0; i<N+1; i++) {
    int id;
    float xx, yy;
    r = fscanf(fTSP,"%d %f %f\n",&id,&xx,&yy);
    vx[i] = xx;
    vy[i] = yy;
  }

  for(int i=0; i<N+1; i++)
    printf("(%d,%d) ",vx[i],vy[i]);
  printf("\n");

  vector<int> voptions;
  for(int i=0; i<N; i++)
     voptions.push_back(i+1);
  random_shuffle(voptions.begin(), voptions.end());

   for(int i=0; i<N; i++)
      S[i] = voptions[i];
   fS = fcost();
   fclose(fTSP);
}

void update2optmove(const optmove om1, optmove& om2) {
  // internal move, update!
  if((om1.move.first < om2.move.first) && (om1.move.second > om2.move.second)) {
     //printf("update (%d,%d) o (%d,%d) = ",om1.move.first,om1.move.second, om2.move.first, om2.move.second);
     om2.move.first  = om1.move.second - om2.move.first  + om1.move.first;
     om2.move.second = om1.move.second - om2.move.second + om1.move.first;
     if(om2.move.first > om2.move.second)
        swap(om2.move.first, om2.move.second);
     //printf("(%d,%d)\n", om2.move.first, om2.move.second);
  }
}

// =================================================================
// =================================================================
// =================================================================

int executeDP() {

  //printf("==============================\n");
  //printf("PERFORMING DYNAMIC PROGRAMMING\n");
  //printf("==============================\n\n");

  for(int i=0; i<N; i++)
     for(int j=0; j<N; j++) {
       DP[i][j] = 0;//-100;
       //if(i<=j)
       //  DP[i][j] = 0;
     }

  for(int i=0; i<N; i++)
    for(int j=0; j<N; j++)
      countMoveCost[i][j]=0;

  for(int i=0; i<N; i++)
    for(int j=0; j<N; j++)
      isOptMove[i][j]=optmove();
  voptmoves.clear();




  for(unsigned d=0; d<N; d++) { // diagonal
       if(d%100==0)
         printf("start d=%d/%d\n",d,N-1);
       for(unsigned e=0; e<N-d; e++) { // elements in diagonal
         solveDP2Opt(e,d+e);
         //printf("calc(a=%d,b=%d) = %d\n",e,d+e, solveDP2Opt(e,d+e));
         //getchar();
       }
  }


  /*
  // print final table
       printf("will print DP:\n");
       printf("*\t");
       for(unsigned i=0; i<N; i++)
         printf("%d\t",i);
       printf("\n\n");
       for(unsigned i=0; i<N; i++) {
         printf("%d:\t",i);
        for(unsigned j=0; j<N; j++) {
           printf("%d", DP[i][j]);
           //if(DP[i][j] == opt[i][j])
              printf("\t");
           //else
           //   printf("/%d\t",opt[i][j]);
         }
        printf("\n");
      }
      printf("optimal value for [0,N-1]=%d\n",DP[0][N-1]);
  */

  /*
      printf("will print isOptMove:\n");
      printf("*\t");
      for(unsigned i=0; i<N; i++)
        printf("%d\t",i);
      printf("\n\n");
      for(unsigned i=0; i<N; i++) {
        printf("%d:\t",i);
       for(unsigned j=0; j<N; j++) {
          printf("(%d,%d)", isOptMove[i][j].move.first, isOptMove[i][j].move.second);
          //if(DP[i][j] == opt[i][j])
             printf("\t");
          //else
          //   printf("/%d\t",opt[i][j]);
        }
       printf("\n");
      }
  */


  /*
  printf("will print countMoveCost:\n");
  printf("*\t");
  for(unsigned i=0; i<N; i++)
    printf("%d\t",i);
  printf("\n\n");
  for(unsigned i=0; i<N; i++) {
    printf("%d:\t",i);
   for(unsigned j=0; j<N; j++) {
      printf("%d", countMoveCost[i][j]);
      //if(DP[i][j] == opt[i][j])
         printf("\t");
      //else
      //   printf("/%d\t",opt[i][j]);
    }
   printf("\n");
  }

  printf("will print isOptMove:\n");
  printf("*\t");
  for(unsigned i=0; i<N; i++)
    printf("%d\t",i);
  printf("\n\n");
  for(unsigned i=0; i<N; i++) {
    printf("%d:\t",i);
   for(unsigned j=0; j<N; j++) {
      printf("(%d,%d)", isOptMove[i][j].first, isOptMove[i][j].second);
      //if(DP[i][j] == opt[i][j])
         printf("\t");
      //else
      //   printf("/%d\t",opt[i][j]);
    }
   printf("\n");
  }
  */

  int old_f1 = fS;
  int x = readDPTable(0,N-1);
  printf("TOTAL COST IN MOVES=%d\n",x);
  printf("optimal value for [0,N-1]=%d\n",DP[0][N-1]);

  printf("will apply %u 2-opt moves\n", unsigned(voptmoves.size()));
  for(unsigned m=0; m<voptmoves.size(); m++) {
    optmove om = voptmoves[m];
    printf("applying move %d: (%d,%d)=%d\n",m,om.move.first, om.move.second, om.cost);
    apply2Opt(om.move.first, om.move.second);

    // update moves!
    for(unsigned k=m+1; k<voptmoves.size(); k++) {
      update2optmove(om, voptmoves[k]);
    }
  }

  //showSolution();

  /*
     int m2x = 0;
     int m2y = 1;

     while(true) {
     cout << "move(" << m2x<<";" << m2y << ")" << endl;
     printf("movecost(%d,%d)=%d\n",m2x, m2y,costmove(m2x,m2y));
     cin >> m2x >> m2y;

     getchar();
     }
  */
     assert(x == DP[0][N-1]);

     int expect = old_f1-DP[0][N-1];
//     printf("optimal value for [0,%d]=%d old_f=%d expecting=%d\n",N-1,DP[0][N-1], old_f, expect);
     fS = fcost();
//     printf("new value for f(S) = %d\n",fS);
     assert(fS == expect);
     return DP[0][N-1];
}


int main() {
  //srand(392893732);
  long long seed = 1508616479;//1508603841;//time(NULL);//1508551134; //time(NULL);
  printf("seed=%lld\n",seed);
  srand(seed);
  //srand(0);

  //makeRand();
  makeTSPBerlin();
  showSolution();

/*

   for(int i=0; i<N; i++)
      for(int j=0; j<N; j++)
         opt[i][j]=0;

   int mx1 = recValue(0, N-1);
   printf("mx=%d\n",mx1);

printf("will print optimal values:\n");
printf("*\t");
for(unsigned i=0; i<N; i++)
  printf("%d\t",i);
printf("\n\n");
for(unsigned i=0; i<N; i++) {
  printf("%d:\t",i);
 for(unsigned j=0; j<N; j++)
    printf("%d\t", opt[i][j]);
 printf("\n");
}

   printf("will print dependencies DT:\n");
   printf("*\t");
   for(unsigned i=0; i<N; i++)
     printf("%d\t",i);
   printf("\n\n");
   for(unsigned i=0; i<N; i++) {
     printf("%d:\t",i);
    for(unsigned j=0; j<N; j++) {
      int sz = DT[i][j].size();
       printf("%d\t", sz);
    }
    printf("\n");
  }

  printf("will print dependencies RDT:\n");
  printf("*\t");
  for(unsigned i=0; i<N; i++)
    printf("%d\t",i);
  printf("\n\n");
  for(unsigned i=0; i<N; i++) {
    printf("%d:\t",i);
   for(unsigned j=0; j<N; j++) {
     int sz = RDT[i][j].size();
      printf("%d\t", sz);
   }
   printf("\n");
 }

   while(0) {
     int i,j;
     cin >> i >> j;
     cout << costmove(i,j) << endl;

     cout << "DT:" << DT[i][j].size() << " : ";
     for(unsigned k=0; k<DT[i][j].size(); k++)
       cout << DT[i][j][k] << " ";
     cout << endl;

     cout << "RDT:" << RDT[i][j].size() << " : ";
     for(unsigned k=0; k<RDT[i][j].size(); k++)
       cout << RDT[i][j][k] << " ";
     cout << endl;

   }
*/

   int gain = -1;
   while(gain != 0) {
      int old_f = fS;
      gain = executeDP();
      printf("optimal value for DP =%d\n",gain);
      showSolution();
      printf("gap reduction = %d / %d = %.2f %%\n",gain, old_f, gain / ((float) old_f)*100 );
      printf("final solution = %d\n",fS);
      //getchar();
   }

   return 0;

/*
   for(int i=0; i<N; i++)
      for(int j=0; j<N; j++)
         M[i][j] = false;

   m2x = 4;
   m2y = 11;

   while(true) {

   for(int x=0; x<N-1; x++)
      for(int y=x; y<N; y++)
         M[x][y] = block2opt(x,y, m2x, m2y);

   cout << "move(" << m2x<<";" << m2y << ")" << endl;
   printM();
   cin >> m2x >> m2y;

   getchar();
   }
   */
}
