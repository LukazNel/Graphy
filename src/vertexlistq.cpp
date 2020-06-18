#include "../include/vertexlistq.h"

// Public Methods
// Getter Methods
auto EdgeListQ::makeEdgesProperty()
-> QQmlListProperty<EdgeQ> {
    return QQmlListProperty<EdgeQ>(this, this,
                                     &EdgeListQ::addEdge,
                                     &EdgeListQ::numEdges,
                                     &EdgeListQ::edgeAt,
                                     &EdgeListQ::clearEdges,
                                     &EdgeListQ::replaceEdge,
                                     &EdgeListQ::truncateEdge);
}

auto EdgeListQ::getBareExpandedList() const
-> std::vector<int> {
    if (m_edges.empty() == true)
        return {};

    std::vector<Edge> copy;

    for (const auto* it : m_edges) {
        copy.push_back(it->getVertex());
    }

    std::sort(copy.begin(), copy.end(),
              [](const Edge& a, const Edge& b) -> bool { return a.to_vertex < b.to_vertex; });

    int max_size = copy.back().to_vertex + 1;
    int i = 0;
    while (i < max_size) {
        int current_label = copy[i].to_vertex;
        if (current_label != i) {
            int diff = current_label - i;
            copy.insert(copy.begin() + i, diff, {i, 0});
            i += diff;
        } else {
            i++;
        }
    }

    std::vector<int> final;
    for (const auto i : copy) {
        final.push_back(i.weight);
    }

    return final;
}

// Setter Methods
void EdgeListQ::updateVertices(int rm_vert_label) {
    remove(rm_vert_label);

    int i = 0;
    for (auto it : m_edges) {
        int label = it->toVertex();
        if (label > rm_vert_label) {
            it->setLabel(--label);
            QModelIndex index = createIndex(i++, 0);
            dataChanged(index, index);
        }
    }
}

// Inherited Read Methods
int EdgeListQ::rowCount(const QModelIndex& /*parent*/) const {
    return m_edges.size();
}

auto EdgeListQ::data(const QModelIndex& index, int role) const
-> QVariant {
    QVariant result;
    switch (role) {
    case LabelNumType : return m_edges.at(index.row())->toVertex();
        break;
    case LabelAlphType : return QString('A' + m_edges.at(index.row())->toVertex());
        break;
    case WeightType : return m_edges.at(index.row())->weight();
        break;
    default : result = QVariant();
    }
    return result;
}

auto EdgeListQ::headerData(int section, Qt::Orientation orientation, int role) const
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

auto EdgeListQ::roleNames() const
-> QHash<int, QByteArray> {
    QHash<int, QByteArray> roles;
    roles[LabelNumType] = "labelNum";
    roles[LabelAlphType] = "labelAlph";
    roles[WeightType] = "weight";
    return roles;
}

// Inherited Write Methods
bool EdgeListQ::setData(const QModelIndex &index, const QVariant &value, int role) {
    bool result = false;
    switch (role) {
    case WeightType : {
        m_edges[index.row()]->setWeight(qvariant_cast<int>(value));
        dataChanged(index, index);
        result = true;
    }
        break;
    case Qt::DisplayRole : result = false;
        break;
    default: ;
    }
    return result;
}

auto EdgeListQ::flags(const QModelIndex& /*index*/) const
-> Qt::ItemFlags {
    return Qt::ItemIsSelectable | Qt::ItemIsEditable
            | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled
            | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}

// Inherited Resize Methods
bool EdgeListQ::insertRows(int before_row, int count, const QModelIndex& parent) {
    beginInsertRows(parent, before_row + 1, before_row + count);
    m_edges.insert(m_edges.begin() + before_row, count, {});
    endInsertRows();
    return true;
}

bool EdgeListQ::removeRows(int from_row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, from_row, from_row + count - 1);
    m_edges.erase(m_edges.begin() + from_row, m_edges.begin() + from_row + count - 1);
    endRemoveRows();
    return true;
}

// Public Slots
void EdgeListQ::append(QObject* object, int label, int weight) {
    if (label == vertex_label)
        return;

    auto duplicate = std::find_if(m_edges.begin(), m_edges.end(),
                                  [label](EdgeQ* a) -> bool { return a->toVertex() == label; });

    if (duplicate == m_edges.end()) {
        int i = m_edges.size();
        beginInsertRows(QModelIndex(), i, i);
        object->setParent(this);
        EdgeQ* obj = qobject_cast<EdgeQ*>(object);
        obj->setLabel(label);
        obj->setWeight(weight);
        m_edges.push_back(obj);
        endInsertRows();
    }
}

QObject* EdgeListQ::remove(int label) {
    auto it = std::find_if(m_edges.begin(), m_edges.end(),
                           [label](const EdgeQ* vert) -> bool {
            return vert->toVertex() == label;
            });

    QObject* removed = nullptr;
    if (it != m_edges.end()) {
        int dist = std::distance(m_edges.begin(), it);
        beginRemoveRows(QModelIndex(), dist, dist);
        removed = (QObject*)*it;
        removed->setParent(nullptr);
        m_edges.erase(it);
        endRemoveRows();
    }

    return removed;
}

// Private Methods
// QQmlListProperty Methods
void EdgeListQ::addEdge(QQmlListProperty<EdgeQ> *list, EdgeQ *item) {
    EdgeListQ* object = qobject_cast<EdgeListQ* >(list->object);
    object->m_edges.push_back(item);
}

auto EdgeListQ::numEdges(QQmlListProperty<EdgeQ>* list) -> int {
    EdgeListQ* object = qobject_cast<EdgeListQ* >(list->object);
    return object->m_edges.size();
}

auto EdgeListQ::edgeAt(QQmlListProperty<EdgeQ>* list, int index)
-> EdgeQ* {
    EdgeListQ* object = qobject_cast<EdgeListQ* >(list->object);
    return object->m_edges.at(index);
}

void EdgeListQ::clearEdges(QQmlListProperty<EdgeQ> *list) {
    EdgeListQ* object = qobject_cast<EdgeListQ* >(list->object);
    object->m_edges.clear();
}

void EdgeListQ::replaceEdge(QQmlListProperty<EdgeQ> *list, int index, EdgeQ *item) {
    EdgeListQ* object = qobject_cast<EdgeListQ* >(list->object);
    object->m_edges[index] = item;
}

void EdgeListQ::truncateEdge(QQmlListProperty<EdgeQ> *list) {
    EdgeListQ* object = qobject_cast<EdgeListQ* >(list->object);
    object->m_edges.pop_back();
}
