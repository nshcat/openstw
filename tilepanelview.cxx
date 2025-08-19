#include <stdexcept>

#include "tilegraphicsitem.hxx"
#include "tilepanelview.hxx"

TilePanelView::TilePanelView()
{
    this->setBackgroundBrush(Qt::white);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void TilePanelView::setTilePanel(Openstw::Simulation::TilePanel *tilePanel)
{
    if (this->m_tilePanel != nullptr)
        throw std::runtime_error("Tile panel was already set");

    this->m_tilePanel = tilePanel;

    // Create scene to be displayed in our graphics view, exactly sized to fit
    // the tile grid modeled by the passed TilePanel instance
    this->m_scene = new QGraphicsScene(0.0f, 0.0f, tilePanel->width() * TileGraphicsItem::tileWidth,
                                       tilePanel->height() * TileGraphicsItem::tileHeight);
    this->setScene(this->m_scene);

    // Populate scene with tile graphics items which
    QList<QGraphicsItem *> tileGraphicsItems{};

    for (std::size_t ix = 0; ix < tilePanel->width(); ++ix)
    {
        for (std::size_t iy = 0; iy < tilePanel->height(); ++iy)
        {
            auto *tile = &((*tilePanel)[ix, iy]);
            auto *tileItem = new TileGraphicsItem(tile);
            tileItem->setPos(ix * TileGraphicsItem::tileWidth, iy * TileGraphicsItem::tileHeight);
            tileGraphicsItems.append(tileItem);
        }
    }

    this->m_tileItemGroup = this->m_scene->createItemGroup(tileGraphicsItems);
}

bool TilePanelView::hasTilePanel() const
{
    return this->m_tilePanel != nullptr;
}

Openstw::Simulation::TilePanel *TilePanelView::tilePanel() const
{
    return this->m_tilePanel;
}
