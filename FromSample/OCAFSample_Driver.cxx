// Created: 2002-02-07
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

#include "Defines.h"
#include "Util.h"

#include "OCAFSample_Driver.hxx"
#include "Tags.h"

#include <TDF_ChildIterator.hxx>
#include <TDF_LabelMap.hxx>
#include <TDF_MapIteratorOfLabelMap.hxx>
#include <TDF_Reference.hxx>
#include <TDocStd_Modified.hxx>
#include <TFunction_DriverTable.hxx>
#include <TFunction_Function.hxx>
#include <TFunction_Logbook.hxx>

IMPLEMENT_STANDARD_RTTIEXT(OCAFSample_Driver, TFunction_Driver)

//=======================================================================
// function : Validate
// purpose  :
//=======================================================================
void OCAFSample_Driver::Validate(Handle(TFunction_Logbook) & theLogbook) const
{
    theLogbook->SetValid(Label().FindChild(tag::Object::Results));
}

//=======================================================================
// function : MustExecute
// purpose  :
//=======================================================================
Standard_Boolean OCAFSample_Driver::MustExecute(const Handle(TFunction_Logbook) & theLogbook) const
{
    const TDF_LabelMap& anImpacted = theLogbook->GetImpacted();

    if (anImpacted.IsEmpty()) {
        return Standard_False;
    }

    TDF_LabelMap args;
    if (!Arguments(args)) {
        return Standard_False;
    }

    TDF_MapIteratorOfLabelMap itr(args);
    for (; itr.More(); itr.Next()) {
        if (anImpacted.Contains(itr.Key())) {
            return Standard_True;
        }
    }
    return Standard_False;
}

//=======================================================================
// function : Arguments
// purpose  :
//=======================================================================
Standard_Boolean OCAFSample_Driver::Arguments(TDF_LabelMap& theArgs) const
{
    auto addReferencedLabel = [this, &theArgs](const TDF_Label& label) {
        Handle(TDF_Reference) aReference;
        if (label.FindAttribute(TDF_Reference::GetID(), aReference)) {
            auto ReferencedLabel = aReference->Get();

            if (!ReferencedLabel.IsNull()) {
                theArgs.Add(ReferencedLabel);
            }
        }
    };

    theArgs.Clear();

    auto function_label = Label();
    addReferencedLabel(function_label);

    const auto& ArgumentsLabel = function_label.FindChild(tag::Object::Argments, Standard_False);
    if (ArgumentsLabel.IsNull()) {
        return Standard_False;
    }

    constexpr bool    allLevels = true;
    TDF_ChildIterator itr(ArgumentsLabel, allLevels);
    for (; itr.More(); itr.Next()) {
        const auto& CurrentLabel = itr.Value();

        // Direct arguments (child labels of the function):
        theArgs.Add(CurrentLabel);

        // Referenced label:
        addReferencedLabel(CurrentLabel);
    }

    return !theArgs.IsEmpty();
}

//=======================================================================
// function : Results
// purpose  :
//=======================================================================
Standard_Boolean OCAFSample_Driver::Results(TDF_LabelMap& theRes) const
{
    theRes.Clear();

    TDF_Label ResultsLabel = Label().FindChild(tag::Object::Results, Standard_False);
    if (ResultsLabel.IsNull()) {
        return Standard_False;
    }

    constexpr bool    allLevels = true;
    TDF_ChildIterator itr(ResultsLabel, allLevels);
    for (; itr.More(); itr.Next()) {
        theRes.Add(itr.Value());
    }

    return !theRes.IsEmpty();
}
