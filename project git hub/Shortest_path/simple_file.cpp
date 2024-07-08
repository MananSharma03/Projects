#ifndef GRAPH_HPP
#define GRAPH_HPP
#include<bits/stdc++.h>
#include <limits>
#include <sstream>

class Graph {
public:
    void addEdge(const std::string& src, const std::string& dest, int weight);
    std::pair<int, std::vector<std::string>> dijkstra(const std::string& src, const std::string& dest, bool nan);

private:
    struct Vertex {
        std::unordered_map<std::string, int> nbrs;
    };

    struct DijkstraPair {
        std::string vname;
        std::string psf;
        int cost;

        bool operator<(const DijkstraPair& other) const {
            return cost > other.cost; // For priority_queue to work as min-heap
        }
    };

    std::unordered_map<std::string, Vertex> vtces;

    // Function to split a string into vector of strings based on delimiter
    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
};

void Graph::addEdge(const std::string& src, const std::string& dest, int weight) {
    vtces[src].nbrs[dest] = weight;
    vtces[dest].nbrs[src] = weight; // Assuming undirected graph
}

std::pair<int, std::vector<std::string>> Graph::dijkstra(const std::string& src, const std::string& dest, bool nan) {
    std::priority_queue<DijkstraPair> pq;
    std::unordered_map<std::string, DijkstraPair> map;
    int val = 0;

    // Initialize DijkstraPair for each vertex
    for (const auto& key : vtces) {
        DijkstraPair dp;
        dp.vname = key.first;
        dp.cost = (key.first == src) ? 0 : std::numeric_limits<int>::max();
        dp.psf = (key.first == src) ? key.first : "";

        pq.push(dp);
        map[key.first] = dp;
    }

    // Dijkstra's algorithm
    while (!pq.empty()) {
        DijkstraPair rp = pq.top();
        pq.pop();

        if (rp.vname == dest) {
            val = rp.cost;
            break;
        }

        // If the cost of current vertex in the priority queue is more than the
        // cost of the same vertex stored in the map, we will continue to the next iteration
        if (rp.cost > map[rp.vname].cost) {
            continue;
        }

        // Process neighbors
        for (const auto& nbr : vtces[rp.vname].nbrs) {
            int nc = rp.cost + (nan ? 120 + 40 * nbr.second : nbr.second);

            if (nc < map[nbr.first].cost) {
                DijkstraPair& gp = map[nbr.first];
                gp.psf = rp.psf + ">" + nbr.first; // Adjust delimiter as needed
                gp.cost = nc;

                pq.push(gp);
            }
        }
    }

    std::vector<std::string> path;
    if (map.find(dest) != map.end() && map[dest].cost != std::numeric_limits<int>::max()) {
        path = split(map[dest].psf, '>'); // Split path using '>'
        // Reverse the path to get it from src to dest
        std::reverse(path.begin(), path.end());
    }

    return std::make_pair(val, path);
}

#endif // GRAPH_HPP



// heap file
#ifndef HEAP_HPP
#define HEAP_HPP

#include <iostream>
#include <vector>
#include <unordered_map>

template <typename T>
class Heap {
public:
    void add(T item);
    void display() const;
    int size() const;
    bool isEmpty() const;
    T remove();
    T get() const;
    void updatePriority(T pair);

private:
    std::vector<T> data;
    std::unordered_map<T, int> map;

    void upheapify(int ci);
    void downheapify(int pi);
    void swap(int i, int j);
    int isLarger(T t, T o) const;
};

template <typename T>
void Heap<T>::add(T item) {
    data.push_back(item);
    map[item] = data.size() - 1;
    upheapify(data.size() - 1);
}

template <typename T>
void Heap<T>::upheapify(int ci) {
    int pi = (ci - 1) / 2;
    if (isLarger(data[ci], data[pi]) > 0) {
        swap(pi, ci);
        upheapify(pi);
    }
}

template <typename T>
void Heap<T>::swap(int i, int j) {
    T ith = data[i];
    T jth = data[j];
    data[i] = jth;
    data[j] = ith;
    map[ith] = j;
    map[jth] = i;
}

