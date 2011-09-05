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
#ifndef __otbWrapperCommandLineLauncher_h
#define __otbWrapperCommandLineLauncher_h

//#include <string>
#include "otbMacro.h"
#include "itkObject.h"
#include "itkObjectFactory.h"

//#include "otbWrapperTypes.h"
//#include "otbWrapperParameterGroup.h"
#include "otbWrapperApplication.h"
#include "otbWrapperParameter.h"
#include "itksys/SystemTools.hxx"
#include "otbWrapperCommandLineParser.h"

namespace otb
{
namespace Wrapper
{
  
/** \class CommandLineLauncher
 *  \brief This class check the validity of a command line application.
 *  
 * To be valid, the expression must be as follow:
 * ModuleName --attribut1_Key attrubut1_Value --attribut2_Key attrubut2_Value
 * After the attribut key, if the user give several values (expression without \"--\" separated by space), it will automacally be interpreted as a list.
 * The module name can be set as the first element of the expression or in the expression with the key --moduleName.
 * The exe path have to be set in with the option --modulePath.
 */

class ITK_EXPORT CommandLineLauncher : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef CommandLineLauncher Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Defining ::New() static method */
  itkNewMacro(Self);
  
  /** RTTI support */
  itkTypeMacro(CommandLineLauncher,itk::Object);
  
  /** Parse result enum */
  typedef CommandLineParser::ParseResultType ParseResultType;
  typedef enum { OKPARAM, MISSINGMANDATORYPARAMETER, MISSINGPARAMETERVALUE, WRONGPARAMETERVALUE,  INVALIDNUMBEROFVALUE, DEFAULT} ParamResultType;
  

  /** Load the application in several steps :
   * - Load the paths
   * - Load the application using the ApplicationRegistry
   */
  void Load();

  /** same as Load method but set the expression before. */
  void Load( const std::string & exp );

  /** Launch the process, using the Execute application method 
   * The method will check if the user asked for help (looking at --help key) before loading parameter and launching process.
   **/
  void Execute();

  /** Launch the process, using the ExecuteAndWriteOutput application method 
   * The method will check if the user asked for help (looking at --help key) before loading parameter and launching process.
   */
  void ExecuteAndWriteOutput();

  /** Create and display the help of the application */   
  void DisplayHelp();

protected:
  /** Constructor */
  CommandLineLauncher();

  CommandLineLauncher(const char * exp);

  /** Destructor */
  virtual ~CommandLineLauncher();

  /** Load the executable path. It looks for the key --modulePath, extract and interpret as path the following strings. */
  void LoadPath();

 /** Load the application calling the CreateApplication method of the ApplicationRegistry classes. 
  * Pay attention, the executable paths have to be loaded or set in the environment before calling the function. */
  void LoadApplication();

  /** Parse the user expression, extract the key and the associated string and set it as value of each corresonding application parameter. */
  CommandLineLauncher::ParamResultType LoadParameters();

  /** Method used to determine the type of a a parameter (trying a dynamic_cast). */
  //template <class TParameterType>
  //bool CanCreateParameter(Parameter * param);
  
  /** Set value to prameter using SetValue(string) method. */
  //template <class TParameterType>
  //void SetValueToParameter(Parameter * param, const std::string & val );
  
  /** Set value to prameter using SetValue(vector<string>) method. */
  //template <class TParameterType>
  //void SetValueToParameter(Parameter * param, const std::vector<std::string> & val);
        
  /** Set value to prameter using SetValue(string) method. */              
  //template <class TParameterType>
  //void SetFileNameToParameter(Parameter * param, const std::string & val );
  
  /** Set value to prameter using SetValue(vector<string>) method. */
  //template <class TParameterType>
  //void SetFileNameToParameter(Parameter * param, const std::vector<std::string> & val);
  
  /** Set value to prameter using SetValue(string) method. */               
  //template <class TParameterType>
  //void SetFromFileNameToParameter(Parameter * param, const std::string & val );
  
  /** Set value to prameter using SetValue(vector<string>) method. */
  //template <class TParameterType>
  //void SetFromFileNameToParameter(Parameter * param, const std::vector<std::string> & val );

private:

  CommandLineLauncher(const CommandLineLauncher &); //purposely not implemented
  void operator =(const CommandLineLauncher&); //purposely not implemented
  
  std::string m_Path;

  Application::Pointer m_Application;
  std::string m_Expression;
  CommandLineParser::Pointer m_Parser;

}; //end class

} // end namespace Wrapper
} //end namespace otb

#endif // __otbWrapperCommandLineLauncher_h_
