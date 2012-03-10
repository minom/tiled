#ifndef PADSLAYERMODEL_H
#define PADSLAYERMODEL_H

#include <QAbstractListModel>

namespace Tiled{
class ObjectGroup;
class MapObject;

namespace Internal{
class PadsLayerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PadsLayerModel(QObject *parent = 0);
    void setPadsLayer(ObjectGroup *layer);

    /**
     * Returns the number of rows.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * Returns the data stored under the given <i>role</i> for the item
     * referred to by the <i>index</i>.
     */
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                                    int role) const;

    QList<MapObject*>* getSpecialMapObjects();
signals:
    
public slots:
private:
    ObjectGroup *mPadsLayer;
    QList<MapObject*>* specialMapObjects;
};


}// Internal
}// Tiled
#endif // PADSLAYERMODEL_H
