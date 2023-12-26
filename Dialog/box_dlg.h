#ifndef MAKE_BOX_DLG_H
#define MAKE_BOX_DLG_H

#include <QDoubleSpinBox>
#include <QLineEdit>

#include "AIS_InteractiveContext.hxx"
#include "AIS_Shape.hxx"
#include "Standard_Handle.hxx"
#include "mainwindow.h"
#include "shape_dlg.h"
#include "values.h"

namespace Ui {
class BoxDlg;
}

class BoxDlg : public ShapeDlg {
    Q_OBJECT

public:
    explicit BoxDlg(Document* document, QWidget* mainwindow);
    ~BoxDlg();

    void setBoxFunctionTreeNode(const Handle(TDataStd_TreeNode) & func_node);

private Q_SLOTS:
    void simulate() override;
    void accept() override;
    void done(int retcode) override;

private:
    static void initDoubleSpinBox(QDoubleSpinBox& box, const double& minimum = std::numeric_limits<double>::lowest());
    BoxValue    boxValue() const;

Q_SIGNALS:
    void makeBoxSignal(const BoxValue& param);
    void editBoxSignal(const Handle(TDataStd_TreeNode) & func_node, const BoxValue& param);

private:
    Ui::BoxDlg* ui;

    Document* m_doc;

    QList<Handle(AIS_InteractiveObject)> m_tmp_shapes;

    Handle(TDataStd_TreeNode) m_func_node;
};

#endif    // MAKE_BOX_DLG_H
