#pragma once

#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <spdr/stelltisch.hxx>

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
    void setTilePanel(Openstw::SpDr::Stelltisch* tilepanel);
    bool hasTilePanel() const;
    Openstw::SpDr::Stelltisch* tilePanel() const;
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
    Openstw::SpDr::Stelltisch* m_tilePanel{};
    QGraphicsScene* m_scene{};
    QGraphicsItemGroup* m_tileItemGroup{}; //< Holds all graphics items that render the stellwerk tiles
};
