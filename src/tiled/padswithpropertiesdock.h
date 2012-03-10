#ifndef PADSWITHPROPERTIESDOCK_H
#define PADSWITHPROPERTIESDOCK_H

#include <QDockWidget>
#include <QTreeView>
#include <QToolButton>

#include "mapdocument.h"
class QLabel;
class QModelIndex;
class QUndoStack;

namespace Tiled {
namespace Internal {


class PadObjectsView;
class PadsLayerModel;

class PadsWithPropertiesDock : public QDockWidget
{
    Q_OBJECT
public:
    PadsWithPropertiesDock(QWidget *parent = 0);
    void setMapDocument(MapDocument *mapDocument);
signals:
    
public slots:
    
private:
    MapDocument *mMapDocument;
    PadObjectsView *mObjectsView;
};


/**
 * This view makes sure the size hint makes sense and implements the context
 * menu.
 */
class PadObjectsView : public QTreeView
{
    Q_OBJECT

public:
    PadObjectsView(QWidget *parent = 0);

    QSize sizeHint() const;
    void setMapDocument(MapDocument *mapDocument);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
//    void keyPressEvent(QKeyEvent *event);

private slots:
    void currentRowChanged(const QModelIndex &index);
    void showProperties(QAction* action);
//    void currentLayerIndexChanged(int index);

//    void editLayerName();

private:
    MapDocument *mMapDocument;
    PadsLayerModel *mPadsLayerModel;
};


} //namespace Internal
} //namespace Tiled

#endif // PADSWITHPROPERTIESDOCK_H



