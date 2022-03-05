#include <atomic>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>

const int MAX_GUESTS = 1024;
const int MAX_TIME_SECS = 10;

std::mt19937_64 rng;
std::uniform_int_distribution<std::mt19937_64::result_type> look_time;
std::uniform_int_distribution<std::mt19937_64::result_type> party_time;

std::mutex showroom;
std::mutex rng_lock;

bool keep_partying = true;
std::atomic_int unique_visits;

void party(int n);
void guest(int i);

int main(int argc, char ** argv)
{
	int n;

	if (argc == 1 || atoi(argv[1]) <= 0)
		n = 10;
	else
		n = std::min(MAX_GUESTS, atoi(argv[1]));

	// Seed the random number generator	
	std::random_device r;
	rng.seed(r());

	// NOTE: I spent way too long tweaking numbers and changing how random wait
	// times were generated, but I could never get anything that appeared very
	// random. These numbers can be adjusted for different runtimes

	// Range of possible time spent looking at vase
	look_time = std::uniform_int_distribution<std::mt19937_64::result_type>(
		(MAX_TIME_SECS * 1e9) / (n * 20.0), (MAX_TIME_SECS * 1e9) / (n * 4.0)
	);

	// Range of possible time spent exploring the party
	party_time = std::uniform_int_distribution<std::mt19937_64::result_type>(
		(MAX_TIME_SECS * 1e9) / 1000.0, (MAX_TIME_SECS * 1e9) / (n)
	);

	std::cout << "Starting the party with " << n << " guests!\n";

	auto start = std::chrono::high_resolution_clock::now();
	party(n);
	auto end = std::chrono::high_resolution_clock::now();

	std::cout << "The party has ended!\n"
			  << unique_visits.load() << " guests got to see the vase!\nThe party lasted "
			  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			  << " milliseconds\n";
}

void party(int n)
{

	std::thread guests[n];
	for (int i = 0; i < n; ++i)
	{
		 guests[i] = std::thread(guest, i);
	}

	auto start = std::chrono::steady_clock::now();
	while (keep_partying)
	{
		if (std::chrono::duration_cast<std::chrono::seconds>
		   (std::chrono::steady_clock::now() - start).count() >= MAX_TIME_SECS ||
			unique_visits.load() == n)
		{
			keep_partying = false;
		}
	}

	for (std::thread & g : guests)
		g.detach();
}

void guest(int i)
{
	int seen_vase = 0;
	while (keep_partying)
	{
		// Enjoying the party!
		// The more times this guest has seen the vase, the longer the time they party!
		rng_lock.lock();
		auto partying = std::chrono::nanoseconds(party_time(rng)) * std::pow(2, seen_vase);
		rng_lock.unlock();

		std::this_thread::sleep_for(partying);
		
		// Check if showroom is AVIALABLE. If it is, set to BUSY
		if (showroom.try_lock())
		{
			// Look at the vase!
			std::this_thread::sleep_for(std::chrono::nanoseconds(look_time(rng)));

			if (!seen_vase++) ++unique_visits;

			// Set showroom to AVILABLE
			showroom.unlock();
		}
	}
}
