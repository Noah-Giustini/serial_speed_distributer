Trying to improve timing further and avoid frequency mismatches. It seems that as time goes on, the frequency mismatch builds up and causes spikes in v5 tests

Logic is as follows: Wait for client 1's request. Once requested, get speed from OBD then send back immediately. Continue to the next client and loop back to the first client after all client have been handled