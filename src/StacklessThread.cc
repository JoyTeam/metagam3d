#include <Python.h>
#include <boost/python.hpp>
#include <metagam3d/StacklessThread>
#include <metagam3d/StacklessMailbox>
#include <metagam3d/Engine>
#include <iostream>

using namespace boost;

namespace mg {

/* This function is called from Stackless helper
 * to process the queue. */
static void processQueue()
{
    StacklessMailbox::instance()->process();
}

/* Return current model time */
static double getTime()
{
    return Engine::instance()->time();
}

void export_math();
void export_variant();
void export_script_assembler();
void export_dynamic_value();
void export_object_params();
void export_objects();
void export_camera();
void export_object_loader();
void export_text();
void export_material_param();
void export_console();
void export_line();

BOOST_PYTHON_MODULE(_metagam3d)
{
    export_math();
    export_variant();
    export_script_assembler();
    export_dynamic_value();
    export_object_params();
    export_objects();
    export_camera();
    export_object_loader();
    export_text();
    export_material_param();
    export_console();
    export_line();
    python::def("processQueue", &processQueue);
    python::def("getTime", &getTime);
}

StacklessThread::StacklessThread(int argc, char *argv[]):
    m_argc(argc),
    m_argv(argv),
    m_initialized(false)
{
}

void StacklessThread::init()
{
    if (m_initialized)
        return;

    Py_Initialize();
    PySys_SetArgv(m_argc, m_argv);

    try  {
        /* Initialize _metagam3d module */
        try {
            init_metagam3d();
            python::object _metagam3d = python::import("_metagam3d");
            python::object _metagam3d_global = _metagam3d.attr("__dict__");

        } catch (python::error_already_set) {
            std::cerr << "Error initializing '_metagam3d' module: ";
            PyErr_Print();
            return;
        }

        /* Initialize mg.events module */
        try {
            python::object mg_events = python::import("metagam3d.events");
            int fd = StacklessMailbox::instance()->readFD();
            mg_events.attr("init")(fd);
        } catch (python::error_already_set) {
            std::cerr << "Error initializing event loop: ";
            PyErr_Print();
            return;
        }
    } catch (std::exception const &exc) {
        std::cerr << "Error initializing Stackless interpreter: ";
        std::cerr << exc.what() << std::endl;
        return;
    }
}

StacklessThread::~StacklessThread()
{
    if (m_initialized)
        Py_Finalize();
}

void StacklessThread::run()
{
    init();

    try {
        /* Run main loop */
        try {
            python::object mainloop = python::import("metagam3d.mainloop");
            mainloop.attr("run")();
        } catch (python::error_already_set) {
            std::cerr << "Error in scripting thread: ";
            PyErr_Print();
            return;
        }
    } catch (std::exception const &exc) {
        std::cerr << "Error running scripting thread: ";
        std::cerr << exc.what() << std::endl;
        return;
    }

    std::cerr << "Python thread finished normally" << std::endl;
}

} // namespace mg
