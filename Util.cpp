
#include "Util.h"
#include "TDataStd_Real.hxx"
#include "Defines.h"

#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>
#include <TDataStd_Name.hxx>

bool util::isValidConfusion(const double &val, const double &confuse)
{
    return std::abs(val) >= confuse;
}

Quantity_Color util::QColor2OccColor(const QColor &color)
{
    const auto     R = color.red();
    const auto     G = color.green();
    const auto     B = color.blue();
    Quantity_Color col(R / 255.0, G / 255.0, B / 255.0, Quantity_TOC_RGB);
    return col;
}

Quantity_NameOfColor util::QColor2OccNamedColor(const QColor &color)
{
    return QColor2OccColor(color).Name();
}

QColor util::OccColor2QColor(const Quantity_Color &color, const int alpha)
{
    const int R = 255.0 * color.Red();
    const int G = 255.0 * color.Green();
    const int B = 255.0 * color.Blue();
    QColor    qcolor(R, G, B);
    return qcolor;
}

double util::QColorOpacity2OccOpacity(const QColor &color)
{
    const double alpha = 255.0 - color.alpha();
    return alpha / 255.0;
}

TCollection_ExtendedString util::toOccStr(const QString &qstr)
{
    return qstr.toStdString().c_str();
}

QString util::toQString(const TCollection_ExtendedString &occstr)
{
    TCollection_AsciiString asciistr(occstr);
    return QString(asciistr.ToCString());
}

QString util::getEntry(const TDF_Label &label)
{
    if (label.IsNull()) {
        return QString();
    }
    TCollection_AsciiString anEntry;
    TDF_Tool::Entry(label, anEntry);

    //    TColStd_ListOfInteger   aTagList;
    //    TDF_Tool::TagList(anEntry, aTagList);
    return anEntry.ToCString();
}

Handle(TDataStd_TreeNode) util::setTreeNode(const TDF_Label &label)
{
    return TDataStd_TreeNode::Set(label);
}

Handle(TDataStd_TreeNode) util::getTreeNode(const TDF_Label &label)
{
    Handle(TDataStd_TreeNode) treenode;
    label.FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), treenode);
    return treenode;
}

void util::setName(const TDF_Label &label, const QString &name)
{
    const auto occname = toOccStr(name);
    TDataStd_Name::Set(label, occname);
}

QString util::getName(const TDF_Label &label)
{
    if (auto attr = getNameAttribute(label)) {
        return util::toQString(attr->Get());
    }
    else {
        FAILASSERT("Cannot find the attribute <TDataStd_Name>.");
        return QString();
    }
}

Handle(TDataStd_Name) util::getNameAttribute(const TDF_Label &label)
{
    Handle(TDataStd_Name) name_attr;
    label.FindAttribute(TDataStd_Name::GetID(), name_attr);
    return name_attr;
}

double util::getRealValue(const TDF_Label &label)
{
    if (auto attr = getRealAttribute(label)) {
        return attr->Get();
    }
    else {
        FAILASSERT("Cannot find the attribute <TDataStd_Real>.");
        return -1.0;
    }
}

Handle(TDataStd_Real) util::getRealAttribute(const TDF_Label &label)
{
    Handle(TDataStd_Real) aReal;
    label.FindAttribute(TDataStd_Real::GetID(), aReal);
    return aReal;
}
