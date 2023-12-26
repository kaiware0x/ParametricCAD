// Created: 2002-02-21
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

#include "OCAFSample_IObject.hxx"
#include "OCAFSample_IDisplayer.hxx"
#include "OCAFSample_ISelection.hxx"
#include "OCAFSample_ITransformParent.hxx"
#include "OCAFSample_SolverEx.hxx"
#include "Tags.h"

#include "Defines.h"
#include "Util.h"

#include <Standard_ConstructionError.hxx>
#include <TCollection_AsciiString.hxx>
#include <TDF_AttributeMap.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_Label.hxx>
#include <TDF_MapIteratorOfAttributeMap.hxx>
#include <TDF_MapIteratorOfLabelMap.hxx>
#include <TDF_Reference.hxx>
#include <TDF_Tool.hxx>
#include <TDataStd_ChildNodeIterator.hxx>
#include <TDataStd_Integer.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_UAttribute.hxx>
#include <TDocStd_Modified.hxx>
#include <TFunction_Function.hxx>
#include <TNaming_NamedShape.hxx>
#include <TNaming_Tool.hxx>
#include <TPrsStd_AISPresentation.hxx>

//=======================================================================
// function : Constructor
// purpose  :
//=======================================================================

OCAFSample_IObject::OCAFSample_IObject(const Handle(TDataStd_TreeNode) & aTreeNode) : myTreeNode(aTreeNode)
{
    if (myTreeNode.IsNull()) throw Standard_ConstructionError("Null TreeNode");
}

//=======================================================================
// function : SetName
// purpose  :
//=======================================================================

void OCAFSample_IObject::SetName(const TCollection_ExtendedString& aName)
{
    if (myTreeNode.IsNull()) return;

    TDataStd_Name::Set(myTreeNode->Label(), aName);
    TDocStd_Modified::Add(myTreeNode->Label());
}

const Standard_GUID& OCAFSample_IObject::GetObjectID()
{
    static Standard_GUID anID("22D22E60-C69A-11d4-8F1A-0060B0EE18E8");
    return anID;
}

Handle(TDataStd_TreeNode) OCAFSample_IObject::AddObject(const TDF_Label& parentLabel)
{
    if (parentLabel.IsNull()) {
        return nullptr;
    }

    Handle(TDataStd_TreeNode) parentNode;
    if (!parentLabel.FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), parentNode)) {
        return nullptr;
    }

    TDF_Label objLabel = parentLabel.NewChild();

    TDataStd_UAttribute::Set(objLabel, GetObjectID());
    Handle(TDataStd_TreeNode) objNode = TDataStd_TreeNode::Set(objLabel);

    parentNode->Append(objNode);

    return objNode;
}

//=======================================================================
// function : SetCenter
// purpose  :
//=======================================================================

void OCAFSample_IObject::SetCenter(const Standard_Real X, const Standard_Real Y, const Standard_Real Z)
{
    if (myTreeNode.IsNull()) return;

    TDF_Label arguments_label = myTreeNode->Label().FindChild(tag::Object::Argments);

    TDF_Label arg_label;

    arg_label = arguments_label.FindChild(tag::Center::X);
    TDataStd_Real::Set(arg_label, X);
    TDocStd_Modified::Add(arg_label);

    arg_label = arguments_label.FindChild(tag::Center::Y);
    TDataStd_Real::Set(arg_label, Y);
    TDocStd_Modified::Add(arg_label);

    arg_label = arguments_label.FindChild(tag::Center::Z);
    TDataStd_Real::Set(arg_label, Z);
    TDocStd_Modified::Add(arg_label);
}

void OCAFSample_IObject::SetCenter(const gp_Pnt& center)
{
    SetCenter(center.X(), center.Y(), center.Z());
}

//=======================================================================
// function : GetCenter
// purpose  :
//=======================================================================

