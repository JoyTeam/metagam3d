#include <metagam3d/errors>
#include <cstring>
#include <cerrno>

namespace mg {

std::string errnoDescription()
{
    char buf[1024];
    strerror_r(errno, buf, 1024);
    buf[1023] = 0;
    return std::string(buf);
}

os_error::os_error(const std::string &operation):
    std::runtime_error(operation + ": " + errnoDescription())
{
}

} // namespace mg
