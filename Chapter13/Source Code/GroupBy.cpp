#include "rxcpp/rx.hpp"

namespace Rx {
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::subjects;
    using namespace rxcpp::util;
}

using namespace std;

struct Employee {
    string name;
    string role;
    int salary;
};

int main()
{
    Rx::subject<Employee> employees;

    // Group Salaries by Role
    auto role_sal = employees.
        get_observable().
        group_by(
            [](Employee& e) { return e.role; },
            [](Employee& e) { return e.salary; });

    // Combine min max and average reductions based on salary.
    auto result = role_sal.
        map([](Rx::grouped_observable<string, int> group) {
        return group.
            count().
            combine_latest(
                [=](int count, int min, int max, double average) {
            return make_tuple(group.get_key(), count, min, max, average);
        },
                group.min(),
            group.max(),
            group.map([](int salary) -> double { return salary; }).average());
    }).
    merge();

    // Display the aggregated result
    result.
        subscribe(Rx::apply_to([](string role, int count, int min, int max, double avg) {
        std::cout << role.c_str() << ":\tCount = " << count << ", Salary Range = [" << min << "-" << max << "], Average Salary = " << avg << endl;
    }));

    // Supplying input data
    Rx::observable<>::from(
        Employee{ "Jon", "Engineer", 60000 },
        Employee{ "Tyrion", "Manager", 120000 },
        Employee{ "Arya", "Engineer", 92000 },
        Employee{ "Sansa", "Manager", 150000 },
        Employee{ "Cersei", "Accountant", 76000 },
        Employee{ "Jaime", "Engineer", 52000 }).
        subscribe(employees.get_subscriber());

    return 0;
}