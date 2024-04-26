#include "Graf.h"
#include <limits>
#include <queue>
#include <vector>
#include <algorithm>
#include "Edge.h"

const int INF = std::numeric_limits<int>::max();
Graf::Graf(){
    maxVert = 1;
    adjList.resize(maxVert);
}
Graf::~Graf() {
}

void Graf::addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2) {
    if(i_Vertex_Index_1>maxVert) maxVert = i_Vertex_Index_1 + 1;
    if(i_Vertex_Index_2>maxVert) maxVert = i_Vertex_Index_2 + 1;
    adjList.resize(maxVert+1);
    Edge edge(i_Vertex_Index_1, i_Vertex_Index_2);
    adjList[i_Vertex_Index_1].push_back(edge);
}
void Graf::addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2, int weight) {
    if(i_Vertex_Index_1>maxVert) maxVert = i_Vertex_Index_1 + 1;
    if(i_Vertex_Index_2>maxVert) maxVert = i_Vertex_Index_2 + 1;
    adjList.resize(maxVert+1);
    Edge edge(i_Vertex_Index_1, i_Vertex_Index_2, weight);
    adjList[i_Vertex_Index_1].push_back(edge);
}

int Graf::getVerticesSize() const{
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

bool Graf::checkEdge(int i_Vertex_Index_1, int i_Vertex_Index_2) const {
    for (const auto& edge : adjList[i_Vertex_Index_1]) {
        if ((edge.getV1() == i_Vertex_Index_1 && edge.getV2() == i_Vertex_Index_2) ||
            (edge.getV1() == i_Vertex_Index_2 && edge.getV2() == i_Vertex_Index_1)) {
            return true;
        }
    }
    return false;
}

std::vector<std::vector<Edge>> Graf::getAdjList() const{
    return adjList;

}

std::vector<int> Graf::getInNeighbourIndices(int idx) const {
    std::vector<int> inNeighbours;
    for (int i = 0; i < maxVert; ++i) {
        for (const auto& edge : adjList[i]) {
            if (edge.getV2() == idx) {
                inNeighbours.push_back(i);
                break;
            }
        }
    }
    return inNeighbours;
}




void Graf::printMatrix(std::ostream& out) const{

    std::vector<std::vector<int>> adjacencyMatrix(maxVert, std::vector<int>(maxVert, 0));

    for (int i = 0; i < maxVert; ++i) {
        for (const auto& edge : adjList[i]) {
            int j = (edge.getV1() == i) ? edge.getV2() : edge.getV1();
            adjacencyMatrix[i][j] = edge.getWeight();
        }
    }

    for (int i = 0; i < maxVert; ++i) {
        for (int j = 0; j < maxVert; ++j) {
            out << adjacencyMatrix[i][j] << " ";
        }
        out << std::endl;
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

void Graf::DFSUtil(int v, std::vector<bool>& visited, std::vector<int>& path, std::vector<std::vector<int>>& allPaths) const {
    visited[v] = true;
    path.push_back(v);

    if (v == destination) {
        allPaths.push_back(path);
    } else {
        std::vector<int> neighbours = getNeighbourIndices(v);
        for (int u : neighbours) {
            if (!visited[u]) {
                DFSUtil(u, visited, path, allPaths);
            }
        }
    }
    path.pop_back();
    visited[v] = false;
}

std::vector<std::vector<int>> Graf::DFS(int start, int end)  {
    std::vector<std::vector<int>> allPaths;
    std::vector<bool> visited(getVerticesSize(), false);
    std::vector<int> path;
    destination = end; // Ustawiamy wierzchołek docelowy

    DFSUtil(start, visited, path, allPaths);

    return allPaths;
}

std::vector<int> Graf::BFS() {
    std::vector<int> bfsOrder;
    std::vector<bool> visited(getVerticesSize(), false);
    std::queue<int> q;
    q.push(0);
    visited[0] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        bfsOrder.push_back(current);

        std::vector<int> neighbours = getNeighbourIndices(current);
        for (int neighbour : neighbours) {
            if (!visited[neighbour]) {
                q.push(neighbour);
                visited[neighbour] = true;
            }
        }
    }

    return bfsOrder;
}
