### testing notes

 testing notes
  - a lot of parameters do not want to have glove interaction
  - reaching the button might require two hands depending on pose
  - pitch seems the most reliable 
  - puppet hand and open hand are often confused
  - i need to show on the gui what parameter I'm modifying
  - to change it to raspberry pi /wifi/send/ip "192.168.0.179"
  - to change it to mac (use glover or) /wifi/send/ip "192.168.0.243"
  - the glove ip is 192.168.1.99 or 192.168.1.216
  - send port 9000


  /wifi/client/ssid - lilmango
  /wifi/client/key - goodlife 
  /wifi/send/port - 9000
  


  set forwards /ahrs/initialize
  /ahrs/zero

  Testing notes part 2

  What parameters were fun to change?
   - exp
   - delay
   - pitch
   - synth voice
   - presets

   What parameters were not ?
   - portamento
   - filter type
   - env type
   - bypass

   What didn't seem to go as planned
   - filter bandwidth
   - filter
   - mix
   - ring mod

Preparing for the next round
 - test the new quaternion config
 - make a nice guide showing the mappings
 - test the parameters that didn't go as planned
 - change the mappings so I only change the fun things (maybe make a new mapping preset)
 - make a Mimu cpp class
 - make the mercury 7 presets 
 - add the Parameter I'm changing and what page I'm on to the gui
 - 
 
 Ngimu stuff
 https://x-io.co.uk/ngimu/
 https://x-io.co.uk/downloads/NGIMU-User-Manual-v1.6.pdf - user manual


 # Testing Setup
 compile on laptop
 use the router on the pedalboard
 send usb midi out from the laptop to the lvx
 connect the glove to the router

 ### things to test
go through the parameters individually and test the effects


all the effects need to be post loop

