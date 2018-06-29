#include <rxcpp/rx.hpp>
#include <memory>

int main(int argc, char *argv[]) {

 auto generator = [&](rxcpp::subscriber<int> s)
    {
        s.on_next(1);
        s.on_next(2);
        // ...
        s.on_completed();
    };

    auto values = rxcpp::observable<>::create<int>(generator).publish();

    auto work = values.
        observe_on(rxcpp::observe_on_new_thread()).
        tap([&](int c) {
            printf("Hello.....in Tap Function\n");
        }).
        finally([](){printf("s1 completed\n");}).as_dynamic();

    auto start = values.
        ref_count().
        finally([](){printf("s2 completed\n");}).
        as_dynamic();

    // wait for all to finish
    rxcpp::observable<>::from(work, start).
        merge(rxcpp::observe_on_new_thread()).
        as_blocking().subscribe();


}

