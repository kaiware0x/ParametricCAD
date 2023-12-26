// Created: 2002-03-07
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

#include "OCAFSample_ISelection.hxx"
#include "Tags.h"

#include <Standard_ConstructionError.hxx>
#include <TDF_Label.hxx>
#include <TDF_Reference.hxx>
#include <TDocStd_Modified.hxx>
#include <TFunction_Function.hxx>
#include <TFunction_Logbook.hxx>
#include <TNaming_NamedShape.hxx>
#include <TNaming_Selector.hxx>
#include <TNaming_Tool.hxx>

//=======================================================================
// function : GetCutID
// purpose  :
//=======================================================================
const Standard_GUID& OCAFSample_ISelection::GetID()
{
    static Standard_GUID aSelectionID("22D22E57-C69A-11d4-8F1A-0060B0EE18E8");
    return aSelectionID;
}

//=======================================================================
// function : MakeCut
// purpose  : Adds a selection object to the document
//=======================================================================
Handle(TDataStd_TreeNode) OCAFSample_ISelection::MakeSelect(const TCollection_ExtendedString& theName,
                                                            const TopoDS_Shape&               theShape,
                                                            const TopoDS_Shape&               theContext,
                                                            const Handle(TDataStd_TreeNode) & AccessTreeNode,
                                                            TCollection_ExtendedString& theError)
{
    if (theShape.IsNull() || theContext.IsNull()) {
        theError = NULL_ARGUMENT;
        return NULL;
    }

    Handle(TDataStd_TreeNode) anObjectNode = AddObject(AccessTreeNode->Label());

    OCAFSample_IObject anInterface(anObjectNode);
    anInterface.SetName(theName);

    Handle(TDataStd_TreeNode) aFunctionNode = anInterface.AddFunction(GetID());

    if (anObjectNode.IsNull() || aFunctionNode.IsNull()) {
        theError = NOTDONE;
        return NULL;
    }

    OCAFSample_ISelection ISelection(aFunctionNode);
    ISelection.SetName("Naming function");
    if (!ISelection.Select(theShape, theContext)) {
        theError = UNABLE_DUE_TO_NAMING;
        return NULL;
    }

    theError = DONE;
    return anObjectNode;
}

//=======================================================================
// function : Select
// purpose  : Performs the topological naming of theShape belonging to theContext
//=======================================================================
Standard_Boolean OCAFSample_ISelection::Select(const TopoDS_Shape& theShape, const TopoDS_Shape& theContext)
{
    TDF_Label aResultLabel         = myTreeNode->Label().FindChild(tag::Object::Results);
    TDF_Label aContexLabel         = myTreeNode->Label().FindChild(tag::Object::Argments).FindChild(tag::Selection::Context);
    Handle(TNaming_NamedShape) aNS = TNaming_Tool::NamedShape(theContext, myTreeNode->Label());

    if (aNS.IsNull() || !aNS->Label().Father().IsAttribute(TFunction_Function::GetID())) return Standard_False;

    TNaming_Selector aSelector(aResultLabel);
    Standard_Boolean aStatus = aSelector.Select(theShape, theContext);

    TDF_Reference::Set(aContexLabel, aNS->Label().Father());    // Set a argument of function (theContext)

    return aStatus;
}

//=======================================================================
// function : GetContext
// purpose  : Returns the context shape
//=======================================================================
TopoDS_Shape OCAFSample_ISelection::GetContext() const
{
    TopoDS_Shape aShape;
    Handle(TDF_Reference) aReference;
    TDF_Label aContexLabel = myTreeNode->Label().FindChild(tag::Object::Argments).FindChild(tag::Selection::Context);

    if (aContexLabel.FindAttribute(TDF_Reference::GetID(), aReference)) {
        const TDF_Label& aLabel = aReference->Get().FindChild(tag::Object::Results, Standard_False);
        Handle(TNaming_NamedShape) aNamedShape;
        if (aLabel.FindAttribute(TNaming_NamedShape::GetID(), aNamedShape)) return aNamedShape->Get();
    }

    return aShape;
}
