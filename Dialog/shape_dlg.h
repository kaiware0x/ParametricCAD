#ifndef MAKESHAPEDLG_H
#define MAKESHAPEDLG_H

#include <QDialog>

class ShapeDlg : public QDialog {
public:
    ShapeDlg(QWidget* parent = nullptr);

protected Q_SLOTS:
    virtual void simulate() = 0;
};

#endif    // MAKESHAPEDLG_H
