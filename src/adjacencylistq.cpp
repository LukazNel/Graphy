#include "../include/adjacencylistq.h"

// Constructors
AdjacencyListQ::AdjacencyListQ(QObject* parent)
    : QAbstractListModel(parent), is_undirected(false),
      is_bad(false), labelA(-1), labelB(-1) {};

// Public Methods
auto AdjacencyListQ::getVertices()
-> QQmlListProperty<VertexListQ> {
    return QQmlListProperty<VertexListQ>( this, this,
                                          &AdjacencyListQ::addVertex,
                                          &AdjacencyListQ::numVertices,
                                          &AdjacencyListQ::vertexAt,
                                          &AdjacencyListQ::clearVertices,
                                          &AdjacencyListQ::replaceVertex,
                                          &AdjacencyListQ::truncateVertex);
}

int AdjacencyListQ::rowCount(const QModelIndex& /*parent*/) const {
    return m_vertices.size();
}


auto AdjacencyListQ::data(const QModelIndex& index, int role) const
-> QVariant {
    QVariant result;
    switch (role) {
        case LabelType : result = QVariant::fromValue((m_vertices.at(index.row())->getLabel()));
            break;
        case EdgeType : result = QVariant::fromValue(m_vertices.at(index.row()));
            break;
        case IsOnPath : result = QVariant::fromValue(m_vertices.at(index.row())->isOnPath());
            break;
        default : result = QVariant();
    }
    return result;
}

auto AdjacencyListQ::headerData(int section, Qt::Orientation orientation, int role) const
-> QVariant {
    if (orientation == Qt::Horizontal)
        return QVariant();

    QVariant result;
    switch (role) {
        case Qt::DisplayRole : result = QString(section);
            break;
        default : result = QVariant();
    }
    return result;
}

auto AdjacencyListQ::roleNames() const
-> QHash<int, QByteArray> {
    QHash<int, QByteArray> roles;
    roles[LabelType] = "label";
    roles[EdgeType] = "edges";
    roles[IsOnPath] = "isOnPath";
    return roles;
}

auto AdjacencyListQ::makeAdjacencyMatrix() const
-> AdjacencyMatrix {
    std::vector<std::vector<int> > copy;
    for (const auto* it : m_vertices) {
        copy.push_back(it->getBareExpandedList());
    }

    AdjacencyMatrix matrix(copy, is_undirected);
    return matrix;
}

bool AdjacencyListQ::setData(const QModelIndex &/*index*/, const QVariant &/*value*/, int /*role*/) {
    return false;
}

auto AdjacencyListQ::flags(const QModelIndex& /*index*/) const
-> Qt::ItemFlags {
    return Qt::ItemIsSelectable | Qt::ItemIsEditable
            | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled
            | Qt::ItemIsEnabled;
}

void AdjacencyListQ::refreshLabels(int rm_vert_label) {
    int num = 0;
    for (auto it : m_vertices) {
        it->updateVertices(rm_vert_label);
        it->setLabel(num++);
    }
    //m_vertices[0]->setLabel(13);
    dataChanged(createIndex(0,0), createIndex(m_vertices.size()-1,0), {LabelType});
}

bool AdjacencyListQ::insertRows(int before_row, int count, const QModelIndex& parent) {
    beginInsertRows(parent, before_row + 1, before_row + count);
    m_vertices.insert(m_vertices.begin() + before_row, count, {});
    endInsertRows();
    return true;
}

bool AdjacencyListQ::removeRows(int from_row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, from_row, from_row + count - 1);
    m_vertices.erase(m_vertices.begin() + from_row, m_vertices.begin() + from_row + count - 1);
    endRemoveRows();
    return true;
}

void AdjacencyListQ::append(QObject* o) {
    int i = m_vertices.size();
    beginInsertRows(QModelIndex(), i, i);
    o->setParent(this);
    VertexListQ* obj = qobject_cast<VertexListQ*>(o);
    obj->setLabel(m_vertices.size());
    m_vertices.push_back(obj);
    endInsertRows();
}

QObject* AdjacencyListQ::remove(int label) {
    beginRemoveRows(QModelIndex(), label, label);
    auto it = m_vertices.erase(m_vertices.begin() + label, m_vertices.begin() + label + 1);
    QObject* removed = (QObject*)*it;
    removed->setParent(nullptr);
    endRemoveRows();
    refreshLabels(label);
    runDijkstraHelper();
    return removed;
}

void AdjacencyListQ::runDijkstra(int label) {
        if (label == -1) {
            setIsBad(false);
            setTotalCost(0);
            if (labelB != -1)
                labelB = -1;
            else
                labelA = -1;
        } else if (labelA == label || labelB == label) {
            setIsBad(false);
            setTotalCost(0);
            if (labelA == label) {
                labelA = labelB;
                labelB = -1;
            } else
                labelB = -1;
        } else {
            if (labelA == -1)
                labelA = label;
            else
                labelB = label;
        }

        runDijkstraHelper();
}

// Private Methods
void AdjacencyListQ::addVertex(QQmlListProperty<VertexListQ>* list, VertexListQ* item) {
    AdjacencyListQ* object = qobject_cast<AdjacencyListQ* >(list->object);
    object->m_vertices.push_back(item);
}

auto AdjacencyListQ::numVertices(QQmlListProperty<VertexListQ>* list) -> int {
    AdjacencyListQ* object = qobject_cast<AdjacencyListQ* >(list->object);
    return object->m_vertices.size();
}

auto AdjacencyListQ::vertexAt(QQmlListProperty<VertexListQ>* list, int index)
-> VertexListQ* {
    AdjacencyListQ* object = qobject_cast<AdjacencyListQ* >(list->object);
    return object->m_vertices.at(index);
}

void AdjacencyListQ::clearVertices(QQmlListProperty<VertexListQ> *list) {
    AdjacencyListQ* object = qobject_cast<AdjacencyListQ* >(list->object);
    object->m_vertices.clear();
}

void AdjacencyListQ::replaceVertex(QQmlListProperty<VertexListQ> *list, int index,
                                   VertexListQ *item) {
    AdjacencyListQ* object = qobject_cast<AdjacencyListQ* >(list->object);
    object->m_vertices[index] = item;
}

void AdjacencyListQ::truncateVertex(QQmlListProperty<VertexListQ> *list) {
    AdjacencyListQ* object = qobject_cast<AdjacencyListQ* >(list->object);
    object->m_vertices.pop_back();
}

void AdjacencyListQ::clearPathHighlights() {
    int i = 0;
    for (auto& it : m_vertices) {
        it->setOnPath(false);
        auto index = createIndex(i++, 0);
        dataChanged(index, index, {IsOnPath});
    }
}

void AdjacencyListQ::runDijkstraHelper() {
    if (labelA == -1 || labelB == -1) {
        clearPathHighlights();
        return;
    }

    auto matrix = makeAdjacencyMatrix();
    auto result = matrix.dijkstra(labelA, labelB);

    QTextStream out(stdout);
    out << matrix << '\n';

    if (result.empty() == true) {
        out << "Path from " << labelA << " to " << labelB << " is impossible\n";
        clearPathHighlights();
        setIsBad(true);
        setTotalCost(0);
    } else {
        setIsBad(false);
        setTotalCost(result.front());
        for (int i = 1; i < result.size(); i++) {
            m_vertices[result[i]]->setOnPath(true);
            auto index = createIndex(result[i], 0);
            dataChanged(index, index, {IsOnPath});
        }
    }
    out.flush();
}
