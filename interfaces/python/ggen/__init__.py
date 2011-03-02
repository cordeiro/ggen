"""This package provides a Python interface for the GGen library.

GGen is a library of reference random graph generation methods for
scheduling applications. Its core functionalities are implemented in a
pure C library. This package provides a Python interface to this
library using igraph's Python interface and PyGSL interface for the
GNU Scientific Library random number generators.

It is composed of the following sub-modules:
  lib     - a module written in pure C that provides a direct interface to
            GGen C library;
  wrapper - provides a convenience Python client to the lib module.


For more information about the methods implemented by GGen
library, please refer to the following research paper:

Random graph generation for scheduling simulations.  
Daniel Cordeiro, Gregory Mounie, Swann Perarnau, Denis Trystram,
Jean-Marc Vincent, and Frederic Wagner. In: 3rd International ICST
Conference on Simulation Tools and Techniques (SIMUTools 2010).
Torremolinos, Malaga, Spain, 2010."""


# package version
__version__   = '1.0.0'

# same format as sys.version_info: A tuple containing the five components of
# the version number: major, minor, micro, releaselevel, and serial. All
# values except releaselevel are integers; the release level is 'alpha',
# 'beta', 'candidate', or 'final'. The version_info value corresponding to the
# Python version 2.0 is (2, 0, 0, 'final', 0).
version_info = (1, 0, 0, 'alpha', 0)


from wrapper import client

_module_client = client()

def set_rng(pygsl_rng):
    """Define the PyGSL random number generator to be used by the
    module's static functions."""

    _module_client = client(pygsl_rng)

def gnp(n, p, rng=None):
    """Erdos and Renyi's classical adjacency matrix method using the
    module's random number generator."""

    if rng is not None:
        set_rng(rng)
    return _module_client.gnp(n, p)

def gnm(n, M, rng=None):
    """Erdos and Renyi's fixed number of edges method using the
    module's random number generator."""

    if rng is not None:
        set_rng(rng)
    return _module_client.gnm(n, M)

def lbl(n, p, number_of_layers, rng=None):
    """Layer-by-Layer method using module's random number
    generator."""

    if rng is not None:
        set_rng(rng)
    return _module_client.lbl(n, p, number_of_layers)

def fifo(n, out_degree, in_degree, rng=None):
    """Fan-in / Fan-out method using module's random number
    generator."""

    if rng is not None:
        set_rng(rng)
    return _module_client.fifo(n, out_degree, in_degree)

def random_orders(n, orders, rng=None):
    """Random-Orders method using module's random number generator."""

    if rng is not None:
        set_rng(rng)
    return _module_client.random_orders(n, orders)

__all__ = ['lib', 'client', 'set_rng', 'gnp', 'gnm', 'lbl', 'fifo', 'random_orders', 'version_info']
