#ifndef BLAB_ENVIRONMENT_H
#define BLAB_ENVIRONMENT_H

#include <string>
#include <vector>

#include "meter.h"

namespace blab {
    class environment {
    private:
        using named_meter = std::pair<std::string, std::unique_ptr<meter>>;

        std::vector<named_meter> meters;

        void add_meter(const std::string&, std::unique_ptr<meter>);
    public:
        template <class T>
        T &create_meter(const std::string &tag) {
	        T *meter_ptr = new T();

            std::unique_ptr<meter> meter_smart_ptr(meter_ptr);

            add_meter(tag, std::move(meter_smart_ptr));

            return *meter_ptr;
        }

        std::vector<std::pair<std::string, std::unique_ptr<metric>>> get_metrics();

        void clear();
    };

    class static_environment {
    private:
        static environment *env;

	friend void static_environment_cleanup();
    public:
        static environment *instance();
    };
}

#endif //BLAB_ENVIRONMENT_H
