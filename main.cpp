#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "omp.h"
using namespace std;


int count=0;
bool find_queens=false;

int queenAccepted(int *tablero, int c, int r)
{
  for(int i=0;i<c;i++)
    if(tablero[i]==r || abs((c+1)-(i+1)) == abs((r+1)-(tablero[i]+1)))
      return 0;
  
  return 1;
}


void printTablero(int *tablero, int n, string& txt)
{
  for(int i=0;i<n;i++)
    txt+=to_string(tablero[i]+1)+" ";
  txt+= +"\n";
}


void printDot(int* tablero, int n, string& output){
  output+="digraph H {\n  Tablero [\n    shape=plaintext\n    color=black\n    label=<\n";
  output+="      <table border=\'0\' cellborder=\'1\' cellspacing=\"0\">";
  for(int i=0;i<n;i++){
    output+="\n";
    output+="\t<tr>";
    for(int j=0;j<n;j++){
      output+=" <td";
      if((i+j)%2!=0)
         output+=" bgcolor=\"black\"><font color=\"#ffffff\">  ";
      else
        output+=">   ";
      if(tablero[i]==j)
        output+="&#9813;";

      if((i+j)%2!=0)
         output+=" </font></td>";
      else
        output+=" </td>";
    }  
    output+="</tr>";
    
  }
  output+="\n      </table>\n    >];\n}";
  
}


void allQueens(int* tablero,int c, int n, string& txt, int &n_sol)
{
  if(c==n)
  {
    ++n_sol;
    printTablero(tablero,n,txt);
    return;
  }
  for(int i = 0; i < n; i++)
  {
    if(queenAccepted(tablero, c, i))
    {     
      tablero[c]=i;
      allQueens(tablero, c + 1,n,txt, n_sol);
    }
  }
}

int findQueens(int *tablero,int c, int n,string& txt)
{
  if(find_queens)
    return 1;
  if(c == n)
  {
    printDot(tablero,n,txt);
    find_queens = true;
  }
  for(int i = 0; i < n; i++)
  {
    if (queenAccepted(tablero, c, i))
    {
      tablero[c]=i;
      if(findQueens(tablero, c + 1,n,txt))
      return 1;
    }
  }
  return 0;
}

void allQueens_p(int n)
{
  string txt="";
  
  #pragma omp parallel for 
  for(int i = 0; i < n; i++)
  {
    string solutions="";
    int *tablero = new int[n];
    int n_sol=0;
    tablero[0] = i;
    allQueens(tablero,1,n,solutions,n_sol);
    //cout << "tablero " << i << endl;
    //for(int i=0; i<n; i++)            
     // cout << tablero[i] << ' ';
    //cout << endl;
    delete [] tablero;
    #pragma omp critical
    {
      txt+=solutions;
      count+=n_sol;
    }
  }

  ofstream file;
  file.open("solutions.txt");
  file<<"#Solutions for " + to_string(n) + " queens\n";
  file<<to_string(count) + "\n" + txt;
  file.close();

}

void findQueens_p(int n)
{
  string dot_str="";
  
  #pragma opm parallel for
  for(int i = 0; i < n; i++)
  {
    int *tablero = new int[n];
    tablero[0] = i;
    findQueens(tablero,1,n,dot_str);
    delete [] tablero;
  }
  
  ofstream dot;
  dot.open("solution.dot");
  dot<<dot_str + "\n";
  dot.close();

}

int main(int argc, char** argv){
  int n=stoi(argv[4]);

  if(!strcmp(argv[2],"all"))
  {
    allQueens_p(n);
  }
  else{
    findQueens_p(n);
  }

  return 0;
}
