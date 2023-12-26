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

#include "OCAFSample_IDisplayer.hxx"
#include "OCAFSample_AISDriver.hxx"
#include "Tags.h"

#include <TDF_ChildIterator.hxx>
#include <TDF_Label.hxx>
#include <TDocStd_Modified.hxx>
#include <TNaming_NamedShape.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TPrsStd_AISViewer.hxx>

#include "Defines.h"
#include "Util.h"

//=======================================================================
// function : Display
// purpose  : Displays the object located on the same label as theAttrib
//=======================================================================

void OCAFSample_IDisplayer::Display(const Handle(TDF_Attribute) & theAttrib, const Standard_Boolean isUpdateViewer)
{
    if (theAttrib.IsNull()) {
        MYWARNLOG << VAROUT(theAttrib.IsNull());
        return;
    }

    TDF_Label ShapeLabel = theAttrib->Label();
    if (ShapeLabel.IsNull()) {
        MYWARNLOG << VAROUT(ShapeLabel.IsNull());
        return;
    }

    Handle(TPrsStd_AISPresentation) aPresentation;
    if (!ShapeLabel.FindAttribute(TPrsStd_AISPresentation::GetID(), aPresentation)) {
        aPresentation = TPrsStd_AISPresentation::Set(ShapeLabel, OCAFSample_AISDriver::GetID());
    }

    aPresentation->Display();

    if (isUpdateViewer) {
        TPrsStd_AISViewer::Update(ShapeLabel);
    }
}

//=======================================================================
// function : Erase
// purpose  : Erases in a viewer the object located on the same label as theAttrib
//=======================================================================

void OCAFSample_IDisplayer::Erase(const Handle(TDF_Attribute) & theAttrib, const Standard_Boolean theRemove)
{
    if (theAttrib.IsNull()) return;

    TDF_Label ShapeLabel = theAttrib->Label();
    if (ShapeLabel.IsNull()) return;

    Handle(TPrsStd_AISPresentation) aPresentation;
    if (ShapeLabel.FindAttribute(TPrsStd_AISPresentation::GetID(), aPresentation)) {
        aPresentation->Erase(theRemove);
        TPrsStd_AISViewer::Update(ShapeLabel);
    }
}

//=======================================================================
// function : Update
// purpose  :
//=======================================================================

void OCAFSample_IDisplayer::Update(const Handle(TDF_Attribute) & theAttrib)
{
    if (theAttrib.IsNull()) return;

    TDF_Label aShapeLabel = theAttrib->Label();
    if (aShapeLabel.IsNull()) return;

    Handle(TPrsStd_AISPresentation) aPresentation;
    if (aShapeLabel.FindAttribute(TPrsStd_AISPresentation::GetID(), aPresentation)) {
        aPresentation->Update();
        TPrsStd_AISViewer::Update(aShapeLabel);
    }
}

//=======================================================================
// function : Update
// purpose  :
//=======================================================================

void OCAFSample_IDisplayer::Update(const TDF_Label& theAccessLabel)
{
    TDF_ChildIterator anIterator(theAccessLabel.Root());
    Handle(TPrsStd_AISPresentation) aPresentation;

    for (; anIterator.More(); anIterator.Next()) {
        MYLOG << VAROUT(util::getName(anIterator.Value()));

        if (anIterator.Value().FindAttribute(TPrsStd_AISPresentation::GetID(), aPresentation)) {
            if (TDocStd_Modified::Contains(aPresentation->Label())) {
                MYLOG << "update";
                aPresentation->Update();
            }
        }
    }

    TPrsStd_AISViewer::Update(theAccessLabel);
}

//=======================================================================
// function : UpdateViewer
// purpose  :
//=======================================================================

void OCAFSample_IDisplayer::UpdateViewer(const TDF_Label& theAccessLabel)
{
    TPrsStd_AISViewer::Update(theAccessLabel);
}

//=======================================================================
// function : DisplayAll
// purpose  :
//=======================================================================

void OCAFSample_IDisplayer::DisplayAll(const TDF_Label& theAccessLabel, const Standard_Boolean isUpdated)
{
    if (theAccessLabel.IsNull()) return;

    TDF_ChildIterator anIterator(theAccessLabel.Root());
    Handle(TPrsStd_AISPresentation) aPresentation;

    for (; anIterator.More(); anIterator.Next()) {
        if (anIterator.Value().FindAttribute(TPrsStd_AISPresentation::GetID(), aPresentation))
            aPresentation->Display(isUpdated);
    }

    TPrsStd_AISViewer::Update(theAccessLabel);
}

//=======================================================================
// function : Remove
// purpose  :
//=======================================================================

void OCAFSample_IDisplayer::Remove(const Handle(TDF_Attribute) & theAttrib)
{
    if (theAttrib.IsNull()) return;

    TDF_Label aShapeLabel = theAttrib->Label();
    if (aShapeLabel.IsNull()) return;

    aShapeLabel.ForgetAttribute(TPrsStd_AISPresentation::GetID());
}

