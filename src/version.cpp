// Local includes
#include "../build/version.hpp"

namespace iipc {

const char* get_runtime_version() {
    return ::iipc::compiletime_version;
}

} // namespace iipc
