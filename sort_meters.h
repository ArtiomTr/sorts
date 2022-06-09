#ifndef PRAT6_SORT_METERS_H
#define PRAT6_SORT_METERS_H

#include "blab/meter.h"
#include "tuple"

std::tuple<blab::quantity_meter &, blab::quantity_meter &, blab::execution_time_meter &> get_sort_meters();

#endif //PRAT6_SORT_METERS_H
