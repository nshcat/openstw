#include "stelltisch.hxx"
#include "features/trackindicatorfeature.hxx"
#include <chrono>
#include <core/exception.hxx>
#include <core/xmlutility.hxx>

namespace Openstw::SpDr
{
    using namespace std::chrono_literals;

    Stelltisch::Stelltisch(QSize dimensions, Interlocking::System* interlocking, QObject* parent)
        : QObject{parent}, m_dimensions{dimensions}, m_interlocking{interlocking}
    {
        // == Setup tile matrix with completely blank tiles.
        this->m_tiles.reserve(dimensions.width() * dimensions.height());

        for (unsigned iy = 0; iy < this->height(); ++iy)
        {
            for (unsigned ix = 0; ix < this->width(); ++ix)
            {
                TileLocation location{ix, iy};
                auto* tile = new Tile{location, this};
                this->m_tiles.append(tile);
            }
        }
        // ==

        // == Setup the timer used to blink all tile apertures in a
        // synchronized way
        this->m_blinkTimer = new QTimer(this);
        this->m_blinkTimer->setTimerType(Qt::TimerType::PreciseTimer);
        this->m_blinkTimer->setInterval(500ms);
        this->m_blinkTimer->setSingleShot(false);
        this->m_blinkTimer->setObjectName("StelltischBlinkTimer");

        // Forward the signal as our own. The timer keeps being an implementation
        // detail.
        connect(this->m_blinkTimer, &QTimer::timeout, this, &Stelltisch::blinkStep);

        // Start the timer.
        // XXXTODO is this the correct location for this?
        this->m_blinkTimer->start();
        // ==
    }

    Stelltisch* Stelltisch::createFromXml(QDomElement xmlElement, Interlocking::System* interlocking, QObject* parent)
    {
        // We need to read the dimensions of the Stelltisch first,
        // otherwise we can't construct the object.

        // First, try to retrieve width.
        const auto width = Core::Xml::intAttribute(xmlElement, u"Width");
        if (!width.has_value())
        {
            throw Core::RuntimeError("SpDr XML: Line {}: Stelltisch element missing valid width attribute",
                                     xmlElement.lineNumber());
        }

        // Then, try to retrieve the height.
        const auto height = Core::Xml::intAttribute(xmlElement, u"Height");
        if (!height.has_value())
        {
            throw Core::RuntimeError("SpDr XML: Line {}: Stelltisch element missing valid height attribute",
                                     xmlElement.lineNumber());
        }

        // Construct Stelltisch object.
        auto* stelltisch = new Stelltisch{{width.value(), height.value()}, interlocking, parent};

        // Delegate further parsing to it.
        stelltisch->populateFromXml(xmlElement);

        // Return it.
        return stelltisch;
    }

    QSize Stelltisch::dimensions() const
    {
        return this->m_dimensions;
    }

    unsigned Stelltisch::width() const
    {
        return this->dimensions().width();
    }

    unsigned Stelltisch::height() const
    {
        return this->dimensions().height();
    }

    std::optional<Tile*> Stelltisch::operator[](unsigned int x, unsigned int y)
    {
        if (x >= this->width() || y >= this->height())
        {
            return std::nullopt;
        }

        return this->m_tiles[this->tileIndex(TileLocation{x, y})];
    }

    std::optional<const Tile*> Stelltisch::operator[](unsigned int x, unsigned int y) const
    {
        if (x >= this->width() || y >= this->height())
        {
            return std::nullopt;
        }

        return this->m_tiles[this->tileIndex(TileLocation{x, y})];
    }

    std::optional<Tile*> Stelltisch::tile(unsigned x, unsigned y)
    {
        if (x >= this->width() || y >= this->height())
        {
            return std::nullopt;
        }

        return this->m_tiles[this->tileIndex(TileLocation{x, y})];
    }

