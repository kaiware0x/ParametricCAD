#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QString>

#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <XCAFApp_Application.hxx>
#include "AIS_Shape.hxx"
#include "TDataStd_TreeNode.hxx"
#include "TPrsStd_AISViewer.hxx"

#include "Tree/treeitem.h"

class ViewerWidget;
class TreeModel;

/**
 * @brief Ensembleの OCAFDoc に相当
 */
class Document : public QObject {
public:
    Document(QObject* parent);

    Handle(AIS_InteractiveContext) context() const { return m_context; }
    Handle(TDocStd_Document) ocafDocument() const { return m_std_doc; }
    ViewerWidget* viewerWidget() const { return m_viewer_widget; }
    TreeModel*    treeModel() const { return m_treemodel; }
    Handle(TDataStd_TreeNode) rootNode() const;

    void deleteAISObjects(const QList<Handle(AIS_InteractiveObject)>& objects);
    void showWireFrameShape(AIS_Shape* shape);
    void showShadedShape(AIS_Shape* shape);
    void showShadedShape(Handle(TDataStd_TreeNode) treenode);
    void hideShape(Handle(TDataStd_TreeNode) treenode);
    void hideShape(AIS_Shape* shape);

    /// for debug
    void save() const;

public Q_SLOTS:
    void changeVisibility(TreeItem* tritem);

private:
    bool newDocument(const QString& filename);

private:
    Handle(XCAFApp_Application) m_ocaf_app;
    Handle(TDocStd_Document) m_std_doc;
    Handle(AIS_InteractiveContext) m_context;
    Handle(TPrsStd_AISViewer) m_viewer_attr;

    ViewerWidget* m_viewer_widget;

    TreeModel* m_treemodel;
};

#endif    // DOCUMENT_H
