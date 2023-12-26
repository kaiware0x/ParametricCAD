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

#ifndef _BRepNaming_LoaderParent_HeaderFile
#define _BRepNaming_LoaderParent_HeaderFile

#include <TopAbs_ShapeEnum.hxx>
#include <TopTools_DataMapOfShapeShape.hxx>

class TopoDS_Shape;
class TNaming_Builder;

class BRepNaming_LoaderParent {
public:
    static void             LoadGeneratedDangleShapes(const TopoDS_Shape& ShapeIn, const TopAbs_ShapeEnum GeneratedFrom,
                                                      TNaming_Builder& GenBuider);
    static Standard_Boolean GetDangleShapes(const TopoDS_Shape& ShapeIn, const TopAbs_ShapeEnum GeneratedFrom,
                                            TopTools_DataMapOfShapeShape& Dangles);
};

#endif
