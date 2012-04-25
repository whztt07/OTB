/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbOGRFieldWrapper_h
#define __otbOGRFieldWrapper_h

#include <vector>
#include <string>
#include <boost/mpl/map.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/at.hpp>

// #include <boost/mpl/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/shared_ptr.hpp>
class OGRFieldDefn;
// class OGRFeature;
#include "ogr_feature.h" // OGRFeature::*field_getters
#include "ogr_core.h" // OGR enums
#include "itkIndent.h"
#include <cassert>

namespace otb { namespace ogr {

/**\ingroup Geometry
 * \defgroup GeometryInternals Geometry Internals
 */

/**\ingroup GeometryInternals
 * Namespace used to host internal meta-prog definitions.
 */
namespace metaprog { // namespace metaprog
using namespace boost::mpl;

/**\ingroup GeometryInternals
 * Associative map of C++ types to OGR field types (\c OGRFieldType).
 * \internal Relies on Boost.MPL
 */
typedef boost::mpl::map
  < pair<int, int_<OFTInteger> >
  , pair<std::vector<int>, int_<OFTIntegerList> >
  , pair<double, int_<OFTReal> >
  , pair<std::vector<double>, int_<OFTRealList> >
  , pair<std::string, int_<OFTString> >
  , pair<std::vector<std::string>, int_<OFTStringList> >
  // OFTBinary
  // OFTDate
  // OFTTime
  // OFTDateTime
  > FieldType_Map;

/**\ingroup GeometryInternals
 * \class MemberGetterPtr
 * Type for hosting simple member-function pointers to field getters.
 * \tparam T type of field according to OGR API.
 * \tparam ptr_to_function member function pointer to a field getter from \c
 *         OGRFeature.
 * \tparam FinalReturnType type of the field according to OTB wrappers (default
 * <tt> = T</tt>)
 *
 * \internal
 * This is a hack to pass a member function pointer as template-parameter.
 */
template
  < typename T
  , T ( OGRFeature::*ptr_to_function )(int)
  , typename FinalReturnType = T
  > class MemberGetterPtr
    {
  public:
    static FinalReturnType call(OGRFeature &f, int index)
      {
      return (f.*ptr_to_function)(index);
      }
    };

/**\ingroup GeometryInternals
 * \class MemberContainerGetterPtr
 * Type for hosting simple member-function pointers to list-field getters.
 * \tparam T type of field according to OGR API.
 * \tparam ptr_to_function member function pointer to a list-field getter from
 * \c OGRFeature.
 * \tparam FinalReturnType type of the list-field according to OTB wrappers
 * (default <tt> = std::vector<T></tt>)
 *
 * \internal
 * This is a hack to pass a member function pointer as template-parameter.
 */
template
  < typename T
  , T const* ( OGRFeature::*ptr_to_function )(int, int*)
  , typename FinalReturnType = std::vector<T>
  > class MemberContainerGetterPtr
    {
  public:
    static FinalReturnType call(OGRFeature &f, int index)
      {
      int nb = 0;
      T const* raw_container = (f.*ptr_to_function)(index, &nb);
      const FinalReturnType res(raw_container+0, raw_container+nb);
      return res;
      }
    };

/**\ingroup GeometryInternals
 * Associative map of OGR field types (\c OGRFieldType) to their associated
 * getters.
 * \internal Relies on Boost.MPL
 */
typedef map
  < pair<int_<OFTInteger>,     MemberGetterPtr<int,             &OGRFeature::GetFieldAsInteger> >
  , pair<int_<OFTIntegerList>, MemberContainerGetterPtr<int,    &OGRFeature::GetFieldAsIntegerList> >
  , pair<int_<OFTReal>,        MemberGetterPtr<double,          &OGRFeature::GetFieldAsDouble> >
  , pair<int_<OFTRealList>,    MemberContainerGetterPtr<double, &OGRFeature::GetFieldAsDoubleList> >
  , pair<int_<OFTString>,      MemberGetterPtr<char const*,     &OGRFeature::GetFieldAsString, std::string> >
  // , pair<int_<OFTStringList>,  MemberGetterPtr<char const*,     &OGRFeature::GetFieldAsString, std::string> >
  > FieldGetters_Map;

} // namespace metaprog

/**\ingroup Geometry
 * \class FieldDefn
 * \brief Encapsulation of \c OGRFieldDefn
 * \invariant <tt>m_definition != 0</tt>
 * \invariant \c m_definition lifetime is of the responsability of the owning \c
 * \c OGRFeatureDefn.
 * \sa OGRFieldDefn
 */
class FieldDefn
  {
public:
  FieldDefn(OGRFieldDefn& definition) : m_definition(&definition){ }
  std::string  GetName() const;
  OGRFieldType GetType() const;

private:
  OGRFieldDefn * m_definition;
  };

#if 0
template <typename FieldType> struct FieldDecodingTraitsGetter {};
template <typename FieldType> struct FieldDecodingTraits
  {
  // good for int, double, char const*, list of strings
  typedef FieldType Type;
  static void Set(OGRFeature &f, int index, Type const& value)
    {
    f.SetField(index, value);
    }
  };

template <> struct FieldDecodingTraitsGetter<int>
  {
  typedef int Type;
  static Type Get(OGRFeature &f, size_t index)
    {
    return f.GetFieldAsInteger(index);
    }
  };

template <> struct FieldDecodingTraitsGetter<double>
  {
  typedef double Type;
  static Type Get(OGRFeature &f, size_t index)
    {
    return f.GetFieldAsDouble(index);
    }
  };

template <> struct FieldDecodingTraitsGetter<std::string>
  {
  typedef std::string Type;
  static Type Get(OGRFeature &f, size_t index)
    {
    return f.GetFieldAsString(index);
    }
  };
#endif

/*===========================================================================*/
/*=================================[ Field ]=================================*/
/*===========================================================================*/
class Feature;
/**\ingroup Geometry
 * \class Field
 * \brief Encapsulation of \c OGRField
 * Instances of \c Field are expected to be built from an existing \c Feature
 * with which they'll share their owning \c OGRFeature.
 * \sa OGRField
 */
class Field
  {
public:
  Field(Feature & feature, size_t index);
  // Field(OGRFeature& field, FieldDefn const& definition);
  FieldDefn const& GetDefinition() const;
  OGRFieldType GetType() const
    { return m_Definition.GetType(); }
  std::string GetName() const
    { return m_Definition.GetName(); }

