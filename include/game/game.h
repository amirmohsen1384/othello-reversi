#pragma once

#include "match.h"
#include <filesystem>

namespace Reversi {
    void Narrate(Match const &match);
    void Initialize(Match &match);
    bool Execute(Match &match);

    namespace IO {
        std::filesystem::path GetFilename();
        void Save(const Match &match);
        void Load(Match &match);
        bool Exists();
    }
}