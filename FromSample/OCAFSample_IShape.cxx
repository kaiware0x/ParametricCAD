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

#include "OCAFSample_IShape.hxx"
// #include "OCAFSample_IBRepImport.hxx"
#include "OCAFSample_IBox.hxx"
// #include "OCAFSample_ICommon.hxx"
// #include "OCAFSample_ICut.hxx"
// #include "OCAFSample_ICylinder.hxx"
#include "OCAFSample_IDisplayer.hxx"
// #include "OCAFSample_IFillet.hxx"
// #include "OCAFSample_IFuse.hxx"
// #include "OCAFSample_IPrism.hxx"
#include "OCAFSample_ISelection.hxx"
// #include "OCAFSample_ISphere.hxx"
// #include "OCAFSample_ITransform.hxx"
#include "OCAFSample_SolverEx.hxx"
#include "Tags.h"

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
#include <TNaming_Iterator.hxx>
#include <TNaming_NamedShape.hxx>
#include <TNaming_Tool.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>

//=======================================================================
// function : GetObjectType
// purpose  :
//=======================================================================

OCAFSample_ObjectType OCAFSample_IShape::GetObjectType(const Handle(TDataStd_TreeNode) & Object)
{
    Handle(TFunction_Function) aFunction;
    if (!Object->FindAttribute(TFunction_Function::GetID(), aFunction)) {
        return OCAFSample_NotDefinedObjectType;
    }

    const Standard_GUID DriverGUID = aFunction->GetDriverGUID();

    if (Standard_GUID::IsEqual(DriverGUID, OCAFSample_IBox::GetID())) return OCAFSample_Box;

    //    if (Standard_GUID::IsEqual(DriverGUID, OCAFSample_ICylinder::GetID())) return OCAFSample_Cylinder;

    //    if (Standard_GUID::IsEqual(DriverGUID, OCAFSample_ISphere::GetID())) return OCAFSample_Sphere;

    //    if (Standard_GUID::IsEqual(DriverGUID, OCAFSample_IPrism::GetID())) return OCAFSample_Prism;

    //    if (Standard_GUID::IsEqual(DriverGUID, OCAFSample_IBRepImport::GetID())) return OCAFSample_BRepImport;

    //    if (Standard_GUID::IsEqual(DriverGUID, OCAFSample_ICut::GetID())) return OCAFSample_Cut;

    //    if (Standard_GUID::IsEqual(DriverGUID, OCAFSample_IFuse::GetID())) return OCAFSample_Fuse;

    if (Standard_GUID::IsEqual(DriverGUID, OCAFSample_ISelection::GetID())) return OCAFSample_Selection;

    //    if (Standard_GUID::IsEqual(DriverGUID, OCAFSample_IFillet::GetID())) return OCAFSample_Fillet;

    //    if (Standard_GUID::IsEqual(DriverGUID, OCAFSample_ITransform::GetID())) return OCAFSample_Transform;

    //    if (Standard_GUID::IsEqual(DriverGUID, OCAFSample_ICommon::GetID())) return OCAFSample_Common;

    return OCAFSample_NotDefinedObjectType;
}

//=======================================================================
// function : IsBoolean
// purpose  : Returns True if theID is an GUID of the Boolean function
//=======================================================================

Standard_Boolean OCAFSample_IShape::IsBoolean(const Standard_GUID& theID)
{
    return false;
    //    return (theID == OCAFSample_ICut::GetID() || theID == OCAFSample_IFuse::GetID()
    //            || theID == OCAFSample_ICommon::GetID());
}
