// Created: 2002-02-21
//
// Copyright (c) 2001-2013 OPEN CASCADE SAS
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

#ifndef _OCAFSample_SolverEx_HeaderFile
#define _OCAFSample_SolverEx_HeaderFile

#include <TDF_AttributeList.hxx>
#include <TDF_AttributeMap.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TFunction_Function.hxx>

class TDF_Label;
class TDataStd_TreeNode;
class TFunction_Function;
class TFunction_Logbook;

class OCAFSample_SolverEx {
public:
    OCAFSample_SolverEx() {}
    Standard_Boolean Solve(const TDF_Label& theAccessLable);
    Standard_Boolean SolveFrom(const Handle(TDataStd_TreeNode) & theNode);
    Standard_Boolean IsCyclicLink(const TDF_Label& theFrom, const TDF_Label& theTo);
    Standard_Boolean ComputeFunction(const Handle(TFunction_Function) & theFunction, TDF_AttributeMap& theSolved);
    void             ComputeExecutionList(const TDF_Label& theAccessLabel);
    const TDF_AttributeList& GetExecutionList() const { return myList; }
    //    void GetAttachments(const Handle(TFunction_Function) & theFunction, TDF_AttributeMap& theMap) const;

    void Dump() const;

private:
    Standard_Boolean PerformSolving(Handle(TFunction_Logbook) & theLogbook,
                                    const Handle(TFunction_Function) & theSkipFunction,
                                    const Standard_Boolean theWithCheck) const;

    TDF_AttributeList myList;
};

#endif
