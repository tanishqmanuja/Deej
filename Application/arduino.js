const fs = require('fs');
const yaml = require('js-yaml');
const SerialPort = require('serialport');

function writeConfig(){
    let doc = yaml.load(fs.readFileSync('./config.yaml', 'utf8'));
    doc.com_port = path
    fs.writeFile('./config.yaml', yaml.dump(doc), (err) => {
        if (err) {
            console.log(err);
        } else {
            console.log('Config Changed')
        }
    });
}

let path = ''

function searchArduino(){
    SerialPort.list().then(ports => {
        let done = false
        let count = 0
        let allports = ports.length
        let manufacturers = ['arduino','wch.cn']
        let vendorIDs = ['1a86']
    
        ports.forEach(port => {
            count = count + 1
            pm = port.manufacturer
            pvid = port.vendorId
    
            if ((typeof pm !== 'undefined' && manufacturers.some(m=>pm.includes(m))) || vendorIDs.some(vid=>pvid==vid)) {
                path = port.path
                done = true
                console.log('Arduino Found at',path)
                writeConfig()
            }
    
            if (count === allports && done === false) {
                console.log('No Arduino Found')
            }
        })
    })
}




searchArduino()