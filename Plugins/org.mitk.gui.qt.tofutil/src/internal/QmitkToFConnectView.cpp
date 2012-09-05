/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

// Qmitk
#include "QmitkToFConnectView.h"

// Qt
#include <QString>
#include <QStringList>
#include <QMessageBox>

#include <usServiceReference.h>
#include <mitkGetModuleContext.h>
#include <mitkIToFDeviceFactory.h>
#include <mitkToFDeviceFactoryManager.h>
#include <mitkToFCameraDevice.h>
#include <mitkIToFDeviceFactory.h>

#include <mitkAbstractToFDeviceFactory.h>

#include <QmitkServiceListWidget.h>



//Microservices
#include <usGetModuleContext.h>
#include <usModule.h>
#include "mitkModuleContext.h"
#include <usServiceInterface.h>
#include <usServiceRegistration.h>


const std::string QmitkToFConnectView::VIEW_ID = "org.mitk.views.tofconnectview";

QmitkToFConnectView::QmitkToFConnectView()
: QmitkAbstractView()
{
}

QmitkToFConnectView::~QmitkToFConnectView()
{
}

void QmitkToFConnectView::SetFocus()
{
  //    this->m_Controls.m_CameraList->setFocus();
  //    GetRegisteredDeviceFactories();
}

void QmitkToFConnectView::CreateQtPartControl( QWidget *parent )
  {
    // create GUI widgets from the Qt Designer's .ui file
    m_Controls.setupUi( parent );

    //CreateDevice-Button
    connect( (QObject*)(m_Controls.m_CreateDevice), SIGNAL(clicked()), this, SLOT(OnToFCameraConnected()) );

    //Initializing the ServiceListWidget with DeviceFactories and Devices on start-uo
    std::string empty= "";
    m_Controls.m_DeviceFactoryServiceListWidget->Initialize<mitk::IToFDeviceFactory>("ToFFactoryName", empty);
    m_Controls.m_ConnectedDeviceServiceListWidget->Initialize<mitk::ToFCameraDevice>("ToFDeviceName", empty);
  }

//Creating a Device
void QmitkToFConnectView::OnToFCameraConnected()
{
  if (m_Controls.m_DeviceFactoryServiceListWidget->GetIsServiceSelected() )
    {
      MITK_INFO << m_Controls.m_DeviceFactoryServiceListWidget->GetSelectedService<mitk::IToFDeviceFactory>()->GetFactoryName();

      mitk::IToFDeviceFactory* factory = m_Controls.m_DeviceFactoryServiceListWidget->GetSelectedService<mitk::IToFDeviceFactory>();
      dynamic_cast<mitk::AbstractToFDeviceFactory*>(factory)->ConnectToFDevice();
    }
    else
    {
      QMessageBox::warning(NULL, "Warning", QString("No Device Factory selected. Unable to create a Device!\nPlease select an other Factory!"));
    }

}
