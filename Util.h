#ifndef UTILITY_H
#define UTILITY_H

#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <Precision.hxx>
#include <Quantity_Color.hxx>
#include "TDataStd_Name.hxx"
#include "TDataStd_Real.hxx"
#include "TDataStd_TreeNode.hxx"

#include <QColor>

namespace util {
bool                       isValidConfusion(const double& val, const double& confuse = Precision::Confusion());
Quantity_Color             QColor2OccColor(const QColor& color);
Quantity_NameOfColor       QColor2OccNamedColor(const QColor& color);
QColor                     OccColor2QColor(const Quantity_Color& color, const int alpha = 0);
double                     QColorOpacity2OccOpacity(const QColor& color);
TCollection_ExtendedString toOccStr(const QString& qstr);
QString                    toQString(const TCollection_ExtendedString& occstr);

QString getEntry(const TDF_Label& label);

Handle(TDataStd_TreeNode) setTreeNode(const TDF_Label& label);
Handle(TDataStd_TreeNode) getTreeNode(const TDF_Label& label);

void    setName(const TDF_Label& label, const QString& name);
QString getName(const TDF_Label& label);
Handle(TDataStd_Name) getNameAttribute(const TDF_Label& label);

double getRealValue(const TDF_Label& label);
Handle(TDataStd_Real) getRealAttribute(const TDF_Label& label);

}    // namespace util

#endif    // UTILITY_H