void OCAFSample_IObject::GetCenter(Standard_Real& X, Standard_Real& Y, Standard_Real& Z) const
{
    if (myTreeNode.IsNull()) return;

    TDF_Label L = myTreeNode->Label().FindChild(tag::Object::Argments, Standard_False);
    if (L.IsNull()) return;

    Handle(TDataStd_Real) aReal;
    TDF_Label L1;

    L1 = L.FindChild(tag::Center::X, Standard_False);
    if (L1.IsNull()) return;
    if (!L1.FindAttribute(TDataStd_Real::GetID(), aReal)) {
        return;
    }
    X = aReal->Get();

    L1 = L.FindChild(tag::Center::Y, Standard_False);
    if (L1.IsNull()) return;
    if (!L1.FindAttribute(TDataStd_Real::GetID(), aReal)) {
        return;
    }
    Y = aReal->Get();

    L1 = L.FindChild(tag::Center::Z, Standard_False);
    if (L1.IsNull()) return;
    if (!L1.FindAttribute(TDataStd_Real::GetID(), aReal)) {
        return;
    }
    Z = aReal->Get();
}

//=======================================================================
// function : Remove
// purpose  :
//=======================================================================
Standard_Boolean OCAFSample_IObject::Remove()
{
    if (HasReferencedObjects()) return Standard_False;

    myTreeNode->Remove();
    myTreeNode->ForgetAllAttributes(Standard_True);
    myTreeNode.Nullify();

    return Standard_True;
}

//=======================================================================
// function : HasReferencedObjects
// purpose  :
//=======================================================================
Standard_Boolean OCAFSample_IObject::HasReferencedObjects() const
{
    TDF_AttributeMap    aMap;
    OCAFSample_SolverEx aSolver;
    Handle(TFunction_Function) aFunction, aMyFunction;

    if (!myTreeNode->FindAttribute(TFunction_Function::GetID(), aMyFunction)) return Standard_False;

    TDataStd_ChildNodeIterator anIterator(myTreeNode->Root(), Standard_True);

    for (; anIterator.More(); anIterator.Next()) {
        if (anIterator.Value() == myTreeNode) continue;

        if (anIterator.Value()->FindAttribute(TFunction_Function::GetID(), aFunction))
            aSolver.ComputeFunction(aFunction, aMap);

        if (aMap.Contains(aMyFunction)) return Standard_True;
    }

    return Standard_False;
}

//=======================================================================
// function : GetLastFunction
// purpose  :
//=======================================================================
Handle(TDataStd_TreeNode) OCAFSample_IObject::GetLastFunction() const
{
    Handle(TDataStd_TreeNode) aNode;
    Handle(TFunction_Function) aFunction;

    if (!myTreeNode->First().IsNull()) {    // Find the last function of the object
        TDataStd_ChildNodeIterator anIterator(myTreeNode);
        for (; anIterator.More(); anIterator.Next())
            aNode = anIterator.Value();
    }

    return aNode;
}

//=======================================================================
// function : GetObjectValue
// purpose  :
//=======================================================================
TopoDS_Shape OCAFSample_IObject::GetObjectValue() const
{
    TopoDS_Shape aShape;
    Handle(TNaming_NamedShape) aNamedShape;
    Handle(TDF_Reference) aRef;

    if (myTreeNode->FindAttribute(TDF_Reference::GetID(), aRef)) {
        if (aRef->Get().IsNull()) return aShape;
        TDF_Label aLabel = aRef->Get().FindChild(tag::Object::Results);
        if (aLabel.FindAttribute(TNaming_NamedShape::GetID(), aNamedShape)) aShape = aNamedShape->Get();
    }

    return aShape;
}

//=======================================================================
// function : GetObjectNode
// purpose  :
//=======================================================================
Handle(TDataStd_TreeNode) OCAFSample_IObject::GetObjectNode() const
{
    Handle(TDataStd_TreeNode) aNode = myTreeNode;

    while (!aNode.IsNull()) {
        if (aNode->IsAttribute(GetObjectID())) {
            return aNode;
        }
        aNode = aNode->Father();
    }

    return aNode;
}

