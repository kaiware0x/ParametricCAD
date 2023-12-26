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

#include "OCAFSample_ITransformParent.hxx"
#include "Standard_GUID.hxx"

//=======================================================================
// function : GetID
// purpose  :
//=======================================================================
const Standard_GUID& OCAFSample_ITransformParent::GetID()
{
    static Standard_GUID anID("22D22E65-C69A-11d4-8F1A-0060B0EE18E8");
    return anID;
}
