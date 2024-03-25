#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Edge.h"

class Graf {
private:
    int maxVert;
    std::vector<std::vector<Edge>> adjList;
public:
    Graf();
    ~Graf();
    void addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2);
    void addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2, int weight);
    int getWeightEdge(int i_Vertex_Index_1, int i_Vertex_Index_2) const;
    std::vector<int> getNeighbourIndices(int idx) const;
    std::vector<int> getOutNeighbourIndices(int idx) const;
    void printMatrix(std::ostream& out = std::cout);
    void printNeighbourIndices(int idx);
    int getNumberOfEdges();
    int getVerticesSize();
};
