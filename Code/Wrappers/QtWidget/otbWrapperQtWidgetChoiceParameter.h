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
#ifndef __otbWrapperQtWidgetChoiceParameter_h
#define __otbWrapperQtWidgetChoiceParameter_h

#include <QtGui>
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"

namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetChoiceParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetChoiceParameter(ChoiceParameter*, QtWidgetModel*);
  virtual ~QtWidgetChoiceParameter();

protected slots:
  void SetValue( int value );

protected:
  void CreateWidget();

  ChoiceParameter::Pointer m_ChoiceParam;

private:
  QtWidgetChoiceParameter(const QtWidgetChoiceParameter&); //purposely not implemented
  void operator=(const QtWidgetChoiceParameter&); //purposely not implemented

  QHBoxLayout*    m_MainHLayout;

  QComboBox*      m_ComboBox;
  QStackedWidget* m_StackWidget;

  QVBoxLayout*    m_VLayout;
  QGroupBox*      m_VLayoutGroup;
};

}
}

#endif
