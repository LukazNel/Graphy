#include "../include/adjacencylistq.h"

// Constructors
AdjacencyListQ::AdjacencyListQ(QObject* parent)
    : QAbstractListModel(parent), is_undirected(false),
      is_bad(false), debug_on(false), labelA(-1), labelB(-1) {
    #ifdef QT_DEBUG
        debug_on = true;
    #endif
};

// Public Getter Methods
auto AdjacencyListQ::makeVerticesProperty()
-> QQmlListProperty<EdgeListQ> {
    return QQmlListProperty<EdgeListQ>( this, this,
                                          &AdjacencyListQ::addVertex,
                                          &AdjacencyListQ::numVertices,
                                          &AdjacencyListQ::vertexAt,
                                          &AdjacencyListQ::clearVertices,
                                          &AdjacencyListQ::replaceVertex,
                                          &AdjacencyListQ::truncateVertex);
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

// Public Setter Methods
void AdjacencyListQ::refreshLabels(int rm_vert_label) {
    int num = 0;
    for (auto it : m_vertices) {
        it->updateVertices(rm_vert_label);
        it->setLabel(num++);
    }
    dataChanged(createIndex(0,0), createIndex(m_vertices.size()-1,0), {LabelAlphType, LabelNumType});
}

void AdjacencyListQ::resetSelected() {
    for (auto it : m_vertices) {
        it->setSelected(false);
    }
    dataChanged(createIndex(0,0), createIndex(m_vertices.size()-1,0), {IsSelected});
}

// Inherited Read Methods
int AdjacencyListQ::rowCount(const QModelIndex& /*parent*/) const {
    return m_vertices.size();
}


auto AdjacencyListQ::data(const QModelIndex& index, int role) const
-> QVariant {
    QVariant result;
    switch (role) {
        case LabelAlphType : result = QVariant::fromValue(QString('A' + m_vertices.at(index.row())->getLabel()));
            break;
        case LabelNumType : result = QVariant::fromValue(m_vertices.at(index.row())->getLabel());
            break;
        case EdgeType : result = QVariant::fromValue(m_vertices.at(index.row()));
            break;
        case IsOnPath : result = QVariant::fromValue(m_vertices.at(index.row())->isOnPath());
            break;
        case IsSelected : result = QVariant::fromValue(m_vertices.at(index.row())->isSelected());
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
    roles[LabelAlphType] = "labelAlph";
    roles[LabelNumType] = "labelNum";
    roles[EdgeType] = "edges";
    roles[IsOnPath] = "isOnPath";
    roles[IsSelected] = "isSelected";
    return roles;
}

// Inherited Write Methods
bool AdjacencyListQ::setData(const QModelIndex& index, const QVariant& value, int role) {
    bool result = false;
    switch (role) {
        case IsSelected : { m_vertices[index.row()]->setSelected(qvariant_cast<bool>(value)); result = true; }
        break;
    }
    return result;
}

auto AdjacencyListQ::flags(const QModelIndex& /*index*/) const
-> Qt::ItemFlags {
    return Qt::ItemIsSelectable | Qt::ItemIsEditable
            | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled
            | Qt::ItemIsEnabled;
}

// Inherited Resize Methods
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

// Public Slots
void AdjacencyListQ::append(QObject* o) {
    reset();
    int i = m_vertices.size();
    beginInsertRows(QModelIndex(), i, i);
    o->setParent(this);
    EdgeListQ* obj = qobject_cast<EdgeListQ*>(o);
    obj->setLabel(m_vertices.size());
    m_vertices.push_back(obj);
    endInsertRows();
}

QObject* AdjacencyListQ::remove(int label) {
    reset();
    labelA = -1; labelB = -1;
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
            reset();
            if (labelB != -1)
                labelB = -1;
            else
                labelA = -1;
        } else if (labelA == label || labelB == label) {
            reset();
            if (labelA == label) {
                labelA = labelB;
                labelB = -1;
            } else
                labelB = -1;
        } else {
            if (labelA == -1)
                labelA = label;
            else if (labelB == -1)
                labelB = label;
            else
                return;
        }

        runDijkstraHelper();
}

// Private Methods
// QQmlProperty Methods
void AdjacencyListQ::addVertex(QQmlListProperty<EdgeListQ>* list, EdgeListQ* item) {
    AdjacencyListQ* object = qobject_cast<AdjacencyListQ* >(list->object);
    object->m_vertices.push_back(item);
}

auto AdjacencyListQ::numVertices(QQmlListProperty<EdgeListQ>* list) -> int {
    AdjacencyListQ* object = qobject_cast<AdjacencyListQ* >(list->object);
    return object->m_vertices.size();
}

auto AdjacencyListQ::vertexAt(QQmlListProperty<EdgeListQ>* list, int index)
-> EdgeListQ* {
    AdjacencyListQ* object = qobject_cast<AdjacencyListQ* >(list->object);
    return object->m_vertices.at(index);
}

void AdjacencyListQ::clearVertices(QQmlListProperty<EdgeListQ> *list) {
    AdjacencyListQ* object = qobject_cast<AdjacencyListQ* >(list->object);
    object->m_vertices.clear();
}

void AdjacencyListQ::replaceVertex(QQmlListProperty<EdgeListQ> *list, int index,
                                   EdgeListQ *item) {
    AdjacencyListQ* object = qobject_cast<AdjacencyListQ* >(list->object);
    object->m_vertices[index] = item;
}

void AdjacencyListQ::truncateVertex(QQmlListProperty<EdgeListQ> *list) {
    AdjacencyListQ* object = qobject_cast<AdjacencyListQ* >(list->object);
    object->m_vertices.pop_back();
}

// Private Setter Methods
void AdjacencyListQ::clearPathHighlights() {
    for (auto& it : m_vertices) {
        it->setOnPath(false);
    }
    dataChanged(createIndex(0, 0), createIndex(m_vertices.size(), 0), {IsOnPath});
}

void AdjacencyListQ::runDijkstraHelper() {
    clearPathHighlights();
    if (labelA == -1 || labelB == -1)
        return;

    auto matrix = makeAdjacencyMatrix();
    auto result = matrix.dijkstra(labelA, labelB);

    if (result.empty() == true) {
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
}
