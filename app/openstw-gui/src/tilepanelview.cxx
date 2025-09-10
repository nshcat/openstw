#include <QFileDialog>
#include <QMenu>
#include <QPainter>
#include <QWheelEvent>
#include <QtSvg/QSvgGenerator>
#include <stdexcept>

#include "rendering/icontextmenuprovider.hxx"
#include "tilegraphicsobject.hxx"
#include "tilepanelview.hxx"

TilePanelView::TilePanelView()
{
    this->setBackgroundBrush(Qt::white);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setDragMode(DragMode::ScrollHandDrag);
}

void TilePanelView::setTilePanel(Openstw::Simulation::TilePanel *tilePanel)
{
    if (this->m_tilePanel != nullptr)
        throw std::runtime_error("Tile panel was already set");

    this->m_tilePanel = tilePanel;

    // Create scene to be displayed in our graphics view, exactly sized to fit
    // the tile grid modeled by the passed TilePanel instance
    this->m_scene = new QGraphicsScene(0.0f, 0.0f, tilePanel->width() * TileGraphicsObject::tileWidth,
                                       tilePanel->height() * TileGraphicsObject::tileHeight);
    this->setScene(this->m_scene);

    // Populate scene with tile graphics object which display the tiles
    QList<QGraphicsItem*> tileGraphicsObject{};

    for (std::size_t ix = 0; ix < tilePanel->width(); ++ix)
    {
        for (std::size_t iy = 0; iy < tilePanel->height(); ++iy)
        {
            auto* tile = tilePanel->operator[](ix, iy);
            auto* tileItem = new TileGraphicsObject(tile);
            tileItem->setPos(ix * TileGraphicsObject::tileWidth, iy * TileGraphicsObject::tileHeight);
            tileGraphicsObject.append(tileItem);
            this->m_scene->addItem(tileItem);
            tileItem->setup();
        }
    }

    this->resetSceneRect();
}

bool TilePanelView::hasTilePanel() const
{
    return this->m_tilePanel != nullptr;
}

Openstw::Simulation::TilePanel *TilePanelView::tilePanel() const
{
    return this->m_tilePanel;
}

void TilePanelView::saveToSvg(const QString& path) const
{
    const auto sceneRect = this->m_scene->sceneRect();

    QSvgGenerator generator{};
    generator.setFileName(path);
    generator.setSize(sceneRect.size().toSize());
    generator.setViewBox(QRect{0, 0, static_cast<int>(sceneRect.width()), static_cast<int>(sceneRect.height())});
    generator.setTitle("SpDrS60 Simulation Export");
    generator.setDescription("Vector image containing exported SpDrS60 simulation state");

    QPainter svgPainter{};
    svgPainter.begin(&generator);
    this->m_scene->render(&svgPainter);
    svgPainter.end();
}

void TilePanelView::resetSceneRect()
{
    this->setSceneRect(this->m_scene->sceneRect());
}

void TilePanelView::mousePressEvent(QMouseEvent* event)
{
    this->QGraphicsView::mousePressEvent(event);
}

void TilePanelView::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers().testFlag(Qt::KeyboardModifier::ControlModifier))
    {
        const auto oldAnchor = this->transformationAnchor();
        this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

        const auto angle = event->angleDelta().y();
        const qreal scaleFactor = angle > 0 ? 1.1f : 0.9f;
        this->scale(scaleFactor, scaleFactor);

        this->setTransformationAnchor(oldAnchor);
    }
}

void TilePanelView::contextMenuEvent(QContextMenuEvent* event)
{
    QPointF p = event->pos();
    QGraphicsItem* item = this->itemAt(p.x(), p.y());
    if (item != NULL)
    {
        auto contextMenuProvider = dynamic_cast<Rendering::IContextMenuProvider*>(item);
        if (contextMenuProvider && contextMenuProvider->providesContextMenu())
        {
            QGraphicsView::contextMenuEvent(event);
            return;
        }
    }

    QMenu contextMenu{this};

    QAction* actionRecenter = contextMenu.addAction("&Recenter View");
    connect(actionRecenter, &QAction::triggered, this, &TilePanelView::onRecenterView);

    QAction* actionSaveToImageFile = contextMenu.addAction("Save to &Image File");
    connect(actionSaveToImageFile, &QAction::triggered, this, &TilePanelView::onSaveToImageFile);

    contextMenu.exec(event->globalPos());
}

void TilePanelView::onRecenterView()
{
    this->resetTransform();
    this->resetSceneRect();
}

void TilePanelView::onSaveToImageFile()
{
    const auto filePath = QFileDialog::getSaveFileName(this, "Save Image File", "image.svg", "SVG Image Files (*.svg)");

    if (filePath.isEmpty())
        return;

    this->saveToSvg(filePath);
}
