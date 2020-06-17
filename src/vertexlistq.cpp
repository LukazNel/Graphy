#include "../include/vertexlistq.h"

// Constructors
VertexListQ::VertexListQ(QObject* parent) : QAbstractListModel(parent), is_on_path(false) {}

// Public Methods
auto VertexListQ::getLabel() const -> int {
    return m_label;
}

auto VertexListQ::getEdges()
-> QQmlListProperty<VertexQ> {
    return QQmlListProperty<VertexQ>(this, this,
                                     &VertexListQ::addEdge,
                                     &VertexListQ::numEdges,
                                     &VertexListQ::edgeAt,
                                     &VertexListQ::clearEdges,
                                     &VertexListQ::replaceEdge,
                                     &VertexListQ::truncateEdge);
}

auto VertexListQ::getVertex(int index) const
-> VertexQ* {
    return m_edges.at(index);
}

void VertexListQ::setVertex(int index, VertexQ* item) {
    m_edges[index] = item;
}

void VertexListQ::updateVertices(int rm_vert_label) {
    remove(rm_vert_label);

    int i = 0;
    for (auto it : m_edges) {
        int label = it->label();
        if (label > rm_vert_label) {
            it->setLabel(--label);
            QModelIndex index = createIndex(i++, 0);
            dataChanged(index, index);
        }
    }
}

void VertexListQ::setLabel(const int label) {
    m_label = label;
}


int VertexListQ::rowCount(const QModelIndex& /*parent*/) const {
    return m_edges.size();
}


auto VertexListQ::data(const QModelIndex& index, int role) const
-> QVariant {
    QVariant result;
    switch (role) {
    case LabelNumType : return m_edges.at(index.row())->label();
        break;
    case LabelAlphType : return QString('A' + m_edges.at(index.row())->label());
        break;
    case WeightType : return m_edges.at(index.row())->weight();
        break;
    default : result = QVariant();
    }
    return result;
}

auto VertexListQ::headerData(int section, Qt::Orientation orientation, int role) const
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

auto VertexListQ::roleNames() const
-> QHash<int, QByteArray> {
    QHash<int, QByteArray> roles;
    roles[LabelNumType] = "labelNum";
    roles[LabelAlphType] = "labelAlph";
    roles[WeightType] = "weight";
    return roles;
}

auto VertexListQ::getBareExpandedList() const
-> std::vector<int> {
    if (m_edges.empty() == true)
        return {};

    std::vector<Vertex> copy;

    for (const auto* it : m_edges) {
        copy.push_back(it->getVertex());
    }

    std::sort(copy.begin(), copy.end(),
              [](const Vertex& a, const Vertex& b) -> bool { return a.label < b.label; });

    int max_size = copy.back().label + 1;
    int i = 0;
    while (i < max_size) {
        int current_label = copy[i].label;
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

bool VertexListQ::setData(const QModelIndex &index, const QVariant &value, int role) {
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

auto VertexListQ::flags(const QModelIndex& /*index*/) const
-> Qt::ItemFlags {
    return Qt::ItemIsSelectable | Qt::ItemIsEditable
            | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled
            | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}

bool VertexListQ::insertRows(int before_row, int count, const QModelIndex& parent) {
    beginInsertRows(parent, before_row + 1, before_row + count);
    m_edges.insert(m_edges.begin() + before_row, count, {});
    endInsertRows();
    return true;
}

bool VertexListQ::removeRows(int from_row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, from_row, from_row + count - 1);
    m_edges.erase(m_edges.begin() + from_row, m_edges.begin() + from_row + count - 1);
    endRemoveRows();
    return true;
}

void VertexListQ::append(QObject* object, int label, int weight) {
    if (label == m_label)
        return;

    auto duplicate = std::find_if(m_edges.begin(), m_edges.end(),
                                  [label](VertexQ* a) -> bool { return a->label() == label; });

    if (duplicate == m_edges.end()) {
        int i = m_edges.size();
        beginInsertRows(QModelIndex(), i, i);
        object->setParent(this);
        VertexQ* obj = qobject_cast<VertexQ*>(object);
        obj->setLabel(label);
        obj->setWeight(weight);
        m_edges.push_back(obj);
        endInsertRows();
    }
}

QObject* VertexListQ::remove(int label) {
    auto it = std::find_if(m_edges.begin(), m_edges.end(),
                           [label](const VertexQ* vert) -> bool {
            return vert->label() == label;
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
void VertexListQ::addEdge(QQmlListProperty<VertexQ> *list, VertexQ *item) {
    VertexListQ* object = qobject_cast<VertexListQ* >(list->object);
    object->m_edges.push_back(item);
}

auto VertexListQ::numEdges(QQmlListProperty<VertexQ>* list) -> int {
    VertexListQ* object = qobject_cast<VertexListQ* >(list->object);
    return object->m_edges.size();
}

auto VertexListQ::edgeAt(QQmlListProperty<VertexQ>* list, int index)
-> VertexQ* {
    VertexListQ* object = qobject_cast<VertexListQ* >(list->object);
    return object->m_edges.at(index);
}

void VertexListQ::clearEdges(QQmlListProperty<VertexQ> *list) {
    VertexListQ* object = qobject_cast<VertexListQ* >(list->object);
    object->m_edges.clear();
}

void VertexListQ::replaceEdge(QQmlListProperty<VertexQ> *list, int index, VertexQ *item) {
    VertexListQ* object = qobject_cast<VertexListQ* >(list->object);
    object->m_edges[index] = item;
}

void VertexListQ::truncateEdge(QQmlListProperty<VertexQ> *list) {
    VertexListQ* object = qobject_cast<VertexListQ* >(list->object);
    object->m_edges.pop_back();
}
