

#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>

#include "box_dlg.h"
#include "Defines.h"
#include "document.h"
#include "FromSample/OCAFSample_IBox.hxx"
#include "Util.h"
#include "ui_box_dlg.h"

BoxDlg::BoxDlg(Document *document, QWidget *mainwindow) : ShapeDlg(mainwindow), ui(new Ui::BoxDlg), m_doc(document)
{
    ui->setupUi(this);
    setWindowTitle("Box");

    initDoubleSpinBox(*ui->dspin_x);
    initDoubleSpinBox(*ui->dspin_y);
    initDoubleSpinBox(*ui->dspin_z);
    initDoubleSpinBox(*ui->dspin_width, 0.0);
    initDoubleSpinBox(*ui->dspin_height, 0.0);
    initDoubleSpinBox(*ui->dspin_length, 0.0);

    ui->dspin_x->setValue(40.0);
    ui->dspin_y->setValue(100.0);
    ui->dspin_z->setValue(0.0);
    ui->dspin_width->setValue(50.0);
    ui->dspin_height->setValue(50.0);
    ui->dspin_length->setValue(50.0);
    ui->edit_name->setText("Box");

    connect(ui->btn_simulate, &QPushButton::clicked, this, &BoxDlg::simulate);
}

BoxDlg::~BoxDlg()
{
    delete ui;
}

void BoxDlg::setBoxFunctionTreeNode(const Handle(TDataStd_TreeNode) & func_node)
{
    m_func_node = func_node;

    OCAFSample_IBox IBox(func_node);
    double          x, y, z;
    IBox.GetCenter(x, y, z);
    const double &w = IBox.GetDX()->Get();
    const double &l = IBox.GetDY()->Get();
    const double &h = IBox.GetDZ()->Get();

    ui->dspin_x->setValue(x);
    ui->dspin_y->setValue(y);
    ui->dspin_z->setValue(z);
    ui->dspin_width->setValue(w);
    ui->dspin_length->setValue(l);
    ui->dspin_height->setValue(h);
}

void BoxDlg::simulate()
{
    MYLOG;

    m_doc->deleteAISObjects(m_tmp_shapes);
    m_tmp_shapes.clear();

    auto value = boxValue();
    if (value.name.isEmpty()) {
        MYWARNLOG << "name is empty";
        return;
    }

    if (!value.isValidConfusion()) {
        MYWARNLOG << "Invalid confusion";
        return;
    }

    BRepPrimAPI_MakeBox makebox(value.center(), value.dx(), value.dy(), value.dz());
    makebox.Build();
    if (!makebox.IsDone()) {
        return;
    }

    /// Preview用にオブジェクトをTopoDS_Edge表示.
    for (TopExp_Explorer ex(makebox.Shape(), TopAbs_EDGE); ex.More(); ex.Next()) {
        TopoDS_Edge edge = TopoDS::Edge(ex.Current());
        if (edge.IsNull()) {
            continue;
        }

        auto tmp_shape = new AIS_Shape(edge);
        m_tmp_shapes << tmp_shape;

        m_doc->showWireFrameShape(tmp_shape);
    }
}

void BoxDlg::accept()
{
    MYLOG;

    if (m_func_node.IsNull()) {
        emit makeBoxSignal(boxValue());
    }
    else {
        emit editBoxSignal(m_func_node, boxValue());
    }

    //    m_doc->occDocument()->OpenCommand();

    //    auto box = makeBoxShape();
    //    if (box.IsNull()) return;

    //    auto aisbox = new AIS_Shape(box);
    //    m_doc->showShadedShape(aisbox);

    QDialog::accept();
}

void BoxDlg::done(int retcode)
{
    m_doc->deleteAISObjects(m_tmp_shapes);
    m_tmp_shapes.clear();
    m_func_node.Nullify();

    QDialog::done(retcode);
}

void BoxDlg::initDoubleSpinBox(QDoubleSpinBox &box, const double &minimum)
{
    box.setDecimals(3);
    box.setMaximum(std::numeric_limits<double>::max());
    box.setMinimum(minimum);
    box.setSingleStep(1.0);
}

BoxValue BoxDlg::boxValue() const
{
    const auto width  = ui->dspin_width->value();
    const auto length = ui->dspin_length->value();
    const auto height = ui->dspin_height->value();

    const auto x = ui->dspin_x->value();
    const auto y = ui->dspin_y->value();
    const auto z = ui->dspin_z->value();

    BoxValue value;
    value.name = ui->edit_name->text();
    value.x.setValue(x);
    value.y.setValue(y);
    value.z.setValue(z);
    value.width.setValue(width);
    value.length.setValue(length);
    value.height.setValue(height);
    return value;
}
