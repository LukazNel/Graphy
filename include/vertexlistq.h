#ifndef VERTEXLISTQ_H
#define VERTEXLISTQ_H

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <vector>
#include <algorithm>
#include <qqml.h>

struct Edge {
    int to_vertex;
    int weight;
};

class EdgeQ : public QObject {
    Q_OBJECT
    Q_PROPERTY(int toVertex READ toVertex WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(int weight READ weight WRITE setWeight NOTIFY weightChanged)
    QML_ELEMENT
private:
    Edge m_data;

public:
    EdgeQ(QObject* parent = nullptr) : QObject(parent) {}
    EdgeQ(Edge init, QObject* parent = nullptr) : QObject(parent), m_data(init) {}

    int toVertex() const { return m_data.to_vertex; }
    int weight() const { return m_data.weight; }

    void setLabel(const int label) { m_data.to_vertex = label; }
    void setWeight(const int weight) { m_data.weight = weight; }
    auto getVertex() const -> Edge { return m_data; };

    ~EdgeQ() {}

signals:
    void labelChanged();
    void weightChanged();
};

class EdgeListQ : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int label READ getLabel NOTIFY labelChanged)
    Q_PROPERTY(bool isOnPath READ isOnPath NOTIFY isOnPathChanged)
    Q_PROPERTY(bool isSelected READ isSelected WRITE setSelected NOTIFY isSelectedChanged)
    Q_PROPERTY(QQmlListProperty<EdgeQ> edges READ makeEdgesProperty)
    QML_ELEMENT

private:
    int vertex_label;
    bool is_on_path;
    bool is_selected;
    std::vector<EdgeQ* > m_edges;

    // QQmlListProperty Methods
    static void addEdge(QQmlListProperty<EdgeQ>* list, EdgeQ* item);
    static auto numEdges(QQmlListProperty<EdgeQ>* list)
        -> int;
    static auto edgeAt(QQmlListProperty<EdgeQ>* list, int index)
        -> EdgeQ*;
    static void clearEdges(QQmlListProperty<EdgeQ>* list);
    static void replaceEdge(QQmlListProperty<EdgeQ>* list, int index, EdgeQ* item);
    static void truncateEdge(QQmlListProperty<EdgeQ>* list);

public:
    enum DataTypes {
        LabelNumType = Qt::UserRole,
        LabelAlphType,
        WeightType
    };

    // Constructors
    EdgeListQ(QObject* parent = nullptr)
        : QAbstractListModel(parent), is_on_path(false), is_selected(false) {}

    // Getter Methods
    int getLabel() const { return vertex_label; }
    bool isOnPath() const { return is_on_path; }
    bool isSelected() const { return is_selected; }
    auto getVertex(int index) const -> EdgeQ* { return m_edges.at(index); }

    auto makeEdgesProperty() -> QQmlListProperty<EdgeQ>;
    auto getBareExpandedList() const -> std::vector<int>;

    // Setter Methods
    void setLabel(const int label) { vertex_label = label; labelChanged(); }
    void setOnPath(const bool b) { is_on_path = b; isOnPathChanged(); }
    void setSelected(const bool b) { is_selected = b; isSelectedChanged(); }
    void setVertex(int index, EdgeQ* item) { m_edges[index] = item; };
    // setVertex: dataChanged called on site

    void updateVertices(int rm_vert_label);

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
    auto setData(const QModelIndex& index, const QVariant& value, int role = Qt::DisplayRole)
        -> bool override;
    auto flags(const QModelIndex& index) const
        -> Qt::ItemFlags override;

    // Resize Methods
    auto insertRows(int before_row, int count, const QModelIndex& parent = QModelIndex())
        -> bool override;
    auto removeRows(int from_row, int count, const QModelIndex& parent = QModelIndex())
        -> bool override;

    ~EdgeListQ() {};

public slots:
    void append(QObject* object, int label, int weight);
    QObject* remove(int label);

signals:
    void labelChanged();
    void isOnPathChanged();
    void isSelectedChanged();
};

#endif // VERTEXLISTQ_H
