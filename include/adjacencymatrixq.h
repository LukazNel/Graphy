#ifndef ADJACENCYMATRIXQ_H
#define ADJACENCYMATRIXQ_H

#include <QAbstractTableModel>
#include <QString>
#include <qqml.h>
#include "../include/adjacencymatrix.h"

#include <string>

class AdjacencyMatrixQ
{
    //Q_OBJECT
    //Q_PROPERTY(QQmlListProperty<unsigned int> roows READ makeEnlargeQml)
    //QML_ELEMENT

private:
    AdjacencyMatrix matrix;

    unsigned int* ptr(int row, int column);
    unsigned int at(int row, int column) const;
    bool set(int row, int column, const unsigned int value);

    static auto appendQml(QQmlListProperty<unsigned int>* list, unsigned int* vertex)
        -> void;
    static auto sizeQml(QQmlListProperty<unsigned int>* list)
        -> int;
    static auto atQml(QQmlListProperty<unsigned int>* list, int index)
        -> unsigned int*;
    static auto clearQml(QQmlListProperty<unsigned int>* list)
        -> void;
    static auto replaceQml(QQmlListProperty<unsigned int>* list, int index, unsigned int* value)
        -> void;
    static auto truncateQml(QQmlListProperty<unsigned int>* list)
        -> void;

public:
    AdjacencyMatrixQ(QObject* parent = nullptr);
    AdjacencyMatrixQ(AdjacencyMatrix copy, QObject* parent = nullptr);

    auto makeEnlargeQml() -> QQmlListProperty<unsigned int>;

    // Read Methods
    auto rowCount(const QModelIndex& parent = QModelIndex()) const
        -> int override;
    auto columnCount(const QModelIndex& parent = QModelIndex()) const
        -> int override;
    auto data(const QModelIndex& index, int role = Qt::DisplayRole) const
        -> QVariant override;
    auto headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
        -> QVariant override;

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
    auto insertColumns(int before_column, int count, const QModelIndex& parent = QModelIndex())
        -> bool override;
    auto removeColumns(int from_column, int count, const QModelIndex& parent = QModelIndex())
        -> bool override;

    ~AdjacencyMatrixQ() {};
};

#endif // ADJACENCYMATRIXQ_H
