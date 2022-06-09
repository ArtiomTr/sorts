#ifndef BLAB_REPORT_H
#define BLAB_REPORT_H

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "metric.h"

namespace blab {
    class report {
    private:
        using named_metric = std::pair<std::string, std::unique_ptr<metric>>; 

        std::vector<std::string> titles;
        std::vector<std::size_t> column_width;
        std::vector<std::vector<std::string>> rows;
    public:
        void add_row(const std::string&, const std::vector<named_metric>&);

        friend std::ostream& operator<<(std::ostream&, const report&);

        std::string to_csv() const;
    };
}

#endif //BLAB_REPORT_H
