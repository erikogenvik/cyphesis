#include "Py_Filter.h"
#include "entityfilter/Filter.h"

///\brief Create a new Filter object for a given query
PyObject * get_filter(PyObject * self, PyObject* query){
    if (!PyString_CheckExact(query)){
            PyErr_SetString(PyExc_TypeError, "Map_get_filter what must be string");
                    return NULL;
        }
        char * query_str = PyString_AsString(query);
        PyFilter* f = newPyFilter();
        try {
            //FIXME: creating and accessing an instance of a factory should be done in a better way
            EntityFilter::MindProviderFactory factory;
            f->m_filter = new EntityFilter::Filter(query_str, &factory);
        }
        catch (std::invalid_argument& e){
            PyErr_SetString(PyExc_TypeError, "Invalid query for Entity Filter");
            return NULL;
        }
        return (PyObject*)f;
}

static void Filter_dealloc(PyFilter *self)
{
    if (self->m_filter != NULL) {
        delete self->m_filter;
    }
    self->ob_type->tp_free((PyObject*)self);
}

PyTypeObject PyFilter_Type = {
        PyObject_HEAD_INIT(&PyType_Type)
        0,                              // ob_size
        "Filter",                       // tp_name
        sizeof(PyFilter),               // tp_basicsize
        0,                              // tp_itemsize
        // methods
        (destructor)Filter_dealloc,     // tp_dealloc
        0,                              // tp_print
        0,                              // tp_getattr
        0,                              // tp_setattr
        0,                              // tp_compare
        0,                              // tp_repr
        0,                              // tp_as_number
        0,                              // tp_as_sequence
        0,                              // tp_as_mapping
        0,                              // tp_hash
        0,                              // tp_call
        0,                              // tp_str
        0,                              // tp_getattro
        0,                              // tp_setattro
        0,                              // tp_as_buffer
        Py_TPFLAGS_DEFAULT,             // tp_flags
        "Filter object",                // tp_doc
        0,                              // tp_travers
        0,                              // tp_clear
        0,                              // tp_richcompare
        0,                              // tp_weaklistoffset
        0,                              // tp_iter
        0,                              // tp_iternext
        0,                              // tp_methods
        0,                              // tp_members
        0,                              // tp_getset
        0,                              // tp_base
        0,                              // tp_dict
        0,                              // tp_descr_get
        0,                              // tp_descr_set
        0,                              // tp_dictoffset
        0,                              // tp_init
        0,                              // tp_alloc
        0,                              // tp_new
};

PyFilter* newPyFilter(){
    return (PyFilter*)PyFilter_Type.tp_new(&PyFilter_Type, 0, 0);
}
