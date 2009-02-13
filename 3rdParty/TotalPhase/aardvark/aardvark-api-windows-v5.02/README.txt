                      Aardvark I2C/SPI Software
                      -------------------------

Introduction
------------
This software is used to interface with the Aardvark I2C/SPI
Embedded Systems Interface.  It provides APIs in a number of
languages for maximum development flexibility.


Directory Structure
-------------------
aainfo        - adapter information utility
c/            - C/C++ language binding files and examples
python/       - Python language binding files and examples
csharp/       - C# language binding files and examples
net/          - .NET language binding files
vb.net/       - Visual Basic .NET language binding files and examples
vb6/          - Visual Basic 6 language binding files and examples

The latter two directories are only available in the Windows package
of the API.

See the EXAMPLES.txt and the README.txt in each language subdirectory
for details on the included source code that demonstrates the usage of
the API.


USB Driver
----------
Under Windows, ensure the device drivers have been installed before
plugging in any devices or running software.  Refer to the Aardvark
datasheet for more details on the Windows USB driver.

Under Linux, the libusb library must be installed since the
Aardvark software library is dynamically linked to libusb.

Under Mac OS X, a specific kernel driver is not required, but it is
recommended to install the latest OS X updates.  The Mac OS X binary
supports Intel versions of Mac OS X 10.4 Tiger or higher.


C/C++ bindings
--------------
1) Create a new C/C++ project or open an existing project
2) Add aardvark.c and aardvark.h to the project
3) Place aardvark.dll (or aardvark.so for Linux/Darwin) in the PATH
4) Place #include "aardvark.h" in any module that uses the API
5) Develop, compile, and run your project


Python bindings
---------------
If not already installed, download Python from:
http://www.python.org/2.5/

1) Copy aardvark_py.py to a new folder
2) Copy aardvark.dll (or aardvark.so for Linux/Darwin) to the same folder
3) Create a new script (i.e. program.py)
4) Put the following line in your script file:
   from aardvark_py import *
5) Develop and run your script

There are two main difference between the Aardvark API documented in the
datasheet and the Aardvark Python API.

1) The "array" type is used for passing data to and from the
Aardvark Python API.  See the Python documentation for more
information on this type.

2) Aardvark Python API functions can return multiple arguments on
the left hand side.  This obviates the need to pass pointers to
output arguments on the right hand side of API functions.

3) There are a variety of ways one can call API functions that have
array arguments.

  All arrays can be passed into the API as an ArrayType object or as
  a tuple (array, length), where array is an ArrayType object and
  length is an integer.  The user-specified length would then serve
  as the length argument to the API funtion (please refer to the
  product datasheet).  If only the array is provided, the array's
  intrinsic length is used as the argument to the underlying API
  function.

  The ability to pass in a pre-allocated array along with a separate
  length allows the performance-minded programmer to use a single
  array for repeated calls to the API, simply changing the contents
  of the array and/or the specified length as needed.

  Additionally, for arrays that are filled by the API function, an
  integer can be passed in place of the array argument and the API
  will automatically create an array of that length.  All output
  arrays, whether passed in or generated, are passed back in the
  returned tuple.

The calling conventions for each Aardvark Python API function is
clearly documented in the comments of aardvark_py.py.


C# bindings
-----------
1) Create a new C/C++ project or open an existing project
2) Add aardvark.cs to the project
3) For Windows, place aardvark.dll in the PATH;
   For Linux rename aardvark.so to libaardvark.so and place in the
   LD_LIBRARY_PATH
4) Develop, compile, and run your project

Every API function that accepts an array argument must be accompanied
by a separate length field, even though the array itself intrinsically
has a length.  See the discussion of the Python API above explaining
the rationale for such an interface.

For C#, structures that contain arrays do not have the length field in
the structure as documented in the datasheet.  Instead, the intrinsic
length of the array is used when the structure is an argument to an
API function.


.NET and VB.NET bindings
------------------------
Copy aardvark.dll and aardvark_net.dll to your application development
environment.  The aardvark.dll contains the APIs themselves and the
aardvark_net.dll provides the .NET interface to the APIs.  For
detailed documentation of APIs refer to the datasheet and the comments
in the C# (aardvark.cs) binding.

Like for C#, every API function that accepts an array argument must be
accompanied by a separate length field.  Also as in C#, arrays in
structures use their intrinsic length instead of having a separate
length field in the structure (see above).


Visual Basic 6
--------------
Copy aardvark.dll to your application development environment and
integrate the aardvark.bas interface layer into your application.
For detailed documentation of the APIs refer to the datasheet
and the comments in the aardvark.bas file.

Like for C#, .NET, and VB.NET, every API function that accepts an
array argument must be accompanied by a separate length field.  Also
as in C#, .NET and VB.NET, arrays in structures use their intrinsic
length instead of having a separate length field in the structure (see
above).


Open Source
-----------
When running on Mac OS X, this software uses a modified version of
the libusb-0.1.12 open source library.  The library is licensed under
LGPL v2.1, the full text of which can be found at the following URL:
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html

To link against a custom version of the library, set the LIBUSBTP
environment variable to the full path of the custom library.  The
software will then link against the specified custom library.
