#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "omp.h"
using namespace std;


int count=0;

void copyVector(vector<unsigned short>& ori,vector<unsigned short>& cop,unsigned short column)
{
  for(int i=0;i<column;i++)
    cop[i]=ori[i];
}

int queenAccepted(vector<unsigned short>& tablero, int c, int r)
{
  for(int i=0;i<c;i++)
  {
    if(tablero[i]==r)
      return 0;
    if(abs((c+1)-(i+1)) == abs((r+1)-(tablero[i]+1)))
      return 0;
  }
  return 1;
}


void printTablero(vector<unsigned short>& tablero, int n, string& txt)
{
  ++count;
  string solution="";
  for(int i=0;i<n;i++)
    solution+=to_string(tablero[i]+1)+" ";
  txt+=solution+"\n";
}


void printDot(vector<unsigned short>& tablero, int n, string& output){
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


void allQueens(vector<unsigned short> tablero,int c, int n, string& txt)
{
  if(c==n)
  {
  #pragma omp critical
    printTablero(tablero,n,txt);
    return;
  }
  #pragma omp parallel for
  for(int i = 0; i < n; i++)
  {
    if(queenAccepted(tablero, c, i))
    {     
      vector<unsigned short> copy(n);
      copyVector(tablero,copy,c);
      copy[c] = i;
      allQueens(copy, c + 1,n,txt);
    }
  }
}

int findQueens(vector<unsigned short> tablero,int c, int n,string& txt)
{
  if(c == n)
  {
    printDot(tablero,n,txt);
    return 1;
  }
#pragma opm parallel for
  for(int i = 0; i < n; i++)
  {
    if (queenAccepted(tablero, c, i))
    {
      vector<unsigned short> copy(n);
      copyVector(tablero,copy,c);
      copy[c] = i;
      if(findQueens(copy, c + 1,n,txt))
        return 1;
      
    }
  }
  return 0;
}


int main(int argc, char** argv){
  int n=stoi(argv[4]);
  vector<unsigned short> tablero(5);

  if(!strcmp(argv[2],"all"))
  {
    string txt="";
    allQueens(tablero,0,n,txt);
    ofstream file;
    file.open("solutions.txt");
    file<<"#Solutions for " + to_string(n) + " queens\n";
    file<<to_string(count) + "\n" + txt;
    file.close();
  }
  else{
    string dot_str="";
    findQueens(tablero,0,n,dot_str);
    ofstream dot;
    dot.open("solution.dot");
    dot<<dot_str + "\n";
    dot.close();
  }


  return 0;

  
}