///=======================================================================
/// function : AddFunction
/// purpose  :
/// return   : function node
///=======================================================================
Handle(TDataStd_TreeNode) OCAFSample_IObject::AddFunction(const Standard_GUID& theID)
{
    Handle(TDataStd_TreeNode) aObjNode = GetObjectNode();

    if (aObjNode.IsNull()) {
        return NULL;
    }

    TDF_Label aFuncLabel                = aObjNode->Label().NewChild();
    Handle(TDataStd_TreeNode) aFuncNode = TDataStd_TreeNode::Set(aFuncLabel);

    aObjNode->Append(aFuncNode);

    Handle(TFunction_Function) aFunction = TFunction_Function::Set(aFuncLabel, theID);

    Handle(TDF_Reference) aRef;
    TDF_Label aContext;

    if (aObjNode->FindAttribute(TDF_Reference::GetID(), aRef)) {
        aContext = aRef->Get();
    }

    /// Set result of the object
    if (theID != OCAFSample_ITransformParent::GetID()) {
        TDF_Reference::Set(aObjNode->Label(), aFuncLabel);
    }

    /// Set a context of the function
    if (!aContext.IsNull()) {
        TDF_Reference::Set(aFuncNode->Label(), aContext);
    }

    return aFuncNode;
}

////=======================================================================
//// function : AddObject
//// purpose  :
////=======================================================================
// Handle(TDataStd_TreeNode) OCAFSample_IObject::AddObject(const TDF_Label& theAccessLabel)
//{
//     /// MakeBoxの場合: theAccessLabel == Root label
//     if (theAccessLabel.IsNull()) {
//         return Handle(TDataStd_TreeNode)();
//     }

//    /// aRoot == Root label
//    TDF_Label aRootLabel = theAccessLabel.Root();
//    CALLOUT << VAROUT(util::getEntry(aRootLabel));

//    Handle(TDataStd_TreeNode) aRootNode;
//    if (!aRootLabel.FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), aRootNode)) {
//        return Handle(TDataStd_TreeNode)();
//    }

//    /// "Model1" に新しい子Label "Box" ができる?
//    TDF_Label anObjectLabel = aRootLabel.NewChild();
//    CALLOUT << VAROUT(util::getEntry(anObjectLabel));

//    TDataStd_UAttribute::Set(anObjectLabel, GetObjectID());
//    Handle(TDataStd_TreeNode) anObjectNode = TDataStd_TreeNode::Set(anObjectLabel);

//    aRootNode->Append(anObjectNode);

//    return anObjectNode;
//}

//=======================================================================
// function : AddLabels
// purpose  : Adds all result labels of the object to theLog
//=======================================================================
void OCAFSample_IObject::AddLabels(const Handle(TDataStd_TreeNode) & theFunctionNode,
                                   Handle(TFunction_Logbook) & theLog)
{
    if (theFunctionNode.IsNull()) return;

    Handle(TDataStd_TreeNode) aNode = theFunctionNode->Previous();

    while (!aNode.IsNull()) {
        TDF_ChildIterator anIterator;
        anIterator.Initialize(aNode->Label().FindChild(tag::Object::Results));

        for (; anIterator.More(); anIterator.Next()) {
            if (anIterator.Value().IsAttribute(TNaming_NamedShape::GetID())) {
                theLog->SetImpacted(anIterator.Value());
            }
        }

        aNode = aNode->Previous();
    }
}

//=======================================================================
// function : IsAuxiliryObject
// purpose  :
//=======================================================================
Standard_Boolean OCAFSample_IObject::IsAuxiliryObject(const Handle(TDataStd_TreeNode) & theObject)
{
    if (theObject->First().IsNull()) return Standard_False;

    Handle(TFunction_Function) aFunction;
    if (!theObject->First()->FindAttribute(TFunction_Function::GetID(), aFunction)) return Standard_False;

    if (aFunction->GetDriverGUID() == OCAFSample_ISelection::GetID()) return Standard_True;

    return Standard_False;
}

