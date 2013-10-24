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

#include "mitkUSCombinedModality.h"
#include "mitkUSDevice.h"
#include "mitkNavigationDataSource.h"

const std::string mitk::USCombinedModality::DeviceClassIdentifier = "org.mitk.modules.us.USCombinedModality";

mitk::USCombinedModality::USCombinedModality(std::string manufacturer, std::string model)
    : mitk::USDevice(manufacturer, model)
{

}

mitk::USCombinedModality::~USCombinedModality()
{

}


std::string mitk::USCombinedModality::GetDeviceClass()
{
    return DeviceClassIdentifier;
}

mitk::USImageSource::Pointer mitk::USCombinedModality::GetUSImageSource()
{
    if (m_UltrasoundDevice.IsNull())
    {
        MITK_ERROR("USCombinedModality")("USDevice") << "UltrasoundDevice must not be null.";
        mitkThrow() << "UltrasoundDevice must not be null.";
    }

    return m_UltrasoundDevice->GetUSImageSource();
}

mitk::USControlInterfaceBMode::Pointer mitk::USCombinedModality::GetControlInterfaceBMode()
{
    if (m_UltrasoundDevice.IsNull())
    {
        MITK_ERROR("USCombinedModality")("USDevice") << "UltrasoundDevice must not be null.";
        mitkThrow() << "UltrasoundDevice must not be null.";
    }

    return m_UltrasoundDevice->GetControlInterfaceBMode();
}

mitk::USControlInterfaceProbes::Pointer mitk::USCombinedModality::GetControlInterfaceProbes()
{
    if (m_UltrasoundDevice.IsNull())
    {
        MITK_ERROR("USCombinedModality")("USDevice") << "UltrasoundDevice must not be null.";
        mitkThrow() << "UltrasoundDevice must not be null.";
    }

    return m_UltrasoundDevice->GetControlInterfaceProbes();
}

mitk::USControlInterfaceDoppler::Pointer mitk::USCombinedModality::GetControlInterfaceDoppler()
{
    if (m_UltrasoundDevice.IsNull())
    {
        MITK_ERROR("USCombinedModality")("USDevice") << "UltrasoundDevice must not be null.";
        mitkThrow() << "UltrasoundDevice must not be null.";
    }

    return m_UltrasoundDevice->GetControlInterfaceDoppler();
}

mitk::AffineTransform3D::Pointer mitk::USCombinedModality::GetCalibration()
{
    std::string calibrationKey = this->GetIdentifierForCurrentCalibration();
    if (calibrationKey.empty())
    {
        MITK_WARN << "Could not get a key for the calibration.";
        return 0;
    }

    // find calibration for combination of probe identifier and depth
    std::map<std::string, mitk::AffineTransform3D::Pointer>::iterator calibrationIterator
            = m_Calibrations.find(calibrationKey);

    if (calibrationIterator == m_Calibrations.end())
    {
        MITK_WARN << "No calibration found for selected probe and depth.";
        return 0;
    }

    return calibrationIterator->second;
}

void mitk::USCombinedModality::SetCalibration (mitk::AffineTransform3D::Pointer calibration)
{
  if (calibration.IsNull())
  {
    MITK_WARN << "Null pointer passed to SetCalibration of mitk::USDevice. Ignoring call.";
    return;
  }

  std::string calibrationKey = this->GetIdentifierForCurrentCalibration();
  if (calibrationKey.empty())
  {
      MITK_WARN << "Could not get a key for the calibration -> Calibration cannot be set.";
      return;
  }

  m_Calibrations[calibrationKey] = calibration;

  m_Metadata->SetDeviceIsCalibrated(true);

  if (m_ServiceRegistration != 0)
  {
    this->UpdateServiceProperty(mitk::USImageMetadata::PROP_DEV_ISCALIBRATED, true);
  }
}

bool mitk::USCombinedModality::OnInitialization()
{
    if (m_UltrasoundDevice.IsNull())
    {
        MITK_ERROR("USCombinedModality")("USDevice") << "UltrasoundDevice must not be null.";
        mitkThrow() << "UltrasoundDevice must not be null.";
    }

    return m_UltrasoundDevice->Initialize();
}

bool mitk::USCombinedModality::OnConnection()
{
    if (m_UltrasoundDevice.IsNull())
    {
        MITK_ERROR("USCombinedModality")("USDevice") << "UltrasoundDevice must not be null.";
        mitkThrow() << "UltrasoundDevice must not be null.";
    }

    return m_UltrasoundDevice->Connect();
}

bool mitk::USCombinedModality::OnDisconnection()
{
    if (m_UltrasoundDevice.IsNull())
    {
        MITK_ERROR("USCombinedModality")("USDevice") << "UltrasoundDevice must not be null.";
        mitkThrow() << "UltrasoundDevice must not be null.";
    }

    return m_UltrasoundDevice->Disconnect();
}

bool mitk::USCombinedModality::OnActivation()
{
    if (m_UltrasoundDevice.IsNull())
    {
        MITK_ERROR("USCombinedModality")("USDevice") << "UltrasoundDevice must not be null.";
        mitkThrow() << "UltrasoundDevice must not be null.";
    }

    return m_UltrasoundDevice->Activate();
}

bool mitk::USCombinedModality::OnDeactivation()
{
    if (m_UltrasoundDevice.IsNull())
    {
        MITK_ERROR("USCombinedModality")("USDevice") << "UltrasoundDevice must not be null.";
        mitkThrow() << "UltrasoundDevice must not be null.";
    }

    m_UltrasoundDevice->Deactivate();

    return m_UltrasoundDevice->GetIsInitialized();
}

void mitk::USCombinedModality::GenerateData()
{
    m_UltrasoundDevice->Update();
    mitk::USImage::Pointer image = m_UltrasoundDevice->GetOutput();

    std::string calibrationKey = this->GetIdentifierForCurrentCalibration();
    if ( ! calibrationKey.empty() )
    {
        std::map<std::string, mitk::AffineTransform3D::Pointer>::iterator calibrationIterator
                = m_Calibrations.find(calibrationKey);
        if ( calibrationIterator != m_Calibrations.end())
        {
            // transform image according to callibration if one is set
            // for current configuration of probe and depth
            image->GetGeometry()->SetIndexToWorldTransform(calibrationIterator->second);
        }
    }

    // TODO: do processing here

    this->SetNthOutput(0, image);
}

std::string mitk::USCombinedModality::GetIdentifierForCurrentCalibration()
{
    mitk::USControlInterfaceProbes::Pointer probesInterface = this->GetControlInterfaceProbes();
    if ( probesInterface && ! probesInterface->GetIsActive())
    {
        MITK_WARN << "Cannot get calibration as probes interface is not active.";
        return std::string();
    }

    // get probe identifier from control interface for probes
    std::string probeName;
    if ( ! probesInterface )
    {
        probeName = "default";
    }
    else
    {
        mitk::USProbe::Pointer curProbe = this->GetControlInterfaceProbes()->GetSelectedProbe();
        if (curProbe.IsNull())
        {
            MITK_WARN << "Cannot get calibration as current probe is null.";
            return std::string();
        }

        probeName = curProbe->GetName();
    }

    // get string for depth value from the micro service properties
    std::string depth;
    us::ServiceProperties::iterator depthIterator = m_ServiceProperties.find(US_PROPKEY_BMODE_DEPTH);
    if (depthIterator == m_ServiceProperties.end())
    {
        depth = depthIterator->second.ToString();
    }
    else
    {
        depth = "0";
    }

    return probeName + depth;
}
