# GM Radio

## Inspiration

* http://www.hohensohn.info/mbus/index.htm
* https://github.com/Olstyle/MBus

## Handy setup bits

```
cd project_folder
platformio init --board=uno # n to auto-upload, y to next question

alias ardb="platformio -f -c atom run"
alias ardu="platformio -f -c atom run -t upload"
alias ardm="platformio serialports monitor"
```

## Development

```sh
git clone https://github.com/jch/gm-radio
rake
```
