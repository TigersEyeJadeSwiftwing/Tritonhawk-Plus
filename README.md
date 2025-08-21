Tritonhawk-Plus, a set of open-source C++ plug-ins for GIMP 3.x.

Original, official GitHub repository:
https://github.com/TigersEyeJadeSwiftwing/Tritonhawk-Plus

Features:
- 128-bit depth floating-point processing for all plug-ins.
- Multi-threaded, taking advantage of multiple cores, using OpenMP.
- Plug-ins have lots of options and parameters, for doing what you want exactly how you want it.
  If you want to resize an image with super-high quality with a lot of picky specifics like
  you gosh darn mean it, and put up with slightly slower processing in favor of very high precision
  and extreme control, this software might be what you're looking for.  Tritonhawk-Plus might be
  the answer for scientific needs where extreme precision is needed, or at least a start.
- These are C/C++ plug-ins, not Python or SCM.
- No external libraries or other software needed for compiled and built binaries, other than
  GIMP version 3.x.
  The source code features it's own custom-coded 128-bit quad precision math routines and functions
  for itself - no external libquadmath library is or should be needed at all, or linked to.
- Tritonhawk-Plus software can also be built from source to use 128-bit "long double" data types and
  use long double C and C++ math functions, as opposed to using the __float128 data type and the
  custom math functions provided in the included source code and project files.
- Buildable from source with open-source and cross-platform Codeblocks IDE, this repository includes
  Codeblocks workspace and project files, and instructions for building from source on Windows
  using MSYS2 and the Clang64 compiler toolchain it provides.
- Information is available for compiling the plug-ins from source, including compiler and linker
  flags, etc., extracted from the Codeblocks project files, for those who wish to build the software
  without using Codeblocks, such as with another IDE, cmake, etc.
- Designed for modern computers and operating systems, typically less than five years old as of 2025.
- No MSVC support.  GIMP itself cannot be built from source using Microsoft Visual Studio or MSVC
  compilers, and neither can Tritonhawk-Plus software.
- The source code takes advantage of GNU extensions that are intended to work with GCC/Clang64.
- GPLv3 license.

[Install instructions](./INSTALL.md)
[Building from source for Windows, with Codeblocks (MSYS2 + GCC/Clang64 + Codeblocks IDE)](./BUILD_WINDOWS_CB.md)
[Building from source for Windows, for other or no IDEs (MSYS2 + GCC/Clang64)](./BUILD_WINDOWS.md)
[Building from source for non-Windows operating systems](./BUILD_NON_WINDOWS.md)
[GPLv3 License](./LICENSE)

    Tritonhawk Plus plug-ins are designed to work with the latest official release of GIMP, which at the time of this writing is version 3.0.4.  Once the plug-ins are built, they should show up under applicable menus in GIMP, after being added to GIMP's plug-in paths.  They are designed to hopefully work with the existing DLL files and libraries that are installed with GIMP and not need any additional utilities, libraries, or other stuff for working.  They use SSE 4.1 if they are built using the source code and included Codeblocks IDE project files.
    Please note that Tritonhawk-Plus software is NOT designed to include, use, or link to any libquadmath headers, static libraries, or DLL files.  Trying to include or link to any such library will likely cause issues.  If you use GIMP and have other plug-ins that use any libquadmath libraries, DLL or otherwise, Tritonhawk-Plus software should simply ignore such things and not use them.  This project's source code originally included some files borrowed from gcc's own libquadmath, and after many changes and replacing those files with new code, has at best only bits and pieces left over from the files at: https://github.com/gcc-mirror/gcc/tree/master/libquadmath.

More information on GIMP is available at: https://www.gimp.org/.
More information on MSYS2 is available at: https://www.msys2.org/.
More information on Codeblocks is available at: https://www.codeblocks.org/.

    Note that MSYS2 has it's own version of GIMP and Codeblocks IDE, including for it's Clang64 compiler toolchain, as well as others (UCRT64, etc.).  If you want to build Tritonhawk-Plus software from source, it is recommended to install MSYS2's version of GIMP, which installs most of Tritonhawk-Plus's dependencies, quick and easily.  If you happen to have GIMP installed from a regular, plain downloaded ZIP or EXE installer from GIMP's website, that is an official release, you can have both versions of GIMP installed on the same machine at the same time, and they typically play nice with each other, while sharing configuration files that keep track of user preferences.  As for Codeblocks, the same thing more or less applies, as their website has more than one installer available for download depending on what your preferences and your needs are, including whether or not you want a MinGW-w64 compiler toolchain included in the installer or not.  (Codeblocks will work with many compilers, from various sorts of GCC compilers to Microsoft compilers, etc.  It will also work with source code far beyond files with C, C++, Java, JavaScript, Python, and many other programming and scripting languages, as an IDE and editor of source.)  Anyways, MSYS2 has it's own Codeblocks IDE packages it provides, including ones for GCC/Clang64.  As far as I know, the official release installers downloaded from the Codeblocks website as well as the MSYS2-provided ones in their package will work to compile Tritonhawk-Plus software, and the regular version as well as the MSYS2 versions can be installed at the same time, and not conflict with each other.

    The GPL version 3 license that Tritonhawk-Plus software has applies to itself, and doesn't apply to any pictures, photographs, or other image data processed with it.  Such created and-or processed content may have it's own license or copyright that applies to it, or lack of such things.  If you use it with GIMP, or some other software that can use it, it's really none of my business what type of image data you do whatever with using my Tritonhawk-Plus software.  I don't really have any way of policing or controlling that type of thing anyways, and I'm not involved in it.  I also don't have any special rules about government or military use, as I'm not involved in any such things and don't care what someone downloads my open-source software for, so if someone wants my software, or it's source code, to work on some super-extreme top-secret government restricted nonsense that can blow a gas giant planet in half or some other extreme whatever, I don't care and wouldn't want to be involved in it, or even know about it, thank you.  Just download it if you want it, and if not, whatever.
