#ifndef WL_ERROR_H
#define WL_ERROR_H

#include <etl/enum_type.h>

namespace wl {

/**
 * @class Error
 * @headerfile error.h "wl/error.h"
 * @brief Represents error codes for device, supporting conversion to readable string representations.
 */
struct Error 
{
    /**
     * @brief Enumeration of error codes.
     */
    enum enum_type
    {
        TCSI__INVALID_SIZE,                  ///< TCSI packet size is too small or doesnt match size byte in header
        TCSI__INVALID_SYNCHRONIZATION_VALUE, ///< TCSI packet synchronization value is invalid
        TCSI__INVALID_STATUS_OR_COMMAND,     ///< TCSI packet status or command byte is invalid
        TCSI__INVALID_CHECKSUM,              ///< TCSI packet checksum is invalid
        TCSI__INVALID_RESPONSE_ADDRESS,      ///< TCSI packet address is invalid
        TCSI__RESPONSE_DEVICE_BUSY,          ///< TCSI packet status is busy
        TCSI__RESPONSE_STATUS_ERROR,         ///< TCSI packet status is error
        
        DATALINK__NO_CONNECTION,             ///< No connection on data link
        DATALINK__TIMEOUT,                   ///< Read/write timed out

        PROTOCOL__NO_DATALINK,               ///< No data link set in protocol layer

        MEMORYSPACE__INVALID_ADDRESS,        ///< Address doesnt match any known address

        DEVICE__NO_PROTOCOL,                 ///< No protocol layer set
        DEVICE__INVALID_DATA_SIZE,           ///< Data size is 0 or is not allowed for device
        DEVICE__INVALID_ADDRESS,             ///< Address is invalid
        DEVICE__DISCONNECTED,                ///< Transfer failed several times, assuming connection broke
        DEVICE__BUSY,                        ///< Device busy for more than allowed time

        INVALID_DATA                         ///< Invalid data for conversion
    };

    ETL_DECLARE_ENUM_TYPE(Error, int)
    ETL_ENUM_TYPE(TCSI__INVALID_SIZE, "TCSI__INVALID_SIZE")
    ETL_ENUM_TYPE(TCSI__INVALID_SYNCHRONIZATION_VALUE, "TCSI__INVALID_SYNCHRONIZATION_VALUE")
    ETL_ENUM_TYPE(TCSI__INVALID_STATUS_OR_COMMAND, "TCSI__INVALID_STATUS_OR_COMMAND")
    ETL_ENUM_TYPE(TCSI__INVALID_CHECKSUM, "TCSI__INVALID_CHECKSUM")
    ETL_ENUM_TYPE(TCSI__INVALID_RESPONSE_ADDRESS, "TCSI__INVALID_RESPONSE_ADDRESS")
    ETL_ENUM_TYPE(TCSI__RESPONSE_DEVICE_BUSY, "TCSI__RESPONSE_DEVICE_BUSY")
    ETL_ENUM_TYPE(TCSI__RESPONSE_STATUS_ERROR, "TCSI__RESPONSE_STATUS_ERROR")
    ETL_ENUM_TYPE(DATALINK__NO_CONNECTION, "DATALINK__NO_CONNECTION")
    ETL_ENUM_TYPE(DATALINK__TIMEOUT, "DATALINK__TIMEOUT")
    ETL_ENUM_TYPE(PROTOCOL__NO_DATALINK, "PROTOCOL__NO_DATALINK")
    ETL_ENUM_TYPE(MEMORYSPACE__INVALID_ADDRESS, "MEMORYSPACE__INVALID_ADDRESS")
    ETL_ENUM_TYPE(DEVICE__NO_PROTOCOL, "DEVICE__NO_PROTOCOL")
    ETL_ENUM_TYPE(DEVICE__INVALID_DATA_SIZE, "DEVICE__INVALID_DATA_SIZE")
    ETL_ENUM_TYPE(DEVICE__INVALID_ADDRESS, "DEVICE__INVALID_ADDRESS")
    ETL_ENUM_TYPE(DEVICE__DISCONNECTED, "DEVICE__DISCONNECTED")
    ETL_ENUM_TYPE(DEVICE__BUSY, "DEVICE__BUSY")
    ETL_ENUM_TYPE(INVALID_DATA, "INVALID_DATA")
    ETL_END_ENUM_TYPE
};

}

#endif // WL_ERROR_H