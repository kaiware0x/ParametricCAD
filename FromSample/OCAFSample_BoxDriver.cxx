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

#include "OCAFSample_BoxDriver.hxx"
#include "BRepNaming_Box.hxx"
#include "BRepNaming_TypeOfPrimitive3D.hxx"
#include "OCAFSample_IBox.hxx"
#include "OCAFSample_IShape.hxx"
#include "Tags.h"

#include <BRepAlgo.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TDocStd_Modified.hxx>
#include <TFunction_Logbook.hxx>
#include <TNaming.hxx>
#include <TNaming_Builder.hxx>
#include <TNaming_NamedShape.hxx>
#include <TopLoc_Location.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>

#define OK_BOX 0
#define X_NOT_FOUND 1
#define Y_NOT_FOUND 2
#define Z_NOT_FOUND 3
#define DX_NOT_FOUND 4
#define DY_NOT_FOUND 5
#define DZ_NOT_FOUND 6
#define EMPTY_BOX 7
#define BOX_NOT_DONE 8
#define NULL_BOX 9

IMPLEMENT_STANDARD_RTTIEXT(OCAFSample_BoxDriver, OCAFSample_Driver)

//=======================================================================
// function : Execute
// purpose  :
//=======================================================================

Standard_Integer OCAFSample_BoxDriver::Execute(Handle(TFunction_Logbook) & theLogbook) const
{
    auto function_label = Label();

    Handle(TDataStd_TreeNode) aFunctionNode;
    if (!function_label.FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), aFunctionNode)) {
        return EMPTY_BOX;
    }

    ///---------------------------------------------------------------------------------------------------
    /// 「Edit」の場合は前回の生成結果を取得して座標を読み取り
    ///---------------------------------------------------------------------------------------------------
    TopLoc_Location aLocation;
    {
        TDF_Label aPrevResultLabel = function_label.FindChild(tag::Object::Results);

        if (!aPrevResultLabel.IsNull()) {
            Handle(TNaming_NamedShape) aPrevNS;
            if (aPrevResultLabel.FindAttribute(TNaming_NamedShape::GetID(), aPrevNS)) {
                if (!aPrevNS->IsEmpty()) {
                    aLocation = aPrevNS->Get().Location();
                }
            }
        }
    }

    ///---------------------------------------------------------------------------------------------------
    /// UIで入力されたBoxの寸法・座標などの値を取得
    ///---------------------------------------------------------------------------------------------------
    Standard_Real X, Y, Z, DX, DY, DZ;
    {
        OCAFSample_IBox anInterface(aFunctionNode);

        anInterface.GetCenter(X, Y, Z);
        DX = anInterface.GetDX()->Get();
        DY = anInterface.GetDY()->Get();
        DZ = anInterface.GetDZ()->Get();

        if (DX == 0 && DY == 0 && DZ == 0) {
            return EMPTY_BOX;
        }
    }

    ///---------------------------------------------------------------------------------------------------
    /// Box新規生成
    ///---------------------------------------------------------------------------------------------------
    BRepPrimAPI_MakeBox mkBox(gp_Pnt(X, Y, Z), DX, DY, DZ);
    mkBox.Build();
    if (!mkBox.IsDone()) {
        return BOX_NOT_DONE;
    }
    if (mkBox.Shape().IsNull()) {
        return NULL_BOX;
    }
    if (!BRepAlgo::IsValid(mkBox.Shape())) {
        return BOX_NOT_DONE;
    }

    ///---------------------------------------------------------------------------------------------------
    /// Result LabelにBoxを登録
    ///---------------------------------------------------------------------------------------------------
    // Name result
    TDF_Label aResultLabel = Label().FindChild(tag::Object::Results, Standard_True);

    BRepNaming_Box aNaming(aResultLabel);
    aNaming.Load(mkBox, BRepNaming_SOLID);

    TDocStd_Modified::Add(aFunctionNode->Father()->Label());

    OCAFSample_IShape::AddLabels(aFunctionNode, theLogbook);

    theLogbook->SetImpacted(function_label);
    TDocStd_Modified::Add(function_label);

    theLogbook->SetImpacted(aResultLabel);

    TDF_ChildIterator anIterator(aResultLabel);
    for (; anIterator.More(); anIterator.Next()) {
        theLogbook->SetImpacted(anIterator.Value());
    }

    if (!aLocation.IsIdentity()) {
        TNaming::Displace(aResultLabel, aLocation, Standard_True);
    }

    return OK_BOX;
}
