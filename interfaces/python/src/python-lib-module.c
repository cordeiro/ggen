/* Copyright Daniel Cordeiro 2011
*
*   contact : ggen-devel@lists.ligforge.imag.fr
*
* This software is a computer program whose purpose is to help the
* random generation of graph structures and adding various properties
* on those structures.
*
* This software is governed by the CeCILL  license under French law and
* abiding by the rules of distribution of free software.  You can  use,
* modify and/ or redistribute the software under the terms of the CeCILL
* license as circulated by CEA, CNRS and INRIA at the following URL
* "http://www.cecill.info".
*
* As a counterpart to the access to the source code and  rights to copy,
* modify and redistribute granted by the license, users are provided only
* with a limited warranty  and the software's author,  the holder of the
* economic rights,  and the successive licensors  have only  limited
* liability.
*
* In this respect, the user's attention is drawn to the risks associated
* with loading,  using,  modifying and/or developing or reproducing the
* software by the user in light of its specific status of free software,
* that may mean  that it is complicated to manipulate,  and  that  also
* therefore means  that it is reserved for developers  and  experienced
* professionals having in-depth computer knowledge. Users are therefore
* encouraged to load and test the software's suitability as regards their
* requirements in conditions enabling the security of their systems and/or
* data to be ensured and,  more generally, to use and operate it in the
* same conditions as regards security.
*
* The fact that you are presently reading this means that you have had
* knowledge of the CeCILL license and that you accept its terms.
*/

/* GGen is a random graph generator :
* it provides means to generate a graph following a
* collection of methods found in the litterature.
*
* This is a research project founded by the MOAIS Team,
* INRIA, Grenoble Universities.
*/

#include <Python.h>
#include <ggen.h>
#include <pygsl/rng.h>
#include <python-igraph/igraphmodule_api.h>

/*
  Calls GGen C library and returns the result converted to a PyObject

  This macro first converts the GSL RNG from the PyObject to C, then
  uses it to call GGen C library and finally convert the graph
  resulting from the call into a iGraph PyObject.
 */
#define call_ggen(ggen_function, python_gsl_rng, ...)  \
  ( (!PyGSL_RNG_Check(python_gsl_rng)) ? NULL : \
    PyIGraph_FromCGraph(ggen_function(PyGSL_gsl_rng_from_pyobject(python_gsl_rng), __VA_ARGS__)) )


static PyObject *
ggen_erdos_gnp(PyObject *self, PyObject *args)
{
  unsigned long n;
  double p;
  PyObject *pygsl_rng;

  if (!PyArg_ParseTuple(args, "Okd", &pygsl_rng, &n, &p)) // kd = unsigned long and double
    return NULL;

  return call_ggen (ggen_generate_erdos_gnp, pygsl_rng, n, p);
}

static PyObject *
ggen_erdos_gnm(PyObject *self, PyObject *args)
{
  unsigned long n, m;
  PyObject *pygsl_rng;

  if (!PyArg_ParseTuple(args, "Okk", &pygsl_rng, &n, &m))
    return NULL;

  return call_ggen (ggen_generate_erdos_gnm, pygsl_rng, n, m);
}

static PyObject *
ggen_erdos_lbl(PyObject *self, PyObject *args)
{
  unsigned long n, nbl;
  double p;
  PyObject *pygsl_rng;

  if (!PyArg_ParseTuple(args, "Okdk", &pygsl_rng, &n, &p, &nbl))
    return NULL;

  return call_ggen (ggen_generate_erdos_lbl, pygsl_rng, n, p, nbl);
}

static PyObject *
ggen_fifo(PyObject *self, PyObject *args)
{
  unsigned long n, od, id;
  PyObject *pygsl_rng;

  if (!PyArg_ParseTuple(args, "Okkk", &pygsl_rng, &n, &od, &id))
    return NULL;

  return call_ggen (ggen_generate_fifo, pygsl_rng, n, od, id);
}

static PyObject *
ggen_random_orders(PyObject *self, PyObject *args)
{
  unsigned long n;
  unsigned int orders;
  PyObject *pygsl_rng;

  if (!PyArg_ParseTuple(args, "OkI", &pygsl_rng, &n, &orders))
    return NULL;

  return call_ggen (ggen_generate_random_orders, pygsl_rng, n, orders);
}

static PyMethodDef GGenMethods[] = {
  {"generate_erdos_gnp", ggen_erdos_gnp, METH_VARARGS, "Generate a new graph using Erdos-Renyi's G(n,p) method"},
  {"generate_erdos_gnm", ggen_erdos_gnm, METH_VARARGS, "Generate a new graph using Erdos-Renyi's G(n,M) method"},
  {"generate_erdos_lbl", ggen_erdos_lbl, METH_VARARGS, "Generate a new graph using Layer-by-Layer method with Erdos-Renyi's G(n,p) probabilities"},
  {"generate_fifo", ggen_fifo, METH_VARARGS, "Generate a new graph using Fan-in/Fan-out method"},
  {"generate_random_orders", ggen_random_orders, METH_VARARGS, "Generate a new graph using Random Orders method"},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initlib(void)
{
  if (Py_InitModule("lib", GGenMethods) == NULL)
    return;

  import_igraph();
  import_pygsl_rng();
}
