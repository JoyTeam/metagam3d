#include <Python.h>
#include <boost/python.hpp>
#include <metagam3d/RunScriptCommand>

using namespace boost;

namespace mg {

RunScriptCommand::RunScriptCommand(const std::string &filename):
    m_filename(filename)
{
}

RunScriptCommand::~RunScriptCommand()
{
}

void RunScriptCommand::deliver()
{
    python::object mg_events = python::import("metagam3d.events");
    mg_events.attr("exec_file")(m_filename);
}

} // namespace mg
