#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H

#include <vector>
#include <stack>
#include <queue>
#include <memory>
#include <QTextStream>


class AdjacencyMatrix {
private:
    std::vector<std::vector<int> > rows;

public:
    AdjacencyMatrix(size_t size = 0);
    AdjacencyMatrix(std::vector<std::vector<int>> copy, bool is_undirected = false);

    // Read Methods
    auto size() const -> size_t;
    auto at(const int row, int column) const -> unsigned int;
    auto dijkstra(int A, int B) -> std::vector<int>;

    // Write Methods
    bool enlarge(const size_t added_size);
    bool erase(const size_t num_elements, const size_t from_index);
    bool shrink(const size_t subtracted_size);
    bool resize(const size_t new_size);
    bool set(int row, int column, const unsigned int value);

    friend QTextStream& operator<<(QTextStream & out, const AdjacencyMatrix& data);
};


#endif // ADJACENCYMATRIX_H