template <typename T>
void Heap<T>::display() const {
    for (const auto& item : data) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

template <typename T>
int Heap<T>::size() const {
    return data.size();
}

template <typename T>
bool Heap<T>::isEmpty() const {
    return data.empty();
}

template <typename T>
T Heap<T>::remove() {
    swap(0, data.size() - 1);
    T rv = data.back();
    data.pop_back();
    downheapify(0);
    map.erase(rv);
    return rv;
}

template <typename T>
void Heap<T>::downheapify(int pi) {
    int lci = 2 * pi + 1;
    int rci = 2 * pi + 2;
    int mini = pi;

    if (lci < data.size() && isLarger(data[lci], data[mini]) > 0) {
        mini = lci;
    }
    if (rci < data.size() && isLarger(data[rci], data[mini]) > 0) {
        mini = rci;
    }
    if (mini != pi) {
        swap(mini, pi);
        downheapify(mini);
    }
}

template <typename T>
T Heap<T>::get() const {
    return data[0];
}

template <typename T>
int Heap<T>::isLarger(T t, T o) const {
    return t > o ? 1 : (t < o ? -1 : 0);
}

template <typename T>
void Heap<T>::updatePriority(T pair) {
    int index = map[pair];
    upheapify(index);
}

#endif // HEAP_HPP


// main_file

#include <bits/stdc++.h>
#include <iostream>

int main() {
    Graph g;
    g.addEdge("A", "B", 5);
    g.addEdge("A", "C", 7);
    g.addEdge("B", "C", 3);
    g.addEdge("B", "I", 17);
    g.addEdge("B", "D", 1);
    g.addEdge("C", "D", 1);
    g.addEdge("C", "E", 3);
    g.addEdge("D", "E", 6);
    g.addEdge("E", "F", 3);
    g.addEdge("F", "G", 7);
    g.addEdge("E", "G", 4);
    g.addEdge("G", "H", 5);
    g.addEdge("H", "I", 2);

    auto result = g.dijkstra("B", "I", false);
    int distance = result.first;
    std::vector<std::string> path = result.second;

    std::cout << "Shortest distance from B to I is " << distance << std::endl;
    if (!path.empty()) {
        std::cout << "Path: ";
        for (int i = path.size() - 1; i >= 0; --i) { // Reverse order of output
            std::cout << path[i];
            if (i > 0) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    } else {
        std::cout << "No path found." << std::endl;
    }

    return 0;
    

    // for STATIONS 

    // g.addEdge("Noida Sector 62", "Botanical Garden", 8);
    // g.addEdge("Botanical Garden", "Yamuna Bank", 10);
    // g.addEdge("Yamuna Bank", "Vaishali~B", 8);
    // g.addEdge("Yamuna Bank", "Rajiv Chowk", 6);
    // g.addEdge("Rajiv Chowk", "Moti Nagar", 9);
    // g.addEdge("Moti Nagar", "Janak Puri West", 7);
    // g.addEdge("Janak Puri West", "Dwarka Sector 21", 6);
    // g.addEdge("Huda City Center", "Saket", 15);
    // g.addEdge("Saket", "AIIMS", 6);
    // g.addEdge("AIIMS", "Rajiv Chowk", 7);
    // g.addEdge("Rajiv Chowk", "New Delhi", 1);
    // g.addEdge("New Delhi", "Chandni Chowk", 2);
    // g.addEdge("Chandni Chowk", "Vishwavidyalaya", 5);
    // g.addEdge("New Delhi", "Shivaji Stadium", 2);
    // g.addEdge("Shivaji Stadium", "DDS Campus", 7);
    // g.addEdge("DDS Campu", "IGI Airport", 8);
    // g.addEdge("Moti Nagar", "Rajouri Garden", 2);
    // g.addEdge("Punjabi Bagh West", "Rajouri Garden", 2);
    // g.addEdge("Punjabi Bagh West", "Netaji Subhash Place", 3);

    // auto result = g.dijkstra("New Delhi", "Rajouri Garden", false);
    // int distance = result.first;
    // std::vector<std::string> path = result.second;

    // std::cout << "Shortest distance from New Delhi to Rajouri Garden is " << distance << std::endl;
    // if (!path.empty()) {
    //     std::cout << "Path: ";
    //     for (const auto& node : path) {
    //         std::cout << node << " -> ";
    //     }
    //     std::cout << std::endl;
    // } else {
    //     std::cout << "No path found." << std::endl;
    // }

    // return 0;
}
