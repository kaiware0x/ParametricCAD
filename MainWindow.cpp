
#include <QDockWidget>
#include <QMenuBar>
#include <QPointer>

#include <AIS_DisplayMode.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <TFunction_DriverTable.hxx>
#include <TPrsStd_DriverTable.hxx>
#include <gp_Ax1.hxx>

#include "Command/edit_box_cmd.h"
#include "Command/edit_parameter_cmd.h"
#include "Command/make_box_cmd.h"
#include "Command/make_parameter_cmd.h"
#include "Defines.h"
#include "FromSample/OCAFSample_AISDriver.hxx"
#include "FromSample/OCAFSample_BoxDriver.hxx"
#include "FromSample/OCAFSample_IBox.hxx"
#include "Graphics/ViewerDraggableObject.h"
#include "Graphics/ViewerWidget.h"
#include "PythonConsole/PythonConsole.h"
#include "dialog/box_dlg.h"
#include "document.h"
#include "mainwindow.h"
#include "tree/treemodel.h"
#include "tree/treeview.h"

/**
 * @brief createCylinder
 * @param location
 * @param direction
 * @param radius
 * @param height
 * @return
 */
TopoDS_Shape createCylinder(const gp_Pnt& location, const gp_Dir& direction, const Standard_Real radius,
                            const Standard_Real height)
{
    return BRepPrimAPI_MakeCylinder(gp_Ax2(location, direction), radius, height);
}

void visualizePresentations(const Handle(AIS_InteractiveContext) & context)
{
    gp_Pnt        location(40., 100., 0.);
    gp_Dir        direction = gp::OZ().Direction();
    Standard_Real radius    = 10.;
    Standard_Real height    = 50.;

    // display near cylinder
    {
        TopoDS_Shape shapeTopo  = createCylinder(location, direction, radius, height);
        Handle(AIS_Shape) shape = new AIS_Shape(shapeTopo);
        shape->SetMaterial(Graphic3d_NameOfMaterial::Graphic3d_NameOfMaterial_Pewter);
        context->Display(shape, AIS_WireFrame, AIS_Shape::SelectionMode(TopAbs_SHAPE), Standard_True);
    }

    // display far cylinder
    {
        location.Translate(gp_Vec(60., 0., 0.));
        TopoDS_Shape shapeTopo = createCylinder(location, direction, radius, height);
        //        Handle(AIS_Shape) shape = new AIS_Shape(shapeTopo);
        Handle(ViewerDraggableObject) shape = new ViewerDraggableObject(shapeTopo);
        shape->SetMaterial(Graphic3d_NameOfMaterial::Graphic3d_NameOfMaterial_Pewter);
        context->Display(shape, AIS_Shaded, AIS_Shape::SelectionMode(TopAbs_SHAPE), Standard_True);
    }
}

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_document(new Document(this))
    , m_treeview(new TreeView(this))
    , m_tree_dock_widget(new QDockWidget(this))
    , m_layout_for_tree(new QVBoxLayout(m_tree_dock_widget))
    , m_undostack(new QUndoStack(this))
    , m_undoview(new QUndoView(m_undostack))
    , m_box_dlg(new BoxDlg(m_document, this))
    , m_param_dlg(new ParameterDlg(m_document, this))
    , m_py_console_dock(new QDockWidget("Python Console", this))
    , m_py_console(new PythonConsole(this))
{
    setupWindow();
    setupMenuBar();

    connect(m_box_dlg, &BoxDlg::makeBoxSignal, this, &MainWindow::makeBox);
    connect(m_box_dlg, &BoxDlg::editBoxSignal, this, &MainWindow::editBox);
    connect(m_param_dlg, &ParameterDlg::addParamSignal, this, &MainWindow::addParam);
    connect(m_param_dlg, &ParameterDlg::editParamSignal, this, &MainWindow::editParam);

    /// viewerWidgetのUpdate前にshowしておかないと,
    /// MainWindowのサイズに対してViewerのサイズが小さくなってしまう.
    show();

    m_document->viewerWidget()->interactor()->fitAll();
    m_document->viewerWidget()->interactor()->zoom(0.75);
    m_document->viewerWidget()->myUpdate();

    m_treeview->setModel(m_document->treeModel());
    m_treeview->expandAll();

    ///---------------------------------------------------------------------------------------------------
    /// Initialize Driver
    ///---------------------------------------------------------------------------------------------------
    TFunction_DriverTable::Get()->AddDriver(OCAFSample_IBox::GetID(), new OCAFSample_BoxDriver());
    //    TFunction_DriverTable::Get()->AddDriver(OCAFSample_ICylinder::GetID(), new OCAFSample_CylinderDriver());
    //    TFunction_DriverTable::Get()->AddDriver(OCAFSample_ISphere::GetID(), new OCAFSample_SphereDriver());
    //    TFunction_DriverTable::Get()->AddDriver(OCAFSample_IPrism::GetID(), new OCAFSample_PrismDriver());
    //    TFunction_DriverTable::Get()->AddDriver(OCAFSample_IBRepImport::GetID(), new OCAFSample_BRepImportDriver());
    //    TFunction_DriverTable::Get()->AddDriver(OCAFSample_ICut::GetID(), new OCAFSample_CutDriver());
    //    TFunction_DriverTable::Get()->AddDriver(OCAFSample_IFuse::GetID(), new OCAFSample_FuseDriver());
    //    TFunction_DriverTable::Get()->AddDriver(OCAFSample_ISelection::GetID(), new OCAFSample_SelectionDriver());
    //    TFunction_DriverTable::Get()->AddDriver(OCAFSample_IFillet::GetID(), new OCAFSample_FilletDriver());
    //    TFunction_DriverTable::Get()->AddDriver(OCAFSample_ITransform::GetID(), new OCAFSample_TransformDriver());
    //    TFunction_DriverTable::Get()->AddDriver(OCAFSample_ICommon::GetID(), new OCAFSample_CommonDriver());
    //    TFunction_DriverTable::Get()->AddDriver(OCAFSample_IPipe::GetID(), new OCAFSample_PipeDriver());
    //    TFunction_DriverTable::Get()->AddDriver(OCAFSample_IPipeShell::GetID(), new OCAFSample_PipeShellDriver());

    TPrsStd_DriverTable::Get()->AddDriver(OCAFSample_AISDriver::GetID(), new OCAFSample_AISDriver());
}

