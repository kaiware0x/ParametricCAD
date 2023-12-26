#ifndef VALUES_H
#define VALUES_H

#include <optional>

#include <QString>

#include <TCollection_ExtendedString.hxx>
#include <TDF_Label.hxx>
#include <TDataStd_TreeNode.hxx>
#include <gp_Pnt.hxx>

#include "Defines.h"
#include "Util.h"

/**
 * @brief ParametricCADのための
 */
class Parameter {
public:
    Parameter() {}
    Parameter(const double& val, const QString& name) : m_value(val), m_name(name) {}
    Parameter(const Handle(TDataStd_TreeNode) & param_node_, const QString& name)
        : m_param_node(param_node_), m_name(name)
    {
    }

    /**
     * @brief 値を直接持つならparameter nodeへの参照は切る
     * @param val
     */
    void setValue(const double& val)
    {
        m_value = val;
        m_param_node.Nullify();
    }

    /**
     * @brief parameter nodeへの参照を持つなら値はリセット
     */
    void setParameterNode(const Handle(TDataStd_TreeNode) & node)
    {
        m_param_node = node;
        m_value.reset();
    }

    double value() const
    {
        if (m_value.has_value()) {
            return m_value.value();
        }
        else if (!m_param_node.IsNull()) {
            return util::getRealValue(m_param_node->Label());
        }
        FAILASSERT("Parameter has not been initialized.");
        return -1;
    }

    QString                    qtName() const { return m_name; }
    TCollection_ExtendedString occName() const { return m_name.toStdString().c_str(); }

private:
    std::optional<double> m_value;
    Handle(TDataStd_TreeNode) m_param_node;
    QString m_name;
};

class BaseValue {
public:
    TCollection_ExtendedString occName() const;

    QString name;
};

class BoxValue : public BaseValue {
    /// それぞれが同じParameterを参照しているとは限らない
    /// TODO: それぞれの値をParameterClassで書き換え
public:
    BoxValue() {}
    Parameter x;
    Parameter y;
    Parameter z;
    Parameter width;
    Parameter length;
    Parameter height;

    double parameterValue() const;
    bool   isValidConfusion() const;
    gp_Pnt center() const;
    double dx() const;
    double dy() const;
    double dz() const;
};

#endif    // VALUES_H
