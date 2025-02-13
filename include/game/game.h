#pragma once

#include "match.h"
#include <filesystem>

namespace Reversi {
    Match Initialize();
    void Play();

    namespace IO {
        std::filesystem::path GetFilename();
        void Save(const Match &match);
        void Load(Match &match);
        bool Exists();
    }
}