/**
 * @brief MainWindow::instance
 * @return
 */
MainWindow* MainWindow::instance()
{
    static MainWindow* mainwindow = nullptr;
    if (!mainwindow) {
        mainwindow = new MainWindow;
    }
    return mainwindow;
}

/**
 * @brief MainWindow::document
 * @return
 */
Document* MainWindow::document() const
{
    return m_document;
}

/**
 * @brief MainWindow::onMakeBoxTriggered
 */
void MainWindow::showBoxDlg(const Handle(TDataStd_TreeNode) & box_func_node)
{
    if (!box_func_node.IsNull()) {
        m_box_dlg->setBoxFunctionTreeNode(box_func_node);
    }
    m_box_dlg->show();
}

void MainWindow::showParameterDlg(const Handle(TDataStd_TreeNode) & parameter_node)
{
    if (!parameter_node.IsNull()) {
        m_param_dlg->setParameterTreeNode(parameter_node);
    }
    m_param_dlg->show();
}

void MainWindow::makeBox(const BoxValue& val)
{
    auto cmd = new MakeBoxCmd(val, m_document);
    if (cmd->canContinue()) {
        m_undostack->push(cmd);
        return;
    }
    delete cmd;
}

void MainWindow::editBox(const Handle(TDataStd_TreeNode) & box_tree_node, const BoxValue& val)
{
    auto cmd = new EditBoxCmd(box_tree_node, val, m_document);
    if (cmd->canContinue()) {
        m_undostack->push(cmd);
        return;
    }
    delete cmd;
}

void MainWindow::addParam(const Parameter& param)
{
    auto cmd = new MakeParameterCmd(param, m_document);
    if (cmd->canContinue()) {
        m_undostack->push(cmd);
        return;
    }
    delete cmd;
}

void MainWindow::editParam(const Handle(TDataStd_TreeNode) & param_tree_node, const Parameter& param)
{
    auto cmd = new EditParameterCmd(param_tree_node, param, m_document);
    if (cmd->canContinue()) {
        m_undostack->push(cmd);
        return;
    }
    delete cmd;
}

/**
 * @brief MainWindow::setupWindow
 */
void MainWindow::setupWindow()
{
    this->setWindowTitle("Parametric CAD Proto");
    this->setCentralWidget(m_document->viewerWidget());
    // auto centralWidget = new QWidget(this);
    // this->setCentralWidget(centralWidget);

    m_py_console_dock->setWidget(m_py_console);
    this->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_py_console_dock);

    // QHBoxLayout* hLayout = new QHBoxLayout(centralWidget);
    // hLayout->setContentsMargins(6, 6, 6, 6);
    // hLayout->addWidget(m_document->viewerWidget());

    //    visualizePresentations(m_document->context());

    m_tree_dock_widget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    m_tree_dock_widget->setWindowTitle("Object Browser");
    m_tree_dock_widget->setWidget(m_treeview);
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_tree_dock_widget);

    this->resize(1000, 800);
}

/**
 * @brief MainWindow::setupMenu
 */
void MainWindow::setupMenuBar()
{
    auto act_undoview = menuBar()->addAction("Undo View");
    connect(act_undoview, &QAction::triggered, this, &MainWindow::showUndoView);

    auto act_redo = menuBar()->addAction("Redo");
    act_redo->setShortcut(QKeySequence::Redo);
    connect(act_redo, &QAction::triggered, m_undostack, &QUndoStack::redo);

    auto act_undo = menuBar()->addAction("Undo");
    act_undo->setShortcut(QKeySequence::Undo);
    connect(act_undo, &QAction::triggered, m_undostack, &QUndoStack::undo);

    auto act_makebox = menuBar()->addAction("Make Box");
    connect(act_makebox, &QAction::triggered, [this]() { showBoxDlg(); });

    auto act_addparam = menuBar()->addAction("Add Parameter");
    connect(act_addparam, &QAction::triggered, [this]() { showParameterDlg(); });
}

void MainWindow::showUndoView()
{
    m_undoview->show();
}
