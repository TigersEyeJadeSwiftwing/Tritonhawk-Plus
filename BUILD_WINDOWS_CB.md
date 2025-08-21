Tritonhawk-Plus, a set of open-source C++ plug-ins for GIMP 3.x.



Original, official GitHub repository:

https://github.com/TigersEyeJadeSwiftwing/Tritonhawk-Plus



\[Main readme file](./README.md)



BUILD INSTRUCTIONS FOR WINDOWS, USING CODEBLOCKS, MSYS2, AND GCC/CLANG64 COMPILER TOOLCHAIN



Download MSYS2 from https://www.msys2.org/.  Run the installer, and follow the directions on the MSYS2 website, except that instead of installing the UCRT compiler toolchain, you want to install the GCC/Clang64 compiler toolchain.  To do this, after running the downloaded installer, open up an MSYS2 Clang64 window, and type the following to update some basic stuff with MSYS2:



'''bash

pacman -Suuyy

'''



You may need to do this more than once, especially if the window needs to close as part of the updating process.  Sometimes this updating command needs to be run more than once anyways, as it doesn't always update everything at once.



After updating the basic components of MSYS2 with that command, open up a Clang64 MSYS2 window, and type, or copy and paste, the following, taking note that CTRL-V doesn't paste from the clipboard with MSYS2, which uses an alternate set of keyboard keys instead.  You can right-click on or near the text cursor in the window to paste from the clipboard, as well as have the drop-down menu that appears tell you the latest keyboard shortcut for copying and pasting from an MSYS2 window where you type in commands:



'''bash

pacman -S \\

mingw-w64-clang-x86\_64-clang \\

mingw-w64-clang-x86\_64-clang-tools-extra \\

mingw-w64-clang-x86\_64-libc++ \\

mingw-w64-clang-x86\_64-llvm-openmp \\

mingw-w64-clang-x86\_64-python \\

mingw-w64-clang-x86\_64-codeblocks \\

mingw-w64-clang-x86\_64-doxygen \\

mingw-w64-clang-x86\_64-graphviz \\

mingw-w64-clang-x86\_64-gimp \\

mingw-w64-clang-x86\_64-hunspell-en \\

mingw-w64-clang-x86\_64-gcc \\

mingw-w64-clang-x86\_64-gcc-libs \\

mingw-w64-clang-x86\_64-lfortran

'''



Hitting the ENTER key (duh) after typing or pasting this, will download and install some MSYS2 packages needed for compiling Tritonhawk-Plus software.  It will also install MSYS2's own copy of GIMP and Codeblocks, which shouldn't conflict with any normal, regular official release version of GIMP or Codeblocks that were downloaded from their websites.  The MSYS2 version of GIMP, when installed, will install LibGimp and other dependencies for both GIMP and Tritonhawk-Plus plug-ins in your MSYS2 environment.  Note that GIMP needs and uses Python, even though Tritonhawk-Plus plug-ins don't require Python.  Graphviz is for Codeblocks and the Doxygen wizard that can run inside of Codeblocks, with a Codeblocks plug-in that is included with the Codeblocks IDE.  Hunspell is a language dictionary for Codeblocks.  The package listed above is for many languages, not just English, even though it says "-en" at the end of the package name.



Once you have the packages installed, you need go through some options and preferences menus in the Codeblocks GUI to tell Codeblocks where clang++.exe is, which is part of the GCC/Clang64 compiler toolchain, as well as where a bunch of header files and other library files are for libgimp and other dependencies, etc.



To start Codeblocks, you can type into the MSYS2 command window for Clang64:



,,,bash

codeblocks

,,,



This will start up the Codeblocks IDE.  Later, you can also type "gimp" to run the MSYS2 version of GIMP that should be installed at this point.  If you find you use the MSYS2 versions of GIMP or Codeblocks often, you can find the .exe files for them in Windows File Explorer, located inside the "bin" folder which is inside of the "clang64" folder, which is inside of your "msys64" folder, which by default is in the root folder of your C: drive.  The entire path, if MSYS2 is installed with default options, would be: "C:\\msys64\\clang64\\bin", where you can find codeblocks.exe and gimp.exe.  You can right-click on the .exe files in file explorer, and, as usual with such programs, create a shortcut on your desktop or add them to your start menu, if you find you like or want GIMP or Codeblocks and want to use them often.



If you already have a copy of Codeblocks installed, and are a regular user, then I'm not sure what you're doing reading even half of this document as you are probably a half-way experienced programmer who is beyond baby steps and doesn't need someone holding their hand, and would already know more than half of the things this document explains.  If that's the case, then you simply need to set up your LLVM Clang compiler so that it finds your clang toolchain, and Tritonhawk-Plus already comes with Codeblocks workspace and project files you can open and get started with, and there's really no point in reading the rest of this document.



