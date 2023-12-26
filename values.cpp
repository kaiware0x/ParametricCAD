
#include "values.h"

TCollection_ExtendedString BaseValue::occName() const
{
    return name.toStdString().c_str();
}

bool BoxValue::isValidConfusion() const
{
    if (!util::isValidConfusion(width.value()) || !util::isValidConfusion(length.value())
        || !util::isValidConfusion(height.value())) {
        return false;
    }

    return true;
}

gp_Pnt BoxValue::center() const
{
    return gp_Pnt(x.value(), y.value(), z.value());
}

double BoxValue::dx() const
{
    return width.value();
}

double BoxValue::dy() const
{
    return height.value();
}

double BoxValue::dz() const
{
    return length.value();
}
