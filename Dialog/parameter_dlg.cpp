#include "parameter_dlg.h"
#include "Defines.h"
#include "Util.h"
#include "ui_parameter_dlg.h"

ParameterDlg::ParameterDlg(Document *document, QWidget *mainwindow) : QDialog(mainwindow), ui(new Ui::ParameterDlg)
{
    ui->setupUi(this);
    setWindowTitle("Parameter");

    initDoubleSpinBox(*ui->dspin_value);
}

ParameterDlg::~ParameterDlg()
{
    delete ui;
}

void ParameterDlg::setParameterTreeNode(const Handle(TDataStd_TreeNode) & param_node)
{
    MY_ASSERT(!param_node.IsNull(), "param node is null.");

    m_param_node = param_node;

    const auto &name = util::getName(param_node->Label());
    ui->edit_name->setText(name);

    const auto &val = util::getRealValue(param_node->Label());
    ui->dspin_value->setValue(val);
}

void ParameterDlg::accept()
{
    if (ui->edit_name->text().isEmpty()) {
        QDialog::accept();
        return;
    }

    const Parameter param(ui->dspin_value->value(), ui->edit_name->text());
    if (m_param_node.IsNull()) {
        emit addParamSignal(param);
    }
    else {
        emit editParamSignal(m_param_node, param);
    }

    QDialog::accept();
}

void ParameterDlg::initDoubleSpinBox(QDoubleSpinBox &box)
{
    box.setDecimals(3);
    box.setMaximum(std::numeric_limits<double>::max());
    box.setMinimum(0.0);
    box.setSingleStep(1.0);
}
