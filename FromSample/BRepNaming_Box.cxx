// Created: 1999-09-24
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

#include "BRepNaming_Box.hxx"
#include "BRepNaming_Loader.hxx"

#include <BRepPrimAPI_MakeBox.hxx>
#include <Standard_NullObject.hxx>
#include <TDF_Label.hxx>
#include <TNaming_Builder.hxx>
#include <TopoDS_Solid.hxx>

//=======================================================================
// function : Load
// purpose  :
//=======================================================================

void BRepNaming_Box::Load(BRepPrimAPI_MakeBox& MS, const BRepNaming_TypeOfPrimitive3D Type) const
{
    TNaming_Builder Builder(ResultLabel());
    if (Type == BRepNaming_SOLID)
        Builder.Generated(MS.Solid());
    else
        Builder.Generated(MS.Shell());

    // Load the faces of the box :
    TNaming_Builder(Bottom()).Generated(MS.BottomFace());
    TNaming_Builder(Top()).Generated(MS.TopFace());
    TNaming_Builder(Front()).Generated(MS.FrontFace());
    TNaming_Builder(Right()).Generated(MS.RightFace());
    TNaming_Builder(Back()).Generated(MS.BackFace());
    TNaming_Builder(Left()).Generated(MS.LeftFace());
}

//=======================================================================
// function : Back
// purpose  :
//=======================================================================

TDF_Label BRepNaming_Box::Back() const
{
    return ResultLabel().FindChild(1, Standard_True);
}

//=======================================================================
// function : Front
// purpose  :
//=======================================================================

TDF_Label BRepNaming_Box::Front() const
{
    return ResultLabel().FindChild(2, Standard_True);
}

//=======================================================================
// function : Left
// purpose  :
//=======================================================================

TDF_Label BRepNaming_Box::Left() const
{
    return ResultLabel().FindChild(3, Standard_True);
}

//=======================================================================
// function : Right
// purpose  :
//=======================================================================

TDF_Label BRepNaming_Box::Right() const
{
    return ResultLabel().FindChild(4, Standard_True);
}

//=======================================================================
// function : Bottom
// purpose  :
//=======================================================================

TDF_Label BRepNaming_Box::Bottom() const
{
    return ResultLabel().FindChild(5, Standard_True);
}

//=======================================================================
// function : Top
// purpose  :
//=======================================================================

TDF_Label BRepNaming_Box::Top() const
{
    return ResultLabel().FindChild(6, Standard_True);
}
