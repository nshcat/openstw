#include "inputprocessor.hxx"

namespace Openstw::SpDr
{
    InputProcessor::InputProcessor(QObject* parent)
        : QObject{parent}
    {
    }

    void InputProcessor::onButtonStateChanged(const Tile* tile, const Button* button, ButtonState newState)
    {
    }
}
