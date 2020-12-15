#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <functional>
#include <string>
#include <algorithm>
#include <chrono>

#include "VEGameJobSystem.h"
#include "VECoro.h"


using namespace std::chrono;


namespace phases {

    using namespace vgjs;

    auto g_global_mem5 = ::n_pmr::synchronized_pool_resource({ .max_blocks_per_chunk = 20, .largest_required_pool_block = 1 << 20 }, n_pmr::new_delete_resource());

    Coro<> phase2() {
        std::cout << "Phase 2" << std::endl;
        co_await thread_index{ 1 };
        co_await phase{ 2 };

        co_return;
    }

    void printPar( int i) {
        std::cout << "i: " << i << std::endl;
    }

    Coro<int> phase1() {
        std::cout << "Phase 1" << std::endl;
        schedule(phase{ 1 });

        schedule([=]() { printPar(4); }, phase{ 2 });
        schedule([=]() { printPar(5); }, phase{ 2 });
        schedule([=]() { printPar(6); }, phase{ 2 });

        co_await phase2();

        //schedule(phase2(), phase{ 2 });
        //schedule(phase{ 2 });
        co_return 0;
    }


    void phase0cont() {
        schedule(phase1());
    }

    void phase0() {
        std::cout << "Phase 0" << std::endl;
        schedule(phase{ 0 });

        schedule([=]() { printPar(0); });

        schedule([=]() { printPar(1); }, phase{ 1 });
        schedule([=]() { printPar(2); }, phase{ 1 });
        schedule([=]() { printPar(3); }, phase{ 1 });

        continuation([]() { phase0cont(); });
    }

    void test() {
        std::cout << "Starting phases test()\n";

        schedule([=](){ phase0(); });

        std::cout << "Ending phases test()\n";
    }

}


