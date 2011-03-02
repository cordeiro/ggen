import sys
from distutils.core import setup, Extension
from distutils.sysconfig import get_python_inc
from subprocess import Popen, PIPE


def check_minimum_requirements():
    # Python 2.6 or later
    if sys.version_info < (2, 6):
        print ("Please upgrade Python to version 2.6 or later.")
        sys.exit(1)

    # igraph library (and python interface) version 0.6 or later
    try:
        import igraph
    except ImportError:
        print ("igraph's python extension module cannot be found. Please install it from: http://igraph.sourceforge.net/download.html")
        sys.exit(1)
    igraph_version = tuple(map(int, igraph.__version__.split(".")))
    if igraph_version < (0, 6):
        print ("You have igraph python extension module version %s installed. Please upgrade it to version 0.6 or later." % igraph.__version__)
        sys.exit(1)

    # PyGSL version 0.9.5
    try:
        import pygsl
    except ImportError:
        print "pygsl (a python wrapper for the GSL library) cannot be found. Please install it from: http://pygsl.sourceforge.net/"
        sys.exit(1)
    import pygsl._version
    pygsl_version = tuple(map(int,pygsl._version.version.split(".")))
    if pygsl_version < (0, 9, 5):
        print ("You have pygsl %s version installed. Please upgrade it to version 0.9.5 or later" % pygsl._version.version)
        sys.exit(1)

def pkgconfig(package):
    def paths(output_from_pkgconfig):
        return map(lambda line: line[2:].strip(), output_from_pkgconfig.split())
    inc_dirs = paths(Popen(["pkg-config", "--cflags-only-I", package], stdout=PIPE).communicate()[0])
    lib_dirs = paths(Popen(["pkg-config", "--libs-only-L", package], stdout=PIPE).communicate()[0])
    return inc_dirs, lib_dirs


check_minimum_requirements()

ggen_includes, ggen_libs = pkgconfig("ggen")

lib_module = Extension('ggen.lib',
                       sources      = ['src/python-lib-module.c'],
                       include_dirs = [get_python_inc(prefix="/usr/local")] + ggen_includes,
                       library_dirs = ggen_libs,
                       libraries    = ['gsl', 'gslcblas', 'm', 'igraph', 'ggen'])

setup(name         = 'GGen',
      version      = '1.0',
      description  = 'Python interface for GGen',
      author       = 'GGen Development Team',
      author_email = 'ggen-devel@lists.ligforge.imag.fr',
      url          = 'http://ggen.ligforge.imag.fr/',
      maintainer       = 'Daniel Cordeiro',
      maintainer_email = 'ggen-devel@lists.ligforge.imag.fr',
      long_description = 'Python interface for GGen random graph generation library',
      license          = 'CeCILL',
      ext_modules  = [lib_module],
      packages     = ['ggen'])
