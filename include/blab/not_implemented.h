#ifndef BLAB_NOT_IMPLEMENTED_H
#define BLAB_NOT_IMPLEMENTED_H

#include <string>
#include <stdexcept>

namespace blab {
    class not_implemented : std::logic_error {
    public:
        explicit not_implemented(const std::string &method_name);
    };
}

#endif //BLAB_NOT_IMPLEMENTED_H
