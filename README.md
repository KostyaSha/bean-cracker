## bean-cracker
BEAN bus analysing tool

Draft tool for working with BEAN bus.

Consists from:
- monitor
- logger
- sender
- player

## Input format
One custom format used from custom adapter is supported atm. For local testing it's possible to send text data via tty.
`socat -d -d pty,raw,echo=0,link=/tmp/ttyBean pty,raw,echo=0,link=/tmp/ttyBeanIn`
`ttyBean` is hardcode added to the list of available serials.
Then send data to linked tty `while read LINE; do echo "$LINE" >> /tmp/ttyBeanIn && sleep 0.5;done < dumps/prius/20/eu/ig_on_from_off.txt`
(Don't forget to press `connect` and open logger before).

## UI
The first goal was to get something handy, under improvements.

## Screenshot
![bean-cracker](https://raw.githubusercontent.com/KostyaSha/bean-cracker/KostyaSha-patch-1/bean-cracker.png)

## Possible bus communication implementations
- https://github.com/specadmin/toyota-mpx
- https://github.com/fiztech-code/BeanMPX
