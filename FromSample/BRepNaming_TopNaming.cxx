// Created: 1999-09-24
//
// Copyright (c) 1999-2013 OPEN CASCADE SAS
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

#include "BRepNaming_TopNaming.hxx"

#include <Standard_NullObject.hxx>

//=======================================================================
// function : BRepNaming_TopNaming
// purpose  : Constructor
//=======================================================================

BRepNaming_TopNaming::BRepNaming_TopNaming(const TDF_Label& Label) : myResultLabel(Label)
{
    if (Label.IsNull())
        throw Standard_NullObject("BRepNaming_TopNaming::BRepNaming_TopNaming: The Result label is Null ...");
}

//=======================================================================
// function : Init
// purpose  :
//=======================================================================

void BRepNaming_TopNaming::Init(const TDF_Label& Label)
{
    if (Label.IsNull()) throw Standard_NullObject("BRepNaming_TopNaming::Init: The Result label is Null ...");
    myResultLabel = Label;
}
