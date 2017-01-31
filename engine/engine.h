#ifndef LEGACYMUD_ENGINE_ENGINE_H
#define LEGACYMUD_ENGINE_ENGINE_H

#include <string>

namespace legacymud { namespace engine {

    // Stubs for engine types
    // TODO: Remove these when engine implementation is available
    enum class ActionType {
        NONE,
        SOME
    };

    enum class PositionType {
        NONE  = 0,
        IN    = 1,
        ON    = (1 << 1),
        UNDER = (1 << 2),
        TO    = (1 << 3)
    };

    class InteractiveNoun {
    public:
        std::string name;
    };
}}

#endif
