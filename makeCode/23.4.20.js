serial.onDataReceived(serial.delimiters(Delimiters.NewLine), function () {
    messages.push(serial.readUntil(serial.delimiters(Delimiters.NewLine)))
})
input.onButtonPressed(Button.A, function () {
    serial.writeLine("{sound,play,4}")
    // serial.writeLine("{light,on,7,15}")
    //serial.writeLine("{light,on,8}")
    // serial.writeLine("{light,breathe,8,1000,1000,1000,1000}")
    // serial.writeLine("{light,blink,0,1000,1000,1000,1000}")
    // serial.writeLine("{light,breathe,7,100,100,100,100}")
    // serial.writeLine("{light,breathe,15,1000,1000,1000,1000}")
})
input.onButtonPressed(Button.B, function () {
    serial.writeLine("{sound,stop}")
    // serial.writeLine("{light,off,0,8,15,7}")
})
// let recievedValue = ""
// let recievedState = ""
// let recievedEvent = ""
let messages: string[] = []
//let parts: string[] = []
//let message = ""
let sourceSound = 10000
let sourceTouchTouched = 10100
let sourceTouchReleased = 10101
let sourceSoundBusy = 10200
let soundBusy = -1
serial.redirect(
    SerialPin.P8,
    SerialPin.P12,
    BaudRate.BaudRate9600
)
basic.showIcon(IconNames.Heart)
basic.forever(function () {
    if (messages.length > 0) {
        let message = messages.shift()

        message = message.substr(1, message.length - 2)

        let parts = message.split(",")

        let recievedEvent = parts[0]
        let recievedState = parts[1]
        let recievedValue = parts[2]

        recievedValue = recievedValue.slice(0, -1) //seems to be needed to remove extra characters from the serial transfer

        serial.writeLine("{message,message:" + message + "}")
        //serial.writeLine("{message,recievedEvent:" + recievedEvent + "}")
        //serial.writeLine("{message,recievedState:" + recievedState + "}")
        //serial.writeLine("{message,recievedValue:" + recievedValue + "}")

        if (recievedEvent == "touch") {
            if (recievedState == "touched") {
                control.raiseEvent(
                    sourceTouchTouched,
                    parseInt(recievedValue)
                )
            }
            if (recievedState == "released") {
                //serial.writeLine("{message," + "touch me in the morning" + "}")
                control.raiseEvent(
                    sourceTouchReleased,
                    parseInt(recievedValue)
                )
            }
        }

        if (recievedEvent == "sound") {
            if (recievedState == "busy") {
                //                 let t = parseInt(recievedValue)
                //serial.writeLine("{message," + "raised busy event" + "}")
                //serial.writeLine("{message,>>" + t + "<<}")
                soundBusy = parseInt(recievedValue);
                control.raiseEvent(
                    sourceSoundBusy,
                    soundBusy
                )
            }
        }
    }
})

control.onEvent(sourceSoundBusy, 0, function () {
    basic.showNumber(soundBusy)
})

control.onEvent(sourceSoundBusy, 1, function () {
    basic.showNumber(soundBusy)
})


control.onEvent(sourceTouchReleased, 1, function () {
    basic.showString("T")
})

control.onEvent(sourceTouchReleased, 10, function () {
    basic.showIcon(IconNames.No)
})

control.onEvent(sourceTouchTouched, 10, function () {
    basic.showIcon(IconNames.Yes)
})



