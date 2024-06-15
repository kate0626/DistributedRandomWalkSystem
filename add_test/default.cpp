#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdlib> // for rand(), srand()
#include <ctime>   // for time()
#include <random>
#include <fstream> // for file operations
#include <sstream> // for string stream

using namespace std;

class Graph
{
public:
    unordered_map<int, vector<int>> adjList;

    void addEdge(int u, int v)
    {
        adjList[u].push_back(v);
    }

    const vector<int> &getNeighbors(int u) const
    {
        static const vector<int> empty;
        auto it = adjList.find(u);
        return it != adjList.end() ? it->second : empty;
    }
};

void loadGraphFromFile(Graph &graph, const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        int u, v;
        if (!(iss >> u >> v))
        {
            break;
        }
        graph.addEdge(u, v);
    }

    file.close();
}

void randomWalk(const Graph &graph, int startNode, double alpha, unordered_map<int, int> &visitCount)
{
    int currentNode = startNode;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    while (true)
    {
        cout << "Visited node: " << currentNode << endl;
        visitCount[currentNode]++;

        // Determine if we should continue or stop
        if (dis(gen) > alpha)
        {
            break;
        }

        const vector<int> &neighbors = graph.getNeighbors(currentNode);
        if (neighbors.empty())
        {
            break; // No neighbors to move to
        }

        // Choose a random neighbor
        uniform_int_distribution<> disNeighbor(0, neighbors.size() - 1);
        currentNode = neighbors[disNeighbor(gen)];
    }
}

int main()
{
    Graph graph;

    // Load graph from file
    loadGraphFromFile(graph, "../dataset/source_graph/karate.txt");

    // Starting node and alpha
    int startNode = 1;
    double alpha = 0.85;

    // Visit count map
    unordered_map<int, int> visitCount;

    randomWalk(graph, startNode, alpha, visitCount);

    // Output the visit count for each node
    cout << "\nVisit count for each node:" << endl;
    for (const auto &pair : visitCount)
    {
        cout << "Node " << pair.first << " was visited " << pair.second << " times." << endl;
    }

    return 0;
}
