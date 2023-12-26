// Created: 2002-02-07
//
// Copyright (c) 2002-2013 OPEN CASCADE SAS
//
// This file is part of commercial software by OPEN CASCADE SAS.
//
// This software is furnished in accordance with the terms and conditions
// of the contract and with the inclusion of this copyright notice.
// This software or any other copy thereof may not be provided or otherwise
// be made available to any third party.
// No ownership title to the software is transferred hereby.
//
// OPEN CASCADE SAS makes no representation or warranties with respect to the
// performance of this software, and specifically disclaims any responsibility
// for any damages, special or consequential, connected with its use.

#include "OCAFSample_IBox.hxx"
#include "OCAFSample_BoxDriver.hxx"
#include "Tags.h"
#include "Defines.h"

#include <Standard_ConstructionError.hxx>
#include <TDF_Data.hxx>
#include <TDF_Label.hxx>
#include <TDataStd_Real.hxx>
#include <TDocStd_Modified.hxx>
#include <TFunction_Function.hxx>
#include <TFunction_Logbook.hxx>

/**
 * @brief Returns the GUID for Box function's driver.
 * @return
 */
const Standard_GUID& OCAFSample_IBox::GetID()
{
    static Standard_GUID anID("22D22E50-C69A-11d4-8F1A-0060B0EE18E8");
    return anID;
}

//=======================================================================
// function : MakeBox
// purpose  : Adds a box object to the document
//=======================================================================

Handle(TDataStd_TreeNode) OCAFSample_IBox::MakeBox(const BoxValue& value,
                                                   const Handle(TDataStd_TreeNode) & AccessTreeNode,
                                                   TCollection_ExtendedString& theError)
{
    if (AccessTreeNode.IsNull() || AccessTreeNode->Label().IsNull()) {
        theError = NULL_ACCESS_NODE;
        MYWARNLOG << "Cannot get Label from TreeNode";
        return Handle(TDataStd_TreeNode)();
    }

    ///---------------------------------------------------------------------------------------------------
    /// 「Box」Label、TreeNodeを生成.
    ///---------------------------------------------------------------------------------------------------
    /// anObjectNode = "Box" Node
    Handle(TDataStd_TreeNode) anObjectNode = AddObject(AccessTreeNode->Label());

    /// Box Labelに名前を設定.
    OCAFSample_IShape anInterface(anObjectNode);
    anInterface.SetName(value.occName());

    ///---------------------------------------------------------------------------------------------------
    /// 「Box Function」Label、TreeNodeを生成.
    ///---------------------------------------------------------------------------------------------------
    /// aFunctionNode = "Box function" Node
    Handle(TDataStd_TreeNode) aFunctionNode = anInterface.AddFunction(GetID());

    if (anObjectNode.IsNull() || aFunctionNode.IsNull()) {
        theError = NOTDONE;
        MYWARNLOG << "anObjectNode.IsNull() or aFunctionNode.IsNull()";
        return Handle(TDataStd_TreeNode)();
    }

    ///---------------------------------------------------------------------------------------------------
    /// Function Nodeにパラメータをセット.
    ///---------------------------------------------------------------------------------------------------
    {
        OCAFSample_IBox IBox(aFunctionNode);
        IBox.SetName("Box function");
        IBox.SetCenter(value.center());

        CALLOUT << VAROUT(value.dx()) << VAROUT(value.dy()) << VAROUT(value.dz());
        IBox.GetDX()->Set(value.dx());
        IBox.GetDY()->Set(value.dy());
        IBox.GetDZ()->Set(value.dz());

        double x, y, z;
        IBox.GetCenter(x, y, z);
        CALLOUT << VAROUT(x) << VAROUT(y) << VAROUT(z);
    }

    ///---------------------------------------------------------------------------------------------------
    /// Box生成.
    ///---------------------------------------------------------------------------------------------------
    {
        OCAFSample_BoxDriver aDriver;
        aDriver.Init(aFunctionNode->Label());

        Handle(TFunction_Logbook) log = TFunction_Logbook::Set(aFunctionNode->Label());

        if (aDriver.Execute(log) > 0) {
            MYWARNLOG << "Fail OCAFSample_BoxDriver::Execute()";
            theError = ALGO_FAILED;
            return Handle(TDataStd_TreeNode)();
        }
    }

    theError = DONE;
    return anObjectNode;
}

//=======================================================================
// function : GetDX
// purpose  : Gets X coordinate attribute
//=======================================================================

Handle(TDataStd_Real) OCAFSample_IBox::GetDX() const
{
    TDF_Label aDXLabel = myTreeNode->Label().FindChild(tag::Object::Argments).FindChild(tag::Box::DeltaX);

    Handle(TDataStd_Real) aReal;
    if (!aDXLabel.FindAttribute(TDataStd_Real::GetID(), aReal)) {
        aReal = TDataStd_Real::Set(aDXLabel, 0);
    }

    return aReal;
}

//=======================================================================
// function : GetDY
// purpose  : Gets Y coordinate attribute
//=======================================================================

Handle(TDataStd_Real) OCAFSample_IBox::GetDY() const
{
    TDF_Label aDYLabel = myTreeNode->Label().FindChild(tag::Object::Argments).FindChild(tag::Box::DeltaY);

    Handle(TDataStd_Real) aReal;
    if (!aDYLabel.FindAttribute(TDataStd_Real::GetID(), aReal)) {
        aReal = TDataStd_Real::Set(aDYLabel, 0);
    }

    return aReal;
}

//=======================================================================
// function : GetDZ
// purpose  : Gets Z coordinate attribute
//=======================================================================

Handle(TDataStd_Real) OCAFSample_IBox::GetDZ() const
{
    TDF_Label aDZLabel = myTreeNode->Label().FindChild(tag::Object::Argments).FindChild(tag::Box::DeltaZ);

    Handle(TDataStd_Real) aReal;
    if (!aDZLabel.FindAttribute(TDataStd_Real::GetID(), aReal)) {
        aReal = TDataStd_Real::Set(aDZLabel, 0);
    }

    return aReal;
}
