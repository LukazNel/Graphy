#include "../include/adjacencymatrixq.h"

unsigned int AdjacencyMatrixQ::at(int row, int column) const {
    return matrix.at(row, column);
}

bool AdjacencyMatrixQ::set(int row, int column, const unsigned int value) {
    return matrix.set(row, column, value);
}

void AdjacencyMatrixQ::appendQml(QQmlListProperty<unsigned int>* list, unsigned int* item) {
    AdjacencyMatrixQ* matrix = qobject_cast<AdjacencyMatrixQ* >(list->object);
    matrix->insertRows(-1, 1);
    matrix->set(0, -1, *item);
}

int AdjacencyMatrixQ::sizeQml(QQmlListProperty<unsigned int>* list) {
    AdjacencyMatrixQ* matrix = qobject_cast<AdjacencyMatrixQ* >(list->object);
    return matrix->rowCount();
}

unsigned int* AdjacencyMatrixQ::atQml(QQmlListProperty<unsigned int>* list, int index) {
    AdjacencyMatrixQ* matrix = qobject_cast<AdjacencyMatrixQ* >(list->object);
    return matrix->ptr(-1, index);
}

void AdjacencyMatrixQ::clearQml(QQmlListProperty<unsigned int> *list) {
    AdjacencyMatrixQ* matrix = qobject_cast<AdjacencyMatrixQ* >(list->object);
    matrix->removeRows(-1, 1);
}

void AdjacencyMatrixQ::replaceQml(QQmlListProperty<unsigned int> *list, int index, unsigned int *value) {
    AdjacencyMatrixQ* matrix = qobject_cast<AdjacencyMatrixQ* >(list->object);
    matrix->set(-1, index, *value);
}

void AdjacencyMatrixQ::truncateQml(QQmlListProperty<unsigned int> *list) {
    AdjacencyMatrixQ* matrix = qobject_cast<AdjacencyMatrixQ* >(list->object);
    matrix->removeRows(-1, 1);
}

AdjacencyMatrixQ::AdjacencyMatrixQ(QObject* parent) : QAbstractTableModel(parent) {}
AdjacencyMatrixQ::AdjacencyMatrixQ(AdjacencyMatrix copy, QObject* parent)
    : QAbstractTableModel(parent), matrix(copy) {}

QQmlListProperty<unsigned int> AdjacencyMatrixQ::makeEnlargeQml() {
    return QQmlListProperty<unsigned int>(this, this,
                    &AdjacencyMatrixQ::appendQml,
                    &AdjacencyMatrixQ::sizeQml,
                    &AdjacencyMatrixQ::atQml,
                    &AdjacencyMatrixQ::clearQml,
                    &AdjacencyMatrixQ::replaceQml,
                    &AdjacencyMatrixQ::truncateQml);
}

int AdjacencyMatrixQ::rowCount(const QModelIndex& /*parent*/) const {
    return matrix.size();
}

int AdjacencyMatrixQ::columnCount(const QModelIndex& /*parent*/) const {
    return matrix.size();
}

QVariant AdjacencyMatrixQ::headerData(int section, Qt::Orientation /*orientation*/, int role) const {
    QVariant result;
    switch (role) {
        case Qt::DisplayRole : result = QString(section);
            break;
        default : result = QVariant();
    }
    return result;
}

QVariant AdjacencyMatrixQ::data(const QModelIndex &index, int /*role*/) const {
    return QVariant(at(index.row(), index.column()));
}

bool AdjacencyMatrixQ::setData(const QModelIndex &index, const QVariant &value, int role) {
    bool result = false;
    switch (role) {
        case Qt::DisplayRole : {
            result = set(index.row(), index.column(),
                                     qvariant_cast<unsigned int>(value));
            if (result == true)
                dataChanged(index, index);
        }
            break;
        default : return false;
    }
    return result;
}

Qt::ItemFlags AdjacencyMatrixQ::flags(const QModelIndex &index) const {
    if (index.row() == index.column()) {
        return Qt::NoItemFlags;
    } else {
        return Qt::ItemIsSelectable | Qt::ItemIsEditable
                | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled
                | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
    }
}

bool AdjacencyMatrixQ::insertRows(int before_row, int count, const QModelIndex &parent) {
    if(before_row != (int)matrix.size()) {
        qWarning("Rows and columns can only be appended in AdjacencyMatrixQ.");
        return false;
    }

    beginInsertRows(parent, matrix.size(), matrix.size() + count - 1);
    beginInsertColumns(parent, matrix.size(), matrix.size() + count - 1);
    bool result = matrix.enlarge(count);
    endInsertColumns();
    endInsertRows();
    return result;
}

bool AdjacencyMatrixQ::removeRows(int from_row, int count, const QModelIndex &parent) {
    if (from_row == -1)
        from_row = matrix.size();

    beginRemoveColumns(parent, from_row, from_row + count - 1);
    beginRemoveRows(parent, from_row, from_row + count - 1);
    bool result = matrix.erase(count, from_row);
    endInsertRows();
    endInsertColumns();
    return result;
}

bool AdjacencyMatrixQ::insertColumns(int before_column, int count, const QModelIndex &parent) {
    if(before_column != (int)matrix.size()) {
        qWarning("Rows and columns can only be appended in AdjacencyMatrixQ.");
        return false;
    }

    beginInsertColumns(parent, matrix.size(), matrix.size() + count - 1);
    beginInsertRows(parent, matrix.size(), matrix.size() + count - 1);
    bool result = matrix.enlarge(count);
    endInsertRows();
    endInsertColumns();
    return result;
}

bool AdjacencyMatrixQ::removeColumns(int from_column, int count, const QModelIndex &parent) {
    if (from_column == -1)
        from_column = matrix.size();

    beginRemoveColumns(parent, from_column, from_column + count - 1);
    beginRemoveRows(parent, from_column, from_column + count - 1);
    bool result = matrix.erase(count, from_column);
    endInsertRows();
    endInsertColumns();
    return result;
}
