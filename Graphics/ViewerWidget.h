
#pragma once

// Qt includes
#include <QMouseEvent>
#include <QWidget>

/// OCC includes
#include <AIS_InteractiveContext.hxx>
#include <AIS_Line.hxx>
#include <AIS_Point.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Aspect_Handle.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_Line.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <TDF_LabelNode.hxx>
#include <V3d_AmbientLight.hxx>
#include <V3d_DirectionalLight.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <WNT_Window.hxx>

/// my includes
#include "ViewerInteractor.h"
#include "document.h"

/**
 * @brief Ensemble の OccView に相当
 */
class ViewerWidget : public QWidget {
    Q_OBJECT

public:
    ViewerWidget(Document* document, QWidget* parent = nullptr);

    Handle(V3d_Viewer) occViewer() const { return m_viewer; }
    Handle(AIS_InteractiveContext) getContext() const { return m_document->context(); }

    //! Returns viewer interactor.
    ViewerInteractor* interactor() const { return m_interactor; }

    virtual QPaintEngine* paintEngine() const override { return 0; }
    void                  myUpdate();

    //! Returns an active/defined view of the context. Gets the context viewer, find the first active view.
    //! If no active view, returns the first defined viewer.
    //! @param context visu context
    static Handle(V3d_View) activeView(const Handle(AIS_InteractiveContext) & context);

protected:
    //! Avoids Qt standard execution of this method, redraw V3d view
    //! \param an event
    virtual void paintEvent(QPaintEvent* theEvent) override;

    //! Avoids Qt standard execution of this method, do mustBeResized for V3d view, Init view if it is the first call
    //! \param an event
    virtual void resizeEvent(QResizeEvent* theEvent) override;

    //! Processes mouse event by viewer interactor.
    //! @param event the mouse event.
    virtual void mousePressEvent(QMouseEvent* event) override;

    //! Processes mouse event by viewer interactor.
    //! @param event the mouse event.
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

    //! Processes mouse event by viewer interactor.
    //! @param event the mouse event.
    virtual void mouseMoveEvent(QMouseEvent* event) override;

    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    //! Converts Qt enum to OCCT enum value.
    //! @param modifierId key modifier.
    static Aspect_VKeyFlags keyFlag(const int modifierId);

    //! Converts Qt enum to OCCT enum value.
    //! @param buttonId key button.
    static Aspect_VKeyMouse keyMouse(const int buttonId);

    void initOccView();

    void drawPoint(const QPoint& p);
    void drawLine(const QPoint& p1, const QPoint& p2);

private:
    Document* m_document;

    Handle(V3d_Viewer) m_viewer;
    Handle(V3d_View) m_view;
    ViewerInteractor* m_interactor;

    bool   m_startDrawing;
    QPoint m_startP;
};
