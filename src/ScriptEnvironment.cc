#include <metagam3d/ScriptEnvironment>

namespace mg {

ScriptEnvironment::ScriptEnvironment()
{
}

void ScriptEnvironment::setGlobal(const std::string &name, const Variant &val)
{
    m_globals[name] = val;
}

const Variant &ScriptEnvironment::getGlobal(const std::string &name) const
{
    std::map<std::string, Variant>::const_iterator it = m_globals.find(name);
    if (it == m_globals.end()) {
        static Variant empty;
        return empty;
    }
    return it->second;
}

} // namespace mg
