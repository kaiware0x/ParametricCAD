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

#ifndef _OCAFSample_ISelection_HeaderFile
#define _OCAFSample_ISelection_HeaderFile

#include "OCAFSample_IShape.hxx"

#include <TDataStd_TreeNode.hxx>

class Standard_GUID;
class TDataStd_TreeNode;
class TCollection_ExtendedString;
class TopoDS_Shape;

class OCAFSample_ISelection : public OCAFSample_IShape {
public:
    static const Standard_GUID& GetID();
    static Handle(TDataStd_TreeNode)
        MakeSelect(const TCollection_ExtendedString& theName, const TopoDS_Shape& theShape,
                   const TopoDS_Shape&         theContext, const Handle(TDataStd_TreeNode) & theAccessTreeNode,
                   TCollection_ExtendedString& theError);
    OCAFSample_ISelection(const Handle(TDataStd_TreeNode) & aTreeNode) : OCAFSample_IShape(aTreeNode) {}
    Standard_Boolean Select(const TopoDS_Shape& theShape, const TopoDS_Shape& theContext);
    TopoDS_Shape     GetContext() const;
};

#endif
