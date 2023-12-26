// Created: 1999-10-25
//
// Copyright (c) 1999-2013 OPEN CASCADE SAS
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

#include "BRepNaming_LoaderParent.hxx"

#include <BRepBuilderAPI_MakeShape.hxx>
#include <TDF_Label.hxx>
#include <TDataStd_Name.hxx>
#include <TNaming.hxx>
#include <TNaming_Builder.hxx>
#include <TNaming_NamedShape.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopLoc_Location.hxx>
#include <TopTools_DataMapIteratorOfDataMapOfShapeShape.hxx>
#include <TopTools_DataMapOfShapeShape.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_MapIteratorOfMapOfShape.hxx>
#include <TopTools_MapOfShape.hxx>
#include <TopoDS_Iterator.hxx>

//=======================================================================
// function : GetDangleShapes
// purpose  : Returns dangle sub shapes Generator - Dangle.
//=======================================================================

Standard_Boolean BRepNaming_LoaderParent::GetDangleShapes(const TopoDS_Shape&           ShapeIn,
                                                          const TopAbs_ShapeEnum        GeneratedFrom,
                                                          TopTools_DataMapOfShapeShape& Dangles)
{
    Dangles.Clear();
    TopTools_IndexedDataMapOfShapeListOfShape subShapeAndAncestors;
    TopAbs_ShapeEnum                          GeneratedTo;
    if (GeneratedFrom == TopAbs_FACE)
        GeneratedTo = TopAbs_EDGE;
    else if (GeneratedFrom == TopAbs_EDGE)
        GeneratedTo = TopAbs_VERTEX;
    else
        return Standard_False;
    TopExp::MapShapesAndAncestors(ShapeIn, GeneratedTo, GeneratedFrom, subShapeAndAncestors);
    for (Standard_Integer i = 1; i <= subShapeAndAncestors.Extent(); i++) {
        const TopoDS_Shape&         mayBeDangle = subShapeAndAncestors.FindKey(i);
        const TopTools_ListOfShape& ancestors   = subShapeAndAncestors.FindFromIndex(i);
        if (ancestors.Extent() == 1) Dangles.Bind(ancestors.First(), mayBeDangle);
    }
    return !Dangles.IsEmpty();
}

//=======================================================================
// function : LoadGeneratedDangleShapes
// purpose  :
//=======================================================================

void BRepNaming_LoaderParent::LoadGeneratedDangleShapes(const TopoDS_Shape&    ShapeIn,
                                                        const TopAbs_ShapeEnum GeneratedFrom, TNaming_Builder& Builder)
{
    TopTools_DataMapOfShapeShape dangles;
    if (!BRepNaming_LoaderParent::GetDangleShapes(ShapeIn, GeneratedFrom, dangles)) return;
    TopTools_DataMapIteratorOfDataMapOfShapeShape itr(dangles);
    for (; itr.More(); itr.Next())
        Builder.Generated(itr.Key(), itr.Value());
}
