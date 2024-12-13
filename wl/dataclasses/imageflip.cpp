#include "wl/dataclasses/imageflip.h"

namespace wl {

ImageFlip::ImageFlip(bool verticalFlip, bool horizontalFlip)
    : m_verticalFlip(verticalFlip)
    , m_horizontalFlip(horizontalFlip)
{

}

bool ImageFlip::getVerticalFlip() const
{
    return m_verticalFlip;
}

void ImageFlip::setVerticalFlip(bool flip)
{
    m_verticalFlip = flip;
}

bool ImageFlip::getHorizontalFlip() const
{
    return m_horizontalFlip;
}

void ImageFlip::setHorizontalFlip(bool flip)
{
    m_horizontalFlip = flip;
}

} // namespace wl
