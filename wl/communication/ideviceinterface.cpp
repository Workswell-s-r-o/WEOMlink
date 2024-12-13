#include "wl/communication/ideviceinterface.h"


namespace wl
{


IDeviceInterface::IDeviceInterface(DeviceEndianity deviceEndianity) :
    m_deviceEndianity(deviceEndianity)
{
}

IDeviceInterface::~IDeviceInterface()
{
}


} // namespace wl
