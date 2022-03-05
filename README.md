# To Build

All:       make all<br>
Problem 1: make labyrinth<br>
Problem 2: make vase<br>

# To Clean

make clean<br>

# To Run

Problem 1: ./labyrinth [n]<br>
n = number of guests (maximum of 1024)<br>
<br>
Problem 2: ./vase [n]<br>
n = number of guests (maximum of 1024)<br>

# Problem 2 Strategy Discussion

### Strategy 1

<p>This strategy allows for the least overhead, and allows the guests to roam the castle. 
However, when a guest does decide to go to the door, they might never get in because 
everyone is crowding around the door. They also cannot enjoy the party while they are 
sitting at the door.</p>

### Strategy 2

<p>This strategy allows the guests to roam the castle and check anytime to see if the 
showroom is available. If it is not they may continue to explore the party. One drawback is 
that the guests may not be able to every get into the room until the party ends. 
Additionally, each guest is now required to manage the sign upon entering and exiting. If a 
guest fails to follow protocol, chaos ensues.</p>

### Strategy 3

<p>This strategy allows for the guests' guaranteed entry into the showroom - eventually.
Unfortunately, this strategy does not allow for the guests to enjoy the party while waiting. 
Guests must also properly handle the queue.</p>

### The Guests' Decision

<p>The guests decided to choose Strategy 2, as it allowed them to enjoy the party as much as 
possible while still having some sense of order. They may not all get to see the vase, but 
they agreed it was better that they aren't stuck waiting at a door or in a line.</p>
