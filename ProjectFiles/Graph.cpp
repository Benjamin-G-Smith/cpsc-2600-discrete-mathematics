// NAME: Ben Smith 
// FILE: Graph.cpp
// DESCRIPTION: Implementation of the Graph class.

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

#include "Graph.h"
#include "QueueInt.h"
#include "StackInt.h"

// Constructor
Graph::Graph()
{
}

// Destructor
Graph::~Graph()
{
  for(int i = 0; i < matrixSize; i++){
    delete [] agencyArray[i];
  }
  delete [] agencyArray;
  delete [] visited;
  matrixSize = 0;
}

// Load the graph from a file
void Graph::load(char *filename)
{
  ifstream file;
  file.open(filename);
  bool start = true;

  if(file.fail()){
    cout << "Unable to open file..." << endl;
    return;
  }

  int value = 0;
  file >> value;

  if(value == 0){
    matrixSize = 0;
    start = false;
  }
  if(start)
    {
      matrixSize = value;
      initalizeMatrix(matrixSize);

      while(file >> value){
        int row = value;
        file >> value;
        int col = value;
        file >> value;
        int data = value;
        agencyArray[row][col] = data;
        agencyArray[col][row] = data;
      }
      visited = new int[matrixSize];
    }
  else{
    cout << "There are no verticis in Graph file" << endl;
  }
}
void Graph::initalizeCheckArray(int matrixSize)
{
  visited = new int[matrixSize];
  for(int r = 0; r < matrixSize; r++){
    visited[r] = 0;
  }
}
void Graph::initalizeMatrix(int matrixSize)
{
  agencyArray = new int* [matrixSize];
  for(int i = 0; i < matrixSize; i++){
    agencyArray[i] = new int[matrixSize];
  }
  for(int r = 0; r < matrixSize; r++){
    for(int c = 0; c < matrixSize; c++){
      if(agencyArray[r][c] < 1 && r != c){
        agencyArray[r][c] = -1;
      }
      else if (r == c ){
        agencyArray[r][c] = 0;
      }
    }
  }
}
// Display the adjacency matrix
void Graph::display() const
{
  cout << endl;
  if(matrixSize == 0 ){
    cout << " 0 " << endl;
  }
  else{
  cout <<"      ";
  for(int i = 0; i < matrixSize; i++){
    if(i >= 10){
      cout <<i<<" ";
    }
    else{
      cout <<i<<"  ";
    }
  }
  cout << endl;
  for(int r = 0; r < matrixSize; r++){
    if(r >= 10){
     cout <<"   "<< r;
    }
    else{
    cout <<"    "<< r;
    }
    for(int c = 0; c < matrixSize; c++){
      if(r == c){
        cout << " 0 ";
      }
      else if(agencyArray[r][c] < 1){
        cout << " x ";
      }
      else{
      if(agencyArray[r][c] >= 10){
        cout <<" "<< agencyArray[r][c]<<"";
      }
      else{
        cout <<" "<< agencyArray[r][c]<<" ";
      }
    }

    }
    cout<<"    "<<endl;
  }
  cout << endl;
}
}

// Displays the depth first search starting at the given vertex
void Graph::displayDFS(int vertex) const
{
  if(matrixSize == 0){
    cout << "DFS at vertex "<< " 0 " << ":" << "0" << endl;
    return;
  }
  else{
  cout << "DFS at vertex "<< vertex << ":" << " ";
  StackInt dfsStack;
  for(int i = 0; i < matrixSize; i++){
    visited[i] = 0;
  }
  visited[vertex] = 1;
  dfsStack.push(vertex);
  dfs(vertex, dfsStack);
  cout << endl;
}

}
void Graph::dfs(int vertex, StackInt dfsStack) const {
  cout << vertex << " ";
  for(int i = 0; i < matrixSize; i++){
    if(agencyArray[vertex][i] != -1 && vertex != i){
      if(visited[i] == 0){
        visited[i] = 1;
        dfsStack.push(i);
        dfs(i, dfsStack);
      }
    }
  }
}


// Perform breadth first search starting at the given vertex
void Graph::displayBFS(int vertex) const
{
  if(matrixSize == 0 ){
    cout << "BFS at vertex "<< "0 "<< ":" << "0" << endl;
  }
  else
  {
  cout << "BFS at vertex "<< vertex << ":" << " ";
  for(int i = 0; i < matrixSize; i++){
    visited[i] = 0;
    }
    QueueInt q;
    q.enqueue(vertex);
    visited[vertex] = 1;
    while(!q.isEmpty()){
      vertex = q.dequeue();
      cout << vertex << " ";
      for(int i = 0; i < matrixSize; i++){
        if(agencyArray[vertex][i] != -1 && visited[i] == 0){
          q.enqueue(i);
          visited[i] = 1;
        }
      }
    }
    cout << endl;
  }
}
// Compute the cost of a minimum spanning tree.
int Graph::computeMSTCost() const
{
  if(matrixSize == 0 ){
    return 0;
  }
  else{
  int visited[matrixSize];
  bool done = false;
  bool VerticiesInGraph;
  bool found;
  bool minEdgeFound;
  int mstCost = 0;
  int minEdge;
  int minVertex;
  int index = 0;

  //Initalize visited array
  for(int i = 0; i < matrixSize; i++){
    visited[i] = -1;
  }
  visited[0] = 0;

  while(!done){
    minEdge = 0;
    minVertex = -1;
    minEdgeFound = false;
    //Smallest weight
    for(int row = 0; row <= index; row++){
      // minimum edge calculation
      for(int col = 0; col < matrixSize; col++){
        if(visited[row] != col && agencyArray[visited[row]][col] != -1){
          found = false;
          for(int k = 0; k < matrixSize && !found; k++){
            if(visited[k] == col){
              found = true;
            }
          }
          if(!found){
            if(!minEdgeFound){
              minEdge = agencyArray[visited[row]][col];
              minVertex = col;
              minEdgeFound = true;
            }
            else if(agencyArray[visited[row]][col] < minEdge){
              minEdge = agencyArray[visited[row]][col];
              minVertex = col;
            }
          }
        }
      }
    }
    if(minEdgeFound){
      index += 1;
      visited[index] = minVertex;
      mstCost += minEdge;
    }

    VerticiesInGraph = true;
    for(int i = 0; i < matrixSize && VerticiesInGraph; i++){
      if(visited[i] == -1){
        VerticiesInGraph = false;
      }
    }
    if(VerticiesInGraph){
      done = true;
    }
    else if(!minEdgeFound){
       done = true;
       mstCost = -1;
    }
  }
  return mstCost;
}
}
