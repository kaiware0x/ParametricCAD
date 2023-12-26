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

#ifndef _BRepNaming_Loader_HeaderFile
#define _BRepNaming_Loader_HeaderFile

#include "BRepNaming_LoaderParent.hxx"

#include <TopAbs_ShapeEnum.hxx>
#include <TopTools_DataMapOfShapeShape.hxx>
#include <TopTools_MapOfShape.hxx>

class BRepBuilderAPI_MakeShape;
class TopoDS_Shape;
class TNaming_Builder;
class TDF_Label;

class BRepNaming_Loader : public BRepNaming_LoaderParent {
public:
    static void LoadGeneratedShapes(BRepBuilderAPI_MakeShape& MakeShape, const TopoDS_Shape& ShapeIn,
                                    const TopAbs_ShapeEnum GeneratedFrom, TNaming_Builder& Buider);
    static void LoadSeparatelyGeneratedShapes(BRepBuilderAPI_MakeShape& MakeShape, const TopoDS_Shape& ShapeIn,
                                              const TopAbs_ShapeEnum GeneratedFrom, const TDF_Label& theFatherlabel);
    static void LoadModifiedShapes(BRepBuilderAPI_MakeShape& MakeShape, const TopoDS_Shape& ShapeIn,
                                   const TopAbs_ShapeEnum ModifiedFrom, TNaming_Builder& Buider,
                                   const Standard_Boolean theBool = Standard_False);
    static void LoadSeparatelyModifiedShapes(BRepBuilderAPI_MakeShape& MakeShape, const TopoDS_Shape& ShapeIn,
                                             const TopAbs_ShapeEnum ModifiedFrom, const TDF_Label& theFatherlabel);
    static void LoadDeletedShapes(BRepBuilderAPI_MakeShape& MakeShape, const TopoDS_Shape& ShapeIn,
                                  const TopAbs_ShapeEnum KindOfDeletedShape, TNaming_Builder& Buider);
    static void LoadSeparatelyDeletedShapes(BRepBuilderAPI_MakeShape& MakeShape, const TopoDS_Shape& ShapeIn,
                                            const TopAbs_ShapeEnum KindOfDeletedShape, const TDF_Label& theFatherlabel);
    static void LoadAndOrientGeneratedShapes(BRepBuilderAPI_MakeShape& MakeShape, const TopoDS_Shape& ShapeIn,
                                             const TopAbs_ShapeEnum GeneratedFrom, TNaming_Builder& Buider,
                                             const TopTools_DataMapOfShapeShape& SubShapesOfResult);
    static void LoadAndOrientModifiedShapes(BRepBuilderAPI_MakeShape& MakeShape, const TopoDS_Shape& ShapeIn,
                                            const TopAbs_ShapeEnum ModifiedFrom, TNaming_Builder& Buider,
                                            const TopTools_DataMapOfShapeShape& SubShapesOfResult);
    static void ModifyPart(const TopoDS_Shape& PartShape, const TopoDS_Shape& Primitive, const TDF_Label& Label);
    static Standard_Boolean HasDangleShapes(const TopoDS_Shape& ShapeIn);
    static void             LoadGeneratedDangleShapes(const TopoDS_Shape& ShapeIn, const TopAbs_ShapeEnum GeneratedFrom,
                                                      TNaming_Builder& GenBuider);
    static void             LoadGeneratedDangleShapes(const TopoDS_Shape& ShapeIn, const TopAbs_ShapeEnum GeneratedFrom,
                                                      const TopTools_MapOfShape& OnlyThese, TNaming_Builder& GenBuider);
    static void             LoadModifiedDangleShapes(BRepBuilderAPI_MakeShape& MakeShape, const TopoDS_Shape& ShapeIn,
                                                     const TopAbs_ShapeEnum GeneratedFrom, TNaming_Builder& GenBuider);
    static void             LoadDeletedDangleShapes(BRepBuilderAPI_MakeShape& MakeShape, const TopoDS_Shape& ShapeIn,
                                                    const TopAbs_ShapeEnum ShapeType, TNaming_Builder& DelBuider);
    static void             LoadDangleShapes(const TopoDS_Shape& theShape, const TDF_Label& theLabelGenerator);
    static void             LoadDangleShapes(const TopoDS_Shape& theShape, const TopoDS_Shape& ignoredShape,
                                             const TDF_Label& theLabelGenerator);
    static Standard_Boolean GetDangleShapes(const TopoDS_Shape& ShapeIn, const TopAbs_ShapeEnum GeneratedFrom,
                                            TopTools_DataMapOfShapeShape& Dangles);
    static Standard_Boolean GetDangleShapes(const TopoDS_Shape& ShapeIn, const TopAbs_ShapeEnum GeneratedFrom,
                                            TopTools_MapOfShape& Dangles);
    static Standard_Boolean IsDangle(const TopoDS_Shape& theDangle, const TopoDS_Shape& theShape);
};

#endif
