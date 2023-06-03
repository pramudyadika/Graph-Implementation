#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <climits>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Edge {
public:
    int source, destination, weight;
};

class Graph {
private:
    int numVertices;
    vector<Edge> edges;
    vector<vector<int>> adjacencyMatrix;

public:
    Graph(int numVertices) {
        this->numVertices = numVertices;
        adjacencyMatrix.resize(numVertices, vector<int>(numVertices, 0));
    }

    void addEdge(int source, int destination, int weight) {
        Edge edge;
        edge.source = source;
        edge.destination = destination;
        edge.weight = weight;
        edges.push_back(edge);
        adjacencyMatrix[source][destination] = weight;
        adjacencyMatrix[destination][source] = weight;
    }

    void printMatrix() {
        for (const auto& row : adjacencyMatrix) {
            for (int weight : row) {
                cout << weight << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    int find(vector<int>& parent, int vertex) {
        if (parent[vertex] == -1)
            return vertex;
        return find(parent, parent[vertex]);
    }

    void Union(vector<int>& parent, int x, int y) {
        int xSet = find(parent, x);
        int ySet = find(parent, y);
        parent[xSet] = ySet;
    }

    void kruskalMST() {
        sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) {
            return e1.weight < e2.weight;
        });

        vector<Edge> minimumSpanningTree;
        vector<int> parent(numVertices, -1);
        int edgeCount = 0;

        for (const auto& edge : edges) {
            int x = find(parent, edge.source);
            int y = find(parent, edge.destination);
            if (x != y) {
                minimumSpanningTree.push_back(edge);
                Union(parent, x, y);
                edgeCount++;
            }

            if (edgeCount == numVertices - 1)
                break;
        }

        cout << "Minimum Spanning Tree (Kruskal):" << endl;
        for (const auto& edge : minimumSpanningTree) {
            cout << "Edge: " << edge.source << "-" << edge.destination << ", Weight: " << edge.weight << endl;
        }
        cout << endl;
    }

    void primMST() {
        vector<int> mstVertices(numVertices, false);
        vector<int> parent(numVertices, -1);
        vector<int> key(numVertices, INT_MAX);

        int startVertex = 0;
        key[startVertex] = 0;

        for (int count = 0; count < numVertices - 1; count++) {
            int minKey = INT_MAX;
            int minIndex = -1;
            for (int v = 0; v < numVertices; v++) {
                if (!mstVertices[v] && key[v] < minKey) {
                    minKey = key[v];
                    minIndex = v;
                }
            }

            mstVertices[minIndex] = true;

            for (int v = 0; v < numVertices; v++) {
                if (adjacencyMatrix[minIndex][v] && !mstVertices[v] && adjacencyMatrix[minIndex][v] < key[v]) {
                    parent[v] = minIndex;
                    key[v] = adjacencyMatrix[minIndex][v];
                }
            }
        }

        cout << "Minimum Spanning Tree (Prim):" << endl;
        for (int i = 1; i < numVertices; i++) {
            cout << "Edge: " << parent[i] << "-" << i << ", Weight: " << adjacencyMatrix[i][parent[i]] << endl;
        }
        cout << endl;
    }
};

int main() {
    int numVertices = 7;
    Graph graph(numVertices);

    // Tambahkan edge dan bobotnya
    graph.addEdge(0, 1, 2);
    graph.addEdge(0, 3, 1);
    graph.addEdge(0, 2, 4);
    graph.addEdge(1, 3, 3);
    graph.addEdge(1, 4, 10);
    graph.addEdge(2, 3, 2);
    graph.addEdge(2, 5, 5);
    graph.addEdge(3, 4, 7);
    graph.addEdge(3, 5, 8);
    graph.addEdge(3, 6, 4);
    graph.addEdge(4, 6, 6);
    graph.addEdge(5, 6, 1);

    cout << "Adjacency Matrix:" << endl;
    graph.printMatrix();

    // Mengukur running time Algoritma Kruskal
    auto startKruskal = high_resolution_clock::now();
    graph.kruskalMST();
    auto endKruskal = high_resolution_clock::now();

    // Mengukur running time Algoritma Prim
    auto startPrim = high_resolution_clock::now();
    graph.primMST();
    auto endPrim = high_resolution_clock::now();

    // Menghitung running time dalam milidetik (ms)
    auto durationKruskal = duration_cast<milliseconds>(endKruskal - startKruskal);
    auto durationPrim = duration_cast<milliseconds>(endPrim - startPrim);

    // Menampilkan running time kedua algoritma
    cout << "Running Time (Kruskal): " << durationKruskal.count() << " ms" << endl;
    cout << "Running Time (Prim): " << durationPrim.count() << " ms" << endl;

    return 0;
}