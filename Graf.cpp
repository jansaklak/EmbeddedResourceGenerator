#include "Graf.h"
#include <limits>
#include <queue>
#include <vector>
#include <algorithm>
#include "Edge.h"

const int INF = std::numeric_limits<int>::max();
Graf::Graf(){
    maxVert = 10;
    adjList.resize(maxVert);
}
Graf::~Graf() {
}

void Graf::addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2) {
    if(i_Vertex_Index_1>maxVert) maxVert = i_Vertex_Index_1 + 1;
    if(i_Vertex_Index_2>maxVert) maxVert = i_Vertex_Index_2 + 1;
    adjList.resize(maxVert);
    Edge edge(i_Vertex_Index_1, i_Vertex_Index_2);
    adjList[i_Vertex_Index_1].push_back(edge);
}
void Graf::addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2, int weight) {
    if(i_Vertex_Index_1>maxVert) maxVert = i_Vertex_Index_1 + 1;
    if(i_Vertex_Index_2>maxVert) maxVert = i_Vertex_Index_2 + 1;
    adjList.resize(maxVert);
    Edge edge(i_Vertex_Index_1, i_Vertex_Index_2, weight);
    adjList[i_Vertex_Index_1].push_back(edge);
}
int Graf::getVerticesSize(){
    return adjList.size();
}

int Graf::getWeightEdge(int i_Vertex_Index_1, int i_Vertex_Index_2) const {
    for (const auto& edge : adjList[i_Vertex_Index_1]) {
        if ((edge.getV1() == i_Vertex_Index_1 && edge.getV2() == i_Vertex_Index_2) ||
            (edge.getV1() == i_Vertex_Index_2 && edge.getV2() == i_Vertex_Index_1)) {
            return edge.getWeight();
        }
    }    return -1;
}

void Graf::printMatrix(std::ostream& out) {

    std::vector<std::vector<int>> adjacencyMatrix(maxVert, std::vector<int>(maxVert, 0));

    for (int i = 0; i < maxVert; ++i) {
        for (const auto& edge : adjList[i]) {
            int j = (edge.getV1() == i) ? edge.getV2() : edge.getV1();
            adjacencyMatrix[i][j] = edge.getWeight();
        }
    }

    for (int i = 0; i < maxVert; ++i) {
        for (int j = 0; j < maxVert; ++j) {
            std::cout << adjacencyMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}



std::vector<int> Graf::getNeighbourIndices(int idx) const {
    std::vector<int> neighbours;
    for (const auto& edge : adjList[idx]) {
        if (edge.getV1() == idx) {
            neighbours.push_back(edge.getV2());
        } else {
            neighbours.push_back(edge.getV1());
        }
    }
    return neighbours;
}

std::vector<int> Graf::getOutNeighbourIndices(int idx) const {
    std::vector<int> neighbours;
    for (const auto& edge : adjList[idx]) {
        if (edge.getV1() == idx) {
            neighbours.push_back(edge.getV2());
        }
    }
    return neighbours;
}
