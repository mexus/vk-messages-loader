#include <manager/manager.h>

int main() {
    manager::Manager manager("config.data");
    manager.LoadFriends();
    manager.LoadMessages();
    return 0;
}
