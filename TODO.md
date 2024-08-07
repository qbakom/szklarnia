# Greenhouse Irrigation System

## Assemble Hardware 
- 
- soldering components
- creating box 
### 1. Serving webserver onto 
- buttons frontend design
- routing 
### 2. Dynamic DNS into host server
### 3. Integrating firebase with esp32 webserver


# I'm building automated irrigation system to control two valves that irrigate 


### IDEA
General idea is to build automated irrigation system in the greenhouse. The irrigation is fully developed and working but
It needs to be physically switched which is quite unconvienient for me. I came up with idea to use esp32, 2 channels relay, electro-valves.
You need to know that I have 4 valves but only 2 are the ones that I want to control. The idea is that I built webserver on my esp32,
but currently It is not handling request and logic on the webserver. Check the code I provided to tell me if how to handle those requests