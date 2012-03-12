#include "padswithpropertiesdock.h"

#include "layer.h"
#include "padslayermodel.h"
#include "map.h"
#include "mapobject.h"
#include "mapdocumentactionhandler.h"
#include "propertiesdialog.h"
#include "objectpropertiesdialog.h"
#include "objectgroup.h"
#include "mainwindow.h"
#include "documentmanager.h"

#include <QBoxLayout>
#include <QApplication>
#include <QContextMenuEvent>
#include <QLabel>
#include <QMenu>
#include <QSlider>
#include <QUndoStack>
#include <QToolBar>

using namespace Tiled;
using namespace Tiled::Internal;

PadsWithPropertiesDock::PadsWithPropertiesDock(QWidget *parent) :
    QDockWidget(parent)
  , mObjectsView(new PadObjectsView)
{
    setObjectName(QLatin1String("objectsWithPropertiesDock"));
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setMargin(5);
    setWidget(widget);

    layout->addWidget(mObjectsView);
    setWindowTitle(tr("Pad Properties"));
}



void PadsWithPropertiesDock::setMapDocument(MapDocument *mapDocument)
{
    if (mMapDocument == mapDocument)
        return;

//    if (mMapDocument)
//        mMapDocument->disconnect(this);

    mMapDocument = mapDocument;
//    if (mMapDocument) {
//        connect(mMapDocument, SIGNAL(currentLayerIndexChanged(int)),
//                this, SLOT(updateOpacitySlider()));
//    }

    mObjectsView->setMapDocument(mapDocument);
}

void PadsWithPropertiesDock::mapObjectSelected(MapObject *mapObj){
    DocumentManager *currentDocument = DocumentManager::instance();
    MapView *currentMapView = currentDocument->currentMapView();
    MapScene *currentMapScene = currentDocument->currentMapScene();

    MapObjectItem *selectedObjectItem = currentMapScene->itemForObject(mapObj);
    QList<MapObjectItem*> newSelectedObjectItems = QList<MapObjectItem*>();
    newSelectedObjectItems.push_back(selectedObjectItem);
    QPointF mapObjPos = currentMapView->mapToScene( mapObj->position().toPoint());


    currentDocument->currentMapView()->centerOn((QGraphicsItem*)selectedObjectItem);
    currentDocument->currentMapScene()->setSelectedObjectItems(newSelectedObjectItems.toSet());
}


PadObjectsView::PadObjectsView(QWidget *parent):
    QTreeView(parent),
    mMapDocument(0),
    mPadsLayerModel(0)
{
    setRootIsDecorated(false);
    setHeaderHidden(true);
    setItemsExpandable(false);
    setUniformRowHeights(true);
}

QSize PadObjectsView::sizeHint() const
{
    return QSize(130, 100);
}

void PadObjectsView::setMapDocument(MapDocument *mapDocument)
{
    if (mMapDocument) {
        mMapDocument->disconnect(this);
        QItemSelectionModel *s = selectionModel();
        disconnect(s, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                   this, SLOT(currentRowChanged(QModelIndex)));
    }

    mMapDocument = mapDocument;

    if (mMapDocument) {
        ObjectGroup *padsLayer = mMapDocument->padsLayer();

        if (mPadsLayerModel){
            delete mPadsLayerModel;
            mPadsLayerModel = 0;
        }

        if (padsLayer){
            mPadsLayerModel = new PadsLayerModel();
            mPadsLayerModel->setPadsLayer(padsLayer);
            setModel(mPadsLayerModel);

            QItemSelectionModel *s = selectionModel();
            connect(s, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                    this, SLOT(currentRowChanged(QModelIndex)));
            connect(mMapDocument, SIGNAL(objectsAdded(QList<MapObject*>)), this, SLOT(layerObjectsChanged()));
            connect(mMapDocument, SIGNAL(objectsChanged(QList<MapObject*>)), this, SLOT(layerObjectsChanged()));
            connect(mMapDocument, SIGNAL(objectsRemoved(QList<MapObject*>)), this, SLOT(layerObjectsChanged()));
        }else {
            setModel(0);
        }
    }
    else {
        setModel(0);
    }
}


void PadObjectsView::layerObjectsChanged(){
    mPadsLayerModel->reloadData();
    this->reset();
}

void PadObjectsView::currentRowChanged(const QModelIndex &index)
{
    MapObject *selectedMapObject = mPadsLayerModel->getSpecialMapObjects()->at(index.row());
    PadsWithPropertiesDock* dockParent =  (PadsWithPropertiesDock*)parent();
    dockParent->mapObjectSelected(selectedMapObject);
}

void PadObjectsView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!mMapDocument)
        return;

    const QModelIndex index = indexAt(event->pos());
    int row = index.row();

    QAction *propertiesMenuAction = new QAction(tr("Properties"), this);

    propertiesMenuAction->setData(QVariant::fromValue(row));

    QMenu menu;
    menu.addAction(propertiesMenuAction);

    connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(showProperties(QAction*)));

    menu.exec(event->globalPos());
}

void PadObjectsView::showProperties(QAction*action){
    int row = action->data().toInt();
    MapObject *selectedMapObject = mPadsLayerModel->getSpecialMapObjects()->at(row);
    ObjectPropertiesDialog propertiesDialog(mMapDocument, selectedMapObject, this);
    propertiesDialog.exec();
}

//void PadObjectsView::keyPressEvent(QKeyEvent *event)
//{
//    if (!mMapDocument)
//        return;

//    const QModelIndex index = currentIndex();
//    if (!index.isValid())
//        return;

//    const LayerModel *m = mMapDocument->layerModel();
//    const int layerIndex = m->toLayerIndex(index);

//    if (event->key() == Qt::Key_Delete) {
//        mMapDocument->removeLayer(layerIndex);
//        return;
//    }

//    QTreeView::keyPressEvent(event);
//}
