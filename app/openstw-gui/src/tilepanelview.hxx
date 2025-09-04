#pragma once

#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <tilepanel.hxx>

/**
 * @brief The TilePanelView class displays a TilePanel using a QGraphicsView,
 * and supports displaying and interacting with auxilliary elements such as magnets
 * placed on the panel
 */
class TilePanelView : public QGraphicsView
{
    Q_OBJECT

public:
    TilePanelView();

public:
    void setTilePanel(Openstw::Simulation::TilePanel* tilePanel);
    bool hasTilePanel() const;
    Openstw::Simulation::TilePanel* tilePanel() const;
    void saveToSvg(const QString& path) const;

protected: // == Internal helper methods
    void resetSceneRect();

protected: // == Input events
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent* event) override;

protected slots:
    void onRecenterView();
    void onSaveToImageFile();

protected:
    Openstw::Simulation::TilePanel* m_tilePanel{};
    QGraphicsScene* m_scene{};
    QGraphicsItemGroup* m_tileItemGroup{}; //< Holds all graphics items that render the stellwerk tiles
};
