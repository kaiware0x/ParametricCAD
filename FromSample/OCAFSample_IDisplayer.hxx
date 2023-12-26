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

#ifndef _OCAFSample_IDisplayer_HeaderFile
#define _OCAFSample_IDisplayer_HeaderFile

#include <AIS_InteractiveContext.hxx>
#include <Quantity_NameOfColor.hxx>
#include <TDF_Attribute.hxx>

class TDF_Label;
class TDF_Attribute;
class Quantity_Color;
class AIS_InteractiveContext;

class OCAFSample_IDisplayer {
public:
    static void Display(const Handle(TDF_Attribute) & theAttrib, const Standard_Boolean isUpdateViewer = Standard_True);
    static void Erase(const Handle(TDF_Attribute) & theAttrib, const Standard_Boolean theRemove = Standard_False);
    static void Update(const Handle(TDF_Attribute) & theAttrib);
    static void Update(const TDF_Label& theAccessLabel);
    static void Remove(const Handle(TDF_Attribute) & theAttrib);
    static void UpdateViewer(const TDF_Label& theAccessLabel);
    static void DisplayAll(const TDF_Label& theAccessLabel, const Standard_Boolean isUpdated = Standard_False);
    static void SetTransparency(const Handle(TDF_Attribute) & theAttrib, const Standard_Real theValue);
    static void SetColor(const Handle(TDF_Attribute) & theAttrib, const Quantity_NameOfColor theColor);
    static void SetColor(const Handle(TDF_Attribute) & theAttrib, const Quantity_Color& theColor);
    static void SetColor(const Handle(TDF_Attribute) & theAttrib, const Standard_Integer R, const Standard_Integer G,
                         const Standard_Integer B);
    static Quantity_Color GetColor(const Handle(TDF_Attribute) & theAttrib);
    static void           SetMode(const Handle(TDF_Attribute) & theAttrib, const Standard_Integer theMode);
    static void           SetWidth(const Handle(TDF_Attribute) & theAttrib, const Standard_Real theWidth);
    static void           Hilight(const Handle(TDF_Attribute) & theAttrib, Handle(AIS_InteractiveContext) & iContext);
};

#endif
