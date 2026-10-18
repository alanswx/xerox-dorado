// Compile with the exact patched Main worker, including its real pthreads.
// The direct condition signal injects an empty/spurious wakeup, not a job.
#include "offload.cpp"
#include <chrono>
#include <future>
#include <stdexcept>
#include <thread>
#include <vector>

using Clock = std::chrono::steady_clock;
static void check(bool ok, const char *why) {
    if (!ok) throw std::runtime_error(why);
}
static bool submit(std::function<void()> work) {
#ifdef CONTROL_BLOCKING_ENQUEUE
    offload_add_work(work);
    return true;
#else
    return offload_try_add_work(work);
#endif
}

int main() {
    bool live = false;
    // Captured objects outlive the catch handler's worker drain on failures.
    std::promise<void> entered;
    std::vector<unsigned> order;
    try {
        offload_start(); live = true;
        // POSIX explicitly permits waking without a queue item. The worker
        // must recheck its predicate instead of invoking an empty handler.
        for (unsigned n = 0; n < 50; ++n) {
            pthread_cond_signal(&s_cond_work);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        check(!offload_try_add_work({}), "empty handler accepted");
        auto entered_future = entered.get_future();
        check(submit([&] {
            entered.set_value();
            std::this_thread::sleep_for(std::chrono::seconds(3));
            order.push_back(0);
        }), "first job rejected");
        check(entered_future.wait_for(std::chrono::seconds(2)) == std::future_status::ready,
              "worker did not start");
        // The running job occupies one of the existing eight queue slots.
        for (unsigned n = 1; n < 8; ++n)
            check(submit([&, n] { order.push_back(n); }), "queue lost capacity");

        unsigned polls = 0;
        double maximum_call_ms = 0, maximum_poll_gap_ms = 0;
        const auto start = Clock::now();
        auto previous = start;
        while (Clock::now() - start < std::chrono::milliseconds(2800)) {
            auto before = Clock::now();
            bool accepted = submit([] {});
            auto after = Clock::now();
            double duration = std::chrono::duration<double, std::milli>(after-before).count();
            double gap = std::chrono::duration<double, std::milli>(before-previous).count();
            if (duration > maximum_call_ms) maximum_call_ms = duration;
            if (gap > maximum_poll_gap_ms) maximum_poll_gap_ms = gap;
            check(duration < 100, "enqueue blocked the polling thread");
            check(!accepted, "full queue accepted a ninth job");
            previous = before; ++polls;
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        check(polls > 100 && maximum_poll_gap_ms < 100, "poll heartbeat stalled");
        // Main's app_restart uses this same stop/join boundary. All accepted
        // callbacks must finish in FIFO order before it returns.
        offload_stop(); live = false;
        check(order == std::vector<unsigned>({0,1,2,3,4,5,6,7}),
              "accepted work lost, duplicated, or reordered during drain");
        check(!offload_try_add_work([] {}), "stopped worker accepted work");
        printf("OFFLOAD-TRY PASS: 3s worker job, %u poll attempts, max enqueue %.3fms, "
               "max poll gap %.3fms; eight-slot bound/FIFO drain/stop/empty wake\n",
               polls, maximum_call_ms, maximum_poll_gap_ms);
        return 0;
    } catch (const std::exception &e) {
        fprintf(stderr, "OFFLOAD-TRY FAIL: %s\n", e.what());
        if (live) offload_stop();
        return 23;
    }
}
