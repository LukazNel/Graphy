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
    Q_PROPERTY(bool isUndirected READ isUndirected WRITE setUndirected NOTIFY isUndirectedChanged)
    Q_PROPERTY(bool isBad READ isBad NOTIFY isBadChanged)
    Q_PROPERTY(bool debugIsOn READ debugIsOn NOTIFY debugIsOnChanged)
    Q_PROPERTY(int totalCost READ totalCost NOTIFY totalCostChanged)
    Q_PROPERTY(QQmlListProperty<EdgeListQ> vertices READ makeVerticesProperty)
    QML_ELEMENT

private:
    std::vector<EdgeListQ* > m_vertices;
    bool is_undirected;
    bool is_bad;
    bool debug_on;
    int labelA;
    int labelB;
    int total_cost;

    // QQmlListProperty Methods
    static void addVertex(QQmlListProperty<EdgeListQ>* list, EdgeListQ* item);
    static auto numVertices(QQmlListProperty<EdgeListQ>* list)
        -> int;
    static auto vertexAt(QQmlListProperty<EdgeListQ>* list, int index)
        -> EdgeListQ*;
    static void clearVertices(QQmlListProperty<EdgeListQ>* list);
    static void replaceVertex(QQmlListProperty<EdgeListQ>* list, int index, EdgeListQ* item);
    static void truncateVertex(QQmlListProperty<EdgeListQ>* list);

    // Setter Methods
    void setIsBad(bool b) { is_bad = b; isBadChanged(); }
    void setTotalCost(int cost) { total_cost = cost; totalCostChanged(); }
    void clearPathHighlights();
    void runDijkstraHelper();
    void reset() { setIsBad(false); setTotalCost(0); resetSelected(); }
public:
    enum DataTypes {
        LabelNumType = Qt::UserRole,
        LabelAlphType,
        EdgeType,
        IsOnPath,
        IsSelected
    };

    AdjacencyListQ(QObject* parent = nullptr);

    // Getter Methods
    bool isUndirected() const { return is_undirected; }
    bool isBad() const { return is_bad; }
    bool debugIsOn() const { return debug_on; }
    int totalCost() const { return total_cost; }

    auto makeVerticesProperty() -> QQmlListProperty<EdgeListQ>;
    auto makeAdjacencyMatrix() const -> AdjacencyMatrix;

    // Setter Methods
    void refreshLabels(int rm_vert_label);
    void resetSelected();

// Inherited Methods
    // Read Methods
    auto rowCount(const QModelIndex& parent = QModelIndex()) const
        -> int override;
    auto data(const QModelIndex& index, int role = Qt::DisplayRole) const
        -> QVariant override;
    auto headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
        -> QVariant override;
    auto roleNames() const
        -> QHash<int, QByteArray> override;

    // Write Methods
    void setUndirected(bool b) { is_undirected = b; runDijkstraHelper(); }
    auto setData(const QModelIndex& index, const QVariant& value, int role = Qt::DisplayRole)
        -> bool override;
    auto flags(const QModelIndex& index) const
        -> Qt::ItemFlags override;

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
