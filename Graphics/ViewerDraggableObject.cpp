//-----------------------------------------------------------------------------
// Created on: 14 June 2022
// Copyright (c) 2022, Quaoar Studio (natalia@quaoar.pro)
//----------------------------------------------------------------------------

#include "ViewerDraggableObject.h"

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

#include <QDebug>

//-----------------------------------------------------------------------------

ViewerDraggableObject::ViewerDraggableObject(const TopoDS_Shape& shape) : AIS_Shape(shape)
{
    SetColor(Quantity_Color(Quantity_NameOfColor::Quantity_NOC_ALICEBLUE));
}

//-----------------------------------------------------------------------------

gp_Vec GetTranslationVector(const Handle(V3d_View) & view, const Standard_Integer startX, const Standard_Integer startY,
                            const Standard_Integer endX, const Standard_Integer endY)
{
    gp_Vec translateVec;

    // start point
    Standard_Real viewX1, viewY1, viewZ1;
    view->Convert(startX, startY, viewX1, viewY1, viewZ1);

    // end point
    Standard_Real viewX2, viewY2, viewZ2;
    view->Convert(endX, endY, viewX2, viewY2, viewZ2);

    return gp_Vec(viewX2 - viewX1, viewY2 - viewY1, viewZ2 - viewZ1);
}

//-----------------------------------------------------------------------------

Standard_Boolean ViewerDraggableObject::ProcessDragging(const Handle(AIS_InteractiveContext) & context,
                                                        const Handle(V3d_View) & view,
                                                        const Handle(SelectMgr_EntityOwner) & owner,
                                                        const Graphic3d_Vec2i& dragFrom, const Graphic3d_Vec2i& dragTo,
                                                        const AIS_DragAction action)
{
    //    return AIS_Shape::ProcessDragging(context, view, owner, dragFrom, dragTo, action);

    auto current_context = InteractiveContext();

    if (action == AIS_DragAction_Start) {
        m_startPosition   = dragFrom;
        m_startTrsf       = Transformation();
        m_startAttributes = Attributes();

        // hide detected presentation in the viewer as it will be changed on drag
        current_context->ClearDetected();
        // set visual parameters into presentation as it's highlighted and recompute presentation
        SetAttributes(current_context->HighlightStyle());
        current_context->RecomputePrsOnly(this, false);
    }
    else if (action == AIS_DragAction_Update) {
        const Handle(V3d_View) lastview = current_context->LastActiveView();

        gp_Vec translationVec =
            GetTranslationVector(lastview, m_startPosition.x(), m_startPosition.y(), dragTo.x(), dragTo.y());

        gp_Trsf DirTrans;
        DirTrans.SetTranslation(translationVec);

        // unites the previuos and the current transformations
        gp_Trsf trsf = DirTrans * m_startTrsf;

        SetLocalTransformation(trsf);
    }
    else if (action == AIS_DragAction_Stop) {
        // restore initial visual attributes and recompute presentation
        SetAttributes(m_startAttributes);
        current_context->RecomputePrsOnly(this, false);

        // recompute selection on the presentation
        current_context->SelectionManager()->Update(this);

        // restore highlight in the viewer
        const Handle(V3d_View) lastview = current_context->LastActiveView();
        current_context->MoveTo(dragTo.x(), dragTo.y(), lastview, true);
    }
    return true;
}