    std::optional<const Tile*> Stelltisch::tile(unsigned x, unsigned y) const
    {
        if (x >= this->width() || y >= this->height())
        {
            return std::nullopt;
        }

        return this->m_tiles[this->tileIndex(TileLocation{x, y})];
    }

    TileLocation Stelltisch::tilePosition(qsizetype tileIndex) const
    {
        return TileLocation{static_cast<unsigned>(tileIndex % this->width()),
                            static_cast<unsigned>(tileIndex / this->width())};
    }

    qsizetype Stelltisch::tileIndex(TileLocation location) const
    {
        return (location.y() * this->width()) + location.x();
    }

    void Stelltisch::populateFromXml(QDomElement xmlElement)
    {
        // == Parse tile descriptors
        auto tilesElement = xmlElement.namedItem("Tiles");

        if (tilesElement.isNull() || !tilesElement.isElement())
        {
            throw Core::RuntimeError("SpDr XML: Expected <Tiles> element");
        }

        this->populateTiles(tilesElement.toElement());
        // ==
    }

    void Stelltisch::populateTiles(QDomElement xmlElement)
    {
        // Parse <Tile> elements, and their contained feature layer
        // elements.
        auto tileElement = xmlElement.firstChildElement("Tile");
        while (!tileElement.isNull())
        {
            // Figure out the location of the referenced tile.
            const auto positionX = Core::Xml::intAttribute(tileElement, u"X");
            if (!positionX.has_value())
            {
                throw Core::RuntimeError("SpDr XML: Line {}: Tile element is missing X position",
                                         xmlElement.lineNumber());
            }

            const auto positionY = Core::Xml::intAttribute(tileElement, u"Y");
            if (!positionY.has_value())
            {
                throw Core::RuntimeError("SpDr XML: Line {}: Tile element is missing Y position",
                                         xmlElement.lineNumber());
            }

            // Retrieve the tile from the Stelltisch.
            auto tile = this->tile(positionX.value(), positionY.value());
            if (!tile.has_value())
            {
                throw Core::RuntimeError(
                    "SpDr XML: Line {}: Tile element references nonexistant tile at location ({}, {})",
                    xmlElement.lineNumber(), *positionX, *positionY);
            }

            // Now, read all feature layers, if given.
            auto layersElement = tileElement.namedItem("Layers");
            if (!layersElement.isNull() && layersElement.isElement())
            {
                this->populateLayers(layersElement.toElement(), tile.value());
            }

            // Continue with the next <Tile> element, if it exists.
            tileElement = tileElement.nextSiblingElement("Tile");
        }
    }

    void Stelltisch::populateLayers(QDomElement xmlElement, Tile* tile)
    {
        // Parse <Layer> elements and associated corresponding feature layer
        // object with the tile.
        auto layerElement = xmlElement.firstChildElement("Layer");
        while (!layerElement.isNull())
        {
            // Read the layer type.
            const auto layerType = Core::Xml::enumAttribute<FeatureType>(layerElement, u"Type");
            if (!layerType.has_value())
            {
                throw Core::RuntimeError("SpDr XML: Line {}: Tile layer element is missing type attribute",
                                         xmlElement.lineNumber());
            }

            // Construct the correct feature layer object.
            std::optional<FeatureLayer*> layer{};
            switch (layerType.value())
            {
            case FeatureType::TrackIndicator:
                layer = TrackIndicatorFeature::createFromXml(layerElement, this->m_interlocking, tile);
                break;

            default:
                break;
            }

            // Ensure that we did create a layer object.
            if (!layer.has_value())
            {
                throw Core::RuntimeError("SpDr XML: Line {}: Tile layer element specifies invalid feature type '{}'",
                                         xmlElement.lineNumber(), Core::toString(*layerType));
            }

            // If we did, associate it with the tile.
            tile->addLayer(layer.value());

            // Continue with the next <Layer> element, if it exists.
            layerElement = layerElement.nextSiblingElement("Layer");
        }
    }
}
