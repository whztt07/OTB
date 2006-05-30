/*=========================================================================

  Program:   KWSys - Kitware System Library
  Module:    $RCSfile: String.hxx.in,v $

  Copyright (c) Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itksys_String_hxx
#define itksys_String_hxx

#include <itksys/stl/string>

namespace itksys
{

/** \class String
 * \brief Short-name version of the STL basic_string class template.
 *
 * The standard library "std::string" type is actually a typedef for
 * "std::basic_string<..long argument list..>".  This string class is
 * simply a subclass of this type with the same interface so that the
 * name is shorter in debugging symbols and error messages.
 */
class itksys_EXPORT String: public itksys_stl::string
{
  /** The original string type.  */
  typedef itksys_stl::string stl_string;

public:

  /** String member types.  */
  typedef stl_string::value_type             value_type;
  typedef stl_string::pointer                pointer;
  typedef stl_string::reference              reference;
  typedef stl_string::const_reference        const_reference;
  typedef stl_string::size_type              size_type;
  typedef stl_string::difference_type        difference_type;
  typedef stl_string::iterator               iterator;
  typedef stl_string::const_iterator         const_iterator;
  typedef stl_string::reverse_iterator       reverse_iterator;
  typedef stl_string::const_reverse_iterator const_reverse_iterator;

  /** String constructors.  */
  String(): stl_string() {}
  String(const value_type* s): stl_string(s) {}
  String(const value_type* s, size_type n): stl_string(s, n) {}
  String(const stl_string& s, size_type pos=0, size_type n=npos):
    stl_string(s, pos, n) {}
}; // End Class: String

} // namespace itksys

#endif
