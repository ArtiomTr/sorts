#ifndef BLAB_METRIC_H
#define BLAB_METRIC_H

#include <string>
#include <chrono>

namespace blab {
    class metric {
    public:
        virtual std::string to_string() const;
    };

    class uint32_metric : public metric {
    private:
        uint32_t value;
    public:
        explicit uint32_metric(uint32_t value);

        std::string to_string() const override;
    };

    class time_metric : public metric {
    private:
        std::chrono::nanoseconds duration;
    public:
        explicit time_metric(std::chrono::nanoseconds duration);

        std::string to_string() const override;
    };

    class bool_metric : public metric {
    private:
        bool result;
    public:
        explicit bool_metric(bool result);

        std::string to_string() const override;
    };
}

#endif //BLAB_METRIC_H
