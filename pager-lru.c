/*
* File: pager-lru.c
* Author:       Andy Sayler
*               http://www.andysayler.com
* Adopted From: Dr. Alva Couch
*               http://www.cs.tufts.edu/~couch/
*
* This file contains the skeleton for an LRU pager
*/

#include <stdio.h> 
#include <stdlib.h>
#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) {

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];

    /* Local vars */
    int proc;
    int page;

    /* initialize timestamp array on first run */
    if (!initialized) {
        for (proc = 0; proc < MAXPROCESSES; proc++) {
            for (page = 0; page < MAXPROCPAGES; page++) {
                timestamps[proc][page] = 0;
            }
        }
        initialized = 1;
    }

    /* TODO: Implement LRU Paging */
    // Following diagram from recitation
    for (proc = 0; proc < MAXPROCESSES; proc++) {
        // Select an active process
        if (q[proc].active != 1) continue;
        // Determine current page
        int pc = q[proc].pc;
        page = pc / PAGESIZE;
        // Is page swapped in? If yes, continue to next process
        if (q[proc].pages[page]) continue;
        // Page not swapped in? Page it in, if that works, continue to the next process
        if (pagein(proc, page)) continue;

        // LRU
        int evict_page = 0;
        int lru_time = tick;
        // Loop through pages in process
        for (page = 0; page < q[proc].npages; page++) {
            // Skip page if not allocated
            if (q[proc].pages[page] != 1) continue;
            // Choose page with the oldest timestamp
            if (timestamps[proc][page] < lru_time) {
                lru_time = timestamps[proc][page];
                evict_page = page;
            }
        }
        // Page out the oldest page
        if (pageout(proc, evict_page)) break;

        // Update the timestamp for page
        timestamps[proc][page] = tick;
    }

    /* advance time for next pageit iteration */
    tick++;
}
