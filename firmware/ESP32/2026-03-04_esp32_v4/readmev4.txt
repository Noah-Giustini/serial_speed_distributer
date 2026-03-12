Ditched dual-threading and tried getSpeed() function

Logic is as follows: Loop will read client 1. If available, getSpeed() is called to get current speed from OBD then sent back to client 1. Move on to client 2 and so forth until the last client. Loop repeats