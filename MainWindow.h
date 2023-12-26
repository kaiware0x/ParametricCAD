#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLayout>
#include <QMainWindow>
#include <QUndoStack>
#include <QUndoView>

#include <TDF_Attribute.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TDocStd_Document.hxx>
#include <TopoDS_Shape.hxx>

#include "Dialog/parameter_dlg.h"
#include "values.h"

class Document;
class TreeView;
class TreeModel;
class ViewerWidget;
class BoxDlg;
class PythonConsole;

class MainWindow : public QMainWindow {
    Q_OBJECT
    explicit MainWindow(QWidget* parent = nullptr);

public:
    static MainWindow* instance();

    Document* document() const;

    void showBoxDlg(const Handle(TDataStd_TreeNode) & box_func_node = nullptr);
    void showParameterDlg(const Handle(TDataStd_TreeNode) & parameter_node = nullptr);

public Q_SLOTS:
    void makeBox(const BoxValue& val);
    void editBox(const Handle(TDataStd_TreeNode) & box_tree_node, const BoxValue& val);

    void addParam(const Parameter& val);
    void editParam(const Handle(TDataStd_TreeNode) & param_tree_node, const Parameter& param);

private:
    void setupWindow();
    void setupMenuBar();

private Q_SLOTS:
    void showUndoView();

private:
    Document*    m_document;
    TreeView*    m_treeview;
    QDockWidget* m_tree_dock_widget;
    QVBoxLayout* m_layout_for_tree;
    QUndoStack*  m_undostack;
    QUndoView*   m_undoview;

    BoxDlg*       m_box_dlg;
    ParameterDlg* m_param_dlg;

    QDockWidget*   m_py_console_dock{nullptr};
    PythonConsole* m_py_console{nullptr};
};

#endif    // MAINWINDOW_H
