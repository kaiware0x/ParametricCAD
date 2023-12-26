//-----------------------------------------------------------------------------
// Created on: 15 February 2022
// Copyright (c) 2022, Quaoar Studio (natalia@quaoar.pro)
//----------------------------------------------------------------------------

#include <QDebug>
#include <QMouseEvent>

#include <AIS_InteractiveContext.hxx>
#include <AIS_Manipulator.hxx>
#include <AIS_RubberBand.hxx>
#include <AIS_Shape.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <WNT_Window.hxx>

#include "ViewerInteractor.h"
#include "ViewerWidget.h"
#include "Defines.h"

//-----------------------------------------------------------------------------

namespace {
void applyLocalTransformation(const Handle(AIS_InteractiveObject) & dobject,
                              const Handle(AIS_InteractiveContext) & context)
{
    // synchronize topology object location with the presenation location
    auto manipulator = Handle(AIS_Manipulator)::DownCast(dobject);
    if (manipulator.IsNull()) return;

    auto shapePrs = Handle(AIS_Shape)::DownCast(manipulator->Object());
    if (shapePrs.IsNull()) return;

    TopoDS_Shape  shape   = shapePrs->Shape();
    const gp_Trsf prsTrsf = shapePrs->LocalTransformation();
    if (Abs(Abs(prsTrsf.ScaleFactor()) - 1.) > TopLoc_Location::ScalePrec())
        return;    // the shape cannot be scaled by manipulator according to #27457

    // update shape's location
    shape = shape.Located(prsTrsf.Multiplied(shape.Location().Transformation()));
    // update presentation to manage the update shape
    shapePrs->SetShape(shape);
    // reset local transformation of the presentation
    shapePrs->ResetTransformation();
    // redisplay presentation. Now it's with zero transformation and
    context->Redisplay(shapePrs, true);
}
};    // namespace

ViewerInteractor::ViewerInteractor(ViewerWidget* viewer_widget)
    : m_viewer_widget(viewer_widget)
    , m_currentAction(CurrentAction::Nothing)
    , m_rubberBand(new AIS_RubberBand(Quantity_NOC_LIGHTBLUE, Aspect_TOL_SOLID, Quantity_NOC_LIGHTBLUE4, 0.5, 1.0))
{
    m_rubberBand->SetZLayer(Graphic3d_ZLayerId_TopOSD);
    m_rubberBand->SetTransformPersistence(new Graphic3d_TransformPers(Graphic3d_TMF_2d, Aspect_TOTP_LEFT_UPPER));
    m_rubberBand->SetDisplayMode(0);
    m_rubberBand->SetMutable(true);
}

//-----------------------------------------------------------------------------

void ViewerInteractor::fitAll()
{
    Handle(V3d_View) view = ViewerWidget::activeView(m_viewer_widget->getContext());
    if (view.IsNull()) return;
    view->FitAll();
    view->Redraw();
}

//-----------------------------------------------------------------------------

void ViewerInteractor::zoom(const Standard_Real delta)
{
    Handle(V3d_View) view = ViewerWidget::activeView(m_viewer_widget->getContext());
    if (view.IsNull()) return;
    view->SetZoom(delta);
    view->Redraw();
}

//-----------------------------------------------------------------------------

void ViewerInteractor::mousePressEvent(const Graphic3d_Vec2i& point, Aspect_VKeyMouse keyMouse,
                                       Aspect_VKeyFlags keyFlag)
{
    auto context          = m_viewer_widget->getContext();
    Handle(V3d_View) view = ViewerWidget::activeView(context);
    if (view.IsNull()) return;

    m_currentAction = CurrentAction::Nothing;
    m_pressed_pos   = point;
    m_moved_pos     = point;

    if (keyMouse == Aspect_VKeyMouse_LeftButton) {
        if (context->HasDetected()) {
            /// Handle(SelectMgr_EntityOwner) で受け取らないとnullになる.
            /// const参照をつけてはいけない.
            auto downer  = context->DetectedOwner();
            auto dobject = context->DetectedInteractive();
            if (dobject->ProcessDragging(context, view, downer, m_pressed_pos, m_moved_pos, AIS_DragAction_Start)) {
                m_currentAction = CurrentAction::Dragging;
                m_draggedOwner  = downer;
            }
        }
        else if (keyFlag == Aspect_VKeyFlags_SHIFT || keyFlag == Aspect_VKeyFlags_CTRL) {
            m_rubberBand->SetRectangle(m_pressed_pos.x(), -m_pressed_pos.y(), m_moved_pos.x(), -m_moved_pos.y());
            context->Display(m_rubberBand, 0, -1, true, AIS_DS_Displayed);
            view->Invalidate();
        }
        else {
            view->StartRotation(m_pressed_pos.x(), m_pressed_pos.y());
            m_currentAction = CurrentAction::Rotation;
        }
    }
    else if (keyMouse == Aspect_VKeyMouse_RightButton) {
        m_currentAction = CurrentAction::Zoom;
    }
    else if (keyMouse == Aspect_VKeyMouse_MiddleButton) {
        m_currentAction = CurrentAction::Pan;
    }
}

