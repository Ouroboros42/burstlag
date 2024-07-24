#include "caching/outputs.hpp"

size_t hash_args::operator()(const std::tuple<size_t, size_t, scalar>& args) const {
    using std::get;

    return (std::hash<size_t>()(get<0>(args)) << 4) ^ std::hash<size_t>()(get<1>(args)) ^ std::hash<double>()(get<2>(args));
}
