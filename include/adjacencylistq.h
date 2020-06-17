#ifndef ADJACENCYLISTQ_H
#define ADJACENCYLISTQ_H

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <vector>
#include <qqml.h>

#include "vertexlistq.h"
#include "adjacencymatrix.h"

class AdjacencyListQ : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<VertexListQ> vertices READ getVertices)
    Q_PROPERTY(bool isUndirected READ isUndirected WRITE setUndirected NOTIFY isUndirectedChanged)
    Q_PROPERTY(bool isBad READ isBad NOTIFY isBadChanged)
    Q_PROPERTY(bool debugIsOn READ debugIsOn NOTIFY debugIsOnChanged)
    Q_PROPERTY(int totalCost READ totalCost NOTIFY totalCostChanged)
    QML_ELEMENT

private:
    std::vector<VertexListQ* > m_vertices;
    bool is_undirected;
    bool is_bad;
    bool debug_on;
    int labelA;
    int labelB;
    int total_cost;

    static void addVertex(QQmlListProperty<VertexListQ>* list, VertexListQ* item);
    static auto numVertices(QQmlListProperty<VertexListQ>* list)
        -> int;
    static auto vertexAt(QQmlListProperty<VertexListQ>* list, int index)
        -> VertexListQ*;
    static void clearVertices(QQmlListProperty<VertexListQ>* list);
    static void replaceVertex(QQmlListProperty<VertexListQ>* list, int index, VertexListQ* item);
    static void truncateVertex(QQmlListProperty<VertexListQ>* list);

    void setIsBad(bool b) { is_bad = b; isBadChanged(); }
    void setTotalCost(int cost) { total_cost = cost; totalCostChanged(); }
    void clearPathHighlights();
    void runDijkstraHelper();
public:
    enum DataTypes {
        LabelNumType = Qt::UserRole,
        LabelAlphType,
        EdgeType,
        IsOnPath
        //IsSelected
    };

    AdjacencyListQ(QObject* parent = nullptr);

    auto getVertices() -> QQmlListProperty<VertexListQ>;
    bool isUndirected() const { return is_undirected; }
    bool isBad() const { return is_bad; }
    bool debugIsOn() const { return debug_on; }
    int totalCost() const { return total_cost; }

    // Read Methods
    auto rowCount(const QModelIndex& parent = QModelIndex()) const
        -> int override;
    auto data(const QModelIndex& index, int role = Qt::DisplayRole) const
        -> QVariant override;
    auto headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
        -> QVariant override;
    auto roleNames() const
        -> QHash<int, QByteArray> override;
    auto makeAdjacencyMatrix() const -> AdjacencyMatrix;

    // Write Methods
    void setUndirected(bool b) { is_undirected = b; runDijkstraHelper(); }
    auto setData(const QModelIndex& index, const QVariant& value, int role = Qt::DisplayRole)
        -> bool override;
    auto flags(const QModelIndex& index) const
        -> Qt::ItemFlags override;
    void refreshLabels(int rm_vert_label);

    // Resize Methods
    auto insertRows(int before_row, int count, const QModelIndex& parent = QModelIndex())
        -> bool override;
    auto removeRows(int from_row, int count, const QModelIndex& parent = QModelIndex())
        -> bool override;

    ~AdjacencyListQ() {};

public slots:
    void append(QObject* o);
    QObject* remove(int label);
    void runDijkstra(int label);

signals:
    void isUndirectedChanged();
    void isBadChanged();
    void debugIsOnChanged();
    void totalCostChanged();
};

#endif // ADJACENCYLISTQ_H
