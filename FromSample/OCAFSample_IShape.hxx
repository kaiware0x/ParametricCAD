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

#ifndef _OCAFSample_IShape_HeaderFile
#define _OCAFSample_IShape_HeaderFile

#include <TDataStd_TreeNode.hxx>

#include "OCAFSample_IObject.hxx"
#include "OCAFSample_ObjectType.hxx"

class TDataStd_TreeNode;
class Standard_GUID;

class OCAFSample_IShape : public OCAFSample_IObject {
public:
    OCAFSample_IShape(const Handle(TDataStd_TreeNode) & aTreeNode) : OCAFSample_IObject(aTreeNode) {}

    static OCAFSample_ObjectType GetObjectType(const Handle(TDataStd_TreeNode) & Object);
    static Standard_Boolean      IsBoolean(const Standard_GUID& theID);
};

#endif
