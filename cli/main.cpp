#include "cli-exceptions-handler.h"
#include "cli.h"

INITIALIZE_EASYLOGGINGPP

int main() {
    const std::string settings_file = "config.data";
    cli::CliExceptionsHandler exceptions_handler;
    exceptions_handler.ProcessFunction([&settings_file](){
                                           manager::Settings settings(settings_file, "settings");
                                           cli::CommandLineInterface cli(&settings);
                                           cli.Execute();
                                       });
    return 0;
}
