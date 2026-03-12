Removed state machines and instead used a function.

Logic is as follows: We first cache speed from OBD then read each client and give them the cached speed. The loop repeats after the last client is handled