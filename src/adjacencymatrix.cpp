#include "../include/adjacencymatrix.h"

AdjacencyMatrix::AdjacencyMatrix(size_t size) : rows(size, std::vector<int>(size, 0)) {
}

AdjacencyMatrix::AdjacencyMatrix(std::vector<std::vector<int> > copy, bool is_undirected) {
    int i = 0;
    for (auto& row : copy) {
        int diff = copy.size() - row.size();
        if (diff > 0) {
            row.insert(row.end(), diff, 0);
        } else if (diff < 0) {
            row.erase(row.end() + diff, row.end());
        }

        row[i++] = 0;
        rows.push_back(row);
    }

    if (is_undirected == true) {
        int size = rows.size();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (rows[i][j] > rows[j][i])
                    rows[j][i] = rows[i][j];
                else if (rows[i][j] < rows[j][i])
                    rows[i][j] = rows[j][i];
            }
        }
    }
}

auto AdjacencyMatrix::size() const -> size_t {
    return rows.size();
}

auto AdjacencyMatrix::at(int row, int column) const -> unsigned int {
    if (row == -1)
        row = rows.size();
    if (column == -1)
        column = rows.size();
    return rows.at(column).at(row);
}

auto AdjacencyMatrix::dijkstra(int A, int B)
-> std::vector<int> {
    struct path {
        std::stack<int> to_here;
        int next;
        int cost;
    };
    using path_ptr = std::unique_ptr<path>;
    auto cmp = [](const path* A, const path* B) -> bool { return A->cost > B->cost; };
    std::priority_queue<path*, std::vector<path*>, decltype(cmp)> next_path(cmp);

    path_ptr current_path = std::make_unique<path>();
    current_path->next = A;
    int max_verts = rows.size();
    while (current_path->next != B) {
        current_path->to_here.push(current_path->next);
        for (int i = 0; i < max_verts; i++) {
            int index = current_path->to_here.top();
            if (i == index)
                continue;

            int next_weight = rows[index][i];
            if (next_weight != 0) {
                auto copy = std::make_unique<path>();
                copy->to_here = current_path->to_here;
                copy->next = i;
                copy->cost = current_path->cost + next_weight;
                next_path.push(copy.release());
            }
        }
        if (next_path.empty())
            break;

        current_path.reset(next_path.top());
        next_path.pop();
    }

    if (current_path->next != B)
        return {};

    std::vector<int> final_path;
    final_path.push_back(current_path->cost);
    while (!current_path->to_here.empty()) {
        final_path.insert(final_path.begin() + 1, current_path->to_here.top());
        current_path->to_here.pop();
    }
    final_path.push_back(current_path->next);
    return final_path;
}

bool AdjacencyMatrix::enlarge(const size_t added_size) {
    for (auto& column : rows) {
        column.insert(column.end(), added_size, 0);
    }
    rows.insert(rows.end(), added_size, std::vector<int>(added_size, 0));
    return true;
}

bool AdjacencyMatrix::erase(const size_t num_elements, const size_t from_index) {
    if (num_elements > (rows.size() - from_index)) {
        return false;
    }

    for (auto& column : rows) {
        column.erase(column.begin() + from_index, column.begin() + num_elements - 1);
    }
    rows.erase(rows.begin() + from_index, rows.begin() + num_elements - 1);
    return true;
}

bool AdjacencyMatrix::shrink(const size_t subtracted_size) {
    return erase(subtracted_size, rows.size() - subtracted_size);
}

bool AdjacencyMatrix::resize(const size_t new_size) {
    if (new_size > rows.size()) {
        return enlarge(rows.size() - new_size);
    } else {
        return shrink(new_size - rows.size());
    }
}

bool AdjacencyMatrix::set(int row, int column, const unsigned int value) {
    if (row == column) {
        return false;
    }

    if (row == -1)
        row = rows.size() - 1;
    if (column == -1)
        column = rows.size() - 1;

    rows.at(column).at(row) = value;
    return true;
}

QTextStream& operator<<(QTextStream& out, const AdjacencyMatrix& data) {
    for (const auto& row : data.rows) {
        for (const auto& item : row) {
            out << '\t' << item;
        }
        out << '\n';
        out.flush();
    }
    return out;
}
