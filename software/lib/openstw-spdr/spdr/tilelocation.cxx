#include "tilelocation.hxx"

namespace Openstw::SpDr
{

    TileLocation::TileLocation(unsigned int x, unsigned int y)
        : m_posX{x}, m_posY{y}
    {
    }

    unsigned int TileLocation::x() const
    {
        return this->m_posX;
    }

    unsigned int TileLocation::y() const
    {
        return this->m_posY;
    }
}

namespace std
{
    std::size_t std::hash<Openstw::SpDr::TileLocation>::operator()(
        Openstw::SpDr::TileLocation const& location) const noexcept
    {
        std::size_t h1 = std::hash<unsigned>{}(location.x());
        std::size_t h2 = std::hash<unsigned>{}(location.y());
        return h1 ^ (h2 << 1);
    }
}