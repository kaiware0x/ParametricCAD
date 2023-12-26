// Created: 2002-03-11
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

#include "OCAFSample_AISDriver.hxx"
#include "OCAFSample_IObject.hxx"
#include "Tags.h"

#include <AIS.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_Shape.hxx>
#include <BRep_Tool.hxx>
#include <Standard_GUID.hxx>
#include <TDF_Label.hxx>
#include <TDataStd_ChildNodeIterator.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TNaming_NamedShape.hxx>
#include <TNaming_Tool.hxx>
#include <TopLoc_Location.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>

IMPLEMENT_STANDARD_RTTIEXT(OCAFSample_AISDriver, TPrsStd_Driver)

//=======================================================================
// function : GetID
// purpose  :
//=======================================================================

const Standard_GUID& OCAFSample_AISDriver::GetID()
{
    static Standard_GUID anID("22D22E59-C69A-11d4-8F1A-0060B0EE18E8");
    return anID;
}

//=======================================================================
// function : Update
// purpose  : Build the AISObject (if null) or update it. No compute is done.
//           Returns <True> if informations was found and AISObject updated.
//=======================================================================

Standard_Boolean OCAFSample_AISDriver::Update(const TDF_Label& theLabel, Handle(AIS_InteractiveObject) & theAISObject)
{
    Handle(TDataStd_TreeNode) aNode;
    if (!theLabel.FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), aNode)) return Standard_False;

    OCAFSample_IObject anInterface(aNode);

    TopoDS_Shape S = anInterface.GetObjectValue();
    if (S.IsNull()) return Standard_False;

    Handle(AIS_Shape) anAISShape;
    if (theAISObject.IsNull())
        anAISShape = new AIS_Shape(S);
    else {
        anAISShape = Handle(AIS_Shape)::DownCast(theAISObject);
        if (anAISShape.IsNull())
            anAISShape = new AIS_Shape(S);
        else {
            TopoDS_Shape oldShape = anAISShape->Shape();
            if (oldShape != S) {
                anAISShape->ResetTransformation();
                anAISShape->Set(S);
                anAISShape->UpdateSelection();
                anAISShape->SetToUpdate();
            }
        }
        anAISShape->SetInfiniteState(S.Infinite());
    }

    theAISObject = anAISShape;

    return Standard_True;
}
