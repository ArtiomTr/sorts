#ifndef BLAB_METER_H
#define BLAB_METER_H

#include "metric.h"

#include <chrono>
#include <memory>

namespace blab {
    class environment;

    class meter {
        friend environment;
    protected:
        explicit meter();

        virtual std::unique_ptr<metric> result() const;
    public:
        ~meter();
    };

    class quantity_meter : public meter {
        friend environment;
    private:
        uint32_t quantity;

        explicit quantity_meter();

	std::unique_ptr<metric> result() const override;
    public:
        void operator+=(uint32_t value);

        void operator++();
    };

    class execution_time_meter : public meter {
        friend environment;
    private:
        std::unique_ptr<std::chrono::time_point<std::chrono::steady_clock>> tp_begin, tp_end;

        explicit execution_time_meter();

	std::unique_ptr<metric> result() const override;

    public:

        void start();

        void stop();
    };
}

#endif //BLAB_METER_H
