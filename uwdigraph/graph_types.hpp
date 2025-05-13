#pragma once

#include <iostream>
#include <cstddef>
#include <utility>
#include <vector>

namespace uwdigraph {
    using node_id = uint32_t;
    using edge = std::pair<node_id, node_id>;
    using edge_id = uint32_t;
};