//=======================================================================
// function : SetTransparency
// purpose  :
//=======================================================================

void OCAFSample_IDisplayer::SetTransparency(const Handle(TDF_Attribute) & theAttrib, const Standard_Real theValue)
{
    if (theAttrib.IsNull()) return;

    TDF_Label ShapeLabel = theAttrib->Label();
    if (ShapeLabel.IsNull()) return;

    Handle(TPrsStd_AISPresentation) aPresentation;
    if (ShapeLabel.FindAttribute(TPrsStd_AISPresentation::GetID(), aPresentation)) {
        aPresentation->SetTransparency(theValue);
        TPrsStd_AISViewer::Update(ShapeLabel);
    }
}

//=======================================================================
// function : SetColor
// purpose  :
//=======================================================================

void OCAFSample_IDisplayer::SetColor(const Handle(TDF_Attribute) & theAttrib, const Quantity_NameOfColor theColor)
{
    if (theAttrib.IsNull()) return;

    TDF_Label ShapeLabel = theAttrib->Label();
    if (ShapeLabel.IsNull()) return;

    Handle(TPrsStd_AISPresentation) aPresentation;
    if (ShapeLabel.FindAttribute(TPrsStd_AISPresentation::GetID(), aPresentation)) {
        aPresentation->SetColor(theColor);
        TPrsStd_AISViewer::Update(ShapeLabel);
    }
}

//=======================================================================
// function : SetColor
// purpose  :
//=======================================================================

void OCAFSample_IDisplayer::SetColor(const Handle(TDF_Attribute) & theAttrib, const Quantity_Color& theColor)
{
    Quantity_NameOfColor aColor = theColor.Name();
    SetColor(theAttrib, aColor);
}

//=======================================================================
// function : SetColor
// purpose  :
//=======================================================================

void OCAFSample_IDisplayer::SetColor(const Handle(TDF_Attribute) & theAttrib, const Standard_Integer R,
                                     const Standard_Integer G, const Standard_Integer B)
{
    Quantity_Color aColor(R / 255., G / 255., B / 255., Quantity_TOC_RGB);
    SetColor(theAttrib, aColor);
}

//=======================================================================
// function : GetColor
// purpose  :
//=======================================================================

Quantity_Color OCAFSample_IDisplayer::GetColor(const Handle(TDF_Attribute) & theAttrib)
{
    Quantity_NameOfColor aColor = Quantity_NOC_BLACK;

    if (theAttrib.IsNull()) return aColor;

    TDF_Label aShapeLabel = theAttrib->Label();
    if (aShapeLabel.IsNull()) return aColor;

    Handle(TPrsStd_AISPresentation) aPresentation;
    if (aShapeLabel.FindAttribute(TPrsStd_AISPresentation::GetID(), aPresentation)) aColor = aPresentation->Color();

    return aColor;
}

//=======================================================================
// function : SetMode
// purpose  :
//=======================================================================

void OCAFSample_IDisplayer::SetMode(const Handle(TDF_Attribute) & theAttrib, const Standard_Integer theMode)
{
    if (theAttrib.IsNull()) return;

    TDF_Label ShapeLabel = theAttrib->Label();
    if (ShapeLabel.IsNull()) return;

    Handle(TPrsStd_AISPresentation) aPresentation;
    if (ShapeLabel.FindAttribute(TPrsStd_AISPresentation::GetID(), aPresentation)) {
        aPresentation->SetMode(theMode);
        TPrsStd_AISViewer::Update(ShapeLabel);
    }
}

//=======================================================================
// function : SetWidth
// purpose  :
//=======================================================================

void OCAFSample_IDisplayer::SetWidth(const Handle(TDF_Attribute) & theAttrib, const Standard_Real theWidth)
{
    if (theAttrib.IsNull()) return;

    TDF_Label ShapeLabel = theAttrib->Label();
    if (ShapeLabel.IsNull()) return;

    Handle(TPrsStd_AISPresentation) aPresentation;
    if (ShapeLabel.FindAttribute(TPrsStd_AISPresentation::GetID(), aPresentation)) {
        aPresentation->SetWidth(theWidth);
        TPrsStd_AISViewer::Update(ShapeLabel);
    }
}

//=======================================================================
// function : Hilight
// purpose  :
//=======================================================================

void OCAFSample_IDisplayer::Hilight(const Handle(TDF_Attribute) & theAttrib, Handle(AIS_InteractiveContext) & iContext)
{
    if (theAttrib.IsNull()) return;

    iContext->ClearSelected(Standard_False);
    TDF_Label ShapeLabel = theAttrib->Label();
    if (!ShapeLabel.IsNull()) {
        Handle(TPrsStd_AISPresentation) aPresentation;
        if (ShapeLabel.FindAttribute(TPrsStd_AISPresentation::GetID(), aPresentation)) {
            Handle(AIS_InteractiveObject) InteractiveObject = aPresentation->GetAIS();
            iContext->AddOrRemoveSelected(InteractiveObject, Standard_False);
        }
    }

    iContext->UpdateCurrentViewer();
}
