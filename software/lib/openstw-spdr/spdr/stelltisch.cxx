#include "stelltisch.hxx"
#include <chrono>

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
}
