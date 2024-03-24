
#include "Graf.h"
#include <limits>
#include <queue>
#include <vector>
#include <algorithm>

const int INF = std::numeric_limits<int>::max();

std::vector<int> Graf::dijkstra(int startVertex, int endVertex) const {
    std::vector<int> distances(SIZE, INF);
    distances[startVertex] = 0;
    std::queue<int> q;
    q.push(startVertex);
    std::vector<int> previous(SIZE, -1);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (u == endVertex) {
            break;
        }
        std::vector<int> neighbours = getOutNeighbourIndices(u);
        for (int v : neighbours) {
            int weight = getWeightEdge(u, v);
            if (weight == 0) {
                continue;
            }
            int alt = distances[u] + weight;
            if (alt < distances[v]) {
                distances[v] = alt;
                previous[v] = u;
                q.push(v);
            }
        }
    }
    std::vector<int> shortestPath;
    int v = endVertex;
    while (v != -1) {
        shortestPath.push_back(v);
        v = previous[v];
    }
    std::reverse(shortestPath.begin(), shortestPath.end());
    return shortestPath;
}

Graf::Graf(){
    maxVert = 0;
    clear();
}

int Graf::getVerticesSize() const {
    return maxVert;
}

bool checkCorrect(int i1, int i2){
    if(i1 > SIZE - 1 || i2 > SIZE - 1 || i1 < 0 || i2 < 0) return false;
    return true;
}

void Graf::addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2){
    if(checkCorrect(i_Vertex_Index_1,i_Vertex_Index_2)){
        matrix[i_Vertex_Index_1][i_Vertex_Index_2] = 1;
        if(i_Vertex_Index_1>maxVert) maxVert = i_Vertex_Index_1;
        if(i_Vertex_Index_2>maxVert) maxVert = i_Vertex_Index_2;
    } 
}

void Graf::addEdge(int i_Vertex_Index_1, int i_Vertex_Index_2, int waga){
    if(checkCorrect(i_Vertex_Index_1,i_Vertex_Index_2)){
        matrix[i_Vertex_Index_1][i_Vertex_Index_2] = waga;
        if(i_Vertex_Index_1>maxVert) maxVert = i_Vertex_Index_1;
        if(i_Vertex_Index_2>maxVert) maxVert = i_Vertex_Index_2;
    } 
}

void Graf::removeEdge(int i_Vertex_Index_1, int i_Vertex_Index_2) {
    if(checkCorrect(i_Vertex_Index_1,i_Vertex_Index_2)) matrix[i_Vertex_Index_1][i_Vertex_Index_2] = 0;
}

bool Graf::checkEdge(int i_Vertex_Index_1, int i_Vertex_Index_2)  {
    if(!checkCorrect(i_Vertex_Index_1,i_Vertex_Index_2)) return false;
    if(matrix[i_Vertex_Index_1][i_Vertex_Index_2]) return true;
    return false;
}

int Graf::getWeightEdge(int i_Vertex_Index_1, int i_Vertex_Index_2) const {
    if(!checkCorrect(i_Vertex_Index_1,i_Vertex_Index_2)) return false;
    return matrix[i_Vertex_Index_1][i_Vertex_Index_2];
}

int Graf::vertexDegree(int idx)  {
    if(idx>SIZE-1 || idx<0) return -1;
    int stopien = 0;
    for(int i = 0; i<SIZE;i++) if(matrix[i][idx] != 0 || matrix[idx][i] != 0 && idx != i) stopien++;
    return stopien;
}

std::vector<int> Graf::getNeighbourIndices(int idx) const {
    std::vector<int> neighbours;
    if(idx>SIZE-1 || idx<0) return neighbours;
    for(int i = 0; i<SIZE;i++) if(matrix[idx][i] != 0  || matrix[i][idx] != 0){
        neighbours.push_back(i);
    }
    return neighbours;
}

std::vector<int> Graf::getOutNeighbourIndices(int idx) const {
    std::vector<int> neighbours;
    if(idx>SIZE-1 || idx<0) return neighbours;
    for(int i = 0; i<SIZE;i++) if(matrix[idx][i] != 0){
        neighbours.push_back(i);
    }
    return neighbours;
}

void Graf::PrintToFile(int size,std::string filename){
    std::ofstream out(filename);
    if (size > SIZE)
        size = SIZE;

    if (!out) {
        std::cerr << "Nie można otworzyć strumienia do zapisu." << std::endl;
        return;
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            out << matrix[i][j] << " "; 
        }
        out << std::endl; 
    }

}

void Graf::printNeighbourIndices(int idx)  {
    std::vector<int> sasiedzi = getNeighbourIndices(idx);
    for(auto& it : sasiedzi) std::cout << it << std::endl;
}

int Graf::getNumberOfEdges()  {
    int licz = 0;
    for(int i = 0; i<SIZE;i++) for(int j = 0; j<SIZE;j++) if(matrix[i][j] && i != j) licz++;
    return licz;
}

void Graf::readFromFile(std::string path,bool wagi)  {
    clear();
    int a,b,c,licznik;
    std::ifstream plik{path};
    plik >> licznik;
    if(wagi){
        while(plik >> a >> b >> c) addEdge(a,b,c);
    }
    else{
        while(plik >> a >> b) addEdge(a,b);
    }
}

void Graf::printMatrix(int size, std::ostream& out) {
    if (size > SIZE)
        size = SIZE;
    if (!out) {
        std::cerr << "Nie można otworzyć strumienia do zapisu." << std::endl;
        return;
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            out << matrix[i][j] << " "; 
        }
        out << std::endl; 
    }
}


Graf::~Graf() {
    // // Zwalnianie pamięci dla macierzy sąsiedztwa
    // for (int i = 0; i < numberOfVertices; ++i) {
    //     delete[] matrix[i];
    // }
}

void Graf::clear(){
    for(int i = 0; i<SIZE;i++) for(int j = 0; j<SIZE;j++) matrix[i][j] = 0;
}
