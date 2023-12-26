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

#ifndef _OCAFSample_IBox_HeaderFile
#define _OCAFSample_IBox_HeaderFile

#include <TDataStd_Real.hxx>
#include <TDataStd_TreeNode.hxx>

#include "OCAFSample_IShape.hxx"

#include "values.h"

class Standard_GUID;
class TDataStd_TreeNode;
class TCollection_ExtendedString;
class TDataStd_Real;

class OCAFSample_IBox : public OCAFSample_IShape {
public:
    static const Standard_GUID& GetID();
    static Handle(TDataStd_TreeNode) MakeBox(const BoxValue& param, const Handle(TDataStd_TreeNode) & AccessTreeNode,
                                             TCollection_ExtendedString& theError);
    OCAFSample_IBox(const Handle(TDataStd_TreeNode) & aTreeNode) : OCAFSample_IShape(aTreeNode) {}
    Handle(TDataStd_Real) GetDX() const;
    Handle(TDataStd_Real) GetDY() const;
    Handle(TDataStd_Real) GetDZ() const;
};

#endif
