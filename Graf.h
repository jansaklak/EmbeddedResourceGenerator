#pragma once


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

const int SIZE = 1001;
class Graf
{
    int matrix[SIZE][SIZE];
    int numberOfVertices;
public:
    Graf();
    ~Graf();
    void createVertices(int ile);
    std::vector<int> dijkstra(int startVertex, int endVertex) const;
    void addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2);
    int getWeightEdge(int i_Vertex_Index_1, int i_Vertex_Index_2) const;
    void addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2,int waga);
    void removeEdge(int i_Vertex_Index_1, int i_Vertex_Index_2);
    bool checkEdge(int i_Vertex_Index_1, int i_Vertex_Index_2);
    void PrintToFile(int size,std::string filename);
    void printMatrix(int size, std::ostream& out = std::cout);
    int vertexDegree(int idx);
    int getVerticlesSize() const;
    std::vector<int> getNeighbourIndices(int idx) const;
    std::vector<int> getOutNeighbourIndices(int idx) const;
    void printNeighbourIndices(int idx);
    int getNumberOfEdges();
    void readFromFile(std::string path,bool wagi);

private:
    void clear();
};
