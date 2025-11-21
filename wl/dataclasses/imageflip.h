#ifndef WL_IMAGEFLIP_H
#define WL_IMAGEFLIP_H

namespace wl {

/**
 * @class ImageFlip
 * @headerfile imageflip.h "wl/dataclasses/imageflip.h"
 * @brief A class to manage and apply image flip transformations (vertical and horizontal).
 * @see registers_image_flip
 */
class ImageFlip
{
public:
    /**
     * @brief Default constructor.
     * Initializes both vertical and horizontal flip values to false (no flip).
     */
    explicit ImageFlip();

    /**
     * @brief Constructor to initialize the flip settings.
     * @param verticalFlip Boolean indicating whether to flip the image vertically.
     * @param horizontalFlip Boolean indicating whether to flip the image horizontally.
     */
    explicit ImageFlip(bool verticalFlip, bool horizontalFlip);

    /**
     * @brief Gets the vertical flip status.
     * @return `true` if the image is set to be flipped vertically, `false` otherwise.
     */
    bool getVerticalFlip() const;

    /**
     * @brief Sets the vertical flip status.
     * @param flip Boolean indicating whether to flip the image vertically (`true` or `false`).
     */
    void setVerticalFlip(bool flip);

    /**
     * @brief Gets the horizontal flip status.
     * @return `true` if the image is set to be flipped horizontally, `false` otherwise.
     */
    bool getHorizontalFlip() const;

    /**
     * @brief Sets the horizontal flip status.
     * @param flip Boolean indicating whether to flip the image horizontally (`true` or `false`).
     */
    void setHorizontalFlip(bool flip);

private:
    bool m_verticalFlip;
    bool m_horizontalFlip;
};

} // namespace wl

#endif // WL_IMAGEFLIP_H