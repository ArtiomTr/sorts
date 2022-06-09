#ifndef BLAB_EXPERIMENT_H
#define BLAB_EXPERIMENT_H

#include <vector>
#include <functional>

#include "report.h"
#include "environment.h"

namespace blab {
    template <class Input, class Output>
    class experiment {
    private:
        using Subject = std::function<Output(Input)>;

        std::vector<std::pair<std::string, Subject>> subjects;
        std::function<bool(Output)> checker;
        Input input;
    public:
        experiment(std::function<bool(Output)> checker,
                   Input input): subjects({}), checker(checker), input(input) {
        }

        void add_subject(const std::string &name, Subject subject) {
            subjects.push_back({name, subject});
        }

        report perform() {
            report result_report{};

            blab::static_environment::instance()->clear();
            for(const auto &[name, subject] : subjects) {
                Input inputCopy = input;
                Output output = subject(inputCopy);

                auto metrics = blab::static_environment::instance()->get_metrics();

                std::unique_ptr<metric> success_metric(new bool_metric(checker(output)));
                
                metrics.push_back({"Success?", std::move(success_metric)});
                
                result_report.add_row(name, metrics);
            }

            return result_report;
        }
    };
}

#endif
