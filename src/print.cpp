//
// Created by Simon Cros on 13/01/2025.
//

#include <iostream>
#include <format>

#include "complexity.hpp"

auto hideCursor() -> void
{
    if (prettyPrint())
    {
        std::cout << "\e[?25l";
    }
}

auto showCursor() -> void
{
    if (prettyPrint())
    {
        std::cout << "\e[?25h";
    }
}

auto getVersion() -> std::string_view
{
    return "Complexity 2.0.0 (2025-01-12)";
}

auto getUsage() -> std::string_view
{
    return "usage: ./complexity [-vh] [-s seed] [-f push_swap] [-t threads] numbers iterations [goal] [checker]";
}

auto getHelp() -> std::string
{
    return
        "\033[1mNAME\033[0m\n"
        "     \033[1mcomplexity\033[0m -- starting benchmark for push_swap\n"
        "\n"
        "\033[1mSYNOPSIS\033[0m\n"
        "     \033[1m./complexity\033[0m [\033[1m-vh\033[0m] [\033[1m-s\033[0m \033[4mseed\033[0m] [\033[1m-o\033[0m \033[4mfile\033[0m] [\033[1m-f\033[0m \033[4mfile\033[0m] [\033[1m-t\033[0m \033[4mthreads\033[0m] \033[4mnumbers\033[0m \033[4miterations\033[0m [\033[4mgoal\033[0m] [\033[4mchecker\033[0m]\n"
        "\n"
        "\033[1mDESCRIPTION\033[0m\n"
        "     Following options are available :\n"
        "     \n"
        "     \033[1m-v\033[0m, \033[1m--version\033[0m\n"
        "             Show version of tester\n"
        "     \n"
        "     \033[1m-h\033[0m, \033[1m--help\033[0m\n"
        "             Show this message\n"
        "     \n"
        "     \033[1m-s\033[0m \033[4mseed\033[0m, \033[1m--seed\033[0m=\033[4mseed\033[0m\n"
        "             Generates the numbers based on the seed\n"
        "     \n"
        "     \033[1m-o\033[0m \033[4moutput\033[0m, \033[1m--output\033[0m=\033[4moutput\033[0m\n"
        "             Specifies an output file for logs\n"
        "     \n"
        "     \033[1m-f\033[0m \033[4mpush_swap\033[0m, \033[1m--file\033[0m=\033[4mpush_swap\033[0m\n"
        "             Use \033[4mpush_swap\033[0m for push_swap executable\n"
        "     \n"
        "     \033[1m-t\033[0m \033[4mthreads\033[0m, \033[1m--threads\033[0m=\033[4mthreads\033[0m\n"
        "             Specifies the amount of threads to use (default: 6)\n";
}

auto printStart(const program_opts& opts, const program_params& params) -> void
{
    if (prettyPrint())
    {
        std::cout << getVersion() << '\n';
        std::cout << std::format(
            "\033[97mStarting the test : \033[95m{}\033[97m elements, \033[95m{}\033[97m iterations\033[0m",
            params.numbers, params.iterations);
        if (opts.seed.has_value())
            std::cout << std::format(" (seed {})", opts.seed.value());
        std::cout << std::endl;
    }
}

auto printStatus(const program_params& params, const results_t& results) -> void
{
    double mean = 0;
    double stddev = 0;
    uint64_t underObjective = 0;
    uint32_t percentDone = 0;

    if (results.done > 0)
    {
        mean = static_cast<double>(results.total) / results.done;
        for (const uint32_t result : results.results)
            stddev += pow(mean - result, 2.0);
        stddev = sqrt(stddev / results.done);
        underObjective = (results.done - results.aboveObjective) * 100 / results.done;
        percentDone = results.done * 100 / params.iterations;
    }

    if (prettyPrint())
    {
        std::cout << std::format("Worst = \033[31m{}\033[0m instructions\n", results.worst);
        std::cout << std::format("Mean = \033[33m{:.1f}\033[0m instructions\n", mean);
        std::cout << std::format("Best = \033[36m{}\033[0m instructions\n", results.best);
        std::cout << std::format("Std. deviation = \033[93m{:.1f}\033[0m instructions\n", stddev);

        if (params.objective.has_value())
            std::cout << std::format(
                "Objective = \033[94m{}\033[0m % under \033[94m{}\033[0m (\033[91m{}\033[0m above)\n",
                underObjective, params.objective.value(), results.aboveObjective);
        else
            std::cout << "Objective = enter a number as the third argument\n";

        // if (params.checker.has_value())
        //     std::cout << "Precision = \033[97m" << (ok * 100 / done) << "\033[0m % OK (\033[91m" << (done - ok) << "\033[0m KO)   " << std::endl;
        // else
        //     std::cout << "Precision = enter a tester as the fourth argument\n";
        std::cout << "Failed = currently not available\n";
        std::cout << std::format("\033[32m{}\033[0m % effective\n", percentDone);
    }
    else
    {
        std::cout << std::format(
            "{{\n"
            "  \"elements\": {},\n"
            "  \"iterations\": {},\n"
            "  \"objective\": {},\n"
            "  \"worst\": {},\n"
            "  \"mean\": {:.6f},\n"
            "  \"best\": {},\n"
            "  \"stddev\": {:.6f},\n"
            "  \"aboveObjective\": {}\n"
            "}}\n",
            params.numbers,
            params.iterations,
            params.objective.value_or(-1),
            results.worst,
            mean,
            results.best,
            stddev,
            results.aboveObjective);
    }
}