Bank simulation using threads in c


Usage instructions for bank and server programs.

Start by going to each directory bank_server and bank_client on 2 terminals.

Run make clean on both to clean up anything that should be there..

Run make on both, now you should have to executables ./server and ./client

Firstly, run ./server on one terminal.

You need to run it in this format: ./server amountofdesks, so for example: ./server 4, the max is 8.

Now you should have the server running, so in the other terminal with the client directory you should do the followig.

Run ./client

Now your client program should connect to the server program, you can open multiple client terminals.

The commands you can input to your client window are:

l acc_number

w acc_number sum

d acc_number sum

t acc_number1 acc_number2 sum

So for example:

l 5

w 6 10500

d 3 800

t 3 6 765

You will receive replies from the server, telling if your command was succesful.

On the server window you will see the data that the server got in response to the commands.