//=======================================================================
// function : RemoveObject
// purpose  :
//=======================================================================
void OCAFSample_IObject::RemoveObject(const Handle(TDataStd_TreeNode) & theObject)
{
    if (theObject.IsNull()) return;

    OCAFSample_IObject anInterface(theObject);

    Handle(TDataStd_TreeNode) aCurrentNode, aRoot, aNode = anInterface.GetLastFunction();
    if (aNode.IsNull()) return;

    aRoot = aNode->Root();

    Handle(TDF_Reference) aRef;
    TDF_ChildIterator anIterator;
    TDF_Label         aLabel;

    while (!aNode.IsNull()) {
        aCurrentNode = aNode->Previous();
        anIterator.Initialize(aNode->Label().FindChild(tag::Object::Argments), Standard_True);

        for (; anIterator.More(); anIterator.Next()) {
            if (!anIterator.Value().FindAttribute(TDF_Reference::GetID(), aRef)) continue;
            aLabel = aRef->Get().Father();
            if (!aLabel.FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), aNode)) continue;
            if (IsAuxiliryObject(aNode)) {
                OCAFSample_IObject IShape(aNode);
                IShape.Remove();
            }
            else
                RemoveObject(aNode);
        }

        aNode = aCurrentNode;
    }

    theObject->Remove();
    theObject->ForgetAllAttributes(Standard_True);
}

////=======================================================================
//// function : GetRootNode
//// purpose  :
////=======================================================================
// Handle(TDataStd_TreeNode) OCAFSample_IObject::GetRootNode(const TDF_Label& theAccessLabel)
//{
//     Handle(TDataStd_TreeNode) aNode;
//     if (!theAccessLabel.IsNull()) theAccessLabel.Root().FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), aNode);
//     return aNode;
// }

////=======================================================================
//// function : GetNode
//// purpose  :
////=======================================================================
// Handle(TDataStd_TreeNode) OCAFSample_IObject::GetNode(const TopoDS_Shape& theShape, const TDF_Label& theAccessLabel)
//{
//     Handle(TDataStd_TreeNode) aNode;
//     if (theAccessLabel.IsNull()) return aNode;

//    Handle(TNaming_NamedShape) aNS = TNaming_Tool::NamedShape(theShape, theAccessLabel);
//    if (aNS.IsNull()) return aNode;

//    TDF_Label aLabel = aNS->Label().Father();                              // Function label
//    aLabel.FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), aNode);    // Return Node of the function

//    if (!aNode.IsNull()) aNode = aNode->Father();

//    return aNode;
//}

////=======================================================================
//// function : MoveObject
//// purpose  :
////=======================================================================
// void OCAFSample_IObject::MoveObject(const Handle(TDataStd_TreeNode) & theObject)
//{
//     if (theObject.IsNull()) return;

//    OCAFSample_IObject anInterface(theObject);

//    Handle(TDataStd_TreeNode) aCurrentNode, aRoot, aNode = anInterface.GetLastFunction();
//    if (aNode.IsNull()) return;

//    aRoot = aNode->Root();

//    Handle(TDF_Reference) aRef;
//    TDF_ChildIterator anIterator;
//    TDF_Label         aLabel;

//    while (!aNode.IsNull()) {
//        aCurrentNode = aNode->Previous();
//        anIterator.Initialize(aNode->Label().FindChild(tag::Object::Argments), Standard_True);

//        for (; anIterator.More(); anIterator.Next()) {
//            if (!anIterator.Value().FindAttribute(TDF_Reference::GetID(), aRef)) continue;
//            aLabel = aRef->Get().Father();
//            if (!aLabel.FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), aNode)) continue;
//            if (IsAuxiliryObject(aNode)) continue;

//            OCAFSample_IDisplayer::Display(aNode, Standard_False);
//        }

//        aNode = aCurrentNode;
//    }
//}

