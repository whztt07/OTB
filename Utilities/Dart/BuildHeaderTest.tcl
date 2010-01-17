# =========================================================================
# 
#   Program:   Insight Segmentation & Registration Toolkit
#   Module:    $RCSfile: BuildHeaderTest.tcl,v $
#   Language:  Tcl
#   Date:      $Date: 2006/09/25 21:12:48 $
#   Version:   $Revision: 1.8 $
# 

# Copyright (c) 2001 Insight Consortium
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:

#  * Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.

#  * Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.

#  * The name of the Insight Consortium, nor the names of any consortium members,
#    nor of any contributors, may be used to endorse or promote products derived
#    from this software without specific prior written permission.

#   * Modified source versions must be plainly marked as such, and must not be
#     misrepresented as being the original software.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


set Header {/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Language:  C++

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
// This file has been generated by BuildHeaderTest.tcl
// Test to include each header file for Insight
}

set Trailer "
int main ( int , char* [] )
{
  
  return EXIT_SUCCESS;
}
"

set D [lsort [glob -nocomplain Code/*]]
set Directories ""
foreach d $D \
{
  if { [file tail $d] != "CVS" && [file isdirectory $d]} \
  {
    lappend Directories [file tail $d]
  }
}

foreach Directory $Directories \
{
  if { ![file exists [file join Testing Code $Directory]] } \
  {
    continue
  }
  set f [open [file join Testing Code $Directory itk${Directory}HeaderTest.cxx] w]
  puts $f $Header
  foreach File [lsort [glob -nocomplain [file join Code $Directory *.h]]] \
  {
      if {[file tail $File] == "itkTestMain.h"} {
	  continue;
      }
      if {[file tail $File] == "itkWindows.h"} {
	  continue;
      }
      if {[file tail $File] == "itkWin32Header.h"} {
	  continue;
      }
      if {[file tail $File] == "itkWin32OutputWindow.h"} {
	  continue;
      }
      if {[file tail $File] == "itkImageViewerWindow.h"} {
	  continue;
      }
#
# include the .h if there is no .txx
# otherwise include the .txx
#
      set txxFile "[file rootname $File].txx"
      if {[file tail $txxFile] == "itkImageViewer.txx"} {
	  continue;
      }
      if { [file exists $txxFile] } {
          puts $f "#include \"[file tail $txxFile]\""
      } else {
        puts $f "#include \"[file tail $File]\""
      }
  }
  puts $f $Trailer
  close $f
}

