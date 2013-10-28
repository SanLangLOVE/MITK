#include "QmitkUSCombinedModalityManagerWidget.h"
#include "ui_QmitkUSCombinedModalityManagerWidget.h"

#include "mitkNavigationDataSource.h"

QmitkUSCombinedModalityManagerWidget::QmitkUSCombinedModalityManagerWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::QmitkUSCombinedModalityManagerWidget)
{
    ui->setupUi(this);

    ui->m_USDevices->Initialize<mitk::USDevice>(mitk::USDevice::US_PROPKEY_LABEL);
    ui->m_TrackingDevices->Initialize<mitk::NavigationDataSource>(mitk::NavigationDataSource::US_PROPKEY_DEVICENAME);
    ui->m_CombinedModalities->Initialize<mitk::USDevice>(mitk::USCombinedModality::US_PROPKEY_LABEL,
                                                         "(" + mitk::USDevice::US_PROPKEY_CLASS + "=" + mitk::USCombinedModality::DeviceClassIdentifier + ")");

    connect( ui->m_USDevices,          SIGNAL( ServiceSelectionChanged(us::ServiceReferenceU) ), this, SLOT(OnSelectedUltrasoundOrTrackingDevice()) );
    connect( ui->m_TrackingDevices,    SIGNAL( ServiceSelectionChanged(us::ServiceReferenceU) ), this, SLOT(OnSelectedUltrasoundOrTrackingDevice()) );
    connect( ui->m_CombinedModalities, SIGNAL( ServiceSelectionChanged(us::ServiceReferenceU) ), this, SLOT(OnSelectedCombinedModality()) );
}

QmitkUSCombinedModalityManagerWidget::~QmitkUSCombinedModalityManagerWidget()
{
    delete ui;
}

void QmitkUSCombinedModalityManagerWidget::OnSelectedUltrasoundOrTrackingDevice()
{
    // create button is enabled only if an ultrasound and a tracking device
    // is selected
    ui->m_CreateCombinedModalityButton->setEnabled(
                ui->m_USDevices->GetIsServiceSelected()
            && ui->m_TrackingDevices->GetIsServiceSelected());
}

void QmitkUSCombinedModalityManagerWidget::OnSelectedCombinedModality()
{
    if ( ui->m_CombinedModalities->GetIsServiceSelected() )
    {
        // remove button is enabled only if a combined modality is selected
        ui->m_RemoveCombinedModalityButton->setEnabled(true);

        mitk::USCombinedModality::Pointer combinedModality =
                dynamic_cast<mitk::USCombinedModality*>(ui->m_CombinedModalities->GetSelectedService<mitk::USDevice>());

        if ( combinedModality.IsNull())
        {
            MITK_WARN << "Cannot get selected combined modality from service list widget. Is the selected item really a USCombinedModality?";
        }

        emit(SignalCombinedModalitySelected(combinedModality));
    }
    else
    {
        // remove button is enabled only if a combined modality is selected
        ui->m_RemoveCombinedModalityButton->setEnabled(false);

        // emit that no combined modality is selected now
        emit(SignalCombinedModalitySelected(0));
    }
}

void QmitkUSCombinedModalityManagerWidget::OnCreateCombinedModalityButtonClicked()
{
    mitk::USDevice::Pointer usDevice = ui->m_USDevices->GetSelectedService<mitk::USDevice>();
    if (usDevice.IsNull())
    {
        MITK_WARN << "No US Device selected for creation of Combined Modality.";
        return;
    }

    mitk::NavigationDataSource::Pointer trackingDevice = ui->m_TrackingDevices->GetSelectedService<mitk::NavigationDataSource>();
    if (trackingDevice.IsNull())
    {
        MITK_WARN << "No Traccking Device selected for creation of Combined Modality.";
        return;
    }

    mitk::USCombinedModality::Pointer combinedModality = mitk::USCombinedModality::New(usDevice, trackingDevice, "", "");

    //combinedModality->SetUltrasoundDevice(usDevice);
    //combinedModality->SetTrackingDevice(trackingDevice);
    combinedModality->Initialize();
}

void QmitkUSCombinedModalityManagerWidget::OnRemoveCombinedModalityButtonClicked()
{
}
