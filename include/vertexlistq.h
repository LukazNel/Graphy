#ifndef VERTEXLISTQ_H
#define VERTEXLISTQ_H

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <vector>
#include <algorithm>
#include <qqml.h>

struct Vertex {
    int label;
    int weight;
};

class VertexQ : public QObject {
    Q_OBJECT
    Q_PROPERTY(int label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(int weight READ weight WRITE setWeight NOTIFY weightChanged)
    QML_ELEMENT
private:
    Vertex m_data;

public:
    VertexQ(QObject* parent = nullptr) : QObject(parent) {}
    VertexQ(Vertex init, QObject* parent = nullptr) : QObject(parent), m_data(init) {}

    int label() const { return m_data.label; }
    int weight() const { return m_data.weight; }

    void setLabel(const int label) { m_data.label = label; }
    void setWeight(const int weight) { m_data.weight = weight; }
    auto getVertex() const -> Vertex { return m_data; };

    ~VertexQ() {}

signals:
    void labelChanged();
    void weightChanged();
};

class VertexListQ : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int label READ getLabel WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(bool isOnPath READ isOnPath WRITE setOnPath NOTIFY isOnPathChanged)
    Q_PROPERTY(QQmlListProperty<VertexQ> edges READ getEdges)
    QML_ELEMENT

private:
    int m_label;
    bool is_on_path;
    std::vector<VertexQ* > m_edges;

    static void addEdge(QQmlListProperty<VertexQ>* list, VertexQ* item);
    static auto numEdges(QQmlListProperty<VertexQ>* list)
        -> int;
    static auto edgeAt(QQmlListProperty<VertexQ>* list, int index)
        -> VertexQ*;
    static void clearEdges(QQmlListProperty<VertexQ>* list);
    static void replaceEdge(QQmlListProperty<VertexQ>* list, int index, VertexQ* item);
    static void truncateEdge(QQmlListProperty<VertexQ>* list);

public:
    enum DataTypes {
        LabelType = Qt::UserRole,
        WeightType,
        MixedType
    };

    VertexListQ(QObject* parent = nullptr);
    //VertexListQ(int label, QVector<VertexQ* > edges, QObject* parent = nullptr);

    int getLabel() const;
    bool isOnPath() const { return is_on_path; }
    auto getEdges() -> QQmlListProperty<VertexQ>;
    auto getVertex(int index) const -> VertexQ*;
    void setVertex(int index, VertexQ* item);
    void updateVertices(int rm_vert_label);

    void setLabel(const int label);
    void setOnPath(bool b) { is_on_path = b; }

    // Read Methods
    auto rowCount(const QModelIndex& parent = QModelIndex()) const
        -> int override;
    auto data(const QModelIndex& index, int role = Qt::DisplayRole) const
        -> QVariant override;
    auto headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
        -> QVariant override;
    auto roleNames() const
        -> QHash<int, QByteArray> override;
    auto getBareExpandedList() const -> std::vector<int>;

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

    ~VertexListQ() {};

public slots:
    void append(QObject* object, int label, int weight);
    QObject* remove(int label);

signals:
    void labelChanged();
    void isOnPathChanged();
};

#endif // VERTEXLISTQ_H
