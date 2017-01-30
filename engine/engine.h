#ifndef LEGACYMUD_ENGINE_ENGINE_H
#define LEGACYMUD_ENGINE_ENGINE_H

#include <string>

namespace legacymud { namespace engine {

    // Stubs for engine types
    // TODO: Remove these when engine implementation is available
    enum class ActionType {
        NONE
    };

    enum class PositionType {
        NONE
    };

    class InteractiveNoun {
    public:
        std::string name;
    };
}}

#endif
