// Created: 2002-03-06
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

#include "OCAFSample_SolverEx.hxx"
#include "OCAFSample_Driver.hxx"
#include "OCAFSample_IObject.hxx"
#include "OCAFSample_ISelection.hxx"
#include "Tags.h"

#include <TCollection_AsciiString.hxx>
#include <TDF_AttributeMap.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_LabelMap.hxx>
#include <TDF_ListIteratorOfAttributeList.hxx>
#include <TDF_MapIteratorOfLabelMap.hxx>
#include <TDF_Reference.hxx>
#include <TDF_Tool.hxx>
#include <TDataStd_ChildNodeIterator.hxx>
#include <TDocStd_Modified.hxx>
#include <TFunction_DriverTable.hxx>
#include <TFunction_Function.hxx>
#include <TFunction_Logbook.hxx>

#include "Defines.h"
#include "Util.h"

//=======================================================================
// function : ComputeExecutionList
// purpose  :
//=======================================================================
void OCAFSample_SolverEx::ComputeExecutionList(const TDF_Label& theAccessLabel)
{
    TDF_Label aRootLabel = theAccessLabel.Root();
    Handle(TDataStd_TreeNode) aRoot;
    if (!aRootLabel.FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), aRoot)) {
        return;
    }

    myList.Clear();

    TDF_AttributeMap aMap;
    TDF_AttributeMap iterated_nodes;

    /// Falseを渡すと直接の子のみを取得する.
    /// この場合はRootの子のMainのみ.
    TDataStd_ChildNodeIterator anIterator(aRoot, Standard_True);

    /// ここで無限ループしてしまっている.
    for (; anIterator.More(); anIterator.Next()) {
        Handle(TDataStd_TreeNode) aNode = anIterator.Value();

        if (iterated_nodes.Contains(aNode)) {
            return;
        }

        iterated_nodes.Add(aNode);

        /// It's not an object
        if (!aNode->IsAttribute(OCAFSample_IObject::GetObjectID())) {
            continue;
        }

        aNode = aNode->First();

        while (!aNode.IsNull()) {
            auto label = aNode->Label();

            Handle(TFunction_Function) aFunction;
            if (label.FindAttribute(TFunction_Function::GetID(), aFunction)) {
                ComputeFunction(aFunction, aMap);
            }

            aNode = aNode->Next();
        }
    }
}

//=======================================================================
// function : Solve
// purpose  : Rebuilds the whole model stored in a document given by theAccessLabel
//=======================================================================
Standard_Boolean OCAFSample_SolverEx::Solve(const TDF_Label& theAccessLabel)
{
    TDF_Label aRootLabel = theAccessLabel.Root();
    Handle(TDataStd_TreeNode) aRootNode;
    if (!aRootLabel.FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), aRootNode)) {
        return Standard_False;
    }

    // Clear the modifications
    TDocStd_Modified::Clear(aRootLabel);
    myList.Clear();

    Handle(TFunction_Logbook) aLog = TFunction_Logbook::Set(aRootLabel);
    aLog->Clear();

    // Build myList that contains an order of the execution of functions

    ComputeExecutionList(theAccessLabel);

    return PerformSolving(aLog, NULL, Standard_False);
}

//=======================================================================
// function : SolveFrom
// purpose  : Rebuilds the model starting from given by theNode object
//=======================================================================
Standard_Boolean OCAFSample_SolverEx::SolveFrom(const Handle(TDataStd_TreeNode) & theNode /*: function node*/)
{
    if (theNode.IsNull()) {
        return Standard_False;
    }

    TDocStd_Modified::Clear(theNode->Label());
    myList.Clear();

    Handle(TFunction_Logbook) aLog = TFunction_Logbook::Set(theNode->Label());
    aLog->Clear();

    ///---------------------------------------------------------------------------------------------------
    /// Build an execution list for the whole model. Not optimal but simple solution
    ///---------------------------------------------------------------------------------------------------
    ComputeExecutionList(theNode->Label());

    ///---------------------------------------------------------------------------------------------------
    /// Recompute the function
    ///---------------------------------------------------------------------------------------------------
    Handle(TFunction_Function) aFunction;
    if (!theNode->FindAttribute(TFunction_Function::GetID(), aFunction)) {
        return Standard_False;
    }

    Handle(TFunction_Driver) aDriver;
    TFunction_DriverTable::Get()->FindDriver(aFunction->GetDriverGUID(), aDriver);
    Handle(OCAFSample_Driver) anOCAFDriver = Handle(OCAFSample_Driver)::DownCast(aDriver);

    if (!anOCAFDriver.IsNull()) {
        anOCAFDriver->Init(aFunction->Label());

        const Standard_Integer aStatus = anOCAFDriver->Execute(aLog);
        if (aStatus > 0) {
            return Standard_False;
        }
    }

    ///---------------------------------------------------------------------------------------------------
    /// Recompute other functions which depends on this function (aFunction)
    /// The order of the execution is given by myList. Only functions with modified
    /// arguments will be recomputed (check MustExecute)
    ///---------------------------------------------------------------------------------------------------
    return PerformSolving(aLog, aFunction, Standard_True);
}

//=======================================================================
// function : IsCyclicLink
// purpose  : Returns True if link between theFrom and theTo is cyclic
//=======================================================================
Standard_Boolean OCAFSample_SolverEx::IsCyclicLink(const TDF_Label& theFrom, const TDF_Label& theTo)
{
    Handle(TFunction_Function) aFromFunction, aToFunction;
    TDF_AttributeMap aMap;

    // Find functions
    theFrom.FindAttribute(TFunction_Function::GetID(), aFromFunction);
    theTo.FindAttribute(TFunction_Function::GetID(), aToFunction);

    // Find dependencies of the aToFunction
    if (aFromFunction.IsNull() || aToFunction.IsNull()) return Standard_False;
    ComputeFunction(aToFunction, aMap);

    if (aMap.Contains(aFromFunction)) return Standard_True;
    return Standard_False;
}