  bool HasBeenSet() const;
  void Unset() const;

#if 0
  template <typename T> void SetValue(T const& v)
    {
    assert(m_Definition.GetType() == FieldTraits<T>::kind);
    FieldDecodingTraits<T>::Set(m_Feature.ogr(), m_index, v);
    }
#endif
  template <typename T> T GetValue() const
    {
    const int VALUE = boost::mpl::at<metaprog::FieldType_Map, T>::type::value;
    typedef typename boost::mpl::at<metaprog::FieldType_Map, T>::type Kind;
    BOOST_STATIC_ASSERT(!(boost::is_same<Kind, boost::mpl::void_>::value));
    assert(m_Definition.GetType() == Kind::value);
    typedef typename boost::mpl::at<metaprog::FieldGetters_Map, Kind>::type GetterType;
    BOOST_STATIC_ASSERT(!(boost::is_same<GetterType, boost::mpl::void_>::value));
    assert(m_index >= 0 && m_index < m_Feature->GetFieldCount());
    return GetterType::call(*m_Feature, m_index);
    }

  std::ostream & PrintSelf(std::ostream&os, itk::Indent indent) const;
private:
  FieldDefn                       m_Definition;
  boost::shared_ptr<OGRFeature> & m_Feature;
  // all the fields decoding is at the wrong place (OGRFeature instead of
  // OGRField) => need for an index
  size_t                          m_index;
  };

} } // end namespace otb::ogr

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbOGRFieldWrapper.txx"
#endif

#endif // __otbOGRFieldWrapper_h