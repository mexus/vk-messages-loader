#include "cli-exceptions-handler.h"
#include "cli.h"

int main() {
    cli::CliExceptionsHandler exceptions_handler;
    cli::CommandLineInterface cli;
    auto execute = std::bind(&cli::CommandLineInterface::Execute, &cli);
    exceptions_handler.ProcessFunction(execute);
    return 0;
}
