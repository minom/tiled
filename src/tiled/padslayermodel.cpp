#include "padslayermodel.h"

#include "objectgroup.h"
#include "mapobject.h"
using namespace Tiled;
using namespace Tiled::Internal;


PadsLayerModel::PadsLayerModel(QObject *parent) :
    QAbstractListModel(parent),
    specialMapObjects(0)
{
}

void PadsLayerModel::setPadsLayer(ObjectGroup *padsLayer){
    if (mPadsLayer == padsLayer){
        return;
    }
    mPadsLayer = padsLayer;

    this->reloadData();
}


void PadsLayerModel::reloadData(){
    if (specialMapObjects){
        delete specialMapObjects;
        specialMapObjects = 0;
    }

    if (mPadsLayer){
        specialMapObjects = new QList<MapObject*>();
        foreach (MapObject *candidate, mPadsLayer->objects()){
            if (candidate->name() != tr("") || candidate->properties().count() > 0 || candidate->type() != tr("")){
                specialMapObjects->push_back( candidate );
            }
        }
    }
}

int PadsLayerModel::rowCount(const QModelIndex &parent) const{
    if (!mPadsLayer){
        return 0;
    }

    if (specialMapObjects){
        return specialMapObjects->count();
    }

    return 0;
}


QVariant PadsLayerModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return tr("Pads Object");
        }
    }
    return QVariant();
}


QVariant PadsLayerModel::data(const QModelIndex &index, int role) const
{
    const MapObject *item = specialMapObjects->at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return item->name() + tr("|") + item->type() + tr("|") +  QString::number(item->properties().count()) + tr(" Properties");
    case Qt::DecorationRole:
        return QVariant();
    default:
        return QVariant();
    }
}

QList<MapObject*>* PadsLayerModel::getSpecialMapObjects(){
    return specialMapObjects;
}