//-----------------------------------------------------------------------------

void ViewerInteractor::mouseMoveEvent(const Graphic3d_Vec2i& point, Aspect_VKeyMouse /*keyMouse*/,
                                      Aspect_VKeyFlags /*keyFlag*/)
{
    auto context          = m_viewer_widget->getContext();
    Handle(V3d_View) view = ViewerWidget::activeView(context);
    if (view.IsNull()) return;

    if (m_currentAction == CurrentAction::Nothing) {
        if (!m_rubberBand.IsNull() && m_rubberBand->HasInteractiveContext()) {
            if (m_pressed_pos != m_moved_pos) {
                m_rubberBand->SetToUpdate();
                m_rubberBand->SetRectangle(m_pressed_pos.x(), -m_pressed_pos.y(), m_moved_pos.x(), -m_moved_pos.y());
                context->Display(m_rubberBand, 0, -1, Standard_True, AIS_DS_Displayed);
            }
        }
        else {
            // do highlight
            context->MoveTo(point.x(), point.y(), view, Standard_True);
        }
    }
    else if (m_currentAction == CurrentAction::Dragging) {
        if (!m_draggedOwner.IsNull()) {
            auto       dobject  = Handle(AIS_InteractiveObject)::DownCast(m_draggedOwner->Selectable());
            const auto dragFrom = m_moved_pos;
            m_moved_pos         = point;
            if (dobject->ProcessDragging(context, view, m_draggedOwner, dragFrom, m_moved_pos, AIS_DragAction_Update)) {
                m_currentAction = CurrentAction::Dragging;
            }
        }
    }
    else if (m_currentAction == CurrentAction::Zoom) {
        view->Zoom(m_moved_pos.x(), m_moved_pos.y(), point.x(), point.y());
    }
    else if (m_currentAction == CurrentAction::Pan) {
        view->Pan(point.x() - m_moved_pos.x(), m_moved_pos.y() - point.y());
    }
    else if (m_currentAction == CurrentAction::Rotation) {
        view->Rotation(point.x(), point.y());
    }
    view->Redraw();
    m_moved_pos = point;
}

//-----------------------------------------------------------------------------

void ViewerInteractor::mouseReleaseEvent(const Graphic3d_Vec2i& point, Aspect_VKeyMouse /*keyMouse*/,
                                         Aspect_VKeyFlags       keyFlag)
{
    auto context          = m_viewer_widget->getContext();
    Handle(V3d_View) view = ViewerWidget::activeView(context);
    if (view.IsNull()) return;

    bool isMoved = m_moved_pos != m_pressed_pos;
    if (m_currentAction == CurrentAction::Nothing || m_currentAction == CurrentAction::Rotation) {
        if (!m_rubberBand.IsNull() && m_rubberBand->HasInteractiveContext()) {
            context->Remove(m_rubberBand, false);
            if (isMoved) {
                context->SelectRectangle(
                    Graphic3d_Vec2i(Min(m_pressed_pos.x(), m_moved_pos.x()), Min(m_pressed_pos.y(), m_moved_pos.y())),
                    Graphic3d_Vec2i(Max(m_pressed_pos.x(), m_moved_pos.x()), Max(m_pressed_pos.y(), m_moved_pos.y())),
                    view, AIS_SelectionScheme_Replace);
            }
            context->UpdateCurrentViewer();
        }
        else {
            // do deselect
            if (!isMoved) {
                context->SelectDetected(keyFlag == Aspect_VKeyFlags_SHIFT ? AIS_SelectionScheme_Add
                                                                          : AIS_SelectionScheme_Replace);
                context->UpdateCurrentViewer();
            }
        }
    }
    else if (m_currentAction == CurrentAction::Dragging) {
        if (!m_draggedOwner.IsNull()) {
            auto            downer   = m_draggedOwner;
            auto            dobject  = Handle(AIS_InteractiveObject)::DownCast(m_draggedOwner->Selectable());
            Graphic3d_Vec2i dragFrom = m_moved_pos;
            m_moved_pos              = point;
            dobject->ProcessDragging(context, view, downer, dragFrom, m_moved_pos, AIS_DragAction_Stop);

            m_draggedOwner = nullptr;
        }
        // do deselect
        if (!isMoved) {
            context->SelectDetected(keyFlag == Aspect_VKeyFlags_SHIFT ? AIS_SelectionScheme_Add
                                                                      : AIS_SelectionScheme_Replace);
            context->UpdateCurrentViewer();
        }
    }
    m_currentAction = CurrentAction::Nothing;
}
