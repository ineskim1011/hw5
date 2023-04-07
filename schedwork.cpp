#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Helper for recursive calling 
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
		vector<vector<size_t>>& numShifts,
		size_t currDay,
		size_t currWorker
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if (avail.size() == 0) {
      return false;
    }
    sched.clear();

		// Initialize the Daily Schedule return value (2D array of Worker IDs)
		for (size_t i = 0; i < avail.size(); i++) {
			sched.push_back(vector<Worker_T>());
		}

		// Initialize a 2D array to count running number of shifts for each worker 
		vector<vector<size_t>> numShifts;
		for (size_t i = 0; i < avail[0].size(); i++) {
			numShifts.push_back(vector<size_t>());
			numShifts[i].push_back(0);
		}
		
		// Call helper function for first day, first shift which will recurse for all days 
		// Returns true if schedule is possible with given parameters, false otherwise
		if (scheduleHelper(avail, dailyNeed, maxShifts, sched, numShifts, 0, 0)) { return true; }

		return false;
}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
		vector<vector<size_t>>& numShifts,
		size_t currDay,
		size_t currWorkers
)
{	
		// Base Case: if current day has been incremented to a non-existent day (i.e. past the alloted days)
		// that means that the schedule is possible 
		if (currDay == avail.size()) {
			return true;
		}
		
		// If number of workers for a day is equal to the amount of workers required per day, 
		// recursively call for next day
		if (currWorkers == dailyNeed) {
			return scheduleHelper(avail, dailyNeed, maxShifts, sched, numShifts, currDay + 1, 0);
		}

		// Tallies number of workers we are adding to number already working 
		size_t addedWorkers = currWorkers;
		// Iterate through workers' availability for the given day 
		for (size_t i = 0; i < avail[currDay].size(); i++) {
			// Runs if the worker is not already scheduled for that day 
			if (find(sched[currDay].begin(), sched[currDay].end(), i) == sched[currDay].end()) {
				// Runs if the worker is available for that day 
				if (avail[currDay][i]) {
					// Runs if the worker has not maxed out their shifts 
					if (numShifts[i][0] < maxShifts) {
						// Add this worker to this day, so increment number of added workers 
						addedWorkers++;
						// Add this worker to the schedule for this day 
						sched[currDay].push_back(i);
						// Increment worker's number of shifts in vector created in main Schedule function 
						numShifts[i][0]++;
						// If number of daily workers needed is reached, reset added workers for the 
						// day to zero and move onto the next day, this one is complete 
						if (dailyNeed == sched[currDay].size()) {
							addedWorkers = 0;
							currDay++;
						}
						// Recursive call to Helper for next day 
						if (scheduleHelper(avail, dailyNeed, maxShifts, sched, numShifts, currDay, addedWorkers)) { return true; }
						// Backtrack case: if recursive call is false, undo by: 
						// decrementing number of shifts,
						numShifts[i][0]--;
						// going back to the past day if the schedule for the current day has not 
						// been made yet (i.e. this day wasn't the problem)
						if (sched[currDay].size() == 0) {
							currDay--;
						}
						// taking the last addition off the scheudle 
						sched[currDay].pop_back();
						// starting over (i.e. added workers reset to current workers)
						addedWorkers = currWorkers;
					}
				}
			}
		}
		// return false otherwise 
		return false;
}