///=======================================================================
/// function : GetFunctionResult
/// purpose  :
///=======================================================================
TopoDS_Shape OCAFSample_IObject::GetFunctionResult(const Handle(TDataStd_TreeNode) & theFunction)
{
    if (theFunction.IsNull()) {
        return TopoDS_Shape();
    }

    TDF_Label aLabel = theFunction->Label().FindChild(tag::Object::Results);
    if (aLabel.IsNull()) {
        return TopoDS_Shape();
    }

    Handle(TNaming_NamedShape) aNS;
    if (aLabel.FindAttribute(TNaming_NamedShape::GetID(), aNS)) {
        return aNS->Get();
    }

    return TopoDS_Shape();
}

////=======================================================================
//// function : HasName
//// purpose  :
////=======================================================================

// Standard_Boolean OCAFSample_IObject::HasName() const
//{
//     if (myTreeNode.IsNull()) return Standard_False;

//    Handle(TDataStd_Name) aName;
//    return myTreeNode->Label().FindAttribute(TDataStd_Name::GetID(), aName);
//}

////=======================================================================
//// function : GetName
//// purpose  :
////=======================================================================

// TCollection_ExtendedString OCAFSample_IObject::GetName() const
//{
//     TCollection_ExtendedString anExtendedString;
//     if (myTreeNode.IsNull()) return anExtendedString;

//    Handle(TDataStd_Name) aName;
//    if (!myTreeNode->Label().FindAttribute(TDataStd_Name::GetID(), aName)) return anExtendedString;

//    anExtendedString = aName->Get();
//    return anExtendedString;
//}

////=======================================================================
//// function : HasCenter
//// purpose  :
////=======================================================================

// Standard_Boolean OCAFSample_IObject::HasCenter() const
//{
//     if (myTreeNode.IsNull()) return Standard_False;

//    TDF_Label L = myTreeNode->Label().FindChild(tag::Object::Argments, Standard_False);
//    if (L.IsNull()) return Standard_False;

//    Handle(TDataStd_Real) aReal;
//    TDF_Label L1;

//    L1 = L.FindChild(X_CENTER_TAG, Standard_False);
//    if (L1.IsNull()) return Standard_False;
//    if (!L1.FindAttribute(TDataStd_Real::GetID(), aReal)) return Standard_False;

//    L1 = L.FindChild(Y_CENTER_TAG, Standard_False);
//    if (L1.IsNull()) return Standard_False;
//    if (!L1.FindAttribute(TDataStd_Real::GetID(), aReal)) return Standard_False;

//    L1 = L.FindChild(Z_CENTER_TAG, Standard_False);
//    if (L1.IsNull()) return Standard_False;
//    if (!L1.FindAttribute(TDataStd_Real::GetID(), aReal)) return Standard_False;

//    return Standard_True;
//}

////=======================================================================
//// function : CanRemove
//// purpose  :
////=======================================================================
// Standard_Boolean OCAFSample_IObject::CanRemove() const
//{
//     return !HasReferencedObjects();
// }

////=======================================================================
//// function : GetReferencedObjects
//// purpose  :
////=======================================================================
// void OCAFSample_IObject::GetReferencedObjects(TDF_AttributeMap& theObjectMap) const
//{
//     TDF_AttributeMap    aMap, aFunctionMap;
//     OCAFSample_SolverEx aSolver;
//     Handle(TDataStd_TreeNode) anObject, aNode;
//     Handle(TFunction_Function) aFunction;
//     TDF_MapIteratorOfAttributeMap aMapIterator;

//    if (myTreeNode->First().IsNull()) return;

//    TDataStd_ChildNodeIterator anIterator(myTreeNode);

//    for (; anIterator.More(); anIterator.Next())
//        if (anIterator.Value()->FindAttribute(TFunction_Function::GetID(), aFunction)) aFunctionMap.Add(aFunction);

//    anIterator.Initialize(myTreeNode->Root(), Standard_True);

//    for (; anIterator.More(); anIterator.Next()) {
//        if (anIterator.Value() == myTreeNode) continue;

