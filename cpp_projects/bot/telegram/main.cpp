#include "bot.h"

int main() {
    const std::string api_endpoint =
        "https://api.telegram.org/.....";
    Bot bot(api_endpoint);
    bot.Run();

    std::cout << "BOT HAS STOPPED" << std::endl;
    return 0;
}
