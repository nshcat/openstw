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
class TilePanelView: public QGraphicsView
{
    Q_OBJECT

public:
    TilePanelView();

public:
    void setTilePanel(Openstw::Simulation::TilePanel* tilePanel);
    bool hasTilePanel() const;
    Openstw::Simulation::TilePanel* tilePanel() const;

protected:
    Openstw::Simulation::TilePanel* m_tilePanel{};
    QGraphicsScene* m_scene{};
    QGraphicsItemGroup* m_tileItemGroup{}; //< Holds all graphics items that render the stellwerk tiles
};