//        OCAFSample_IObject anInterface(anIterator.Value());
//        anObject = anInterface.GetObjectNode();

//        if (theObjectMap.Contains(anObject)) continue;

//        aMap.Clear();

//        if (anIterator.Value()->FindAttribute(TFunction_Function::GetID(), aFunction)) {
//            if (aFunctionMap.Contains(aFunction)) continue;
//            aSolver.ComputeFunction(aFunction, aMap);

//            for (aMapIterator.Initialize(aFunctionMap); aMapIterator.More(); aMapIterator.Next()) {
//                aFunction = Handle(TFunction_Function)::DownCast(aMapIterator.Key());
//                if (!aFunction.IsNull() && aMap.Contains(aFunction)) {
//                    aNode = anObject;

//                    while (!aNode.IsNull() || aNode != myTreeNode->Root()) {
//                        if (aNode->Father() == myTreeNode->Root()) {
//                            theObjectMap.Add(anObject);
//                            break;
//                        }
//                        aNode = aNode->Father();
//                        if (aNode.IsNull() || aNode == myTreeNode) break;
//                    }

//                    break;
//                }
//            }
//        }
//    }
//}

////=======================================================================
//// function : GetPresentation
//// purpose  :
////=======================================================================
// Handle(TPrsStd_AISPresentation) OCAFSample_IObject::GetPresentation() const
//{
//     TDF_Label aResultsLabel = myTreeNode->Label();
//     Handle(TPrsStd_AISPresentation) aPresenation;
//     aResultsLabel.FindAttribute(TPrsStd_AISPresentation::GetID(), aPresenation);

//    return aPresenation;
//}

////=======================================================================
//// function : AddSelectionArgument
//// purpose  :
////=======================================================================
// Standard_Boolean OCAFSample_IObject::AddSelectionArgument(const int theTag, const Handle(TDataStd_TreeNode) &
// theObject)
//{
//     if (theObject.IsNull()) return Standard_False;

//    Handle(TDataStd_TreeNode) aFunctionNode = (OCAFSample_IObject(theObject)).GetLastFunction();
//    if (aFunctionNode.IsNull()) return Standard_False;

//    OCAFSample_SolverEx aSolver;
//    if (aSolver.IsCyclicLink(aFunctionNode->Label(), myTreeNode->Label())) return Standard_False;

//    TDF_Label L      = myTreeNode->Label().FindChild(tag::Object::Argments);
//    TDF_Label aLabel = L.FindChild(theTag);
//    Handle(TDF_Reference) aRef;
//    Handle(TDataStd_TreeNode) aNode;

//    if (aLabel.FindAttribute(TDF_Reference::GetID(), aRef)) {
//        if (aRef->Get().FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), aNode)) {
//            aNode = aNode->Father();
//            if (!aNode.IsNull()) {
//                aNode->Remove();
//                aNode->ForgetAllAttributes(Standard_True);    // Delete the old selected edge
//            }
//        }
//    }

//    TDF_Reference::Set(aLabel, aFunctionNode->Label());    // Set a reference to the function that produced <theBasis>

//    // Detach the basis from the root node (capture)

//    theObject->Remove();
//    myTreeNode->Append(theObject);

//    return Standard_True;
//}

////=======================================================================
//// function : GetSelectionArgument
//// purpose  :
////=======================================================================
// TopoDS_Shape OCAFSample_IObject::GetSelectionArgument(const int theTag) const
//{
//     TopoDS_Shape aShape;
//     Handle(TDF_Reference) aReference;

//    TDF_Label aLabel = myTreeNode->Label().FindChild(tag::Object::Argments).FindChild(theTag);
//    if (aLabel.FindAttribute(TDF_Reference::GetID(), aReference)) {
//        aLabel = aReference->Get().FindChild(tag::Object::Results);
//        Handle(TNaming_NamedShape) aNamedShape;
//        if (aLabel.FindAttribute(TNaming_NamedShape::GetID(), aNamedShape)) return aNamedShape->Get();
//    }

//    return aShape;
//}
