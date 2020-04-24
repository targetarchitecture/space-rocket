serial.onDataReceived(serial.delimiters(Delimiters.NewLine), function () {
    message = serial.readUntil(serial.delimiters(Delimiters.NewLine))
    serial.writeLine("{@," + Math.randomRange(0, 100).toString() + "}")
})
input.onButtonPressed(Button.A, function () {
    serial.writeLine("{sound,play,1}")
    // serial.writeLine("{light,on,7,15}")
    serial.writeLine("{light,on,8}")
    //serial.writeLine("{light,breathe,8,1000,1000,1000,1000}")
    serial.writeLine("{light,blink,0,1000,1000,1000,1000}")
    serial.writeLine("{light,breathe,7,100,100,100,100}")
    serial.writeLine("{light,breathe,15,1000,1000,1000,1000}")
    //serial.writeLine("{light,on,7}")
    //serial.writeLine("{light,on,8}")
    //serial.writeLine("{light,on,15}")
})
input.onButtonPressed(Button.B, function () {
    serial.writeLine("{sound,stop}")
    serial.writeLine("{light,off,0,8,15,7}")
    //serial.writeLine("{light,off,7}")
    //serial.writeLine("{light,off,8}")
    //serial.writeLine("{light,off,15}")
})
let arrow = 0
let message = ""
serial.redirect(
    SerialPin.P8,
    SerialPin.P12,
    BaudRate.BaudRate115200
)
basic.showIcon(IconNames.Heart)
basic.forever(function () {
    if (message != "") {
        message = ""

    }
})
