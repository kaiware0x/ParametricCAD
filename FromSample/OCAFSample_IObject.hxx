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

#ifndef _OCAFSample_IObject_HeaderFile
#define _OCAFSample_IObject_HeaderFile

#include <TDF_AttributeMap.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TFunction_Logbook.hxx>
#include <TPrsStd_AISPresentation.hxx>

class TDataStd_TreeNode;
class TDF_Label;
class TopoDS_Shape;
class TCollection_ExtendedString;
class TPrsStd_AISPresentation;

class OCAFSample_IObject {
public:
    OCAFSample_IObject(const Handle(TDataStd_TreeNode) & aTreeNode);
    void             SetName(const TCollection_ExtendedString& aName);
    void             SetCenter(const Standard_Real X, const Standard_Real Y, const Standard_Real Z);
    void             SetCenter(const gp_Pnt& center);
    void             GetCenter(Standard_Real& X, Standard_Real& Y, Standard_Real& Z) const;
    Standard_Boolean Remove();
    Standard_Boolean HasReferencedObjects() const;
    Handle(TDataStd_TreeNode) GetLastFunction() const;
    TopoDS_Shape GetObjectValue() const;
    Handle(TDataStd_TreeNode) GetObjectNode() const;
    Handle(TDataStd_TreeNode) AddFunction(const Standard_GUID& theID);

    static const Standard_GUID& GetObjectID();
    static Handle(TDataStd_TreeNode) AddObject(const TDF_Label& parent_label);
    //    static Handle(TDataStd_TreeNode) AddObject(const TDF_Label& theAccessLabel);
    static void AddLabels(const Handle(TDataStd_TreeNode) & aFunctionNode, Handle(TFunction_Logbook) & theLog);
    static Standard_Boolean IsAuxiliryObject(const Handle(TDataStd_TreeNode) & theObject);
    static void             RemoveObject(const Handle(TDataStd_TreeNode) & theObject);

    //    static Handle(TDataStd_TreeNode) GetRootNode(const TDF_Label& theAccessLabel);
    //    static Handle(TDataStd_TreeNode) GetNode(const TopoDS_Shape& theShape, const TDF_Label& theAccessLabel);
    //    static void             MoveObject(const Handle(TDataStd_TreeNode) & theObject);
    static TopoDS_Shape GetFunctionResult(const Handle(TDataStd_TreeNode) & theFunction);

    //    const Handle(TDataStd_TreeNode) & GetTreeNode() const { return myTreeNode; }
    //    Standard_Boolean           HasName() const;
    //    TCollection_ExtendedString GetName() const;
    //    Standard_Boolean           HasCenter() const;
    //    Standard_Boolean           CanRemove() const;
    //    void                       GetReferencedObjects(TDF_AttributeMap& theObjectMap) const;
    //    Handle(TPrsStd_AISPresentation) GetPresentation() const;
    //    Standard_Boolean AddSelectionArgument(const int theTag, const Handle(TDataStd_TreeNode) & theObject);
    //    TopoDS_Shape     GetSelectionArgument(const int theTag) const;

protected:
    Handle(TDataStd_TreeNode) myTreeNode;
};

#endif
