%include "stl.i"
%include "std_string.i"

%{
#include "src/swig/handler.h"
%}

%module bazel_template_swig;

%include "src/swig/handler.h"
