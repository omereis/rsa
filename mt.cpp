#include <mutex>

std::queue<data_chunk> data_queue;
std::condition_variable data_cond;

void GetSignal ()
{
}