//=======================================================================
// function : ComputeFunction
// purpose  : Finds dependecies of the function
//=======================================================================
Standard_Boolean OCAFSample_SolverEx::ComputeFunction(const Handle(TFunction_Function) & theFunction,
                                                      TDF_AttributeMap& theSolved)
{
    if (theSolved.Contains(theFunction)) {
        return Standard_True;
    }

    // Find function driver
    Handle(TFunction_Driver) aDriver;
    TFunction_DriverTable::Get()->FindDriver(theFunction->GetDriverGUID(), aDriver);
    Handle(OCAFSample_Driver) anOCAFDriver = Handle(OCAFSample_Driver)::DownCast(aDriver);

    if (anOCAFDriver.IsNull()) {
        theSolved.Add(theFunction);
        myList.Append(theFunction);
        return Standard_True;
    }

    // Find arguments of the function

    TDF_LabelMap aMap;
    Handle(TFunction_Function) aFunction;
    anOCAFDriver->Init(theFunction->Label());
    anOCAFDriver->Arguments(aMap);

    TDF_MapIteratorOfLabelMap anIterator(aMap);
    for (; anIterator.More(); anIterator.Next()) {
        if (anIterator.Key().FindAttribute(TFunction_Function::GetID(), aFunction)) {
            if (theSolved.Contains(aFunction)) {
                continue;
            }
            ComputeFunction(aFunction, theSolved);    // Compute all function that produce arguments
        }
    }

    theSolved.Add(theFunction);
    myList.Append(theFunction);
    return Standard_True;
}

//=======================================================================
// function : PerformSolving
// purpose  : Calls driver method Execute for functions
//=======================================================================
Standard_Boolean OCAFSample_SolverEx::PerformSolving(Handle(TFunction_Logbook) & theLogbook,
                                                     const Handle(TFunction_Function) & theSkipFunction,
                                                     const Standard_Boolean theWithCheck) const
{
    TDF_ListIteratorOfAttributeList anAttrIter(myList);

    if (myList.IsEmpty()) {
        return false;
    }

    // Iterate all functions in the myList
    for (; anAttrIter.More(); anAttrIter.Next()) {
        Handle(TFunction_Function) aFunction = Handle(TFunction_Function)::DownCast(anAttrIter.Value());

        if (aFunction.IsNull()) {
            return Standard_False;
        }

        if (aFunction == theSkipFunction) {
            continue;    // Already computed in SolveFrom method
        }

        Handle(TFunction_Driver) aDriver;
        TFunction_DriverTable::Get()->FindDriver(aFunction->GetDriverGUID(), aDriver);

        Handle(OCAFSample_Driver) anOCAFDriver = Handle(OCAFSample_Driver)::DownCast(aDriver);

        if (anOCAFDriver.IsNull()) {
            continue;
        }

        anOCAFDriver->Init(aFunction->Label());
        if (theWithCheck && !anOCAFDriver->MustExecute(theLogbook)) {
            continue;    // No need to execute
        }

        const Standard_Integer aStatus = anOCAFDriver->Execute(theLogbook);
        if (aStatus > 0) {
            return Standard_False;
        }
    }

    MYLOG << "success";
    return Standard_True;
}

////=======================================================================
//// function : GetAttachments
//// purpose  : Fills the list with attachemnts of the function
////=======================================================================
// void OCAFSample_SolverEx::GetAttachments(const Handle(TFunction_Function) & theFunction, TDF_AttributeMap& theMap)
// const
//{
//     Handle(TDataStd_TreeNode) aNode;
//     Handle(TFunction_Function) aFunction;
//     Handle(TDF_Reference) aRef;
//     if (theFunction.IsNull()) return;

//    if (!theFunction->FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), aNode)) return;

//    TDataStd_ChildNodeIterator anIterator(aNode->Root(), Standard_True);

//    for (; anIterator.More(); anIterator.Next()) {
//        if (anIterator.Value()->FindAttribute(TFunction_Function::GetID(), aFunction)) {
//            if (aFunction == theFunction) continue;
//            if (aFunction->GetDriverGUID() == OCAFSample_ISelection::GetID()) {
//                TDF_ChildIterator anItr(aFunction->Label().FindChild(tag::Object::Argments));
//                for (; anItr.More(); anItr.Next()) {
//                    if (anItr.Value().FindAttribute(TDF_Reference::GetID(), aRef)) {
//                        if (aRef->Get() == theFunction->Label()) {
//                            if (!aFunction->FindAttribute(TDataStd_TreeNode::GetDefaultTreeID(), aNode)) continue;
//                            theMap.Add(aNode);
//                            break;
//                        }
//                    }
//                }
//            }
//        }
//    }
//}

//=======================================================================
// function : Dump
// purpose  : Service function
//=======================================================================
void OCAFSample_SolverEx::Dump() const
{
    TCollection_AsciiString         anEntry;
    TDF_ListIteratorOfAttributeList anIterator(myList);

    std::cout << "Execution list contains the following functions: " << std::endl;
    for (; anIterator.More(); anIterator.Next()) {
        TDF_Tool::Entry(anIterator.Value()->Label(), anEntry);
        std::cout << "   " << anEntry << std::endl;
    }
}
