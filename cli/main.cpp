#include "cli-exceptions-handler.h"
#include "cli.h"

int main() {
    cli::CliExceptionsHandler exceptions_handler;
    exceptions_handler.ProcessFunction([](){
                                           cli::CommandLineInterface cli;
                                           cli.Execute();
                                       });
    return 0;
}
