#ifndef PARAMETER_DLG_H
#define PARAMETER_DLG_H

#include <QDialog>
#include <QDoubleSpinBox>

#include <TDataStd_TreeNode.hxx>

#include "document.h"
#include "values.h"

namespace Ui {
class ParameterDlg;
}

class ParameterDlg : public QDialog {
    Q_OBJECT

public:
    explicit ParameterDlg(Document* document, QWidget* mainwindow);
    ~ParameterDlg();

    void setParameterTreeNode(const Handle(TDataStd_TreeNode) & param_node);

private Q_SLOTS:
    void accept() override;

private:
    static void initDoubleSpinBox(QDoubleSpinBox& box);

Q_SIGNALS:
    void addParamSignal(const Parameter& param);
    void editParamSignal(const Handle(TDataStd_TreeNode) & param_node, const Parameter& param);

private:
    Ui::ParameterDlg* ui;

    Handle(TDataStd_TreeNode) m_param_node;
};

#endif    // PARAMETER_DLG_H
