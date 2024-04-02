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
    void DFSUtil(int v, std::vector<bool>& visited, std::vector<int>& path, std::vector<std::vector<int>>& allPaths) const;
    int destination;
public:
    Graf();
    ~Graf();
    void addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2);
    void addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2, int weight);
    int getWeightEdge(int i_Vertex_Index_1, int i_Vertex_Index_2) const;
    std::vector<int> getNeighbourIndices(int idx) const;
    std::vector<int> getOutNeighbourIndices(int idx) const;
    bool checkEdge(int i_Vertex_Index_1, int i_Vertex_Index_2) const;
    void printMatrix(std::ostream& out = std::cout) const;
    std::vector<std::vector<Edge>> getAdjList() const;
    void printNeighbourIndices(int idx) const;
    int getNumberOfEdges() const;
    int getVerticesSize() const;
    std::vector<std::vector<int>> DFS(int start, int end);
    std::vector<int> BFS();
    
};
