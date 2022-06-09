#include "sort_meters.h"

#include "blab/environment.h"

std::tuple<blab::quantity_meter &, blab::quantity_meter &, blab::execution_time_meter &> get_sort_meters() {
    blab::environment *env = blab::static_environment::instance();

    auto &assignments_meter = env->create_meter<blab::quantity_meter>("Assignments");
    auto &comparison_meter = env->create_meter<blab::quantity_meter>("Comparisons");
    auto &time_meter = env->create_meter<blab::execution_time_meter>("Time");

    return {assignments_meter, comparison_meter, time_meter};
}
