original code has two clients connected but exhibits bad frequency odometer reading results.

testing to see which logic fixes from different AI shows best results

The prompt is:
"When I remove the extra client, the code does exactly what I want, which is for one client to send requests for speed and receive the current speed from OBD at 1Hz. But this code implements an extra client. During my tests, both clients are receiving speed messages between 0.5Hz and 0.2Hz. I am using an EPS32-WROOM-32 and using its mutli threading capabilities while using mutex to handle shared resources. The obd is connected to uart 1, the main client connected to uart 2. The extra client is connected to uart 0, which I knew would be bad. Can it still be possible to still acheive 1Hz for both clients? All my systems should be operating at 115200 baud rate so software serial can't be a solution.

If there are any fixes please make minimal changes to the code while mainting my style and strucutre.

If there is any clarification that you need let me know"