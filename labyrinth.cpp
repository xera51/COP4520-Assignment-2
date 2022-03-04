#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

void cupcake(int n);
void counter(int n);
void maze();

std::mutex maze_full;
std::atomic_bool has_cupcake(true);
std::atomic_int visits;
bool keep_running = true;
const int MAX_GUESTS = 1024;

int main(int argc, char ** argv)
{
	int n;

	if (argc == 1 || atoi(argv[1]) <= 0)
		n = 8;
	else
		n = std::min(MAX_GUESTS, atoi(argv[1]));

	std::cout << "Starting the game with " << n << " guests!\n";
	auto start = std::chrono::high_resolution_clock::now();
	cupcake(n);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Done!\n";
	std::cout << "The guests took " 
			  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			  << " milliseconds and completed the labyrinth " << visits << " times\n";

	return 0;
}

void cupcake(int n)
{
	std::thread guests[n];

	guests[0] = std::thread(counter, n);
	for (short i = 1; i < n; ++i)
		guests[i] = std::thread(maze);
		
	for (std::thread & g : guests)
		g.join();
}

void counter(int n)
{
	int count = 0;

	// Keep going until we know that everyone has gone
	while(keep_running)
	{
		// Wait until the Minotaur to select us to enter the labyrinth
		maze_full.lock();

		// If the cupcake has been eaten, then we can count one more person
		// as gone through the labyrinth that we have yet to count before
		if (!has_cupcake.load())
		{
			++count;
			has_cupcake.store(true);
		}
		visits++;

		// Exit the labyrinth
		maze_full.unlock();

		// If everyone has gone, then tell the Minotaur everyone has
		// been through the labyrinth
		if (count + 1 == n) keep_running = false;
	}
}

void maze()
{
	bool eaten = false;

	// Keep going until the counter knows everyone has gone
	while(keep_running)
	{
		// Wait until the Minotaur to select us to enter the labyrinth
		maze_full.lock();

		// If there is a cupcake and we have not eaten one yet, eat it
		if (has_cupcake.load() && !eaten)
		{
			has_cupcake.store(false);
			eaten = true;
		}
		visits++;

		// Exit the labyrinth
		maze_full.unlock();
	}
}

