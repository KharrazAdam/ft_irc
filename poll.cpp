#include <iostream>
#include <sys/poll.h>
#include <unistd.h>

int main()
{
	// Create an array of pollfd structures to hold the file descriptors and events
	const int MAX_EVENTS = 2;
	struct pollfd fds[MAX_EVENTS];

	// Add file descriptors to the array
	fds[0].fd = STDIN_FILENO;  // STDIN file descriptor
	fds[0].events = POLLIN;    // Check for data to read

	fds[1].fd = STDOUT_FILENO; // STDOUT file descriptor
	fds[1].events = POLLOUT;   // Check if ready to write


	// Use poll() to wait for events
	int timeout_ms = 500000; // Timeout value in milliseconds

	while (1)
	{
		int result = poll(fds, MAX_EVENTS, timeout_ms);
		if (result == -1) {
			// std::cerr << "Error in poll()\n";
			return 1;
		}

		if (result == 0) {
			std::cout << "Timeout occurred\n";
		} else {
			// Check events on file descriptors
			// if (fds[0].revents & POLLIN) {
			//     std::cout << "Data is available to read from STDIN\n";
			// }

			if (fds[1].revents & POLLOUT) {
				std::cout << "Ready to write to STDOUT\n";
			}
		}
	}
	return 0;
}
